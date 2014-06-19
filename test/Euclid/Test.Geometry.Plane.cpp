
#include <UnitTest/UnitTest.h>

#include <Euclid/Geometry/Plane.h>
#include <Euclid/Geometry/Line.h>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite PlaneSuite {
			"Euclid::Geometry::Plane",
			
			{"Plane-Line Intersection",
				[](UnitTest::Examiner & examiner) {
					Plane3 p1(10.0, {1.0, 0.0, 0.0});

					examiner << "Closest point is correct" << std::endl;
					examiner.check(p1.closest_point(ZERO) == Vec3{10, 0, 0});

					Plane3 p2(10.0, Vec3(0.0, 1.0, 0.0));
					Line3 l1, l2(Vec3(10, 10, 0), Vec3(0, 0, 1));

					examiner << "Planes intersect" << std::endl;
					examiner.check(p1.intersects_with(p2, l1));
					examiner << "Planes intersect at correct line" << std::endl;
					examiner.check(l1.equivalent(l2));
				}
			},
		};
	}
}
