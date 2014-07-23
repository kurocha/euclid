//
//  Matrix.Projections.h
//  Euclid
//
//  Created by Samuel Williams on 27/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_MATRIX_PROJECTIONS_H
#define _EUCLID_NUMERICS_MATRIX_PROJECTIONS_H

#include "Matrix.hpp"

namespace Euclid {
	namespace Numerics {
		template <typename NumericT>
		Matrix<4, 4, NumericT> perspective_projection_matrix (const NumericT & field_of_view, const NumericT & aspect_ratio, const NumericT & near, const NumericT & far) {
			NumericT f = 1.0 / std::tan(field_of_view * 0.5);
			NumericT n = 1.0 / (near - far);

			Matrix<4, 4, NumericT> result(ZERO);

			result[0] = f / aspect_ratio;
			result[5] = f;
			result[10] = (far + near) * n;
			result[11] = -1.0;
			result[14] = (2 * far * near) * n;

			return result;
		}

		template <typename NumericT>
		Matrix<4, 4, NumericT> orthographic_projection_matrix (const Vector<3, NumericT> & translation, const Vector<3, NumericT> & size) {
			Matrix<4, 4, NumericT> result(ZERO);

			result[0] = 2.0 / size[X];
			result[5] = 2.0 / size[Y];
			result[10] = -2.0 / size[Z];

			result[12] = -translation[X];
			result[13] = -translation[Y];
			result[14] = -translation[Z];
			result[15] = 1.0;

			return result;
		}
	}
}

#endif
