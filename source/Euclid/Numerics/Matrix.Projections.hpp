//
//  Matrix.Projections.h
//  Euclid
//
//  Created by Samuel Williams on 27/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#pragma once

#include "Matrix.hpp"

// Interesting math for different projections can be found here:
// https://github.com/g-truc/glm/blob/f48fe286ad88f9ffd5c5e9f0d95a6cd1107ac40b/glm/gtc/matrix_transform.inl

namespace Euclid {
	namespace Numerics {
		/// Accepts any numeric data-type but considers field_of_view to be radians. This is a left hand implementation with the clip box from 0..1
		template <typename NumericT>
		Matrix<4, 4, NumericT> perspective_projection_matrix (const Radians<NumericT> & field_of_view, const NumericT & aspect_ratio, const NumericT & near, const NumericT & far) {
			//assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

			Matrix<4, 4, NumericT> result(ZERO);

			const NumericT t = (field_of_view / static_cast<NumericT>(2)).tan();

			result[0] = static_cast<NumericT>(1) / (aspect_ratio * t);
			result[5] = static_cast<NumericT>(1) / t;
			result[11] = - static_cast<NumericT>(1);

			result[10] = far / (near - far);
			result[14] = -(far * near) / (far - near);

			return result;
		}

		template <typename NumericT>
		Matrix<4, 4, NumericT> orthographic_projection_matrix (const Vector<3, NumericT> & min, const Vector<3, NumericT> max) {
			Matrix<4, 4, NumericT> result(ZERO);
			
			auto size = max - min;

			result[0] = static_cast<NumericT>(2) / size[X];
			result[5] = static_cast<NumericT>(2) / size[Y];
			result[10] = static_cast<NumericT>(1) / size[Z];

			result[12] = - (min[X] + max[X]) / size[X];
			result[13] = - (min[Y] + max[Y]) / size[Y];
			result[14] = - min[Z] / size[Z];
			result[15] = static_cast<NumericT>(1);

			return result;
		}
	}
}
