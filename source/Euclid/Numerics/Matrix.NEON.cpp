//
//  Matrix.NEON.c
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 14/08/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#include "Matrix.NEON.hpp"

#ifdef __arm__

#include <arm/arch.h>
#ifdef _ARM_ARCH_7

namespace Euclid {
	namespace Numerics {
		// This is an optimised specialization for ARMv7 NEON:
		void multiply(Matrix<4, 4, float> & result, const Matrix<4, 4, float> & left, const Matrix<4, 4, float> & right) {
			asm volatile (
						  "vldmia      %1, {q8-q11}\n"     // load 16 elements of matrix left
						  "vldmia      %2, {q0-q3}\n"      // load 16 elements of matrix right

						  "vmul.f32    q12, q8, d0[0]\n"   // rslt col0  = (mat0 col0) * (mat1 col0 elt0)
						  "vmul.f32    q13, q8, d2[0]\n"   // rslt col1  = (mat0 col0) * (mat1 col1 elt0)
						  "vmul.f32    q14, q8, d4[0]\n"   // rslt col2  = (mat0 col0) * (mat1 col2 elt0)
						  "vmul.f32    q15, q8, d6[0]\n"   // rslt col3  = (mat0 col0) * (mat1 col3 elt0)

						  "vmla.f32    q12, q9, d0[1]\n"   // rslt col0 += (mat0 col1) * (mat1 col0 elt1)
						  "vmla.f32    q13, q9, d2[1]\n"   // rslt col1 += (mat0 col1) * (mat1 col1 elt1)
						  "vmla.f32    q14, q9, d4[1]\n"   // rslt col2 += (mat0 col1) * (mat1 col2 elt1)
						  "vmla.f32    q15, q9, d6[1]\n"   // rslt col3 += (mat0 col1) * (mat1 col3 elt1)

						  "vmla.f32    q12, q10, d1[0]\n"  // rslt col0 += (mat0 col2) * (mat1 col0 elt2)
						  "vmla.f32    q13, q10, d3[0]\n"  // rslt col1 += (mat0 col2) * (mat1 col1 elt2)
						  "vmla.f32    q14, q10, d5[0]\n"  // rslt col2 += (mat0 col2) * (mat1 col2 elt2)
						  "vmla.f32    q15, q10, d7[0]\n"  // rslt col3 += (mat0 col2) * (mat1 col2 elt2)

						  "vmla.f32    q12, q11, d1[1]\n"  // rslt col0 += (mat0 col3) * (mat1 col0 elt3)
						  "vmla.f32    q13, q11, d3[1]\n"  // rslt col1 += (mat0 col3) * (mat1 col1 elt3)
						  "vmla.f32    q14, q11, d5[1]\n"  // rslt col2 += (mat0 col3) * (mat1 col2 elt3)
						  "vmla.f32    q15, q11, d7[1]\n"  // rslt col3 += (mat0 col3) * (mat1 col3 elt3)

						  // output = result registers
						  "vstmia      %0, {q12-q15}\n"

						  // no output
						  :
						  // input - note *value* of pointer doesn't change
						  : "r" (result.data()), "r" (left.data()), "r" (right.data())
						  : "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15" //clobber
						  );
		}

		void multiply(Vector<4, float> & result, const Matrix<4, 4, float> & left, const Vector<4, float> & right)
		{
			asm volatile (
						  // Store matrix & vector leaving room at top of registers for result (q0)
						  "vldmia      %1, {q1-q4}\n"      // q1-q4 = m
						  "vldmia      %2, {q5}\n"         // q0 = v

						  "vmul.f32    q0, q1, d10[0]\n"
						  "vmla.f32    q0, q2, d10[1]\n"
						  "vmla.f32    q0, q3, d11[0]\n"
						  "vmla.f32    q0, q4, d11[1]\n"

						  "vstmia      %0, {q0}"           // output = result registers

						  : // no output
						  : "r" (result.data()), "r" (left.data()), "r" (right.data()) // input - note *value* of pointer doesn't change
						  : "memory", "q0", "q1", "q2", "q3", "q4", "q5" //clobber
						  );
		}
	}
}

#endif

#endif
