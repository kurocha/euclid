//
//  Numerics/Number.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 31/10/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#include "Number.h"

namespace Euclid
{
	namespace Numerics
	{
// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Number)
		{
			testing("Clamping");

			check(Number<int>::clamp(10, 0, 5) == 5) << "Number clamped at top";
			check(Number<int>::clamp(-10, -5, 5) == -5) << "Number clamped at bottom";
			check(Number<int>::clamp(2, -5, 5) == 2) << "Number not clamped";

			check(Number<float>::clamp_top(25, 20) == 20) << "Number clamped at top";
			check(Number<float>::clamp_top(15, 20) == 15) << "Number not clamped";

			check(Number<unsigned>::clamp_bottom(15, 20) == 20) << "Number clamped at top";
			check(Number<unsigned>::clamp_bottom(25, 20) == 25) << "Number not clamped";

			testing("Min and Max");

			check(Number<float>::min(5.5, 4.5) == 4.5) << "Minimum selected correctly";
			check(Number<float>::max(5.5, 4.5) == 5.5) << "Maximum selected correctly";

			testing("Reciprocal");

			check(Number<float>::recip(10) == 0.1f) << "Reciprocal calculated correctly";

			testing("Wrap");

			check(Number<int>::wrap(12, 5) == 2) << "Number wrapped correctly";
			check(Number<int>::wrap(-12, 5) == 3) << "Number wrapped correctly";

			testing("Floor Value");

			check(Number<float>::floor(5.4) == 5.0) << "Number rounded down";
			check(Number<float>::floor(-5.4) == -6.0) << "Number rounded down";

			testing("Ceiling Value");

			check(Number<float>::ceil(-5.4) == -5.0) << "Number rounded up";
			check(Number<float>::ceil(5.4) == 6.0) << "Number rounded up";

			testing("Absolute Value");

			check(Number<double>::abs(-1) == 1) << "Number inverted";
			check(Number<double>::abs(4) == 4) << "Number not inverted";

			testing("Trigonometry");

			check(equivalent(Number<float>::sin(0.0), 0.0f)) << "Correct result from sin";
			check(equivalent(Number<float>::cos(R90), 0.0f)) << "Correct result from cos";
			check(equivalent(Number<float>::acos(0.0), (float)R90)) << "Correct result from acos";
		}
#endif
	}
}

#ifdef ENABLE_TESTING
namespace Euclid
{
	namespace Core
	{
		namespace CodeTestAssertions
		{
			bool equivalent (const float & lhs, const float & rhs)
			{
				return Numerics::Number<float>::equivalent(lhs, rhs);
			}

			bool equivalent (const double & lhs, const double & rhs)
			{
				return Numerics::Number<double>::equivalent(lhs, rhs);
			}
		}
	}
}
#endif
