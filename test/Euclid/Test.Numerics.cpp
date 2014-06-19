
#include <UnitTest/UnitTest.h>

#include <Euclid/Numerics/Numerics.h>
#include <Euclid/Numerics/Angle.h>
#include <Euclid/Numerics/Integer.h>
#include <Euclid/Numerics/Float.h>
#include <Euclid/Numerics/Average.h>
#include <Euclid/Numerics/Interpolate.h>

#include <iomanip>

namespace Euclid {
	namespace Numerics {
		template <typename FloatT>
		void calculate_accuracy() {
			using F = FloatEquivalenceTraits<FloatT>;

			auto epsilon = EpsilonTraits<FloatT, 0>::EPSILON;
			auto zero = F::convert_to_integer(FloatT(0.0));
			auto zero_plus_epsilon = F::convert_to_integer(FloatT(0.0) + EpsilonTraits<FloatT, 0>::EPSILON);

			std::cout << "Accuracy of float(" << sizeof(FloatT) << ") = " << (zero_plus_epsilon - zero) << "; epsion = " << epsilon << std::endl;
		}

		UnitTest::Suite NumericsTestSuite {
			"Euclid::Numerics",

			{"Accuracy",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Numerics;

					calculate_accuracy<float>();
					calculate_accuracy<double>();

					// Floating point precision is about 7 places.
					examiner << "Float values are equal" << std::endl;
					examiner.check(equivalent<float>(0.1, 0.1000000001));

					// We expect better accuracy for double precision:
					examiner << "Float values are equal" << std::endl;
					examiner.check(!equivalent<double>(0.1, 0.1000000001));

					auto a = number(0.000001f), b = number(0.0000011f), c = number(0.000002f);

					examiner << "Similar numbers are equivalent." << std::endl;
					examiner.check(a.equivalent(b));

					examiner << "Different numbers are not equivalent." << std::endl;
					examiner.check(!a.equivalent(c));

					
				}
			},

			{"Identities",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Numerics;

					float a = ZERO;
					float b = IDENTITY;

					examiner.check_equal(a, 0.0f);
					examiner.check_equal(b, 1.0f);
				}
			},

			{"Angles",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Numerics;

					examiner << "R45 is correct" << std::endl;
					examiner.check(equivalent(R45 * 2.0, R90));
					examiner << "R90 is correct" << std::endl;
					examiner.check(equivalent(R90 * 2.0, R180));
					examiner << "R360 is correct" << std::endl;
					examiner.check(equivalent(R180 * 2.0, R360));

					examiner.check_equal(M_PI_2, R90.value);

					examiner.check_equal(R90.sin(), 1.0);

					auto r = R90;

					examiner << "Angle was divided." << std::endl;
					examiner.check((r / 2).equivalent(R45));

					r /= 3;

					examiner << "In-place division" << std::endl;
					examiner.check(r.equivalent(R30));
				}
			},

			{"Squares",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Numerics;

					examiner.check_equal(power_of_2(0U), 2);
					examiner.check_equal(power_of_2(15U), 16);
					examiner.check_equal(power_of_2(16U), 16);
					examiner.check_equal(power_of_2(17U), 32);
				}
			},

			{"Floating Point",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Numerics;

					examiner.check_equal(FloatEquivalenceTraits<float>::convert_to_integer(0.0f), 0);
					examiner.check_equal(FloatEquivalenceTraits<float>::convert_to_integer(-0.0f), 0);
					examiner.check_equal(FloatEquivalenceTraits<double>::convert_to_integer(0.0), 0);
					examiner.check_equal(FloatEquivalenceTraits<double>::convert_to_integer(-0.0), 0);

					examiner.check(FloatEquivalenceTraits<float>::equivalent(0.0f, -4.37114e-08f));

					typedef FloatEquivalenceTraits<float> F;

					// Convert an integer to a float and then back again - this process should be non-lossy.
					auto integer_value = F::convert_to_integer(1.5f);
					auto float_value = F::convert_to_float(integer_value);

					examiner.check_equal(1.5f, float_value);

					// cos(R90) should normally be considered zero:
					examiner << "cos(R90) is zero = " << R90.cos() << std::endl;
					examiner.check(R90.cos().equivalent(0.0));
				}
			},

			{"Averages",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Numerics;

					Average<int> s;
					s += 5;
					s += 15;

					examiner.check_equal(s.value(), 10);

					Average<int> q;
					q += 20;
					q += 20;
					q += s;

					examiner.check_equal(q.value(), 15);
				}
			},

			{"Interpolation",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Numerics;

					examiner.check_equal(5.0, linear_interpolate(0.75, -10.0, 10.0));
					examiner.check_equal(-30.0, linear_interpolate(-1.0, -10.0, 10.0));
					examiner.check_equal(-10.0, linear_interpolate(0.0, -10.0, 10.0));
					examiner.check_equal(10.0, linear_interpolate(1.0, -10.0, 10.0));
					examiner.check_equal(30.0, linear_interpolate(2.0, -10.0, 10.0));
					
					examiner.check_equal(-10.0, cosine_interpolate(0.0, -10.0, 10.0));
					examiner.check_equal(10.0, cosine_interpolate(1.0, -10.0, 10.0));
					
					// Would be nice to have additional tests for cubic_interpolate and hermite_polynomial
				}
			},
		};
	}
}