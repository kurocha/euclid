//
//  Numerics/Matrix.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 13/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_MATRIX_H
#define _EUCLID_NUMERICS_MATRIX_H

#include "Numerics.h"

#include "Vector.h"

#include <algorithm>

namespace Euclid
{
	namespace Numerics
	{
		template <std::size_t, std::size_t, typename>
		class Matrix;

// MARK: -
// MARK: Matrix Inverse Traits

		template <std::size_t R, std::size_t C, typename NumericT>
		class MatrixInverseTraits {
		};

		template <typename NumericT>
		class MatrixInverseTraits<4, 4, NumericT>{
		private:
			typedef Matrix<4, 4, NumericT> MatrixT;

			inline const NumericT * values () const;

		public:
			Matrix<4, 4, NumericT> inverse_matrix () const;
		};

// MARK: -
// MARK: Matrix Square Traits

		template <dimension R, dimension C, typename NumericT>
		class MatrixSquareTraits {};

		template <dimension N, typename NumericT>
		class MatrixSquareTraits<N, N, NumericT>{
		private:
			typedef Matrix<N, N, NumericT> MatrixT;

		public:
			template <dimension K>
			static MatrixT scaling_matrix (const Vector<K, NumericT> & amount);

			template <dimension K>
			static MatrixT translating_matrix (const Vector<K, NumericT> & amount);

			// Rotations in 3D.
			static MatrixT rotating_matrix (const NumericT & radians, const Vector<3, NumericT> & normal);
			static MatrixT rotating_matrix (const NumericT & radians, const Vector<3, NumericT> & normal, const Vector<3, NumericT> & point);
			static MatrixT rotating_matrix (const Vector<3, NumericT> & from, const Vector<3, NumericT> & to, const Vector<3, NumericT> & normal);

			// Rotations in 2D.
			static MatrixT rotating_matrix_around_x (const NumericT & radians);
			static MatrixT rotating_matrix_around_y (const NumericT & radians);
			/// Also works for Matrix<2,2>
			static MatrixT rotating_matrix_around_z (const NumericT & radians);

			// Convenience Functions
			MatrixT rotated_matrix (const NumericT & radians, const Vector<3, NumericT> & normal);
			MatrixT rotated_matrix (const NumericT & radians, const Vector<3, NumericT> & normal, const Vector<3, NumericT> & point);

			template <dimension K>
			MatrixT scaled_matrix (const Vector<K, NumericT> & amount);

			template <dimension K>
			MatrixT translated_matrix (const Vector<K, NumericT> & amount);

			/// In-place transposition (transpose_matrix)
			MatrixT & transpose ();
		};

// MARK: -
// MARK: Matrix Class

		std::size_t row_major_offset(std::size_t row, std::size_t col, std::size_t sz);
		std::size_t column_major_offset(std::size_t row, std::size_t col, std::size_t sz);

		/** A 2-dimentional set of numbers that can represent useful transformations in n-space.

		Standard mathematical notation is column order, therefore regardless of row-major or column-major memory layout,
		the interface will assume access is done via rows and columns according to this standard notation.
		 */
		template <dimension _R = 4, dimension _C = 4, typename _NumericT = RealT>
		class Matrix : public MatrixSquareTraits<_R, _C, _NumericT>, public MatrixInverseTraits<_R, _C, _NumericT> {
		public:
			static const std::size_t R = _R;
			static const std::size_t C = _C;
			typedef _NumericT NumericT;

		protected:
			_NumericT _data[_R * _C] __attribute__((aligned(16)));

		public:
			// Uninitialized constructor
			Matrix ();
			Matrix (const Zero &);
			Matrix (const Identity &);

			Matrix (const Matrix<R, C, NumericT> & other);

			template <dimension S, dimension T, typename OtherNumericT>
			Matrix (const Matrix<S, T, OtherNumericT> & other) {
				set(other);
			}

			Matrix (const NumericT * data)
			{
				set(data);
			}

			void set (const NumericT * data)
			{
				memcpy(_data, data, sizeof(_data));
			}

			template <typename AnyT>
			void set (const AnyT * data)
			{
				for (dimension i = 0; i < R*C; i++) {
					_data[i] = data[i];
				}
			}

			template <dimension S, dimension T, typename OtherNumericT>
			void set (const Matrix<S, T, OtherNumericT> & other) {
				for (std::size_t s = 0; s < S; s += 1) {
					for (std::size_t t = 0; t < T; t += 1) {
						at(s, t) = other.at(s, t);
					}
				}
			}

			void zero ();
			void load_identity (const NumericT & n = 1);

			std::size_t offset(std::size_t row, std::size_t column) const {
				assert(row < R && column < C);
				
				return column_major_offset(row, column, R);
				//return row_major_offset(row, column, C);
			}

			// Accessors
			const NumericT & at (dimension r, dimension c) const
			{
				return _data[offset(r, c)];
			}

			NumericT & at (dimension r, dimension c)
			{
				return _data[offset(r, c)];
			}
			
			const NumericT & operator[] (dimension i) const
			{
				return _data[i];
			}

			NumericT & operator[] (dimension i)
			{
				return _data[i];
			}

			NumericT * value ()
			{
				return (NumericT*)_data;
			}

			const NumericT * value () const
			{
				return (const NumericT*)_data;
			}

			/// @todo Write get equivalent of set functions for retriving Vector data

			/// Copy a vector into the matix at position r, c
			/// This copies the vector in the direction of the major format,
			/// i.e. in column major format it will appear as a column
			template <std::size_t D>
			void set (const std::size_t & r, const std::size_t & c, const Vector<D, NumericT> & v)
			{
				memcpy(&at(r, c), v.data(), sizeof(NumericT) * D);
			}

			/// Copy a vector into the matrix at position r, c, with element_offset distance between each element.
			/// The purpose of this function is primarily to facilitate copying a vector into a matrix in an order
			/// other than the major.
			/// i.e. set(0, 0, Vec4(...), 4) will set a row in a column major matrix.
			template <std::size_t D>
			void set (const std::size_t & r, const std::size_t & c, const Vector<D, NumericT> & v, std::size_t element_offset)
			{
				std::size_t start_offset = offset(r, c);

				for (std::size_t i = 0; i < D; i += 1) {
					_data[start_offset + element_offset * i] = v[i];
				}
			}

			/// Return a copy of this matrix, transposed.
			Matrix<C, R, NumericT> transposed_matrix () const
			{
				Matrix<C, R, NumericT> result;

				for (dimension c = 0; c < C; ++c)
					for (dimension r = 0; r < R; ++r)
						result.at(c, r) = at(r, c);

				return result;
			}

			/// Load a test patern into the matrix. Used for testing.
			void load_test_pattern ()
			{
				NumericT i = 0;

				for (dimension c = 0; c < C; c += 1)
					for (dimension r = 0; r < R; r += 1)
						at(r, c) = i++;
			}

			/// Check if a matrices components are exactly equal.
			bool operator== (const Matrix & other) const
			{
				for (dimension c = 0; c < C; ++c)
					for (dimension r = 0; r < R; ++r)
						if (at(r, c) != other.at(r, c))
							return false;

				return true;
			}

			bool equivalent(const Matrix & other) const
			{
				for (dimension i = 0; i < R*C; i += 1) {
					if (!Numerics::equivalent(_data[i], other[i])) {
						return false;
					}
				}

				return true;
			}
		};
				
		/// Short-hand notation
		template <dimension R, dimension C, typename NumericT>
		Vector<C, NumericT> operator* (const Matrix<R, C, NumericT> & left, const Vector<R, NumericT> & right)
		{
			Vector<R, NumericT> result(ZERO);
			
			multiply(result, left, right);
			
			return result;
		}
		
		/// Short-hand notation for non-homogeneous vectors
		template <dimension R, dimension C, typename NumericT>
		Vector<C-1, NumericT> operator* (const Matrix<R, C, NumericT> & left, const Vector<R-1, NumericT> & right)
		{
			Vector<C, NumericT> result(ZERO);
			
			multiply(result, left, right << 1);
			
			result /= result[C-1];
			
			return result.reduce();
		}

		/// Short hand for matrix multiplication
		template <dimension R, dimension C, dimension T, typename NumericT>
		Matrix<R, C, NumericT> operator* (const Matrix<R, T, NumericT> & left, const Matrix<T, C, NumericT> & right)
		{
			Matrix<R, C, NumericT> result(ZERO);

			// For column-major matricies, the right hand transform is applied first when result * vector
			multiply(result, left, right);
			
			return result;
		}

		template <dimension R, dimension C, typename NumericT>
		Matrix<R, C, NumericT> & operator*= (Matrix<R, C, NumericT> & transform, const Matrix<R, C, NumericT> & step)
		{
			return (transform = transform * step);
		}

		template <dimension R, dimension C, typename NumericT>
		Matrix<R, C, NumericT> & operator<< (Matrix<R, C, NumericT> & transform, const Matrix<R, C, NumericT> & step)
		{
			// To simplify the order of operations, we reverse the order of multiplcation so that transforms can be supplied in forwards order (well, technically backwards).
			return (transform = step * transform);
		}

// MARK: -
// MARK: Static Matrix Constructors

		/// Convenience type for matrix class
		typedef Matrix<4, 4, RealT> Mat44;
		/// Convenience type for matrix class
		typedef Matrix<3, 3, RealT> Mat33;
		/// Convenience type for matrix class
		typedef Matrix<2, 2, RealT> Mat22;

		/// Convenience constructor for matrix class
		Mat44 rotation (const RealT & radians, const Vec3 & around_normal, const Vec3 & around_point);
		/// Convenience constructor for matrix class
		Mat44 rotation (const RealT & radians, const Vec3 & around_normal);
		/// Convenience constructor for matrix class
		Mat44 rotation (const Vec3 & from_unit_vector, const Vec3 & to_unit_vector, const Vec3 & around_normal);

		template <typename NumericT>
		Matrix<4, 4, NumericT> perspective_matrix (const NumericT & field_of_view, const NumericT & aspect_ratio, const NumericT & near, const NumericT & far) {
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
		Matrix<4, 4, NumericT> orthographic_matrix (const Vector<3, NumericT> & translation, const Vector<3, NumericT> & size) {
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

#include "Matrix.impl.h"

#endif
