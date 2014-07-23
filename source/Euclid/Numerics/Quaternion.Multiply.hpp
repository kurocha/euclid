//
//  Quaternion.Multiply.h
//  Euclid
//
//  Created by Samuel Williams on 27/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_QUATERNION_MULTIPLY_H
#define _EUCLID_NUMERICS_QUATERNION_MULTIPLY_H

#include "Quaternion.hpp"

namespace Euclid {
	namespace Numerics {

		template <typename NumericT>
		Quaternion<NumericT> multiply (const Quaternion<NumericT> & q1, const Quaternion<NumericT> & q2)
		{
			Quaternion<NumericT> result;

			result[X] = (q1[W]*q2[X] + q1[X]*q2[W] + q1[Y]*q2[Z]) - q1[Z]*q2[Y];
			result[Y] = (q1[W]*q2[Y] + q1[Y]*q2[W] + q1[Z]*q2[X]) - q1[X]*q2[Z];
			result[Z] = (q1[W]*q2[Z] + q1[X]*q2[Y] + q1[Z]*q2[W]) - q1[Y]*q2[X];
			result[W] =  q1[W]*q2[W] - q1[X]*q2[X] - q1[Y]*q2[Y]  - q1[Z]*q2[Z];

			return result;
		}

		template <typename NumericT>
		Quaternion<NumericT> operator* (const Quaternion<NumericT> & q1, const Quaternion<NumericT> & q2) {
			return multiply(q1, q2);
		}

		template <typename NumericT>
		Quaternion<NumericT> & operator*= (Quaternion<NumericT> & q1, const Quaternion<NumericT> & q2) {
			return (q1 = multiply(q1, q2));
		}
	}
}

#endif
