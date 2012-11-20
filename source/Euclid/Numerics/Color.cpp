//
//  Numerics/Color.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 2/05/10.
//  Copyright (c) 2010 Samuel Williams. All rights reserved.
//
//

#include "Color.h"

namespace Euclid
{
	namespace Numerics
	{
// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Color)
		{
			Vec3 c1(0.2, 0.4, 0.6), c2(0.4, 1.0, 0.0), c3(0.9, 0.9, 0.9);

			Vec3 hsl1(0.58333333333, 0.5, 0.4), hsl2(0.266666666, 1.0, 0.5), hsl3(0.0, 0.0, 0.9);
			Vec3 hsv1(0.58333333333, 0.66666666666, 0.59999999999), hsv2(0.26666666666, 1.0, 1.0), hsv3(0.0, 0.0, 0.9);

			Vec3 t1, t2;

			testing("RGB <-> HSL");

			t1 = RGB2HSL(c1);
			check(t1.equivalent(hsl1)) << "RGB -> HSL";

			t2 = HSL2RGB(t1);
			check(c1.equivalent(t2)) << "RGB -> HSL -> RGB";

			t1 = RGB2HSL(c2);
			check(t1.equivalent(hsl2)) << "RGB -> HSL";

			t2 = HSL2RGB(t1);
			check(c2.equivalent(t2)) << "RGB -> HSL -> RGB";

			t1 = RGB2HSL(c3);
			check(t1.equivalent(hsl3)) << "RGB -> HSL";

			t2 = HSL2RGB(t1);
			check(c3.equivalent(t2)) << "RGB -> HSL -> RGB";

			testing("RGB <-> HSV");

			t1 = RGB2HSV(c1);
			check(t1.equivalent(hsv1)) << "RGB -> HSV";

			t2 = HSV2RGB(t1);
			check(c1.equivalent(t2)) << "RGB -> HSV -> RGB";

			t1 = RGB2HSV(c2);
			check(t1.equivalent(hsv2)) << "RGB -> HSV";

			t2 = HSV2RGB(t1);
			check(c2.equivalent(t2)) << "RGB -> HSV -> RGB";

			t1 = RGB2HSV(c3);
			check(t1.equivalent(hsv3)) << "RGB -> HSV";

			t2 = HSV2RGB(t1);
			check(c3.equivalent(t2)) << "RGB -> HSV -> RGB";
		}
#endif
	}
}
