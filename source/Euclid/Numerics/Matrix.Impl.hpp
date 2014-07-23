//
//  Numerics/Matrix.hpp
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 21/07/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_MATRIX_H
#error This header should not be included manually. Include Matrix.h instead.
#else

#include "Quaternion.hpp"

namespace Euclid
{
	namespace Numerics
	{
		template <dimension R, dimension C, typename NumericT> template <typename QuaternionNumericT>
		Matrix<R, C, NumericT>::Matrix (const Quaternion<QuaternionNumericT> & rotation) : Matrix(IDENTITY)
		{
			static_assert(R >= 3 && C >= 3, "Matrix must be at least 3x3 to contain rotation!");
			assert(rotation.length().equivalent(1) && "Quaternion magnitude must be 1");

			auto x = rotation[X], y = rotation[Y], z = rotation[Z], w = rotation[W];

			at(0, 0) = 1.0 - 2.0 * (y*y + z*z);
			at(0, 1) =       2.0 * (x*y - w*z);
			at(0, 2) =       2.0 * (x*z + w*y);

			at(1, 0) =       2.0 * (x*y + w*z);
			at(1, 1) = 1.0 - 2.0 * (x*x + z*z);
			at(1, 2) =       2.0 * (y*z - w*x);

			at(2, 0) =       2.0 * (x*z - w*y);
			at(2, 1) =       2.0 * (y*z + w*x);
			at(2, 2) = 1.0 - 2.0 * (x*x + y*y);
		}
	}
}

#endif
