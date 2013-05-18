
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
			
			{"Line Normals",
				[](UnitTest::Examiner & examiner) {
					Line2 a(ZERO, Vec2(0.5, 0.75).normalize());
					
					Vec2 n1 = normal(a);
					
					examiner << "Line normal is perpendicular." << std::endl;
					examiner.check(n1.dot(a.direction()).equivalent(0));

					LineSegment2 b(Vec2(-5, 2), Vec2(12, 11));

					Vec2 n2 = normal(b);

					examiner << "Line normal is perpendicular." << std::endl;
					examiner.check(n2.dot(b.direction()).equivalent(0));
				}
			},
			
			{"Line Segments",
				[](UnitTest::Examiner & examiner) {
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

			{"Line Segment Clipping",
				[](UnitTest::Examiner & examiner) {
					LineSegment3 segment(-10, 10), result;
					AlignedBox3 box(-5, 5);

					examiner << "Line segment intersects box" << std::endl;
					examiner.check(segment.clip(box, result));

					examiner << "Line segment was clipped correctly" << std::endl;
					examiner.check(result.equivalent({box.min(), box.max()}));
				}
			},
		};

	}
}