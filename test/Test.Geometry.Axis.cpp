
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

					auto a2 = Axis<RealT>{{1, 2, 3}, rotate<Z>(R90)};

					examiner << "Axis has correct translation." << std::endl;
					examiner.check(a2.translation().equivalent({1, 2, 3}));

					examiner << "Axis has correct rotation." << std::endl;
					examiner.check(a2.rotation().equivalent((Quat)rotate<Z>(R90)));

					// This transform should move any points from axis-space to origin-space
					auto p1 = a2.to_origin() * Vec3(1, 2, 3);
					
					/// This transform should move any points from origin-space to axis-space
					auto p2 = a2.from_origin() * Vec3(0);

					examiner << "Point is transformed to origin correctly." << std::endl;
					examiner.check(p1.equivalent(0));

					examiner << "Point is transformed from origin correctly." << std::endl;
					examiner.check(p2.equivalent({1, 2, 3}));

					auto p3 = a2.to_origin() * Vec3(2, 2, 3);

					examiner << "Point is rotated correctly around Z." << std::endl;
					examiner.check(p3.equivalent({0, 1, 0}));
				}
			},

			{"Axis-Axis Mating",
				[](UnitTest::Examiner & examiner) {
					auto a1 = Axis<RealT>({10, 10, 10}, IDENTITY);
					auto a2 = Axis<RealT>({-5, -5, -5}, rotate<Z>(R90));

					auto m = a1.mate_with(a2);

					auto p1 = m * Vec3(10, 10, 10);

					examiner << "Point is translated correctly" << std::endl;
					examiner.check(p1.equivalent({-5, -5, -5}));

					auto p2 = m * Vec3(10, 10, 15);

					examiner << "Point is translated correctly" << std::endl;
					examiner.check(p2.equivalent({-5, -5, 0}));

					auto p3 = m * Vec3(10, 15, 10);

					examiner << "Point is translated and rotated correctly" << std::endl;
					examiner.check(p3.equivalent({-10, -5, -5}));
				}
			},
		};
	}
}
