//
//  Numerics/Numerics.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 13/05/06.
//  Copyright 2006 Samuel WilliamsWilliams. All rights reserved.
//
//

#include "Numerics.h"

#include "Number.h"

#include <iostream>
#include <cstdlib>

namespace Euclid
{
	namespace Numerics
	{
		template <typename t>
		class FloatingPointIntegerTraits {
		};

		template <>
		class FloatingPointIntegerTraits<float>{
		public:
			typedef int32_t FIntT;
			typedef uint32_t FUIntT;

			/// Manually calculated - approximate accuracy of zero point for libm when dealing with trigonometric functions.
			static const uint32_t ACCURACY = 897988541;
		};

		template <>
		class FloatingPointIntegerTraits<double>{
		public:
			typedef int64_t FIntT;
			typedef uint64_t FUIntT;

			/// Manually calculated - approximate accuracy of zero point for libm when dealing with trigonometric functions.
			static const uint64_t ACCURACY = 4427486594234968593ULL;
		};

		template <typename t>
		class FloatingPointTraits {
		public:
			typedef t FloatT;
			typedef typename FloatingPointIntegerTraits<t>::FIntT FIntT;

			union Conversion
			{ FloatT f; FIntT i; };

			static const FIntT S = FIntT (0x80ULL << ((sizeof (FloatT) - 1) * 8));

			static FIntT convert_to_integer (const FloatT & f)
			{
				Conversion c;
				c.f = f;

				if (c.i < 0)
					return S - c.i;
				else
					return c.i;
			}

			static FloatT convert_to_float (const FIntT & i) {
				Conversion c;
				c.i = i;

				if (c.i < 0)
					c.i = S - c.i;

				return c.f;
			}

			static bool is_zero (const FIntT & value, const FIntT & threshold) {
				if (value < 0)
					return (value + threshold) > 0;
				else
					return (value - threshold) < 0;
			}

			static bool is_zero (const FloatT & value, const FIntT & threshold) {
				FIntT p = convert_to_integer(value);

				return is_zero (p, threshold);
			}

			static bool is_zero (const FloatT & value) {
				return is_zero(value, FloatingPointIntegerTraits<t>::ACCURACY);
			}

			static bool is_zero (const FIntT & value) {
				return is_zero(value, FloatingPointIntegerTraits<t>::ACCURACY);
			}

			static unsigned integral_difference(const FloatT & a, const FloatT & b) {
				// Make lexicographically ordered as a twos-complement int
				FIntT a_integral = convert_to_integer(a);
				FIntT b_integral = convert_to_integer(b);

				if (a_integral < b_integral)
					return b_integral - a_integral;
				else
					return a_integral - b_integral;
			}

			static bool equal_within_tolerance (const FloatT & a, const FloatT & b, const unsigned & max_ulps)
			{
				// Make sure max_ulps is non-negative and small enough that the
				// default NAN won't compare as equal to anything.
				assert(max_ulps < 4 * 1024 * 1024);

				if (integral_difference(a, b) <= max_ulps)
					return true;

				return false;
			}

			static bool equivalent (const FloatT & a, const FloatT & b)
			{
				// Make lexicographically ordered as a twos-complement int
				FIntT a_integral = convert_to_integer(a);
				FIntT b_integral = convert_to_integer(b);

				if (a_integral == b_integral) return true;

				if (is_zero(a_integral) && is_zero(b_integral)) return true;

				// This is a arbitrarily chosen constant, perhaps it could be improved?
				if (integral_difference(a, b) <= 100)
					return true;

				return false;
			}
		};

		bool is_power_of_2 (uint32_t k)
		{
			return (k & (k-1)) == 0;
		}

		// Usable AlmostEqual function
		bool equal_within_tolerance (const float & a, const float & b, const unsigned & max_ulps)
		{
			return FloatingPointTraits<float>::equal_within_tolerance(a, b, max_ulps);
		}

		// Usable AlmostEqual function
		bool equal_within_tolerance (const double & a, const double & b, const unsigned & max_ulps)
		{
			return FloatingPointTraits<double>::equal_within_tolerance(a, b, max_ulps);
		}

		// Usable AlmostEqual function
		bool equivalent (const float & a, const float & b)
		{
			return FloatingPointTraits<float>::equivalent(a, b);
		}

		// Usable AlmostEqual function
		bool equivalent (const double & a, const double & b)
		{
			return FloatingPointTraits<double>::equivalent(a, b);
		}

// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Numerics)
		{
			const double v = 5193823413.0;

			testing("Equality");
			float a, b, c;
			double d, e, f;

			a = v / 7.5;
			b = v / 7.500001;
			c = v / 7.5001;

			check(equal_within_tolerance(a, b)) << "Float values are equal";
			check(!equal_within_tolerance(a, c)) << "Float values are not equal";

			d = v / 7.5;
			e = v / 7.50000000001;
			f = v / 7.50000000000001;

			check(!equal_within_tolerance(d, e)) << "Double values are not equal";
			check(equal_within_tolerance(d, f)) << "Double values are equal";
		}

		UNIT_TEST(FloatingPointTraits)
		{
			typedef FloatingPointTraits<float> F;
			F::FIntT i = F::convert_to_integer(1.0056f);
			F::FloatT f = F::convert_to_float(i);
			check(1.0056f == f) << "Integer - float conversion correct for 1.0056";

			i = F::convert_to_integer(-0.52f);
			f = F::convert_to_float(i);
			check(-0.52f == f) << "Integer - float conversion correct for -0.52";

			double t = 0.0000001;
			std::cout << "acosf: " << cos(R90) << std::endl;
			check(FloatingPointTraits<double>::is_zero(cos(R90)) << FloatingPointTraits<double>::convert_to_integer(t)) << "cosf is zero";

			std::cout << "Accuracy of float: " << FloatingPointTraits<double>::convert_to_integer(t) << std::endl;
		}

		UNIT_TEST(PowerOfTwo)
		{
			int k = next_highest_power_of_2(16);
			check(k == 16) << "Next power of two calculated correctly";

			k = next_highest_power_of_2(17);
			check(k == 32) << "Next power of two calculated correctly";
		}

		/// Calculate the accuracy of cos function. Interesting results..
		template <typename t>
		void calculate_float_accuracy ()
		{
			using namespace std;
			typedef FloatingPointTraits<t> F;
			typedef typename F::FIntT FIntT;
			typedef typename F::FloatT FloatT;

			FloatT f = R90;
			FloatT v1 = Number<FloatT>::cos(f);

			if (v1 < (FloatT) 0.0) {
				// Increment floating point number to next discrete step
				f = F::convert_to_float (F::convert_to_integer (f) + 1);
			} else {
				// Decrement floating point number to previous discrete step
				f = F::convert_to_float (F::convert_to_integer (f) - 1);
			}

			FloatT v2 = Number<FloatT>::cos(f);

			FIntT d1 = Number<FIntT>::abs(F::convert_to_integer(v1));
			FIntT d2 = Number<FIntT>::abs(F::convert_to_integer(v2));
			FIntT d3 = Number<FIntT>::max(d1, d2);

			std::cout << "d1 : " << d1 << " d2 : " << d2 << " = d3 : " << d3 << std::endl;
			std::cout << "Trigonometric precision of " << typeid(t).name() << " around zero is ";
			std::cout << F::convert_to_float(d3) << std::endl;
		}

		UNIT_TEST(CalculateFloatAccuracy)
		{
			calculate_float_accuracy<float>();
			calculate_float_accuracy<double>();

			std::cout << "   Float ACCURACY: " << FloatingPointTraits<float>::convert_to_integer(0.000001) << std::endl;
			std::cout << "  Double ACCURACY: " << FloatingPointTraits<double>::convert_to_integer(0.000000000001) << std::endl;
		}

		UNIT_TEST(CheckRotationAccuracy)
		{
			testing("Rotations");

			check(equivalent(R45 * 2, R90)) << "R45 is correct";
			check(equivalent(R90 * 2, R180)) << "R90 is correct";
			check(equivalent(R180 * 2, R360)) << "R360 is correct";
		}
#endif
	}
}
