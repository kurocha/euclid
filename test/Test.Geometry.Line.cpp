
#include <UnitTest/UnitTest.h>
#include <Euclid/Geometry/Line.h>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite LineTestSuite {
			"Euclid::Geometry::Line",

			{"Line Intersections",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Geometry;

					Line3 a(ZERO, Vec3{10, 5, 3}.normalize());
					Line3 b({10, 5, 3}, Vec3{5, 2, -8}.normalize());

					RealT ta, tb;
					examiner << "Intersection exists between lines" << std::endl;
					examiner.check(a.intersects_with(b, ta, tb));

					Line2 c(ZERO, Vec2{1, 1}.normalize());
					Line2 d({1, 0}, Vec2{-1, 1}.normalize());

					examiner << "Intersection exists between lines" << std::endl;
					examiner.check(c.intersects_with(d, ta, tb));

					examiner << "The intersection was at the correct point" << std::endl;
					examiner.check(c.point_at_time(ta).equivalent({0.5, 0.5}));
				}
			},
			/*
			 {"Line Transformations",
			 [](UnitTest::Examiner & examiner) {
			 using namespace Euclid::Geometry;

			 Line3 a(ZERO, Vec3{10, 5, 3}.normalize());
			 Line3 b({10, 5, 3}, Vec3{5, 2, -8}.normalize());

			 Vec3 norm = {0, 0, 1};
			 Vec3 v1 = {10, 0, 0};
			 a.set_point({10, 0, 0});
			 a.set_direction({1, 0, 0});

			 b.set_point(Vec3(ZERO));
			 b.set_direction({-1, 0, 0});

			 Mat44 mat = a.transformation_to_mate_with_line(b, norm);
			 Vec3 r = mat * v1;

			 examiner.check(r.equivalent(Vec3(ZERO))) << "Transformed vertex is correct";

			 v1 *= 2;
			 r = mat * v1;

			 examiner.check(r.equivalent({-10, 0, 0})) << "Transformed vertex is correct";

			 Line3 q = a.rotated_line(norm, R90);

			 examiner.check(q.direction().equivalent({0, 1, 0})) << "Rotated line is correct";
			 }
			 },
			 */
			{"Line Segments",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Geometry;

					LineSegment3 a(0, 10);
					examiner << "Line is correct length" << std::endl;
					examiner.check(a.offset().length_squared() == (10 * 10) * 3);

					LineSegment3 d(ZERO);
					examiner << "Line segment is degenerate" << std::endl;
					examiner.check(d.is_degenerate());

					Line3 la(1);
					LineSegment3 e(la, 10, 30);
					examiner << "LineSegment is correct size" << std::endl;
					examiner.check(e.offset().equivalent(20));
					examiner << "LineSegment starts at correct point" << std::endl;
					examiner.check(e.start().equivalent(10));
				}
			},

			{"Line Segment Intersections",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Geometry;

					LineSegment3 a(ZERO, 10);
					LineSegment3 b({0, 10, 0}, {10, 0, 10});

					RealT ta, tb;
					examiner << "Line segments intersect" << std::endl;
					examiner.check(a.intersects_with(b, ta, tb));
					examiner << "Intersection point is correct" << std::endl;
					examiner.check(a.point_at_time(ta).equivalent({5, 5, 5}));
					
					LineSegment3 c(0, 10.1);
					examiner << "Line segments don't intersect" << std::endl;
					examiner.check(!a.intersects_with(c, ta, tb));
				}
			},
		};

	}
}