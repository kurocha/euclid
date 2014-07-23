//
//  Matrix.SSE.cpp
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 16/08/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#include "Matrix.SSE.hpp"

#ifdef __SSE2__

#include <xmmintrin.h>

namespace Euclid {
	namespace Numerics {
		// This is an optimised specialization for SSE2:
		void multiply(Matrix<4, 4, float> & result, const Matrix<4, 4, float> & left, const Matrix<4, 4, float> & right) {
			const float * a = left.data();
			const float * b = right.data();
			float * r = result.data();

			__m128 a_line, b_line, r_line;
			
			for (int i=0; i<16; i+=4) {
				// unroll the first step of the loop to avoid having to initialize r_line to zero
				a_line = _mm_load_ps(a);         // a_line = vec4(column(a, 0))
				b_line = _mm_set1_ps(b[i]);      // b_line = vec4(b[i][0])
				r_line = _mm_mul_ps(a_line, b_line); // r_line = a_line * b_line
				
				for (int j=1; j<4; j++) {
					a_line = _mm_load_ps(&a[j*4]); // a_line = vec4(column(a, j))
					b_line = _mm_set1_ps(b[i+j]);  // b_line = vec4(b[i][j])
					// r_line += a_line * b_line
					r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
				}
				_mm_store_ps(&r[i], r_line);     // r[i] = r_line
			}
		}

		//void multiply(Vector<4, float> & result, const Matrix<4, 4, float> & left, const Vector<4, float> & right) {
		//}
	}
}

#endif
