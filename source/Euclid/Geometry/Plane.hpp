//
//  Geometry/Plane.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 23/09/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_PLANE_H
#define _EUCLID_GEOMETRY_PLANE_H

#include "Geometry.hpp"
#include "Line.hpp"
#include "Triangle.hpp"

namespace Euclid {
	namespace Geometry {
		template <dimension D, typename NumericT>
		class Plane {
			static_assert(D >= 3, "Planes only exist in 3-dimensional space or higher!");
			
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

			Plane (const NumericT & distance, const VectorT & normal) : _distance(distance), _normal(normal) {}

			Plane (const VectorT & point, const VectorT & normal) : _distance((-normal).dot(point)), _normal(normal) {}

			/// Point is a point on the plain, and direction is the normal
			Plane (const Line<D, NumericT> & line) : Plane(line.point(), line.direction()) {}

			Plane (const Triangle<D, NumericT> & triangle) : Plane(triangle[0], surface_normal(triangle)) {}

			const RealT & distance() const { return _distance; }
			const VectorT & normal() const { return _normal; }

			void set_distance(const RealT & r) { _distance = r; }
			void set_normal(const VectorT & n) { _normal = n; }

			bool is_parallel(const Plane & other) const
			{
				return _normal.equivalent(other._normal) || _normal.equivalent(-other._normal);
			}

			bool intersects_with (const Plane & plane, Line<3, NumericT> & line) const;
			bool intersects_with (const Line<3, NumericT> & line, VectorT & at) const;

			/// Finds the closed point on a plane to another point
			VectorT closest_point (const VectorT & point) const {
				Vec3 at;

				intersects_with(Line<D, NumericT>(point, _normal), at);

				return at;
			}

			Intersection intersects_with (const Sphere<D, NumericT> & sphere) const;

			/// Can be used to test sphere intersection
			NumericT distance_to_point (const VectorT &at) const
			{
				/* Because the normal is normalized, it will always be 1.0, however it is possible to generalize
				 this algorithm for planes of the form ax+by+cz+d */

				return (_normal.dot(at) + _distance) /* / _normal.length()*/;
			}
		};

		template <dimension D, typename NumericT>
		Vector<D, NumericT> surface_normal (const Plane<D, NumericT> & plane) {
			return plane.normal();
		}

		template <dimension D, typename NumericT>
		std::ostream &operator<< (std::ostream &out, const Plane<D, NumericT> & p);

		typedef Plane<3, RealT> Plane3;

		extern template class Plane<3, RealT>;
	}
}

#include "Plane.Impl.hpp"

#endif
