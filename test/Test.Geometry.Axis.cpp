
#include <UnitTest/UnitTest.h>

#include <Euclid/Geometry/Axis.h>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite AxisTestSuite {
			"Euclid::Geometry::Axis",

			{"Construction",
				[](UnitTest::Examiner & examiner) {
					auto a1 = Axis<RealT>{IDENTITY};

					examiner << "Identity axis has zero translation." << std::endl;
					examiner.check(a1.translation().equivalent(0));

					examiner << "Identity axis has zero rotation." << std::endl;
					examiner.check(a1.rotation().angle().equivalent(0));
				}
			},
		};
	}
}
