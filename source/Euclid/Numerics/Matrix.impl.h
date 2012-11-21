//
//  Numerics/Matrix.impl.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 21/07/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_MATRIX_H
#error This header should not be included manually. Include Matrix.h instead.
#else

#include <iostream>
#include <iomanip>

#include "Matrix.NEON.h"
#include "Matrix.SSE.h"

namespace Euclid
{
	namespace Numerics
	{
		// If we increase row by 1, the offset will increase by sz (number of elements per row i.e. number of columns)
		// If we increase col by 1, the offset will increase by 1
		inline std::size_t row_major_offset(std::size_t row, std::size_t col, std::size_t sz)
		{
			return col + row * sz;
		}

		// If we increase col by 1, the offset will increase by sz (number of elements per column i.e. number of rows)
		// If we increase row by 1, the offset will increase by 1
		inline std::size_t column_major_offset(std::size_t row, std::size_t col, std::size_t sz)
		{
			return row + col * sz;
		}

// MARK: -
// MARK: Matrix Multiplication

		/*

						  | |1|
                          | |2|
					  C = 4 |3|
						  | |4|
			    - C = 4 -
		      | |1 2 3 4|   | |
		  R = 4 |5 6 7 8|   |R|
		      | |9 8 7 6|   | |
		      | |5 4 3 2|   | |

		 */

		template <dimension R, dimension C, typename NumericT>
		void multiply (Vector<R, NumericT> & result, const Matrix<R, C, NumericT> & left, const Vector<C, NumericT> & right)
		{
			for (dimension r = 0; r < R; ++r)
				for (dimension c = 0; c < C; ++c)
					result[r] += right[c] * left.at(r, c);
		}

		/*
						  top
							  -  C = 5  -
							| |1 2 3 4 5|
							| |6 7 8 9 8|
						T = 4 |7 6 5 4 3|
		     left           | |2 1 0 1 2|
				  - T = 4 -
		        | |1 2 3 4|   |         |
		    R = 3 |5 6 7 8|   |  R x C  |
		        | |9 8 7 6|   |         |
		                          result
		 
		 */
		
		template <dimension R, dimension C, dimension T, typename NumericT>
		void multiply (Matrix<R, C, NumericT> & result, const Matrix<R, T, NumericT> & left, const Matrix<T, C, NumericT> & top)
		{
			for (dimension r = 0; r < R; ++r) {
				for (dimension c = 0; c < C; ++c) {
					NumericT & value = result.at(r, c);
					
					for (dimension t = 0; t < T; t++)
						value += left.at(r, t) * top.at(t, c);
				}
			}
		}

// MARK: -
// MARK: Matrix Inverse

		template <typename NumericT>
		void invert4x4Matrix (const NumericT * mat, NumericT * dst)
		{
			// Temp array for pairs
			NumericT tmp[12];
			// Array of transpose source matrix
			NumericT src[16];
			// Determinant
			NumericT det;

			// transpose matrix
			for (int i = 0; i < 4; i++) {
				src[i]        = mat[i*4];
				src[i + 4]    = mat[i*4 + 1];
				src[i + 8]    = mat[i*4 + 2];
				src[i + 12]   = mat[i*4 + 3];
			}

			// calculate pairs for first 8 elements (cofactors)
			tmp[0]  = src[10] * src[15];
			tmp[1]  = src[11] * src[14];
			tmp[2]  = src[9]  * src[15];
			tmp[3]  = src[11] * src[13];
			tmp[4]  = src[9]  * src[14];
			tmp[5]  = src[10] * src[13];
			tmp[6]  = src[8]  * src[15];
			tmp[7]  = src[11] * src[12];
			tmp[8]  = src[8]  * src[14];
			tmp[9]  = src[10] * src[12];
			tmp[10] = src[8]  * src[13];
			tmp[11] = src[9]  * src[12];

			// calculate first 8 elements (cofactors)
			dst[0]  = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
			dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
			dst[1]  = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
			dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
			dst[2]  = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
			dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
			dst[3]  = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
			dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
			dst[4]  = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
			dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
			dst[5]  = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
			dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
			dst[6]  = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
			dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
			dst[7]  = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
			dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

			// calculate pairs for second 8 elements (cofactors)
			tmp[0]  = src[2]*src[7];
			tmp[1]  = src[3]*src[6];
			tmp[2]  = src[1]*src[7];
			tmp[3]  = src[3]*src[5];
			tmp[4]  = src[1]*src[6];
			tmp[5]  = src[2]*src[5];
			tmp[6]  = src[0]*src[7];
			tmp[7]  = src[3]*src[4];
			tmp[8]  = src[0]*src[6];
			tmp[9]  = src[2]*src[4];
			tmp[10] = src[0]*src[5];
			tmp[11] = src[1]*src[4];

			// calculate second 8 elements (cofactors)
			dst[8]  = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
			dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
			dst[9]  = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
			dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
			dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
			dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
			dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
			dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
			dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
			dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
			dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
			dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
			dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
			dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
			dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
			dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

			// calculate determinant
			det = src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];

			// calculate matrix inverse
			det = 1.0/det;

			for (int j = 0; j < 16; j++)
				dst[j] *= det;
		}

		template <typename NumericT>
		Matrix<4, 4, NumericT> MatrixInverseTraits<4, 4, NumericT>::inverse_matrix () const
		{
			const MatrixT * t = static_cast<const MatrixT*>(this);

			Matrix<4, 4, NumericT> result;

			invert4x4Matrix(t->value(), result.value());

			return result;
		}

// MARK: -
// MARK: Matrix Square Implementation

		template <dimension N, typename NumericT> template <dimension K>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::scaling_matrix (const Vector<K, NumericT> & amount)
		{
			static_assert(K <= N, "Vector must be equal or smaller than matrix!");

			Matrix<N, N, NumericT> result;
			result.load_identity();

			// Copy the vector along the diagonal
			for (dimension i = 0; i < K; i += 1) result.at(i, i) = amount[i];

			return result;
		}

		template <dimension N, typename NumericT> template <dimension K>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::translating_matrix (const Vector<K, NumericT> & amount)
		{
			static_assert(K <= N, "Vector must be equal or smaller than matrix!");

			Matrix<N, N, NumericT> result(IDENTITY);

			// Copy the vector along bottom row
			for (dimension i = 0; i < K; i += 1) result.at(i, N-1) = amount[i];

			return result;
		}

		template <dimension N, typename NumericT>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::rotating_matrix (const NumericT & angle, const Vector<3, NumericT> & p)
		{
			static_assert(N == 3 || N == 4, "Matrix rotation is only implemented for size 3x3 or 4x4");

			Matrix<N, N, NumericT> result(IDENTITY);

			if (equal_within_tolerance(angle, (NumericT)0.0)) return result;

			NumericT s = Number<NumericT>::sin(angle);
			NumericT c = Number<NumericT>::cos(angle);
			NumericT a = 1.0 - c;

			Vector<3, NumericT> sp = p * s;

			result.at(0, 0) = p[X] * p[X] * a + c;
			result.at(0, 1) = p[X] * p[Y] * a - sp[Z];
			result.at(0, 2) = p[X] * p[Z] * a + sp[Y];

			result.at(1, 0) = p[Y] * p[X] * a + sp[Z];
			result.at(1, 1) = p[Y] * p[Y] * a + c;
			result.at(1, 2) = p[Y] * p[Z] * a - sp[X];

			result.at(2, 0) = p[Z] * p[X] * a - sp[Y];
			result.at(2, 1) = p[Z] * p[Y] * a + sp[X];
			result.at(2, 2) = p[Z] * p[Z] * a + c;

			return result;
		}

		template <dimension N, typename NumericT>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::rotating_matrix (const NumericT & angle, const Vector<3, NumericT> & normal, const Vector<3, NumericT> & pt)
		{
			Matrix<N, N, NumericT> t, rotation = Matrix<N, N, NumericT>::rotating_matrix(angle, normal);

			bool at_origin = pt.is_zero();

			if (angle != 0) {
				if (!at_origin) {
					t = Matrix<N, N, NumericT>::translating_matrix(-pt);
					t << rotation;
					t << Matrix<N, N, NumericT>::translating_matrix(pt);
				} else {
					return rotation;
				}
			}

			return t;
		}

		template <dimension N, typename NumericT>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::rotating_matrix (const Vector<3, NumericT> & from, const Vector<3, NumericT> & to, const Vector<3, NumericT> & normal)
		{
			NumericT angle = to.angle_between(from);

			if (equal_within_tolerance(angle, (NumericT)0.0)) {
				return IDENTITY;
			} else if (Math::abs(angle) == R180) {
				return Matrix<N, N, NumericT>::rotating_matrix(angle, from.cross(normal).normalize());
			} else {
				return Matrix<N, N, NumericT>::rotating_matrix(angle, from.cross(to).normalize());
			}
		}

		template <dimension N, typename NumericT>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::rotating_matrix_around_x (const NumericT & radians)
		{
			static_assert(N >= 3, "Matrix must be size 3 or bigger!");

			NumericT ca = Number<NumericT>::cos(radians);
			NumericT sa = Number<NumericT>::sin(radians);

			Matrix<N, N, NumericT> result(IDENTITY);

			if (equal_within_tolerance(radians, 0.0f)) return result;

			result.at(1, 1) = ca;
			result.at(1, 2) = -sa;
			result.at(2, 1) = sa;
			result.at(2, 2) = ca;

			return result;
		}

		template <dimension N, typename NumericT>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::rotating_matrix_around_y (const NumericT & radians)
		{
			static_assert(N >= 3, "Matrix must be size 3 or bigger!");

			NumericT ca = Number<NumericT>::cos(radians);
			NumericT sa = Number<NumericT>::sin(radians);

			Matrix<N, N, NumericT> result(IDENTITY);

			if (equal_within_tolerance(radians, 0.0f)) return result;

			result.at(0, 0) = ca;
			result.at(2, 2) = ca;
			result.at(2, 0) = -sa;
			result.at(0, 2) = sa;

			return result;
		}

		// This rotation can be used on 2D matricies, unlike the above
		template <dimension N, typename NumericT>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::rotating_matrix_around_z (const NumericT & radians)
		{
			static_assert(N >= 2, "Matrix must be size 3 or bigger!");

			NumericT ca = Number<NumericT>::cos(radians);
			NumericT sa = Number<NumericT>::sin(radians);

			Matrix<N, N, NumericT> result(IDENTITY);

			if (equal_within_tolerance(radians, 0.0f)) return result;

			result.at(0, 0) = ca;
			result.at(1, 1) = ca;
			result.at(0, 1) = -sa;
			result.at(1, 0) = sa;

			return result;
		}

		template <dimension N, typename NumericT>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::rotated_matrix (const NumericT & angle, const Vector<3, NumericT> & n)
		{
			MatrixT * t = static_cast<MatrixT*>(this);

			return (*t) * Matrix<N, N, NumericT>::rotating_matrix(angle, n);
		}

		template <dimension N, typename NumericT>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::rotated_matrix (const NumericT & angle, const Vector<3, NumericT> & n,
		                                                                           const Vector<3, NumericT> & p)
		{
			MatrixT * t = static_cast<MatrixT*>(this);
			
			return (*t) * Matrix<N, N, NumericT>::rotating_matrix(angle, n, p);
		}

		template <dimension N, typename NumericT> template <dimension K>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::translated_matrix (const Vector<K, NumericT> & amount)
		{
			MatrixT * t = static_cast<MatrixT*>(this);
			
			return (*t) * Matrix<N, N, NumericT>::translating_matrix(amount);
		}

		template <dimension N, typename NumericT> template <dimension K>
		Matrix<N, N, NumericT> MatrixSquareTraits<N, N, NumericT>::scaled_matrix (const Vector<K, NumericT> & amount)
		{
			MatrixT * t = static_cast<MatrixT*>(this);

			return (*t) * Matrix<N, N, NumericT>::scaling_matrix(amount);
		}

		template <dimension N, typename NumericT>
		Matrix<N, N, NumericT> & MatrixSquareTraits<N, N, NumericT>::transpose ()
		{
			MatrixT * t = static_cast<MatrixT*>(this);

			for (dimension c = 0; c < (N-1); ++c)
				for (dimension r = c+1; r < N; ++r)
					std::swap(t->at(r, c), t->at(c, r));

			return *t;
		}

// MARK: -
// MARK: IO Operators
		template <dimension R, dimension C, typename NumericT>
		inline std::ostream & operator<< (std::ostream & out, const Matrix<R, C, NumericT> & m)
		{
			using namespace std;

			dimension k = 0;
			for (dimension c = 0; c < C; ++c)
				for (dimension r = 0; r < R; ++r) {
					out << setw(10) << setprecision(4) << m.at(r, c);

					if (k % R == (R-1))
						out << endl;
					else
						out << " ";

					k += 1;
				}

			return out;
		}

// MARK: -
// MARK: class Matrix
		template <dimension R, dimension C, typename N>
		Matrix<R, C, N>::Matrix ()
		{
		}

		template <dimension R, dimension C, typename N>
		Matrix<R, C, N>::Matrix (const Identity &)
		{
			load_identity();
		}

		template <dimension R, dimension C, typename N>
		Matrix<R, C, N>::Matrix (const Zero &)
		{
			zero();
		}

		template <dimension R, dimension C, typename N>
		Matrix<R, C, N>::Matrix (const Matrix<R, C, N> & other)
		{
			set(other._data);
		}

		template <dimension R, dimension C, typename N>
		void Matrix<R, C, N>::zero ()
		{
			memset(this->_data, 0, sizeof(this->_data));
		}

		template <dimension R, dimension C, typename N>
		void Matrix<R, C, N>::load_identity (const NumericT & n)
		{
			static_assert(R == C, "Matrix must be square");

			zero();

			if (n == 0) {
				return;
			}

			for (dimension i = 0; i < R; i += 1)
				at(i, i) = n;
		}
	}
}

#endif
