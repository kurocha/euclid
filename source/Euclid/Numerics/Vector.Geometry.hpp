//
//  Vector.Geometry.h
//  Euclid
//
//  Created by Samuel Williams on 25/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_VECTOR_GEOMETRY_H
#define _EUCLID_NUMERICS_VECTOR_GEOMETRY_H

#include "Vector.hpp"

namespace Euclid {
	namespace Numerics {
		/// The 3-dimentional cross product:
		template <typename NumericT>
		Vector<3, NumericT> cross_product (const Vector<3, NumericT> & u, const Vector<3, NumericT> & v) 
		{
			Vector<3, NumericT> result;

			result[0] = u[1] * v[2] - u[2] * v[1];
			result[1] = u[2] * v[0] - u[0] * v[2];
			result[2] = u[0] * v[1] - u[1] * v[0];

			return result;
		}

		/// The 4-dimentional cross product:
		template <typename NumericT>
		Vector<4, NumericT> cross_product (const Vector<4, NumericT> & u, const Vector<4, NumericT> & v, const Vector<4, NumericT> & w)
		{
			Vector<4, NumericT> result;

			// intermediate values
			NumericT a, b, c, d, e, f;

			// calculate intermediate values.
			a = (v[0] * w[1]) - (v[1] * w[0]);
			b = (v[0] * w[2]) - (v[2] * w[0]);
			c = (v[0] * w[3]) - (v[3] * w[0]);
			d = (v[1] * w[2]) - (v[2] * w[1]);
			e = (v[1] * w[3]) - (v[3] * w[1]);
			f = (v[2] * w[3]) - (v[3] * w[2]);

			// calculate the result-vector components.
			result[0] =  (u[1] * f) - (u[2] * e) + (u[3] * d);
			result[1] = -(u[0] * f) + (u[2] * c) - (u[3] * b);
			result[2] =  (u[0] * e) - (u[1] * c) + (u[3] * a);
			result[3] = -(u[0] * d) + (u[1] * b) - (u[2] * a);

			return result;
		}

		/// Calculates the surface normal of a triangle given by three points.
		template <typename NumericT>
		Vector<3, NumericT> surface_normal (const Vector<3, NumericT> & a, const Vector<3, NumericT> & b, const Vector<3, NumericT> & c)
		{
			Vector<3, NumericT> a1 = b - a;
			Vector<3, NumericT> b1 = c - b;

			return cross_product(a1, b1).normalize();
		}

		// Calculate a clockwise normal to the 2d vector.
		template <typename NumericT>
		Vector<2, NumericT> normal(const Vector<2, NumericT> & direction)
		{
			return Vector<2, NumericT>(direction[Y], -direction[X]);
		}
	}
}

#endif
