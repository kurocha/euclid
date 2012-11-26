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
#include <cfloat>

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

		/// These traits give specific accuracy information around +/- zero. The calculated precision is proportional to the number of integral positions between 0.0 and 0.0+EPSILON (where epsilon is the smallest increment from 10.0^exponent).
		template <typename FloatT, dimension Exponent>
		struct EpsilonTraits {};

		template <>
		struct EpsilonTraits<float, 0> {
			typedef typename IntegerSizeTraits<sizeof(float)>::UnsignedT UnitT;

			constexpr static double SCALE = 1.0;
			constexpr static UnitT UNITS = 2;
			constexpr static float EPSILON = FLT_EPSILON * UNITS;
		};

		template <>
		struct EpsilonTraits<double, 0> {
			typedef typename IntegerSizeTraits<sizeof(double)>::UnsignedT UnitT;

			constexpr static double SCALE = 1.0;
			constexpr static UnitT UNITS = 2;
			constexpr static double EPSILON = DBL_EPSILON * UNITS;
		};

		template <typename _FloatT>
		struct FloatEquivalenceTraits {
			typedef _FloatT FloatT;

			typedef typename IntegerSizeTraits<sizeof(FloatT)>::SignedT IntegralT;

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

			static IntegralT integral_difference(const FloatT & a, const FloatT & b) {
				// Make lexicographically ordered as a twos-complement int
				IntegralT i = convert_to_integer(a), j = convert_to_integer(b);

				if (i < j)
					std::swap(i, j);

				return i - j;
			}

			// Large MAX_DEVIATIONS may allow NAN to compare equivalent to large floating point numbers and other strange edge cases.
			static bool equivalent (const FloatT & a, const FloatT & b)
			{
				if (std::isnan(a) || std::isnan(b))
					return false;

				using E = EpsilonTraits<FloatT, 0>;

				if (std::fabs(a) < E::SCALE || std::fabs(b) < E::SCALE) {
					auto epsilon = EpsilonTraits<FloatT, 0>::EPSILON;
					auto difference = std::fabs(a - b);
					
					return difference <= epsilon;
				} else {
					return integral_difference(a, b) <= E::UNITS;
				}
			}
		};

		/// Proportional equivalence of floating point numbers.
		inline bool equivalent (const float & a, const float & b) {
			return FloatEquivalenceTraits<float>::equivalent(a, b);
		}

		inline bool equivalent (const double & a, const double & b) {
			return FloatEquivalenceTraits<double>::equivalent(a, b);
		}
	}
}

#endif
