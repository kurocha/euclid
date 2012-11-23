//
//  Float.h
//  Euclid
//
//  Created by Samuel Williams on 21/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_FLOAT_H
#define _EUCLID_NUMERICS_FLOAT_H

#include "../Euclid.h"
#include "Integer.h"

#include <cmath>
#include <cstdint>
#include <type_traits>
#include <iostream>

namespace Euclid
{
	namespace Numerics
	{
		/// Helper to get floating point type from a fixed point type.
		/// By default we map all integer types to single precision floats.
		template <typename TypeT>
		struct RealTypeTraits {
			typedef float RealT;
		};

		template <>
		struct RealTypeTraits<double>{
			typedef double RealT;
		};
		
		template <typename FloatT>
		struct FloatAccuracyTraits {
		};

		template <>
		struct FloatAccuracyTraits<float> {
			typedef float FloatT;
			
			typedef IntegerSizeTraits<sizeof(FloatT)> IntegerT;

			/// Manually calculated - approximate accuracy of zero point when dealing with trigonometric functions.
			static const IntegerT::UnsignedT ACCURACY = 897988541;
		};

		template <>
		struct FloatAccuracyTraits<double>{
			typedef double FloatT;
			
			typedef IntegerSizeTraits<sizeof(FloatT)> IntegerT;

			/// Manually calculated - approximate accuracy of zero point when dealing with trigonometric functions.
			static const IntegerT::UnsignedT ACCURACY = 4427486594234968593ULL;
		};

		template <typename _FloatT>
		struct FloatEquivalenceTraits {
			typedef _FloatT FloatT;

			typedef typename IntegerSizeTraits<sizeof(FloatT)>::SignedT IntegralT;
			static constexpr uint64_t ACCURACY = FloatAccuracyTraits<FloatT>::ACCURACY;

			union Conversion
			{
				FloatT float_value;
				IntegralT integer_value;
			};

			static constexpr IntegralT NEGATIVE_OFFSET = IntegralT(0x80ULL << ((sizeof(FloatT) - 1) * 8));

			static IntegralT convert_to_integer (const FloatT & value)
			{
				Conversion conversion;
				conversion.float_value = value;

				if (conversion.integer_value < 0)
					return NEGATIVE_OFFSET - conversion.integer_value;
				else
					return conversion.integer_value;
			}

			static FloatT convert_to_float (const IntegralT & value) {
				Conversion conversion;
				conversion.integer_value = value;

				if (conversion.integer_value < 0)
					conversion.integer_value = NEGATIVE_OFFSET - conversion.integer_value;

				return conversion.float_value;
			}

			static bool is_zero (const IntegralT & value, const IntegralT & threshold) {
				if (value < 0)
					return (value + threshold) > 0;
				else
					return (value - threshold) < 0;
			}

			static bool is_zero (const FloatT & value, const IntegralT & threshold) {
				IntegralT p = convert_to_integer(value);

				return is_zero (p, threshold);
			}

			static bool is_zero (const FloatT & value) {
				return is_zero(value, ACCURACY);
			}

			static bool is_zero (const IntegralT & value) {
				return is_zero(value, ACCURACY);
			}

			static IntegralT integral_difference(const FloatT & a, const FloatT & b) {
				// Make lexicographically ordered as a twos-complement int
				IntegralT i = convert_to_integer(a), j = convert_to_integer(b);

				if (i < j)
					std::swap(i, j);

				return i - j;
			}

			// Large MAX_DEVIATIONS may allow NAN to compare equivalent to large floating point numbers and other strange edge cases.
			template <std::size_t MAX_DEVIATION = ACCURACY>
			static bool equivalent (const FloatT & a, const FloatT & b)
			{
				if (std::isnan(a) || std::isnan(b))
					return false;
				
				auto deviation = integral_difference(a, b);
				
				if (deviation <= MAX_DEVIATION)
					return true;

				return false;
			}
		};

		/// Proportional equivalence of floating point numbers.
		inline bool equivalent (const float & a, const float & b) {
			return FloatEquivalenceTraits<float>::equivalent(a, b);
		}

		inline bool equivalent (const double & a, const double & b) {
			return FloatEquivalenceTraits<double>::equivalent(a, b);
		}

		/// Proportionally close to zero.
		inline bool is_zero (const float & value) {
			return FloatEquivalenceTraits<float>::is_zero(value);
		}

		inline bool is_zero (const double & value) {
			return FloatEquivalenceTraits<double>::is_zero(value);
		}
	}
}

#endif
