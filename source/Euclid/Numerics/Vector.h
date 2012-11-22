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

#include <type_traits>
#include <array>

namespace Euclid
{
	namespace Numerics
	{
		template <dimension E, typename NumericT>
		class Vector;

// MARK: -
// MARK: Vector Math

		/** Vector conversion traits. Default specialization.
		 */
		template <dimension E, typename NumericT>
		class VectorConversionTraits {
		};

		template <typename NumericT>
		class VectorConversionTraits<1, NumericT>{
		public:
			operator NumericT () const;
		};

		/** Vector traits for specific template parameters. Default specialization.
		 */
		template <dimension E, typename NumericT>
		class VectorTraits {
		};

		template <typename NumericT>
		class VectorTraits<2, NumericT>{
		private:
			typedef Vector<2, NumericT> VectorT;

		public:
			/// Returns an orthogonal 2D vector.
			Vector<2, NumericT> normal () const;

			NumericT aspect_ratio () const;
			Vector<2, NumericT> shrink_to_fit_aspect_ratio(NumericT n) const;
			Vector<2, NumericT> shrink_to_fit_aspect_ratio(const Vector<2, NumericT> & other) const;

			Vector<2, NumericT> expand_to_fit_aspect_ratio(NumericT n) const;
			Vector<2, NumericT> expand_to_fit_aspect_ratio(const Vector<2, NumericT> & other) const;
		};

		/// Calculates the surface normal of a triangle given by three points.
		template <typename NumericT>
		Vector<3, NumericT> surface_normal (const Vector<3, NumericT> & a, const Vector<3, NumericT> & b, const Vector<3, NumericT> & c);

		/** 3-space Vector traits specialization.

		 3-space vectors nave a number of special traits. This allows the calculation of cross products and angles.
		 */
		template <typename NumericT>
		class VectorTraits<3, NumericT>{
		private:
			typedef Vector<3, NumericT> VectorT;

		public:
			/// Calculate the cross product of this vector with another.
			Vector<3, NumericT> cross (const Vector<3, NumericT> & other) const;

			/// Assuming you have two unit vectors, you can compute the surface normal using this method.
			Vector<3, NumericT> normal (const Vector<3, NumericT> & other) const;

			/// Given three points, you can calculate the surface normal.
			/// @sa surface_normal
			Vector<3, NumericT> normal (const Vector<3, NumericT> & a, const Vector<3, NumericT> & b) const;
		};

		/// 4-space cross product.
		template <typename NumericT>
		Vector<4, NumericT> cross_product (const Vector<4, NumericT> & u, const Vector<4, NumericT> & v,
		                                   const Vector<4, NumericT> & w);

		/** 4-space vector traits specialization.

		 4-space vectors have a number of special traits. This allows the calculation of the 4-space cross product.

		 @sa cross_product
		 */
		template <typename NumericT>
		class VectorTraits<4, NumericT>{
		public:
			/// Calculate the 4-space cross product.
			Vector<4, NumericT> cross (const Vector<4, NumericT> & v, const Vector<4, NumericT> & w);
		};

// MARK: -
// MARK: Vector

		/** A fixed-size numeric vector.

		 This class provides a flexible interface to an n-component vector of the specified type. There are numerous constructors and methods to make working
		 with vectors easy.

		 There are two kinds of comparisons that Vector supports: Geometric comparisons and numeric comparisons. It is important to understand the difference
		 when using the two.

		 Geometric comparisons use the functions less_than(), less_than_or_equal(), greater_than(), greater_than_or_equal() to compare the geometric differences between
		 two vectors.

		 @image html "Vector Comparisons.png"

		 Given a 2-space point <tt>pt</tt> at the origin, the diagram above shows when a relative point would return true with the given function.

		 Numeric comparisons use the functions operator<(), operator<=(), operator>(), operator>=() to compare the numeric position of two vectors. This
		 operation is actually very simple when you consider a vector in this case to be just a compond number. For example, the coordinate <tt><3, 5></tt> can
		 be though of as the number 35. The numeric comparisons essentially just compare vectors in this way, for example <tt><3, 5></tt> is less than
		 <tt><4, 2></tt> because <tt>35 < 42</tt>. However, this comparison also works in the general sense: <tt><55, 29></tt> is bigger than <tt><4, 50></tt>.

		 The purpose of this comparison is ordering. It is possible to order a sequence of points in size. Thus, it is possible to store points in containers
		 that rely on strict ordering behaviour such as <tt>std::set</tt> and <tt>std::map</tt>.
		 */
		template <dimension E, typename NumericT = RealT>
		class Vector : public std::array<NumericT, E>, public VectorTraits<E, NumericT>, public VectorConversionTraits<E, NumericT> {
			static_assert(std::is_arithmetic<NumericT>::value, "Vector only supports numeric data-types!");

		public:
			/// The type of the vector elements.
			typedef typename RealType<NumericT>::RealT RealT;

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

			/// Check if the value of this instance is zero.
			bool is_zero () const {
				for (auto item : *this)
					if (!Numerics::is_zero(item)) return false;

				return true;
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
			template <typename OtherT>
			bool less_than (const OtherT & other) const;

			/// Geometric comparison.
			/// @returns true if all components are numerically greater than the others.
			template <typename OtherT>
			bool greater_than (const OtherT & other) const;

			/// Geometric comparison.
			/// @returns true if all components are numerically lesser than or equal to the others.
			template <typename OtherT>
			bool less_than_or_equal (const OtherT & other) const;

			/// Geometric comparison.
			/// @returns true if all components are numerically greater than or equal to the others.
			template <typename OtherT>
			bool greater_than_or_equal (const OtherT & other) const;

			/// Return the length of the vector squared.
			/// This method avoids calculating the square root, therefore is faster when you only need to compare the relative lengths of vectors.
			NumericT length2 () const
			{
				return this->dot(*this);
			}

			/// Return the length of the vector.
			RealT length () const
			{
				return Number<RealT>::sqrt(this->length2());
			}

			/// Calculate the dot product of two vectors.
			template <typename OtherT>
			NumericT dot (const OtherT & other) const;

			/// Calculates the angle between this vector and another.
			template <typename OtherT>
			NumericT angle_between (const OtherT & other) const;

			/// Calculate the sum of all components of the vector.
			NumericT sum () const;
			/// Calculate the product of all components of the vector.
			NumericT product () const;

			/// Reflect a vector around a given normal.
			/// Be aware that the vector returned still points in the same direction. You generally want to reverse this if you are bouncing a ball against a
			/// wall.
			Vector reflect (const Vector & normal) const
			{
				return (*this) - (normal * (2.0 * this->dot(normal)));
			}

			/// Distribute an index into a given space.
			/// For example, when considering a size vector <tt><10, 15></tt>, we have defined a space which is 10 units wide and 15 units high. Therefore, an
			/// index in the range 0 to 9 will be in the first row, and 10 to 19 will be in the second row.
			/// @sa index
			Vector distribute (NumericT index) const;

			/// Given a size vector (this) and a coordinate, return an index.
			/// @sa distribute
			NumericT index (const Vector & coord) const;

			/// Clamp a specific component of the vector between given values.
			/// @sa Number::clamp
			Vector & clamp (const unsigned i, const NumericT & min = 0.0, const NumericT & max = 1.0);
			/// Clamp all components of the vector between given values.
			/// @sa Number::clamp
			Vector & clamp (const NumericT & min = 0.0, const NumericT & max = 1.0);

			/// Set the vector components to the absolute value.
			Vector & abs ();
			/// Set the vector components to the floor value.
			Vector & floor ();
			/// Set the vector components to the ceil value.
			Vector & ceil ();
			/// Set the vector components to the fractional value.
			/// For example, <tt><5.2, 4.89></tt> becomes <tt><0.2, 0.89></tt>
			Vector & frac ();

			/// Normalize the vector to the given length. Defaults to 1.
			Vector & normalize (const NumericT & to_length = 1);

			/// Return a copy of the vector that is normalized
			Vector normalized_vector (const NumericT & to_length = 1) const;

			/// Constrain this vector with another, choosing the smaller or larger value.
			/// Compare each component of this and another vector. If limits[i] is -1, we choose the smaller of the two values. If limits[i] is +1, we choose
			/// the larger of the two values. We finally multiple the result by the absolute value of limits[i].
			template <typename OtherT>
			void constrain (const Vector<E, NumericT> & with, const OtherT & limits);

			/// Calculate the negated vector in-place.
			Vector & negate ();
			/// Calculate the negated vector and return it as a copy.
			Vector operator- () const;
			/// Calculate the inverse vector in-place.
			Vector & inverse ();
			/// Calculate the inverse vector and return it as a copy.
			Vector operator! () const;

			/// Linearly interpolate between this and another vector.
			Vector lerp (const NumericT & t, Vector other) const;

			/// Increment this vector within the space defined by max.
			/// @sa index
			bool increment (const Vector & max);

			/// Pack the vector into a single variable.
			template <typename PackedT>
			void pack (unsigned bits, PackedT & p) const;

			/// Unpack elements of a vector from a single variable
			template <typename PackedT>
			void unpack (unsigned bits, const PackedT & p);

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
// MARK: Binary Operators

#define OPERATOR(OP, OPE) \
	template <dimension E, typename NumericT, typename AnyT> \
	inline Vector<E, NumericT> operator OP (const Vector<E, NumericT> & lhs, const AnyT & n) \
	{ \
		Vector<E, NumericT> tmp(lhs); \
		return tmp OPE n; \
	}

		OPERATOR(+, +=)
		OPERATOR(-, -=)
		OPERATOR(*, *=)
		OPERATOR(/, /=)
		OPERATOR(%, %=)
		OPERATOR(&, &=)
		OPERATOR(|, |=)

#undef OPERATOR

// MARK: -
// MARK: Unary Operators

#define OPERATOR(OP) \
	template <dimension E, typename NumericT, typename OtherNumericT> \
	Vector<E, NumericT> & operator OP (Vector<E, NumericT> & lhs, const Vector<E, OtherNumericT> & n) \
	{ \
		for (dimension i = 0; i < E; ++i) \
			lhs[i] OP n[i]; \
		return lhs; \
	}

		OPERATOR(+=)
		OPERATOR(-=)
		OPERATOR(*=)
		OPERATOR(/=)
		OPERATOR(&=)
		OPERATOR(|=)

		template <dimension E, typename NumericT, typename OtherNumericT>
		Vector<E, NumericT> & operator%= (Vector<E, NumericT> & lhs, const Vector<E, OtherNumericT> & n)
		{
			for (dimension i = 0; i < E; ++i)
				lhs[i] = Number<NumericT>::mod(lhs[i], n[i]);

			return lhs;
		}

#undef OPERATOR

#define OPERATOR(OP) \
	template <dimension E, typename NumericT, typename OtherNumericT> \
	Vector<E, NumericT> & operator OP (Vector<E, NumericT> & lhs, const OtherNumericT & n) \
	{ \
		for (dimension i = 0; i < E; ++i) \
			lhs[i] OP n; \
		return lhs; \
	}

		OPERATOR(+=)
		OPERATOR(-=)
		OPERATOR(*=)
		OPERATOR(/=)
		OPERATOR(&=)
		OPERATOR(|=)

		template <dimension E, typename NumericT, typename OtherNumericT>
		Vector<E, NumericT> & operator%= (Vector<E, NumericT> & lhs, const OtherNumericT & n)
		{
			for (dimension i = 0; i < E; ++i)
				lhs[i] = Number<NumericT>::mod(lhs[i], n);

			return lhs;
		}

#undef OPERATOR

// MARK: -
// MARK: IO Operators

		/// Write a vector to an std::ostream
		template <dimension E, typename NumericT>
		std::ostream & operator<<(std::ostream & out, const Vector<E, NumericT> & vec)
		{
			for (dimension i = 0; i < E; ++i) {
				// We use this helper to ensure that char and unsigned char are printed correctly.
				out << (typename NumericType<NumericT>::NumericT)(vec[i]) << ((i + 1) != E ? " " : "");
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

#include "Vector.impl.h"

#endif
