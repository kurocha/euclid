//
//  Numerics/Vector.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 19/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_VECTOR_H
#define _EUCLID_NUMERICS_VECTOR_H

#include "Numerics.h"
#include "Number.h"
#include "Interpolate.h"

#include <type_traits>
#include <array>

namespace Euclid
{
	namespace Numerics
	{
// MARK: -
// MARK: Vector

		/** A fixed-size numeric vector.

		 This class provides a flexible interface to an n-component vector of the specified type. There are numerous constructors and methods to make working with vectors easy.

		 There are two kinds of comparisons that Vector supports: Geometric comparisons and numeric comparisons. It is important to understand the difference when using the two.

		 Geometric comparisons use the functions less_than(), less_than_or_equal(), greater_than(), greater_than_or_equal() to compare the geometric differences between two vectors.

		 @image html "Vector Comparisons.png"

		 Given a 2-space point <tt>pt</tt> at the origin, the diagram above shows when a relative point would return true with the given function.

		 Numeric comparisons use the functions operator<(), operator<=(), operator>(), operator>=() to compare the numeric position of two vectors. This operation is actually very simple when you consider a vector in this case to be just a compond number. For example, the coordinate <tt><3, 5></tt> can be though of as the number 35. The numeric comparisons essentially just compare vectors in this way, for example <tt><3, 5></tt> is less than <tt><4, 2></tt> because <tt>35 < 42</tt>. However, this comparison also works in the general sense: <tt><55, 29></tt> is bigger than <tt><4, 50></tt>.

		 The purpose of this comparison is ordering. It is possible to order a sequence of points in size. Thus, it is possible to store points in containers that rely on strict ordering behaviour such as <tt>std::set</tt> and <tt>std::map</tt>.
		 */
		template <dimension E, typename NumericT = RealT>
		class Vector : public std::array<NumericT, E> {
			static_assert(std::is_arithmetic<NumericT>::value, "Vector only supports numeric data-types!");

		public:
			/// The type of the vector elements.
			typedef typename RealTypeTraits<NumericT>::RealT RealT;

			/// Empty constructor. Value of vector is undefined.
			Vector () = default;

			Vector (const NumericT & value)
			{
				this->fill(value);
			}

			Vector (const Vector & other) : std::array<NumericT, E>(other)
			{
			}

			template <dimension F, typename OtherNumericT>
			Vector (const Vector<F, OtherNumericT> & other)
			{
				auto next = std::copy(other.begin(), other.begin() + std::min(F, E), this->begin());
				std::fill(next, this->end(), 0);
			}

			template <typename... TailT>
			Vector (const NumericT & head, const TailT&&... tail) : std::array<NumericT, E>{{head, (NumericT)tail...}}
			{
			}

			Vector (std::initializer_list<NumericT> list)
			{
				auto next = std::copy(list.begin(), list.begin() + std::min(list.size(), E), this->begin());
				std::fill(next, this->end(), 0);
			}
			
			/// Copy count elements of raw data into the vector.
			void set (const NumericT * other, unsigned count = E, unsigned offset = 0) {
				std::copy(other, other+count, this->begin() + offset);
			}

			/// Check for equivalence, which is typically relaxed for floating point numbers:
			bool equivalent(const Vector<E, NumericT> & other) const {
				for (dimension i = 0; i < E; ++i) {
					if (!Numerics::equivalent(this->data()[i], other[i]))
						return false;
				}

				return true;
			}

			/// Geometric comparison.
			/// @returns true if all components are numerically lesser than the others.
			bool less_than (const Vector & other) const
			{
				for (dimension i = 0; i < E; ++i)
					if ((*this)[i] >= other[i])
						return false;

				return true;
			}

			/// Geometric comparison.
			/// @returns true if all components are numerically greater than the others.
			bool greater_than (const Vector & other) const
			{
				for (dimension i = 0; i < E; ++i)
					if ((*this)[i] <= other[i])
						return false;

				return true;
			}

			/// Geometric comparison.
			/// @returns true if all components are numerically lesser than or equal to the others.
			bool less_than_or_equal (const Vector & other) const
			{
				for (dimension i = 0; i < E; ++i)
					if ((*this)[i] > other[i])
						return false;

				return true;
			}
			
			/// Geometric comparison.
			/// @returns true if all components are numerically greater than or equal to the others.
			bool greater_than_or_equal (const Vector & other) const
			{
				for (dimension i = 0; i < E; ++i)
					if ((*this)[i] < other[i])
						return false;

				return true;
			}

			/// Calculate the dot product of two vectors.
			Number<NumericT> dot (const Vector & other) const
			{
				NumericT result = 0;

				for (dimension i = 0; i < E; ++i) {
					result += (*this)[i] * other[i];
				}

				return result;
			}

			/// Return the length of the vector squared.
			/// This method avoids calculating the square root, therefore is faster when you only need to compare the relative lengths of vectors.
			Number<NumericT> length_squared () const
			{
				return this->dot(*this);
			}

			/// Return the length of the vector.
			Number<RealT> length () const
			{
				return this->length_squared().square_root();
			}

			/// Calculates the angle between this vector and another.
			Number<NumericT> angle_between (const Vector & other) const
			{
				NumericT r = this->dot(other) / (this->length() * other.length());

				return number(r).clamp(-1, 1).acos();
			}

			/// Calculate the sum of all components of the vector.
			Number<NumericT> sum () const
			{
				NumericT result = 0;

				for (dimension i = 0; i < E; ++i)
					result += (*this)[i];

				return result;
			}

			/// Calculate the product of all components of the vector.
			Number<NumericT> product () const
			{
				NumericT result = 1;
				
				for (dimension i = 0; i < E; ++i)
					result *= (*this)[i];
				
				return result;
			}

			/// Reflect a vector around a given normal.
			/// Be aware that the vector returned still points in the same direction. You generally want to reverse this if you are bouncing a ball against a
			/// wall.
			Vector reflect (const Vector & normal) const
			{
				return (*this) - (normal * (2.0 * this->dot(normal)));
			}

			/// Distribute an index into a given space.
			/// For example, when considering a size vector <tt><10, 15></tt>, we have defined a space which is 10 units wide and 15 units high. Therefore, an index in the range 0 to 9 will be in the first row, and 10 to 19 will be in the second row.
			/// @sa index
			Vector distribute (NumericT k) const
			{
				Vector r(ZERO);
				
				NumericT m = this->product();

				assert(m);

				for (dimension i = E; i > 0; i -= 1) {
					m /= (*this)[i-1];

					r[i-1] = number(k / m).truncate();

					k = number(k).modulo(m);
				}

				assert(m == 1);

				return r;
			}

			/// Given a size vector (this) and a coordinate, return an index.
			/// @sa distribute
			NumericT index (const Vector & coord) const
			{
				NumericT idx = 0;
				NumericT m = 1;

				//		x(1) + y(sx) + z(sx*sy)
				for (dimension i = 0; i < E; i += 1) {
					idx += coord[i] * m;
					m *= (*this)[i];
				}
				
				return idx;
			}

			/// Clamp all components of the vector between given values.
			Vector clamp (const NumericT & min = 0, const NumericT & max = 1) {
				Vector result;

				for (dimension i = 0; i < E; ++i)
					result[i] = number((*this)[i]).clamp(min, max);

				return result;
			}

			/// Linearly interpolate from (-1...1) between the smallest and largest components of the individual axes.
			Vector constrain(const Vector & b, const Vector<E, RealT> & constraints) {
				const Vector & a = *this;

				Vector result;

				for (std::size_t i = 0; i < E; i += 1) {
					NumericT minimum = a[i], maximum = b[i];

					if (minimum > maximum) std::swap(minimum, maximum);

					result[i] = linear_interpolate(constraints[i], minimum, maximum);
				}

				return result;
			}

			/// Linearly interpolate from (-1...1) between the smallest and largest components of the individual axes.
			Vector constrain(const Vector & b, bool minimum = true) {
				const Vector & a = *this;

				Vector result;

				for (std::size_t i = 0; i < E; i += 1) {
					if (minimum)
						result[i] = std::min(a[i], b[i]);
					else
						result[i] = std::max(a[i], b[i]);
				}

				return result;
			}

			/// Calculate the absolute value of the vector:
			Vector absolute () const {
				Vector result;

				for (dimension i = 0; i < E; ++i)
					result[i] = number((*this)[i]).absolute();

				return result;
			}

			/// Truncate the values of the vector:
			Vector truncate (bool up = false) const {
				Vector result;

				for (dimension i = 0; i < E; ++i)
					result[i] = number((*this)[i]).truncate(up);

				return result;
			}

			Vector fraction() const {
				return *this - truncate();
			}

			/// Normalize the vector to the given length. Defaults to 1.
			Vector normalize (NumericT & length, const NumericT & desired_length = 1) const {
				if (!Numerics::equivalent(length, desired_length)) {
					NumericT factor = desired_length / length;
					
					return (*this) * factor;
				}
				
				return (*this);
			}

			/// Normalize the vector to the given length. Defaults to 1.
			Vector normalize (const NumericT & desired_length = 1) const {
				auto length = this->length();

				if (length.equivalent(0)) return *this;

				return normalize(length, desired_length);
			}

			/// Calculate the negated vector and return it as a copy.
			Vector operator- () const {
				Vector result;

				for (std::size_t i = 0; i < E; i += 1) {
					result[i] = -(*this)[i];
				}

				return result;
			}

			/// Calculate the inverse vector and return it as a copy.
			Vector operator! () const {
				Vector result;

				for (std::size_t i = 0; i < E; i += 1) {
					result[i] = !(*this)[i];
				}

				return result;
			}

			/// Returns a vector with F components, by default one less than the current size.
			template <dimension F = E - 1>
			Vector<F, NumericT> reduce() const {
				static_assert(F <= E, "Cannot reduce size of vector to larger size");

				Vector<F, NumericT> result;

				std::copy(this->begin(), this->begin() + F, result.begin());

				return result;
			}

			template <typename... ArgumentsT>
			Vector<E+sizeof...(ArgumentsT), NumericT> expand(ArgumentsT... arguments) const {
				Vector<E+sizeof...(ArgumentsT), NumericT> result;

				auto next = std::copy(this->begin(), this->end(), result.begin());

				NumericT tail[] = {(NumericT)arguments...};
				std::copy(std::begin(tail), std::end(tail), next);

				return result;
			}

			Vector<E+1, NumericT> operator<<(const NumericT & tail) const {
				Vector<E+1, NumericT> result;

				auto next = std::copy(this->begin(), this->end(), result.begin());
				*next = tail;

				return result;
			}
		};

// MARK: -
// MARK: Static Matrix Constructors

		/// Short hand notation for 1-space vector type.
		typedef Vector<1, RealT> Vec1;
		typedef Vector<1, unsigned> Vec1u;
		typedef Vector<1, int> Vec1i;
		/// Short hand notation for 2-space vector type.
		typedef Vector<2, RealT> Vec2;
		typedef Vector<2, unsigned> Vec2u;
		typedef Vector<2, int> Vec2i;
		typedef Vector<2, bool> Vec2b;
		/// Short hand notation for 3-space vector type.
		typedef Vector<3, RealT> Vec3;
		typedef Vector<3, unsigned> Vec3u;
		typedef Vector<3, int> Vec3i;
		typedef Vector<3, bool> Vec3b;
		/// Short hand notation for 4-space vector type.
		typedef Vector<4, RealT> Vec4;
		typedef Vector<4, unsigned> Vec4u;
		typedef Vector<4, int> Vec4i;
		typedef Vector<4, bool> Vec4b;

		/// Construct a 1-space vector
		template <typename HeadT, typename... TailT>
		constexpr inline Vector<1+sizeof...(TailT), HeadT> vector (const HeadT & head, TailT... tail)
		{
			return {head, (HeadT)tail...};
		}

// MARK: -
// MARK: Operators

#define EUCLID_NUMERICS_VECTOR_OPERATOR(OP, OPE) \
	template <dimension E, typename NumericT, typename AnyT> \
	inline Vector<E, NumericT> operator OP (const Vector<E, NumericT> & lhs, const AnyT & n) \
	{ \
		Vector<E, NumericT> tmp(lhs); \
		return tmp OPE n; \
	}

		EUCLID_NUMERICS_VECTOR_OPERATOR(+, +=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(-, -=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(*, *=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(/, /=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(%, %=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(&, &=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(|, |=)

#undef EUCLID_NUMERICS_VECTOR_OPERATOR

// MARK: -

#define EUCLID_NUMERICS_VECTOR_OPERATOR(OP) \
	template <dimension E, typename NumericT, typename OtherNumericT> \
	Vector<E, NumericT> & operator OP (Vector<E, NumericT> & lhs, const Vector<E, OtherNumericT> & n) \
	{ \
		for (dimension i = 0; i < E; ++i) \
			lhs[i] OP n[i]; \
		return lhs; \
	}

		EUCLID_NUMERICS_VECTOR_OPERATOR(+=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(-=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(*=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(/=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(&=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(|=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(%=)

#undef EUCLID_NUMERICS_VECTOR_OPERATOR

#define EUCLID_NUMERICS_VECTOR_OPERATOR(OP) \
	template <dimension E, typename NumericT, typename OtherNumericT> \
	Vector<E, NumericT> & operator OP (Vector<E, NumericT> & lhs, const OtherNumericT & n) \
	{ \
		for (dimension i = 0; i < E; ++i) \
			lhs[i] OP n; \
		return lhs; \
	}

		EUCLID_NUMERICS_VECTOR_OPERATOR(+=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(-=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(*=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(/=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(&=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(|=)
		EUCLID_NUMERICS_VECTOR_OPERATOR(%=)

#undef EUCLID_NUMERICS_VECTOR_OPERATOR

// MARK: -
// MARK: IO Operators

		/// Write a vector to an std::ostream
		template <dimension E, typename NumericT>
		std::ostream & operator<<(std::ostream & out, const Vector<E, NumericT> & vec)
		{
			for (dimension i = 0; i < E; ++i) {
				// We use this helper to ensure that char and unsigned char are printed correctly.
				out << (typename NumericOutputTraits<NumericT>::NumericT)(vec[i]) << ((i + 1) != E ? " " : "");
			}

			return out;
		}

		/// Read a vector from a std::istream
		template <dimension E, typename NumericT>
		std::istream & operator>> (std::istream & in, Vector<E, NumericT> & vec)
		{
			for (dimension i = 0; i < E; ++i) {
				in >> vec[i];
			}

			return in;
		}

		template <dimension N>
		bool equivalent (const Vector<N, float> a, const Vector<N, float> b)
		{
			return a.equivalent(b);
		}

		template <dimension N>
		bool equivalent (const Vector<N, double> a, const Vector<N, double> b)
		{
			return a.equivalent(b);
		}

// MARK: -

		extern template class Vector<2, RealT>;
		extern template class Vector<3, RealT>;
		extern template class Vector<4, RealT>;
	}
}

#endif
