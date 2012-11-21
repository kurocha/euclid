//
//  Numerics/Number.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 31/10/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_NUMBER_H
#define _EUCLID_NUMERICS_NUMBER_H

#include "Numerics.h"
#include "Integer.h"
#include "Float.h"

#include <algorithm>

namespace Euclid
{
	namespace Numerics
	{
		template <typename NumericT>
		class Number;

		/** Functionality common to all numeric types.
		 */
		template <typename NumericT>
		class NumberTraits {
		public:
			/// Clamp a value between a lower and upper bound.
			template <typename t>
			static inline t clamp (const t & v, const t & lower = 0.0, const t & upper = 1.0)
			{
				if (v < lower) return lower;

				if (v > upper) return upper;

				return v;
			}

			/// Clamp a value at a lower bound.
			template <typename t>
			static inline t clamp_bottom (const t & v, const t & l = 0.0)
			{
				if (v < l) return l;

				return v;
			}

			/// Clamp a value at an upper bound.
			template <typename t>
			static inline t clamp_top (const t & v, const t & u = 1.0)
			{
				if (v > u) return u;

				return v;
			}

			/// Return the minimum of two numeric values.
			static inline RealT min (const NumericT & a, const NumericT & b)
			{
				return std::min(a, b);
			}

			/// Return the maximum of two numeric values.
			static inline RealT max (const NumericT & a, const NumericT & b)
			{
				return std::max(a, b);
			}

			/// Return the reciprocal (1 / n) of a number.
			static inline NumericT recip (const NumericT & n)
			{
				return (NumericT)1.0 / n;
			}

			/// Returns a number in the range 0...m
			static inline NumericT wrap (const NumericT & n, const NumericT & m)
			{
				if (n < 0) {
					return Number<NumericT>::mod(n, m) + m;
				} else {
					return Number<NumericT>::mod(n, m);
				}
			}
		};

		/** Generic number class for signed and unsigned integers.
		 */
		template <typename NumericT>
		class Number : public NumberTraits<NumericT>{
		public:
			static inline NumericT floor (const NumericT & n)
			{
				return n;
			}

			static inline NumericT ceil (const NumericT & n)
			{
				return n;
			}

			static inline NumericT abs (const NumericT & n)
			{
				if (n < (NumericT)0)
					return -n;
				else
					return n;
			}

			static inline NumericT mod (const NumericT & n, const NumericT & m)
			{
				return n % m;
			}

			static inline bool equivalent(const NumericT & n, const NumericT & m)
			{
				return n == m;
			}
		};

		/** Specific Number class for single floating point numbers.
		 */
		template <>
		class Number<float>: public NumberTraits<float>{
		public:
			typedef float NumericT;

			static inline NumericT floor (const NumericT & n)
			{
				return ::floorf(n);
			}

			static inline NumericT ceil (const NumericT & n)
			{
				return ::ceilf(n);
			}

			static inline NumericT abs (const NumericT & n)
			{
				return ::fabsf(n);
			}

			static inline NumericT cos (const NumericT & n)
			{
				return ::cosf(n);
			}

			static inline NumericT sin (const NumericT & n)
			{
				return ::sinf(n);
			}

			static inline NumericT tan (const NumericT & n)
			{
				return ::tanf(n);
			}

			static inline NumericT acos (const NumericT & n)
			{
				return ::acosf(n);
			}

			static inline NumericT asin (const NumericT & n)
			{
				return ::asinf(n);
			}

			static inline NumericT atan (const NumericT & n)
			{
				return ::atanf(n);
			}

			static inline NumericT pow (const NumericT & n, const NumericT & m)
			{
				return ::powf(n, m);
			}

			static inline NumericT exp (const NumericT & n)
			{
				return ::expf(n);
			}

			static inline NumericT sqrt (const NumericT & n)
			{
				return ::sqrtf(n);
			}

			static inline NumericT mod (const NumericT & n, const NumericT & m)
			{
				return ::fmodf(n, m);
			}

			static inline bool equivalent(const NumericT & n, const NumericT & m)
			{
				return Numerics::equivalent(n, m);
			}
		};

		/** Specific Number class for double floating point numbers.
		 */
		template <>
		class Number<double>: public NumberTraits<double>{
		public:
			typedef double NumericT;

			static inline NumericT floor (const NumericT & n)
			{
				return ::floor(n);
			}

			static inline NumericT ceil (const NumericT & n)
			{
				return ::ceil(n);
			}

			static inline NumericT abs (const NumericT & n)
			{
				return ::fabs(n);
			}

			static inline NumericT cos (const NumericT & n)
			{
				return ::cos(n);
			}

			static inline NumericT sin (const NumericT & n)
			{
				return ::sin(n);
			}

			static inline NumericT tan (const NumericT & n)
			{
				return ::tan(n);
			}

			static inline NumericT acos (const NumericT & n)
			{
				return ::acos(n);
			}

			static inline NumericT asin (const NumericT & n)
			{
				return ::asin(n);
			}

			static inline NumericT atan (const NumericT & n)
			{
				return ::atan(n);
			}

			static inline NumericT pow (const NumericT & n, const NumericT & m)
			{
				return ::pow(n, m);
			}

			static inline NumericT exp (const NumericT & n)
			{
				return ::exp(n);
			}

			static inline NumericT sqrt (const NumericT & n)
			{
				return ::sqrt(n);
			}

			static inline NumericT mod (const NumericT & n, const NumericT & m)
			{
				return ::fmod (n, m);
			}

			static inline bool equivalent(const NumericT & n, const NumericT & m)
			{
				return Numerics::equivalent(n, m);
			}
		};

		/// Standard set of mathematical functions for RealT.
		typedef Number<RealT> Math;
	}
}

#ifdef ENABLE_TESTING
namespace Euclid
{
	namespace Core
	{
		namespace CodeTestAssertions
		{
			/// CodeTest helpers for testing floating point numbers.
			bool equivalent (const float & lhs, const float & rhs);

			/// CodeTest helpers for testing floating point numbers.
			bool equivalent (const double & lhs, const double & rhs);
		}
	}
}
#endif

#endif
