
#include <UnitTest/UnitTest.h>
#include <Euclid/Geometry/Line.h>

UnitTest::Suite TestGeometryLinesSuite {
	"Test Lines",
	
	{"Line Intersections",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Geometry;
			
			Line3 a(ZERO, Vec3{10, 5, 3}.normalize());
			Line3 b({10, 5, 3}, Vec3{5, 2, -8}.normalize());

			RealT ta, tb;
			examiner->check(a.intersects_with(b, ta, tb)) << "Intersection exists";

			Line2 c(ZERO, Vec2{1, 1}.normalize());
			Line2 d({1, 0}, Vec2{-1, 1}.normalize());

			examiner->check(c.intersects_with(d, ta, tb)) << "Intersection exists";
			examiner->check(c.point_at_time(ta).equivalent({0.5, 0.5})) << "Intersection at correct point";
		}
	},
	
	{"Line Transformations",
		[](UnitTest::Examiner * examiner) {
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

			examiner->check(r.equivalent(Vec3(ZERO))) << "Transformed vertex is correct";

			v1 *= 2;
			r = mat * v1;

			examiner->check(r.equivalent({-10, 0, 0})) << "Transformed vertex is correct";

			Line3 q = a.rotated_line(norm, R90);

			examiner->check(q.direction().equivalent({0, 1, 0})) << "Rotated line is correct";
		}
	},
	
	{"Line Segments",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Geometry;
			
			LineSegment3 a(ZERO, 10);
			examiner->check(a.offset().length_squared() == (10 * 10) * 3) << "Line is correct length";

			LineSegment3 d(ZERO);
			examiner->check(d.is_degenerate()) << "Line segment is degenerate";

			Line3 la(IDENTITY, 1);
			LineSegment3 e(la, 10, 30);
			examiner->check(e.offset().equivalent(20)) << "LineSegment is correct size";
			examiner->check(e.start().equivalent(10)) << "LineSegment starts at correct point";
		}
	},
	
	{"Line Segment Intersections",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Geometry;
			
			LineSegment3 a(ZERO, 10);
			LineSegment3 b({0, 10, 0}, {10, 0, 10});

			RealT ta, tb;
			examiner->check(a.intersects_with(b, ta, tb)) << "Line segments intersect";
			examiner->check(a.point_at_time(ta).equivalent({5, 5, 5})) << "Intersection point is correct";

			LineSegment3 c(0, 10.1);
			examiner->check(!a.intersects_with(c, ta, tb)) << "Line segments don't intersect";
		}
	},
};
