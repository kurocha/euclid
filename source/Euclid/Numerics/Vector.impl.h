//
//  Numerics/Vector.impl.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 22/11/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_VECTOR_H
#error This header should not be included manually. Include Vector.h instead.
#else

// memset
#include <string.h>

namespace Euclid
{
	namespace Numerics
	{
// MARK: -
// MARK: Vector Traits

		template <typename NumericT>
		VectorConversionTraits<1, NumericT>::operator NumericT () const
		{
			const Vector<1, NumericT> &v = static_cast<const Vector<1, NumericT> &>(*this);

			return v[0];
		}

		template <dimension E>
		bool VectorEqualityTraits<E, float>::equivalent (const Vector<E, float> & other) const
		{
			const Vector<E, float> &v = static_cast<const Vector<E, float> &>(*this);

			for (dimension i = 0; i < E; ++i) {
				if (!Numerics::equivalent(v[i], other[i]))
					return false;
			}

			return true;
		}

		template <dimension E>
		bool VectorEqualityTraits<E, double>::equivalent (const Vector<E, double> & other) const
		{
			const Vector<E, double> &v = static_cast<const Vector<E, double> &>(*this);

			for (dimension i = 0; i < E; ++i) {
				if (!Numerics::equivalent(v[i], other[i]))
					return false;
			}

			return true;
		}

		template <typename NumericT>
		Vector<3, NumericT> surface_normal (const Vector<3, NumericT> & a, const Vector<3, NumericT> & b, const Vector<3, NumericT> & c)
		{
			Vector<3, NumericT> a1 = b - a;
			Vector<3, NumericT> b1 = c - b;

			return a1.cross(b1).normalize();
		}

		template <typename NumericT>
		Vector<2, NumericT> VectorTraits<2, NumericT>::normal () const
		{
			const VectorT &v = *static_cast<const VectorT*>(this);
			Vector<3, NumericT> up(0.0, 0.0, 1.0);

			return (v << 0.0).cross(up).reduce();
		}

		template <typename NumericT>
		NumericT VectorTraits<2, NumericT>::aspect_ratio () const
		{
			const VectorT &v = *static_cast<const VectorT*>(this);

			return v[WIDTH] / v[HEIGHT];
		}

		template <typename NumericT>
		Vector<2, NumericT> VectorTraits<2, NumericT>::shrink_to_fit_aspect_ratio(NumericT n) const
		{
			const VectorT &v = *static_cast<const VectorT*>(this);

			Vector<2, NumericT> result;

			result = v;
			result[WIDTH] = n * result[HEIGHT];

			if (result[WIDTH] <= v[WIDTH]) {
				return result;
			}

			result = v;
			result[HEIGHT] = (1.0 / n) * result[WIDTH];

			return result;
		}

		template <typename NumericT>
		Vector<2, NumericT> VectorTraits<2, NumericT>::shrink_to_fit_aspect_ratio(const Vector<2, NumericT> & other) const
		{
			const VectorT &v = *static_cast<const VectorT*>(this);

			return this->shrink_to_fit_aspect_ratio(other.aspect_ratio());
		}

		template <typename NumericT>
		Vector<2, NumericT> VectorTraits<2, NumericT>::expand_to_fit_aspect_ratio(NumericT n) const
		{
			const VectorT &v = *static_cast<const VectorT*>(this);

			Vector<2, NumericT> result;

			result = v;
			result[WIDTH] = n * result[HEIGHT];

			if (result[WIDTH] >= v[WIDTH]) {
				return result;
			}

			result = v;
			result[HEIGHT] = (1.0 / n) * result[WIDTH];

			return result;
		}

		template <typename NumericT>
		Vector<2, NumericT> VectorTraits<2, NumericT>::expand_to_fit_aspect_ratio(const Vector<2, NumericT> & other) const
		{
			const VectorT &v = *static_cast<const VectorT*>(this);

			return this->expand_to_fit_aspect_ratio(other.aspect_ratio());
		}

		template <typename NumericT>
		Vector<3, NumericT> VectorTraits<3, NumericT>::cross (const Vector<3, NumericT> & other) const
		{
			const VectorT &v = *static_cast<const VectorT*>(this);
			Vector<3, NumericT> result;

			result[0] = v[1] * other[2] - v[2] * other[1];
			result[1] = v[2] * other[0] - v[0] * other[2];
			result[2] = v[0] * other[1] - v[1] * other[0];

			return result;
		}

		template <typename NumericT>
		Vector<3, NumericT> VectorTraits<3, NumericT>::normal (const Vector<3, NumericT> & other) const
		{
			return cross(other).normalize();
		}

		template <typename NumericT>
		Vector<3, NumericT> VectorTraits<3, NumericT>::normal (const Vector<3, NumericT> & a, const Vector<3, NumericT> & b) const
		{
			return surface_normal (*static_cast<const VectorT*>(this), a, b);
		}

		template <typename NumericT>
		Vector<4, NumericT> cross_product (const Vector<4, NumericT> & u, const Vector<4, NumericT> & v, const Vector<4, NumericT> & w)
		{
			Vector<4, NumericT> result;

			// intermediate values
			NumericT a, b, c, d, e, f;

			// calculate intermediate values.
			a = (v[0] * w[1]) - (v[1] * w[0]);
			b = (v[0] * w[2]) - (v[2] * w[0]);
			c = (v[0] * w[3]) - (v[3] * w[0]);
			d = (v[1] * w[2]) - (v[2] * w[1]);
			e = (v[1] * w[3]) - (v[3] * w[1]);
			f = (v[2] * w[3]) - (v[3] * w[2]);

			// calculate the result-vector components.
			result[0] =   (u[1] * f) - (u[2] * e) + (u[3] * d);
			result[1] = -(u[0] * f) + (u[2] * c) - (u[3] * b);
			result[2] =   (u[0] * e) - (u[1] * c) + (u[3] * a);
			result[3] = -(u[0] * d) + (u[1] * b) - (u[2] * a);

			return result;
		}

		template <typename NumericT>
		Vector<4, NumericT> VectorTraits<4, NumericT>::cross (const Vector<4, NumericT> & v, const Vector<4, NumericT> & w)
		{
			return cross_product(*this, v, w);
		}

// MARK: -
// MARK: Vector

		template <dimension E, typename NumericT>
		Vector<E, NumericT>::Vector (const NumericT & x)
		{
			for (dimension i = 0; i < E; i += 1) {
				_vector[i] = x;
			}
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT>::Vector (const NumericT & x, const NumericT & y)
		{
			static_assert(E == 2, "Vector must be correct size!");

			_vector[0] = x;
			_vector[1] = y;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT>::Vector (const NumericT & x, const NumericT & y, const NumericT & z)
		{
			static_assert(E == 3, "Vector must be correct size!");

			_vector[0] = x;
			_vector[1] = y;
			_vector[2] = z;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT>::Vector (const NumericT & x, const NumericT & y, const NumericT & z, const NumericT & w)
		{
			static_assert(E == 4, "Vector must be correct size!");

			_vector[0] = x;
			_vector[1] = y;
			_vector[2] = z;
			_vector[3] = w;
		}

		template <dimension E, typename NumericT> template <typename other_numeric_t>
		Vector<E, NumericT>::Vector (const Vector<E, other_numeric_t> & other)
		{
			(*this) = other;
		}

		template <dimension E, typename NumericT>
		void Vector<E, NumericT>::zero ()
		{
			memset(_vector, 0, sizeof(_vector));
		}

		template <dimension E, typename NumericT>
		void Vector<E, NumericT>::load_identity (const NumericT & value)
		{
			(*this) = value;
		}

		template <dimension E, typename NumericT>
		bool Vector<E, NumericT>::is_zero () const
		{
			return !this->sum();
		}

		template <dimension E, typename NumericT> template <dimension E2, typename other_numeric_t>
		void Vector<E, NumericT>::set (const Vector<E2, other_numeric_t> & other)
		{
			const dimension I = E < E2 ? E : E2;

			for (dimension i = 0; i < I; ++i)
				this->_vector[i] = (NumericT)other[i];
		}

		template <dimension E, typename NumericT>
		void Vector<E, NumericT>::set (const NumericT * other)
		{
			memcpy(this->_vector, other, sizeof(_vector));
		}

		template <dimension E, typename NumericT>
		void Vector<E, NumericT>::set (const NumericT * other, unsigned count, unsigned offset)
		{
			memcpy(this->_vector + offset, other, sizeof(NumericT) * count);
		}

		template <dimension E, typename NumericT> template <typename OtherNumericT>
		void Vector<E, NumericT>::set (const OtherNumericT * other) {
			for (dimension i = 0; i < E; i += 1)
				this->_vector[i] = other[i];
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::operator== (const OtherT & other) const
		{
			return !(*this != other);
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::operator!= (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if (_vector[i] != other[i]) return true;

			return false;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::less_than (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if (_vector[i] >= other[i])
					return false;

			return true;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::greater_than (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if (_vector[i] <= other[i])
					return false;

			return true;
		}

		// Numeric comparisons
		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::less_than_or_equal (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if (_vector[i] > other[i])
					return false;

			return true;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::greater_than_or_equal (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if (_vector[i] < other[i])
					return false;

			return true;
		}

		// Set comparisons
		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::operator< (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if (_vector[i] > other[i]) return false;
				else if (_vector[i] < other[i]) return true;

			return false;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::operator> (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if (_vector[i] < other[i]) return false;
				else if (_vector[i] > other[i]) return true;

			return false;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::operator<= (const OtherT & other) const
		{
			if (operator!=(other))
				return operator<(other);

			return true;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::operator>= (const OtherT & other) const
		{
			if (operator!=(other))
				return operator>(other);

			return true;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		NumericT Vector<E, NumericT>::dot (const OtherT & other) const
		{
			NumericT result = 0;

			for (dimension i = 0; i < E; ++i) {
				result += _vector[i] * other[i];
			}

			return result;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		NumericT Vector<E, NumericT>::angle_between (const OtherT & other) const
		{
			NumericT r = this->dot(other) / (this->length() * other.length());

			return Number<NumericT>::acos(Number<NumericT>::clamp(r, (NumericT)-1.0, (NumericT)1.0));
		}

		template <dimension E, typename NumericT>
		NumericT Vector<E, NumericT>::sum () const
		{
			NumericT result = 0;

			for (dimension i = 0; i < E; ++i)
				result += _vector[i];

			return result;
		}

		template <dimension E, typename NumericT>
		NumericT Vector<E, NumericT>::product () const
		{
			NumericT result = 1;
			for (dimension i = 0; i < E; ++i)
				result *= _vector[i];

			return result;
		}

		// Given a size vector (x, y, [z]), distribute i as if
		// it was an index into that space
		template <dimension E, typename NumericT>
		Vector<E, NumericT> Vector<E, NumericT>::distribute (NumericT k) const
		{
			Vector r(ZERO);
			NumericT m = this->product();

			DREAM_ASSERT(m);

			for (dimension i = E; i > 0; i -= 1) {
				m /= _vector[i-1];
				r[i-1] = Number<NumericT>::floor(k / m);
				k = Number<NumericT>::mod(k, m);
			}

			DREAM_ASSERT(m == 1);

			return r;
		}

		// Given a size vector (this) and a coordinate, return an index
		// for access into a 1E array.
		template <dimension E, typename NumericT>
		NumericT Vector<E, NumericT>::index (const Vector<E, NumericT> & coord) const
		{
			NumericT idx = 0;
			NumericT m = 1;

			//		x(1) + y(sx) + z(sx*sy)
			for (dimension i = 0; i < E; i += 1) {
				idx += coord[i] * m;
				m *= _vector[i];
			}

			return idx;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::clamp (const unsigned i, const NumericT & min, const NumericT & max)
		{
			_vector[i] = Math::clamp(_vector[i], min, max);

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::clamp (const NumericT & min, const NumericT & max)
		{
			for (dimension i = 0; i < E; ++i)
				_vector[i] = Number<NumericT>::clamp(_vector[i], min, max);

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::abs ()
		{
			for (dimension i = 0; i < E; ++i) {
				_vector[i] = Math::abs(_vector[i]);
			}

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::floor ()
		{
			for (dimension i = 0; i < E; ++i)
				_vector[i] = Number<NumericT>::floor(_vector[i]);

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::ceil ()
		{
			for (dimension i = 0; i < E; ++i)
				_vector[i] = Number<NumericT>::ceil(_vector[i]);

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::frac ()
		{
			for (dimension i = 0; i < E; ++i)
				_vector[i] = _vector[i] - Number<NumericT>::floor(_vector[i]);

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::normalize (const NumericT & to_length)
		{
			NumericT n = this->length();

			if (n == 0) {
				return *this;
			}

			//DREAM_ASSERT(n > 0 && "Trying to normalize zero-length vector!");

			if (to_length != n) {
				NumericT factor = to_length / n;
				*this *= factor;
			}

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> Vector<E, NumericT>::normalized_vector (const NumericT & to_length) const
		{
			// Copy the vector
			Vector<E, NumericT> copy(*this);

			// Normalize the copy and return it.
			return copy.normalize(to_length);
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		void Vector<E, NumericT>::constrain (const Vector<E, NumericT> & with, const OtherT & limits)
		{
			for (dimension i = 0; i < E; ++i) {
				if (limits[i] < 0) {
					_vector[i] = (_vector[i] < with[i] ? _vector[i] : with[i]) * -(NumericT)limits[i];
				} else if (limits[i] > 0) {
					_vector[i] = (_vector[i] > with[i] ? _vector[i] : with[i]) * (NumericT)limits[i];
				}
			}
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::negate ()
		{
			for (dimension i = 0; i < E; ++i)
				_vector[i] = -_vector[i];

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> Vector<E, NumericT>::operator- () const
		{
			return this->copy().negate();
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::inverse ()
		{
			for (dimension i = 0; i < E; ++i)
				_vector[i] = !_vector[i];

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> Vector<E, NumericT>::operator! () const
		{
			return this->copy().inverse();
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> Vector<E, NumericT>::lerp (const NumericT & t, Vector<E, NumericT> other) const
		{
			return (*this) + t * (other - *this);
		}

		// Is there a more generic way to do this?
		template <dimension E, typename NumericT>
		bool Vector<E, NumericT>::increment (const Vector<E, NumericT> & max)
		{
			for (dimension i = 0; i < E; ++i) {
				if ((_vector[i] + 1) < max[i]) {
					_vector[i] += 1;

					if (i) _vector[i-1] = 0;

					return true;
				}
			}

			// Could not increment
			return false;
		}

		// Pack the vector into a single variable
		template <dimension E, typename NumericT> template <typename PackedT>
		void Vector<E, NumericT>::pack (unsigned bits, PackedT & p) const
		{
			unsigned mask = ((1 << bits) - 1);
			for (unsigned i = 0; i < E; ++i) {
				p <<= bits;

				// Mask out higher order bits
				p += _vector[i] & mask;
			}
		}

		// Unpack elements of a vector from a single variable
		template <dimension E, typename NumericT> template <typename PackedT>
		void Vector<E, NumericT>::unpack (unsigned bits, const PackedT & p)
		{
			for (unsigned i = 0; i < E; ++i) {
				_vector[E-(i+1)] = (p >> (bits*i)) & ((1 << bits) - 1);
			}
		}
	}
}

#endif
