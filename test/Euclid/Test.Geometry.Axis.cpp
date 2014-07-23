
#include <UnitTest/UnitTest.hpp>

#include <Euclid/Geometry/Axis.hpp>

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
					examiner.check(a1.rotation().angle().equivalent(0_deg));

					auto a2 = Axis<RealT>{{1, 2, 3}, rotate<Z>(R90)};

					examiner << "Axis has correct translation." << std::endl;
					examiner.check(a2.translation().equivalent({1, 2, 3}));

					examiner << "Axis has correct rotation." << std::endl;
					examiner.check(a2.rotation().equivalent((Quat)rotate<Z>(R90)));

					// This transform should move any points from axis-space to origin-space
					auto p1 = a2.to_origin() * Vec3(1, 2, 3);
					
					/// This transform should move any points from origin-space to axis-space
					auto p2 = a2.from_origin() * Vec3(0);

					examiner << "Point is transformed to origin." << std::endl;
					examiner.check(p1.equivalent(0));

					examiner << "Point is transformed from origin." << std::endl;
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

					auto m1 = a1.mate_with(a2);

					auto p1 = m1 * Vec3(10, 10, 10);

					examiner << "Point is translated." << std::endl;
					examiner.check(p1.equivalent({-5, -5, -5}));

					auto p2 = m1 * Vec3(10, 10, 15);

					examiner << "Point is translated." << std::endl;
					examiner.check(p2.equivalent({-5, -5, 0}));

					auto p3 = m1 * Vec3(10, 15, 10);

					examiner << "Point is translated and rotated." << std::endl;
					examiner.check(p3.equivalent({-10, -5, -5}));

					auto m2 = a1.mate_with(a2, translate(Vec3{1, 1, 1}));

					auto p4 = m2 * Vec3(10, 10, 10);

					examiner << "Point is transformed with intermediate translation." << std::endl;
					examiner.check(p4.equivalent({-6, -4, -4}));
				}
			},
		};
	}
}
