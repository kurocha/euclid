//
//  Geometry/Plane.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 23/09/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_PLANE_H
#define _EUCLID_GEOMETRY_PLANE_H

#include "Geometry.h"
#include "Line.h"

namespace Euclid {
	namespace Geometry {
		template <unsigned D, typename NumericT>
		class Plane {
		protected:
			typedef Vector<D, NumericT> VectorT;

			NumericT _distance;
			VectorT _normal;

			void convert_from_point_normal_form (const VectorT & point, const VectorT & normal)
			{
				_normal = normal;
				_distance = (-normal).dot(point);
			}
		public:
			Plane ()
			{
			}

			/// Point is a point on the plain, and direction is the normal
			Plane (const Line<D, NumericT> & line)
			{
				convert_from_point_normal_form(line.point(), line.direction());
			}

			Plane (const VectorT & point, const VectorT & normal)
			{
				convert_from_point_normal_form(point, normal);
			}

			Plane (const NumericT & d, const VectorT & n) : _distance(d), _normal(n)
			{
			}

			const RealT & distance() const { return _distance; }
			const VectorT & normal() const { return _normal; }

			void set_distance(const RealT & r) { _distance = r; }
			void set_normal(const VectorT & n) { _normal = n; }

			bool is_parallel(const Plane & other) const;

			bool intersects_with (const Plane & plane, Line<3, NumericT> & line) const;
			bool intersects_with (const Line<3, NumericT> & line, VectorT & at) const;

			/// Finds the closed point on a plane to another point
			VectorT closest_point (const VectorT & point) const {
				Vec3 at;

				intersects_with(Line<3, NumericT>(point, _normal), at);

				return at;
			}

			IntersectionResult intersects_with (const Sphere<D, NumericT> & sphere) const;

			/// Can be used to test sphere intersection
			NumericT distance_to_point (const VectorT &at) const
			{
				/* Because the normal is normalized, it will always be 1.0, however it is possible to generalize
				 this algorithm for planes of the form ax+by+cz+d */

				return (_normal.dot(at) + _distance) /* / _normal.length()*/;
			}
		};

		template <unsigned D, typename NumericT>
		std::ostream &operator<< (std::ostream &out, const Plane<D, NumericT> & p);

		typedef Plane<2, RealT> Plane2;
		typedef Plane<3, RealT> Plane3;
	}
}

#include "Plane.impl.h"

#endif
