//
//  Geometry/Sphere.impl.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 16/11/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_SPHERE_H
#error This header should not be included manually. Include Sphere.h instead.
#endif

#include "Line.h"

namespace Euclid
{
	namespace Geometry
	{
		template <dimension D, typename NumericT>
		Sphere<D, NumericT>::Sphere ()
		{
		}

		template <dimension D, typename NumericT>
		Sphere<D, NumericT>::Sphere (const Zero &) : _center(ZERO), _radius(0)
		{
		}

		template <dimension D, typename NumericT>
		Sphere<D, NumericT>::Sphere (const Identity &, const NumericT & n) : _center(ZERO), _radius(n)
		{
		}

		template <dimension D, typename NumericT>
		Sphere<D, NumericT>::Sphere(Vector<D, NumericT> center, NumericT radius) : _center(center), _radius(radius)
		{
		}

		template <dimension D, typename NumericT>
		const Vector<D, NumericT> & Sphere<D, NumericT>::center () const
		{
			return _center;
		}

		template <dimension D, typename NumericT>
		void Sphere<D, NumericT>::set_center (const Vector<D> & center)
		{
			_center = center;
		}

		template <dimension D, typename NumericT>
		const NumericT & Sphere<D, NumericT>::radius () const
		{
			return _radius;
		}

		template <dimension D, typename NumericT>
		void Sphere<D, NumericT>::set_radius (const NumericT & radius)
		{
			_radius = radius;
		}

		template <dimension D, typename NumericT>
		typename RealType<NumericT>::RealT Sphere<D, NumericT>::distance_between_edges (const Sphere<D, NumericT> & other, Vector<D, NumericT> & displacement) const
		{
			displacement = _center - other._center;
			typename RealType<NumericT>::RealT total_radius = _radius + other._radius;

			return displacement.length() - total_radius;
		}

		template <dimension D, typename NumericT>
		typename RealType<NumericT>::RealT Sphere<D, NumericT>::distance_from_point (const Vector<D, NumericT> & point, Vector<D, NumericT> & displacement) const
		{
			displacement = point - _center;

			return displacement.length() - _radius;
		}

		template <dimension D, typename NumericT>
		IntersectionResult Sphere<D, NumericT>::intersects_with (const Sphere<D, NumericT> & other, Vector<D, NumericT> & displacement) const
		{
			typename RealType<NumericT>::RealT d = distance_between_edges(other, displacement);

			if (d < 0.0) {
				return SHAPES_INTERSECT;
			} else if (d == 0.0) {
				return EDGES_INTERSECT;
			} else {
				return NO_INTERSECTION;
			}
		}

		template <dimension D, typename NumericT>
		IntersectionResult Sphere<D, NumericT>::intersects_with (const Vector<D, NumericT> & point, Vector<D, NumericT> & displacement) const
		{
			typename RealType<NumericT>::RealT d = distance_from_point(point, displacement);

			if (d < 0.0) {
				return SHAPES_INTERSECT;
			} else if (d == 0.0) {
				return EDGES_INTERSECT;
			} else {
				return NO_INTERSECTION;
			}
		}

		template <dimension D, typename NumericT>
		IntersectionResult Sphere<D, NumericT>::intersects_with (const Line<D, NumericT> &line, RealT & entry_time, RealT & exit_time) const
		{
			//Optimized method sphere/ray intersection
			Vector<D> dst(line.point() - _center);

			RealT b = dst.dot(line.direction());
			RealT c = dst.dot(dst) - (_radius * _radius);

			// If d is negative there are no real roots, so return
			// false as ray misses sphere
			RealT d = b * b - c;

			if (d == 0.0) {
				entry_time = (-b) - Number<NumericT>::sqrt(d);
				exit_time = entry_time;
				return EDGES_INTERSECT;
			}

			if (d > 0) {
				entry_time = (-b) - Number<NumericT>::sqrt(d);
				exit_time = (-b) + Number<NumericT>::sqrt(d);
				return SHAPES_INTERSECT;
			} else {
				return NO_INTERSECTION;
			}
		}

		template <dimension D, typename NumericT>
		IntersectionResult Sphere<D, NumericT>::intersects_with (const LineSegment<D, NumericT> &segment, RealT & entry_time, RealT & exit_time) const
		{
			Line<D> line = segment.to_line();

			IntersectionResult result = intersects_with(line, entry_time, exit_time);

			// time_at_intersection is the time in unit length from segment.start() in direction segment.offset()
			// We will normalize the time in units of segment.offset()s, so that segment.point_at_time(time_at_intersection)
			// works as expected

			RealT n = segment.offset().length();
			entry_time = entry_time / n;
			exit_time = exit_time / n;

			// The line segment intersects the sphere at one or two points:
			if ((entry_time >= 0.0 && entry_time <= 1.0) || (exit_time >= 0.0 && exit_time <= 1.0)) {
				return result;
			}

			// Line segment is completely within sphere:
			if (entry_time < 0.0 && exit_time > 1.0) {
				return SHAPE_EMBEDDED;
			}

			// The line segment did not intersect.
			return NO_INTERSECTION;
		}
	}
}
