
#include <UnitTest/UnitTest.h>

#include <Euclid/Geometry/Sphere.h>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite SphereTestSuite {
			"Euclid::Geometry::Sphere",

			{"Sphere-Sphere Intersection",
				[](UnitTest::Examiner * examiner) {
					using namespace Euclid::Geometry;

					Sphere<2> a({-10, 0}, 10);
					Sphere<2> b({0, 0}, 10);
					Sphere<2> c({20, 0}, 10);

					Vec2 d;
					examiner->check(a.intersects_with(b, d) == Intersection::OVERLAP) << "Spheres overlap";
					examiner->check(d.length() == 10.0) << "Spheres displaced by the distance between their centers";

					examiner->check(b.intersects_with(c, d) == Intersection::TOUCH) << "Sphere's edges overlap";
					examiner->check(d.length() == 20.0) << "Spheres displaced by the distance between their centers";
				}
			},

			{"Sphere-Point Intersection",
				[](UnitTest::Examiner * examiner) {
					using namespace Euclid::Geometry;

					Sphere2 a({-10, 0}, 10);
					Sphere2 b({0, 0}, 10);

					Vec2 d;

					examiner->check(b.intersects_with(Vec2{5.0, 0.0}, d) == Intersection::OVERLAP) << "Point lies within sphere";
					examiner->check(a.intersects_with(Vec2{5.0, 0.0}, d) == Intersection::DISJOINT) << "Point lies outside of sphere";
				}
			},


			{"Sphere-Line Intersection",
				[](UnitTest::Examiner * examiner) {
					using namespace Euclid::Geometry;

					Sphere<2> b(Vec2{0, 0}, 10);

					RealT t1, t2;
					LineSegment<2> segmentA({-50, 0}, {50, 0});
					LineSegment<2> segmentB({-50, -10}, {50, -10});

					examiner->check(b.intersects_with(segmentA, t1, t2) == Intersection::OVERLAP) << "Line and sphere overlap at two points";
					examiner->check(segmentA.point_at_time(t1).equivalent(Vec2{-10.0, 0})) << "Line intersects surface of sphere";
					examiner->check(segmentA.point_at_time(t2).equivalent(Vec2{10.0, 0})) << "Line intersects surface of sphere";

					examiner->check(b.intersects_with(segmentB, t1, t2) == Intersection::TOUCH) << "Line and sphere touch at one point";
					examiner->check(segmentB.point_at_time(t1).equivalent(Vec2{0, -10.0})) << "Line intersects surface of sphere";
				}
			},
		};

	}
}
