

#include <UnitTest/UnitTest.h>

#include <Euclid/Numerics/Vector.IO.h>
#include <Euclid/Geometry/AlignedBox.h>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite AlignedBoxTestSuite {
			"Euclid::Geometry::AlignedBox",

			{"Construction",
				[](UnitTest::Examiner * examiner) {
					AlignedBox<2> a(ZERO), b(IDENTITY), c(ZERO);
					examiner->check(a.min().equivalent(0)) << "Zero box";
					examiner->check(a.max().equivalent(0)) << "Zero box";

					examiner->check(b.min().equivalent(0)) << "Identity box";
					examiner->check(b.max().equivalent(1)) << "Identity box";

					examiner->check(a.is_degenerate()) << "Zero box is degenerate";
					examiner->check(b.is_normal()) << "Identity box is normal";
				}
			},

			{"Box-Point Intersection",
				[](UnitTest::Examiner * examiner) {
					AlignedBox<3> box1(vector(0.0, 0.0, 0.0), vector(1.0, 1.0, 1.0)),
					box2(vector(0.0, 0.0, 0.0), vector(10.0, 10.0, 10.0)),
					box3(vector(5.0, 5.0, 5.0), vector(10.0, 10.0, 10.0)),
					box4(vector(9.0, 9.0, 9.0), vector(10.0, 10.0, 10.0)),
					box5(vector(-5.0, 0.0, 0.0), vector(5.0, 10.0, 10.0)),
					box5a(vector(0.0, 0.0, 0.0), vector(5.0, 10.0, 10.0)),
					box6(vector(5.0, 0.0, 0.0), vector(10.0, 10.0, 10.0));

					examiner->check(box1.contains_point(vector(0.0, 0.0, 0.0), true )) << "Contains point was correct";
					examiner->check(!box1.contains_point(vector(0.0, 0.0, 0.0), false)) << "Contains point was correct";
					examiner->check(box1.contains_point(vector(1.0, 1.0, 0.5), true )) << "Contains point was correct";
					examiner->check(!box1.contains_point(vector(1.0, 1.0, 0.5), false)) << "Contains point was correct";
					examiner->check(box1.contains_point(vector(1.0, 1.0, 1.0), true )) << "Contains point was correct";
					examiner->check(!box1.contains_point(vector(1.0, 1.0, 1.0), false)) << "Contains point was correct";
					examiner->check(!box1.contains_point(vector(1.0, 2.0, 1.0), true )) << "Contains point was correct";
					examiner->check(!box1.contains_point(vector(1.0, 1.0, 2.0), false)) << "Contains point was correct";
					examiner->check(!box1.contains_point(vector(3.0, 1.0, 0.0), false)) << "Contains point was correct";
				}
			},

			{"Box-Box Intersection",
				[](UnitTest::Examiner * examiner) {
					AlignedBox<3> box1(vector(0.0, 0.0, 0.0), vector(1.0, 1.0, 1.0)),
						box2(vector(0.0, 0.0, 0.0), vector(10.0, 10.0, 10.0)),
						box3(vector(5.0, 5.0, 5.0), vector(10.0, 10.0, 10.0));

					examiner->check(box1.intersects_with(box2)) << "Intersection result was correct";
					examiner->check(!box1.intersects_with(box3)) << "Intersection result was correct";
				}
			},

			{"Orientation",
				[](UnitTest::Examiner * examiner) {
					AlignedBox<3> box1(vector(0.0, 0.0, 0.0), vector(1.0, 1.0, 1.0));
					
					AlignedBox<3> box1a(box1),
						box2(vector(0.0, 0.0, 0.0), vector(10.0, 10.0, 10.0)),
						box3(vector(5.0, 5.0, 5.0), vector(10.0, 10.0, 10.0)),
						box4(vector(9.0, 9.0, 9.0), vector(10.0, 10.0, 10.0));

					examiner->check(box2.orientation_of(box3).equivalent(1.0)) << "Box orientation was correct";
					box1a.align_within_super_box(box2, vector(1.0, 1.0, 1.0));
					examiner->check(box1a.equivalent(box4)) << "Box was aligned correctly";
					box1a.align_within_super_box(box2, vector(0.0, 0.0, 0.0));
					examiner->check(box1.equivalent(box1a)) << "Box was aligned correctly";

					// Is this desirable behaviour?
					AlignedBox<2> o1(ZERO, 10), o2(ZERO, 5), o3(20, 30);

					examiner->check(o1.orientation_of(o2) == Vec2(0, 0));
					examiner->check(o1.orientation_of(o3) == Vec2(1, 1));
				}
			},

			{"Subtraction",
				[](UnitTest::Examiner * examiner) {
					AlignedBox<3> box1(vector(0.0, 0.0, 0.0), vector(1.0, 1.0, 1.0)),
					box2(vector(0.0, 0.0, 0.0), vector(10.0, 10.0, 10.0)),
					box3(vector(5.0, 5.0, 5.0), vector(10.0, 10.0, 10.0)),
					box4(vector(9.0, 9.0, 9.0), vector(10.0, 10.0, 10.0)),
					box5(vector(-5.0, 0.0, 0.0), vector(5.0, 10.0, 10.0)),
					box5a(vector(0.0, 0.0, 0.0), vector(5.0, 10.0, 10.0)),
					box6(vector(5.0, 0.0, 0.0), vector(10.0, 10.0, 10.0));
					
					AlignedBox<3> box2a(box2);
					// Subtract left half
					box2a.subtract_in_order(box5, vector<unsigned>(0, 1, 2));

					// We should have the right half
					examiner->check(box2a == box6) << "Box was subtracted correctly";

					AlignedBox<3> box2b(box2);
					// Subtract right half
					box2b.subtract_in_order(box6, vector<unsigned>(0, 1, 2));

					// We should have the left half
					examiner->check(box2b == box5a) << "Box was subtracted correctly";

					AlignedBox<3> c1(Vec3(0, 0, 0), Vec3(50, 10, 10)),
					c2(Vec3(5, 5, 0), Vec3(15, 15, 15)),
					c3(Vec3(5, 10, 0), Vec3(15, 15, 15));

					AlignedBox<3> c2a(c2);

					c2a.subtract_in_order(c1, vector<unsigned>(0, 1, 2));
					examiner->check(c2a == c3) << "Box was subtracted correctly";
				}
			},
		};
	}
}
