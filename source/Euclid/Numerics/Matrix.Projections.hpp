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

// Interesting math for different projections can be found here:
// https://github.com/g-truc/glm/blob/f48fe286ad88f9ffd5c5e9f0d95a6cd1107ac40b/glm/gtc/matrix_transform.inl

namespace Euclid {
	namespace Numerics {
		/// Accepts any numeric data-type but considers field_of_view to be radians. This is a left hand implementation with the clip box from 0..1
		template <typename T>
		Matrix<4, 4, T> perspective_projection_matrix (const T & field_of_view, const T & aspect_ratio, const T & near, const T & far) {
			//assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

			Matrix<4, 4, T> result(ZERO);

			const T tanHalfFovy = std::tan(field_of_view / static_cast<T>(2));

			result[0] = static_cast<T>(1) / (aspect_ratio * tanHalfFovy);
			result[5] = static_cast<T>(1) / (tanHalfFovy);
			result[11] = static_cast<T>(1);

			result[10] = far / (far - near);
			result[14] = -(far * near) / (far - near);

			return result;
		}

		template <typename NumericT>
		Matrix<4, 4, NumericT> orthographic_projection_matrix (const Vector<3, NumericT> & translation, const Vector<3, NumericT> & size) {
			Matrix<4, 4, NumericT> result(ZERO);

			result[0] = 2.0 / size[X];
			result[5] = 2.0 / size[Y];
			result[10] = 1.0 / size[Z];

			result[12] = -translation[X];
			result[13] = -translation[Y];
			result[14] = -translation[Z];
			result[15] = 1.0;

			return result;
		}
	}
}

#endif
