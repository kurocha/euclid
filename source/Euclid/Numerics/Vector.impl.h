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

#include "Number.h"

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

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::less_than (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if ((*this)[i] >= other[i])
					return false;

			return true;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::greater_than (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if ((*this)[i] <= other[i])
					return false;

			return true;
		}

		// Numeric comparisons
		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::less_than_or_equal (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if ((*this)[i] > other[i])
					return false;

			return true;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		bool Vector<E, NumericT>::greater_than_or_equal (const OtherT & other) const
		{
			for (dimension i = 0; i < E; ++i)
				if ((*this)[i] < other[i])
					return false;

			return true;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		NumericT Vector<E, NumericT>::dot (const OtherT & other) const
		{
			NumericT result = 0;

			for (dimension i = 0; i < E; ++i) {
				result += (*this)[i] * other[i];
			}

			return result;
		}

		template <dimension E, typename NumericT> template <typename OtherT>
		NumericT Vector<E, NumericT>::angle_between (const OtherT & other) const
		{
			NumericT r = this->dot(other) / (this->length() * other.length());

			return std::acos(number(r).clamp(-1, 1));
		}

		template <dimension E, typename NumericT>
		NumericT Vector<E, NumericT>::sum () const
		{
			NumericT result = 0;

			for (dimension i = 0; i < E; ++i)
				result += (*this)[i];

			return result;
		}

		template <dimension E, typename NumericT>
		NumericT Vector<E, NumericT>::product () const
		{
			NumericT result = 1;
			for (dimension i = 0; i < E; ++i)
				result *= (*this)[i];

			return result;
		}

		// Given a size vector (x, y, [z]), distribute i as if
		// it was an index into that space
		template <dimension E, typename NumericT>
		Vector<E, NumericT> Vector<E, NumericT>::distribute (NumericT k) const
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
				m *= (*this)[i];
			}

			return idx;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::clamp (dimension i, const NumericT & min, const NumericT & max)
		{
			(*this)[i] = number((*this)[i]).clamp(min, max);

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::clamp (const NumericT & min, const NumericT & max)
		{
			for (dimension i = 0; i < E; ++i)
				clamp(i, min, max);

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::abs ()
		{
			for (dimension i = 0; i < E; ++i) {
				(*this)[i] = std::abs((*this)[i]);
			}

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::floor ()
		{
			for (dimension i = 0; i < E; ++i)
				(*this)[i] = number((*this)[i]).truncate();

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::ceil ()
		{
			for (dimension i = 0; i < E; ++i)
				(*this)[i] = number((*this)[i]).truncate(true);

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::frac ()
		{
			for (dimension i = 0; i < E; ++i)
				(*this)[i] = (*this)[i] - number((*this)[i]).truncate();

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::normalize (const NumericT & to_length)
		{
			NumericT n = this->length();

			if (n == 0) {
				return *this;
			}

			//assert(n > 0 && "Trying to normalize zero-length vector!");

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
					(*this)[i] = ((*this)[i] < with[i] ? (*this)[i] : with[i]) * -(NumericT)limits[i];
				} else if (limits[i] > 0) {
					(*this)[i] = ((*this)[i] > with[i] ? (*this)[i] : with[i]) * (NumericT)limits[i];
				}
			}
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::negate ()
		{
			for (dimension i = 0; i < E; ++i)
				(*this)[i] = -(*this)[i];

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> Vector<E, NumericT>::operator- () const
		{
			auto copy = *this;
			
			return copy.negate();
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> & Vector<E, NumericT>::inverse ()
		{
			for (dimension i = 0; i < E; ++i)
				(*this)[i] = !(*this)[i];

			return *this;
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> Vector<E, NumericT>::operator! () const
		{
			auto copy = *this;

			return copy.inverse();
		}

		template <dimension E, typename NumericT>
		Vector<E, NumericT> Vector<E, NumericT>::lerp (const NumericT & t, Vector<E, NumericT> other) const
		{
			return (*this) + (other - *this) * t;
		}

		// Is there a more generic way to do this?
		template <dimension E, typename NumericT>
		bool Vector<E, NumericT>::increment (const Vector<E, NumericT> & max)
		{
			for (dimension i = 0; i < E; ++i) {
				if (((*this)[i] + 1) < max[i]) {
					(*this)[i] += 1;

					if (i) (*this)[i-1] = 0;

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
				p += (*this)[i] & mask;
			}
		}

		// Unpack elements of a vector from a single variable
		template <dimension E, typename NumericT> template <typename PackedT>
		void Vector<E, NumericT>::unpack (unsigned bits, const PackedT & p)
		{
			for (unsigned i = 0; i < E; ++i) {
				(*this)[E-(i+1)] = (p >> (bits*i)) & ((1 << bits) - 1);
			}
		}
	}
}

#endif
