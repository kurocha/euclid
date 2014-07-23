//
//  Quaternion.Interpolate.h
//  Euclid
//
//  Created by Samuel Williams on 27/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_QUATERNION_INTERPOLATE_H
#define _EUCLID_NUMERICS_QUATERNION_INTERPOLATE_H

#include "Quaternion.hpp"

namespace Euclid {
	namespace Numerics {

		/// Spherical linear interpolation is typically used to interpolate between rotations represented in quaternion space.
		template <typename NumericT>
		Vector<4, NumericT> spherical_linear_interpolate (NumericT t, const Vector<4, NumericT> & q0, const Vector<4, NumericT> & q1)
		{
			// Compute the cosine of the angle between the two vectors.
			auto dot = q0.dot(q1);

			const NumericT DOT_THRESHOLD = 0.9995;
			if (dot > DOT_THRESHOLD) {
				// If the inputs are too close for comfort, linearly interpolate
				// and normalize the result.
				return (q0 + (q1 - q0) * t).normalize()
			}

			// theta_0 = angle between input vectors:
			auto theta_0 = dot.acos();

			// theta = angle between v0 and result:
			auto theta = theta_0 * t;

			auto v2 = (v1 - v0 * dot).normalize();

			// { v0, v2 } is now an orthonormal basis.
			return Quaternion(v0*cos(theta) + v2*sin(theta));
		}
		
	}
}

#endif
