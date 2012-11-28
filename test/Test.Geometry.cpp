
#include <UnitTest/UnitTest.h>

#include <Euclid/Numerics/Vector.IO.h>
#include <Euclid/Geometry/Geometry.h>
#include <Euclid/Geometry/Intersection.h>
#include <Euclid/Geometry/Triangle.h>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite GeometryTestSuite {
			"Euclid::Geometry",

			{"Shape",
				[](UnitTest::Examiner * examiner) {
					using namespace Euclid::Geometry;

					typedef Shape<2, 4, int> ShapeT;

					ShapeT s;
					s.points[0] = {-5, -5};
					s.points[1] = {5, -5};
					s.points[2] = {5, 5};
					s.points[3] = {-5, 5};

					examiner->check_equal(s.center(), Vec2i(0));
				}
			},

			{"Intersection",
				[](UnitTest::Examiner * examiner) {
					using namespace Euclid::Geometry;

					Vec2i overlap;
					//Vector<2, int> s1{0, 5}, s2(2, 4), s3(3, 10), s4(-5, 10), s5(-10, -5), s6(5, 6), o;

					examiner->check(segments_intersect({0, 5}, {2, 4}, overlap)) << "Segments overlap";
					examiner->check(overlap == Vec2i{2, 4}) << "Segment overlap is correct";

					examiner->check(!segments_intersect({0, 5}, {-10, -5}, overlap)) << "Segments don't overlap";

					examiner->check(segments_intersect({0, 5}, {3, 10}, overlap)) << "Segments overlap";
					examiner->check(overlap == Vec2i{3, 5}) << "Segment overlap is correct";

					examiner->check(segments_intersect({0, 5}, {-5, 10}, overlap)) << "Segments overlap";
					examiner->check(overlap == Vec2i{0, 5}) << "Segment overlap is correct";

					examiner->check(segments_intersect({0, 5}, {5, 6}, overlap)) << "Segments overlap";
					examiner->check(overlap == Vec2i{5, 5}) << "Segment overlap is correct";
				}
			},

			{"Triangle",
				[](UnitTest::Examiner * examiner) {
					using namespace Euclid::Geometry;

					Triangle3 t(Vec3(ZERO), Vec3{5.0, 5.0, 5.0}, Vec3{5.0, 5.0, 0.0});
					
					examiner->check(surface_normal(t).dot({-1, 1, 1})) << "Normal is correct";
				}
			},
		};
	}
}
