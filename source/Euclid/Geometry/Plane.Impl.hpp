//
//  Geometry/Plane.Intersection.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 2/12/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_PLANE_INTERSECTION_H
#define _EUCLID_GEOMETRY_PLANE_INTERSECTION_H

#include "Plane.hpp"
#include "Sphere.hpp"
#include "../Numerics/Vector.Geometry.hpp"

namespace Euclid
{
	namespace Geometry
	{
		template <dimension D, typename NumericT>
		bool Plane<D, NumericT>::intersects_with (const Plane<D, NumericT> & other, Line<3, NumericT> & line) const
		{
			using Numerics::cross_product;
			
			/* Planes are parallel? */
			if (other.normal() == _normal)
				return false;

			VectorT u = cross_product(_normal, other._normal).normalize();

			line.set_direction(u);
			line.set_point(-cross_product((_normal*other._distance) - (other._normal*_distance), u) / u.length_squared());

			return true;
		}

		template <dimension D, typename NumericT>
		bool Plane<D, NumericT>::intersects_with (const Line<3, NumericT> & line, VectorT & at) const
		{
			NumericT d = _normal.dot(line.direction());

			/* Line and Plane are parallel? */
			if (d == 0.0) return false;

			// This minus sign may need to be inside the (-_normal)
			NumericT r = -(_normal.dot(line.point()) - _distance);
			NumericT t = r / d;

			at = line.point() + line.direction() * t;

			return true;
		}

		template <dimension D, typename NumericT>
		Intersection Plane<D, NumericT>::intersects_with (const Sphere<D, NumericT> & sphere) const
		{
			NumericT d = distance_to_point(sphere.center());

			if (d > sphere.radius())
				return Intersection::DISJOINT;
			else if (equivalent(d, sphere.radius()))
				return Intersection::TOUCH;
			else
				return Intersection::OVERLAP;
		}
	}
}

#endif
