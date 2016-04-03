
#include <UnitTest/UnitTest.hpp>

#include <Euclid/Geometry/Sphere.hpp>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite SphereTestSuite {
			"Euclid::Geometry::Sphere",

			{"Sphere-Sphere Intersection",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Geometry;

					Sphere<2> a({-10, 0}, 10);
					Sphere<2> b({0, 0}, 10);
					Sphere<2> c({20, 0}, 10);

					Vec2 d;
					examiner << "Spheres overlap" << std::endl;
					examiner.check(a.intersects_with(b, d) == Intersection::OVERLAP);
					examiner << "Spheres displaced by the distance between their centers" << std::endl;
					examiner.check(d.length() == 10.0);

					examiner << "Sphere's edges overlap" << std::endl;
					examiner.check(b.intersects_with(c, d) == Intersection::TOUCH);
					examiner << "Spheres displaced by the distance between their centers" << std::endl;
					examiner.check(d.length() == 20.0);
				}
			},

			{"Sphere-Point Intersection",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Geometry;

					Sphere2 a({-10, 0}, 10);
					Sphere2 b({0, 0}, 10);

					Vec2 d;

					examiner << "Point lies within sphere" << std::endl;
					examiner.check(b.intersects_with(Vec2{5.0, 0.0}, d) == Intersection::OVERLAP);
					examiner << "Point lies outside of sphere" << std::endl;
					examiner.check(a.intersects_with(Vec2{5.0, 0.0}, d) == Intersection::DISJOINT);
				}
			},


			{"Sphere-Line Intersection",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Geometry;

					Sphere<2> b(Vec2{0, 0}, 10);

					RealT t1, t2;
					LineSegment<2> segmentA({-50, 0}, {50, 0});
					LineSegment<2> segmentB({-50, -10}, {50, -10});

					examiner << "Line and sphere overlap at two points" << std::endl;
					examiner.check(b.intersects_with(segmentA, t1, t2) == Intersection::OVERLAP);
					examiner << "Line intersects surface of sphere" << std::endl;
					examiner.check(segmentA.point_at_time(t1).equivalent(Vec2{-10.0, 0}));
					examiner << "Line intersects surface of sphere" << std::endl;
					examiner.check(segmentA.point_at_time(t2).equivalent(Vec2{10.0, 0}));

					examiner << "Line and sphere touch at one point" << std::endl;
					examiner.check(b.intersects_with(segmentB, t1, t2) == Intersection::TOUCH);
					examiner << "Line intersects surface of sphere" << std::endl;
					examiner.check(segmentB.point_at_time(t1).equivalent(Vec2{0, -10.0}));
				}
			},
		};

	}
}
