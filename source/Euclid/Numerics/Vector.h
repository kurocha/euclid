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

		/** Vector equality traits for specific template parameters. Default specialization.
		 */
		template <dimension E, typename NumericT>
		class VectorEqualityTraits {
		};

		/** Single floating point vector equality traits.
		 */
		template <dimension E>
		class VectorEqualityTraits<E, float>{
		public:
			/// Check whether a vector is equal to another using the equal_within_tolerance set of functions.
			bool equal_within_tolerance (const Vector<E, float> & other, const unsigned & ulps = DEFAULT_ULPS) const;

			bool equivalent (const Vector<E, float> & other) const;
		};

		/** Double floating point vector equality traits.
		 */
		template <dimension E>
		class VectorEqualityTraits<E, double>{
		public:
			/// Check whether a vector is equal to another using the equal_within_tolerance set of functions.
			bool equal_within_tolerance (const Vector<E, double> & other, const unsigned & ulps = DEFAULT_ULPS) const;

			bool equivalent (const Vector<E, double> & other) const;
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
		class Vector : public VectorTraits<E, NumericT>, public VectorEqualityTraits<E, NumericT>, public VectorConversionTraits<E, NumericT>{
		public:
			typedef NumericT ElementT;

			/// The type of the vector elements.
			typedef typename RealType<NumericT>::RealT NumericRealT;

			/// The actual data.
			NumericT _vector[E];

			/// Empty constructor. Value of vector is undefined.
			Vector () = default;

			// Default copy constructor and assignment operator
			Vector (const Vector & other) {
				this->set(other._vector);
			}

			Vector & operator= (const Vector & other) {
				this->set(other._vector);

				return *this;
			}

			/// Zero constructor. Value is zeroed.
			Vector (const Zero &)
			{
				zero();
			}

			/// Identity constructor. Value is identity.
			Vector (const Identity &, const NumericT & n = 1)
			{
				load_identity(n);
			}

			/// Construct a vector from a different type.
			template <typename OtherNumericT>
			Vector (const Vector<E, OtherNumericT> & other);

			/// Construct a vector with all elements of value x.
			Vector (const NumericT & x);

			/// Construct a 2-space vector.
			Vector (const NumericT & x, const NumericT & y);
			/// Construct a 3-space vector.
			Vector (const NumericT & x, const NumericT & y, const NumericT & z);
			/// Construct a 4-space vector.
			Vector (const NumericT & x, const NumericT & y, const NumericT & z, const NumericT & w);

			/// Construct a vector from raw data.
			Vector (const NumericT * data)
			{
				set(data);
			}

			/// Copy raw data into the vector.
			void set (const NumericT * data);
			/// Copy count elements of raw data into the vector.
			void set (const NumericT * other, unsigned count, unsigned offset = 0);
			/// Copy raw data into the vector
			template <typename OtherNumericT>
			void set (const OtherNumericT * other);

			/// Set the vector to the value of another of a different type and length.
			/// Will only copy as much as possible.
			template <dimension E2, typename OtherNumericT>
			void set (const Vector<E2, OtherNumericT> & other);

			/// Set the value of this instance to zero.
			void zero ();
			/// Check if the value of this instance is zero.
			bool is_zero () const;
			/// Set the value of all components of the vector to the value specified.
			/// The default value is 1.
			void load_identity (const NumericT & n = 1);

			/// Return a pointer to the raw data.
			const NumericT * value () const
			{
				return _vector;
			}

			/// Return a copy of the vector.
			Vector copy () const
			{
				return *this;
			}

			/// Return a vector which has one less component.
			/// Removes the last component of the vector.
			Vector<E-1, NumericT> reduce () const
			{
				Vector<E-1, NumericT> result;

				result.set (value());
				return result;
			}

			/// Assign a value to all components of the vector.
			template <typename OtherNumericT>
			Vector & operator= (const OtherNumericT & n)
			{
				if ((NumericT)n)
					for (unsigned i = 0; i < E; ++i)
						_vector[i] = (NumericT)n;

				else
					this->zero();

				return *this;
			}

			/// Assignment of a Vec<N> to a Vec<E> results in copying as much as possible and setting remaining elements to zero.
			template <dimension N, typename OtherNumericT>
			Vector & operator=(const Vector<N, OtherNumericT> & other) {
				for (std::size_t i = 0; i < E; i += 1) {
					if (i < N)
						_vector[i] = other[i];
					else
						_vector[i] = 0;
				}

				return *this;
			}

			/// Assign components from another vector to this vector.
			template <typename OtherNumericT>
			Vector & operator= (const Vector<E, OtherNumericT> & other)
			{
				for (unsigned i = 0; i < E; ++i)
					_vector[i] = (NumericT)other[i];

				return *this;
			}

			/// Access a component of the vector.
			NumericT & operator[] (dimension i)
			{
				return _vector[i];
			}
			/// Access a component of the vector.
			const NumericT & operator[] (dimension i) const
			{
				return _vector[i];
			}

			/// Check for exact equivalence.
			template <typename OtherT>
			bool operator== (const OtherT & other) const;

			/// Check for exact non-equivalence.
			template <typename OtherT>
			bool operator!= (const OtherT & other) const;

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

			/// Numeric comparison.
			/// @returns true if the vector is numerically lesser than the other.
			template <typename OtherT>
			bool operator< (const OtherT & other) const;

			/// Numeric comparison.
			/// @returns true if the vector is numerically greater than the other.
			template <typename OtherT>
			bool operator> (const OtherT & other) const;

			/// Numeric comparison.
			/// @returns true if the vector is numerically lesser than or equal to the other.
			template <typename OtherT>
			bool operator<= (const OtherT & other) const;

			/// Numeric comparison.
			/// @returns true if the vector is numerically greater than or equal to the other.
			template <typename OtherT>
			bool operator>= (const OtherT & other) const;

			/// Return the length of the vector squared.
			/// This method avoids calculating the square root, therefore is faster when you only need to compare the relative lengths of vectors.
			NumericT length2 () const
			{
				return this->dot(*this);
			}

			/// Return the length of the vector.
			NumericRealT length () const
			{
				return Number<NumericRealT>::sqrt(this->length2());
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
		template <typename NumericT>
		inline Vector<1, NumericT> vec (const NumericT & x)
		{
			return Vector<1, NumericT>(x);
		}

		/// Construct a 2-space vector
		template <typename NumericT, typename numeric1_t>
		inline Vector<2, NumericT> vec (const NumericT & x, const numeric1_t & y)
		{
			return Vector<2, NumericT>((NumericT)x, (NumericT)y);
		}

		/// Construct a 3-space vector
		template <typename NumericT, typename numeric1_t, typename numeric2_t>
		inline Vector<3, NumericT> vec (const NumericT & x, const numeric1_t & y, const numeric2_t & z)
		{
			return Vector<3, NumericT>((NumericT)x, (NumericT)y, (NumericT)z);
		}

		/// Construct a 4-space vector
		template <typename NumericT, typename numeric1_t, typename numeric2_t, typename numeric3_t>
		inline Vector<4, NumericT> vec (const NumericT & x, const numeric1_t & y, const numeric2_t & z,
		                                const numeric3_t & w)
		{
			return Vector<4, NumericT>((NumericT)x, (NumericT)y, (NumericT)z, (NumericT)w);
		}

// MARK:
// MARK: Vector Combinations

		/// Append a component to a vector
		template <dimension E, typename NumericT, typename OtherNumericT>
		inline Vector<E+1, NumericT> operator<< (const Vector<E, NumericT> & vector, const OtherNumericT & next)
		{
			Vector<E+1, NumericT> result;

			result.set(vector.value(), E);
			result[E] = next;
			return result;
		}

		/// Join two vectors together
		template <dimension E1, dimension E2, typename n1_t, typename n2_t>
		inline Vector<E1+E2> operator<< (const Vector<E1, n1_t> & v1, const Vector<E2, n2_t> & v2)
		{
			Vector<E1+E2, n1_t> result;

			result.set(v1.value(), E1);
			for (dimension i = 0; i < E2; i += 1)
				result[E1+i] = v2[i];

			return result;
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
	}
}

#include "Vector.impl.h"

#endif
