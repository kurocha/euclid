
#include <UnitTest/UnitTest.h>

#include <Euclid/Numerics/Numerics.h>
#include <Euclid/Numerics/Angles.h>
#include <Euclid/Numerics/Integer.h>
#include <Euclid/Numerics/Float.h>
#include <Euclid/Numerics/Average.h>
#include <Euclid/Numerics/Interpolate.h>

UnitTest::Suite TestNumericsSuite {
	"Test Numerics",

	{
		"Accuracy",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			std::cout << "Accuracy (float): " << FloatEquivalenceTraits<float>::convert_to_integer(0.000001) << std::endl;
			std::cout << "Accuracy (double): " << FloatEquivalenceTraits<double>::convert_to_integer(0.00000000000000001) << std::endl;

			// Floating point precision is about 7 places.
			examiner->check(equivalent<float>(0.1, 0.1000000001)) << "Float values are equal";

			// We expect better accuracy for double precision:
			examiner->check(!equivalent<double>(0.1, 0.1000000001)) << "Float values are equal";
		}
	},

	{"Identities",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			float a = ZERO;
			float b = IDENTITY;

			examiner->check_equal(a, 0.0f);
			examiner->check_equal(b, 1.0f);
		}
	},

	{"Angles",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			examiner->check(equivalent(R45 * 2.0, R90)) << "R45 is correct";
			examiner->check(equivalent(R90 * 2.0, R180)) << "R90 is correct";
			examiner->check(equivalent(R180 * 2.0, R360)) << "R360 is correct";

			examiner->check_equal(M_PI_2, R90.value);
		}
	},

	{"Squares",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			examiner->check_equal(power_of_2(0U), 2);
			examiner->check_equal(power_of_2(15U), 16);
			examiner->check_equal(power_of_2(16U), 16);
			examiner->check_equal(power_of_2(17U), 32);
		}
	},

	{"Floating Point",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			examiner->check_equal(FloatEquivalenceTraits<float>::convert_to_integer(0.0f), 0);
			examiner->check_equal(FloatEquivalenceTraits<float>::convert_to_integer(-0.0f), 0);
			examiner->check_equal(FloatEquivalenceTraits<double>::convert_to_integer(0.0), 0);
			examiner->check_equal(FloatEquivalenceTraits<double>::convert_to_integer(-0.0), 0);

			examiner->check(FloatEquivalenceTraits<float>::is_zero(-4.37114e-08f));

			typedef FloatEquivalenceTraits<float> F;

			// Convert an integer to a float and then back again - this process should be non-lossy.
			auto integer_value = F::convert_to_integer(1.5f);
			auto float_value = F::convert_to_float(integer_value);
			
			examiner->check_equal(1.5f, float_value);

			// cos(R90) should normally be considered zero:
			examiner->check(is_zero(std::cos(R90))) << "cos(R90) is zero";
		}
	},

	{"Averages",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			Average<int> s;
			s += 5;
			s += 15;

			examiner->check_equal(s.average(), 10);

			Average<int> q;
			q += 20;
			q += 20;
			q += s;

			examiner->check_equal(q.average(), 15);
		}
	},

	{"Interpolation",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			examiner->check_equal(5.0, linear_interpolate(0.75, -10.0, 10.0));
			examiner->check_equal(-30.0, linear_interpolate(-1.0, -10.0, 10.0));
			examiner->check_equal(-10.0, linear_interpolate(0.0, -10.0, 10.0));
			examiner->check_equal(10.0, linear_interpolate(1.0, -10.0, 10.0));
			examiner->check_equal(30.0, linear_interpolate(2.0, -10.0, 10.0));

			examiner->check_equal(-10.0, cosine_interpolate(0.0, -10.0, 10.0));
			examiner->check_equal(10.0, cosine_interpolate(1.0, -10.0, 10.0));

			// Would be nice to have additional tests for cubic_interpolate and hermite_polynomial
		}
	},
};
