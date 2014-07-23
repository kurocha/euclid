//
//  Sphere.Intersection.h
//  Euclid
//
//  Created by Samuel Williams on 28/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_GEOMETRY_SPHERE_INTERSECTION_H
#define _EUCLID_GEOMETRY_SPHERE_INTERSECTION_H

#include "Sphere.hpp"
#include "Line.hpp"

namespace Euclid
{
	namespace Geometry
	{
		template <dimension D, typename NumericT>
		Intersection Sphere<D, NumericT>::intersects_with (const Line<D, NumericT> &line, RealT & entry_time, RealT & exit_time) const
		{
			//Optimized method sphere/ray intersection
			auto dst = line.point() - _center;

			RealT b = dst.dot(line.direction());
			RealT c = dst.dot(dst) - (_radius * _radius);

			// If d is negative there are no real roots, so return
			// false as ray misses sphere
			RealT d = b * b - c;

			if (d == 0.0) {
				entry_time = (-b) - std::sqrt(d);
				exit_time = entry_time;
				return Intersection::TOUCH;
			}

			if (d > 0) {
				entry_time = (-b) - std::sqrt(d);
				exit_time = (-b) + std::sqrt(d);
				return Intersection::OVERLAP;
			} else {
				return Intersection::DISJOINT;
			}
		}

		template <dimension D, typename NumericT>
		Intersection Sphere<D, NumericT>::intersects_with (const LineSegment<D, NumericT> & segment, RealT & entry_time, RealT & exit_time) const
		{
			auto line = segment.to_line();

			Intersection result = intersects_with(line, entry_time, exit_time);

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
				return Intersection::ENCLOSED;
			}

			// The line segment did not intersect.
			return Intersection::DISJOINT;
		}

		template <dimension D, typename NumericT>
		Intersection Sphere<D, NumericT>::intersects_with (const Sphere & other, Vector<D, NumericT> & displacement) const
		{
			auto d = distance_between_edges(other, displacement);

			if (d < 0.0) {
				return Intersection::OVERLAP;
			} else if (d == 0.0) {
				return Intersection::TOUCH;
			} else {
				return Intersection::DISJOINT;
			}
		}

		template <dimension D, typename NumericT>
		Intersection Sphere<D, NumericT>::intersects_with (const Vector<D, NumericT> & point, Vector<D, NumericT> & displacement) const
		{
			auto d = distance_from_point(point, displacement);

			if (d < 0.0) {
				return Intersection::OVERLAP;
			} else if (d == 0.0) {
				return Intersection::TOUCH;
			} else {
				return Intersection::DISJOINT;
			}
		}
	}
}

#endif
