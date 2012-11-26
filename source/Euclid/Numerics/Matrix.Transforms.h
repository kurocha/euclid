//
//  Matrix.Transforms.h
//  Euclid
//
//  Created by Samuel Williams on 24/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_MATRIX_TRANSFORMS_H
#define _EUCLID_NUMERICS_MATRIX_TRANSFORMS_H

#include "Matrix.h"
#include "Transforms.h"

namespace Euclid {
	namespace Numerics {
		template <dimension N, typename NumericT>
		Matrix<N, N, NumericT> & operator<< (Matrix<N, N, NumericT> & left, const Translation<N, NumericT> & translation) {
			if (!translation.identity()) {
				Matrix<N, N, NumericT> transform(IDENTITY);

				// Copy the vector along bottom row
				for (dimension i = 0; i < N; i += 1)
					transform.at(i, N-1) = translation.offset[i];

				left = left * transform;
			}

			return left;
		}
	}
}

#endif
