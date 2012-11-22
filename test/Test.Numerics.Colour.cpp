
#include <UnitTest/UnitTest.h>

#include <Euclid/Numerics/Color.h>

UnitTest::Suite TestNumericsColourSuite {
	"Test Colour",

	{"RGB <-> HSL",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			Vec3 c1 = {0.2, 0.4, 0.6}, c2 = {0.4, 1.0, 0.0}, c3 = {0.9, 0.9, 0.9};
			Vec3 hsl1 = {0.58333333333, 0.5, 0.4}, hsl2 = {0.266666666, 1.0, 0.5}, hsl3 = {0.0, 0.0, 0.9};

			Vec3 t1, t2;

			t1 = RGB2HSL(c1);
			examiner->check(t1.equivalent(hsl1)) << "RGB -> HSL";

			t2 = HSL2RGB(t1);
			examiner->check(c1.equivalent(t2)) << "RGB -> HSL -> RGB";

			t1 = RGB2HSL(c2);
			examiner->check(t1.equivalent(hsl2)) << "RGB -> HSL";

			t2 = HSL2RGB(t1);
			examiner->check(c2.equivalent(t2)) << "RGB -> HSL -> RGB";

			t1 = RGB2HSL(c3);
			examiner->check(t1.equivalent(hsl3)) << "RGB -> HSL";

			t2 = HSL2RGB(t1);
			examiner->check(c3.equivalent(t2)) << "RGB -> HSL -> RGB";
		}
	},
	
	{"RGB <-> HSV",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			Vec3 c1 = {0.2, 0.4, 0.6}, c2 = {0.4, 1.0, 0.0}, c3 = {0.9, 0.9, 0.9};
			Vec3 hsv1 = {0.58333333333, 0.66666666666, 0.59999999999}, hsv2 = {0.26666666666, 1.0, 1.0}, hsv3 = {0.0, 0.0, 0.9};
			
			Vec3 t1, t2;

			t1 = RGB2HSV(c1);
			examiner->check(t1.equivalent(hsv1)) << "RGB -> HSV";

			t2 = HSV2RGB(t1);
			examiner->check(c1.equivalent(t2)) << "RGB -> HSV -> RGB";

			t1 = RGB2HSV(c2);
			examiner->check(t1.equivalent(hsv2)) << "RGB -> HSV";
			
			t2 = HSV2RGB(t1);
			examiner->check(c2.equivalent(t2)) << "RGB -> HSV -> RGB";
			
			t1 = RGB2HSV(c3);
			examiner->check(t1.equivalent(hsv3)) << "RGB -> HSV";
			
			t2 = HSV2RGB(t1);
			examiner->check(c3.equivalent(t2)) << "RGB -> HSV -> RGB";
		}
	},
};
