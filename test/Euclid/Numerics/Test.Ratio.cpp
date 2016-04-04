
#include <UnitTest/UnitTest.hpp>

#include <Euclid/Numerics/Ratio.hpp>

namespace Euclid
{
	namespace Numerics
	{
		UnitTest::Suite RatioTestSuite {
			"Euclid::Numerics::Ratio",

			{"Ratios",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Numerics;

					auto size = ratios(Vec2{10.0, 20.0});
					examiner.check_equal(size.aspect_ratio(), 0.5);

					auto resized_smaller = ratios(Vec2{20.0, 10.0}).shrink_to_fit_aspect_ratio(0.5);
					examiner.check_equal(resized_smaller, Vec2{5.0, 10.0});

					auto resized_larger = ratios(Vec2{20.0, 10.0}).expand_to_fit_aspect_ratio(0.5);
					examiner.check_equal(resized_larger, Vec2{20.0, 40.0});
				}
			},
		};
	}
}
