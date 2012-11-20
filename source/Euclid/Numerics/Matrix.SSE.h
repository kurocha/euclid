//
//  Matrix.SSE.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 16/08/12.
//  Copyright (c) 2012 Orion Transfer Ltd. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_MATRIX_SSE_H
#define _EUCLID_NUMERICS_MATRIX_SSE_H

#include "Matrix.h"

#ifdef __SSE2__

namespace Euclid {
	namespace Numerics {
		// This is an optimised specialization for SSE2:
		void multiply(Matrix<4, 4, float> & result, const Matrix<4, 4, float> & left, const Matrix<4, 4, float> & right);
		//void multiply(Vector<4, float> & result, const Matrix<4, 4, float> & left, const Vector<4, float> & right);
	}
}

#endif

#endif
