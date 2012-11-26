//
//  Geometry/Line.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 15/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "Line.h"

namespace Euclid {
	namespace Geometry {
// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Line)
		{
			testing("Intersections");

			Line3 a(Vec3(0, 0, 0), Vec3(10, 5, 3).normalize());
			Line3 b(Vec3(10, 5, 3), Vec3(5, 2, -8).normalize());

			RealT ta, tb;
			check(a.intersects_with(b, ta, tb)) << "Intersection exists";

			Line2 c(Vec2(0, 0), Vec2(1, 1).normalize());
			Line2 d(Vec2(1, 0), Vec2(-1, 1).normalize());

			check(c.intersects_with(d, ta, tb)) << "Intersection exists";
			check(c.point_at_time(ta).equivalent(Vec2(0.5, 0.5))) << "Intersection at correct point";

			testing("Transformations");

			Vec3 norm(0, 0, 1);
			Vec3 v1(10, 0, 0);
			a.set_point(Vec3(10, 0, 0));
			a.set_direction(Vec3(1, 0, 0));

			b.set_point(Vec3(ZERO));
			b.set_direction(Vec3(-1, 0, 0));

			Mat44 mat = a.transformation_to_mate_with_line(b, norm);
			Vec3 r = mat * v1;

			check(r.equivalent(Vec3(ZERO))) << "Transformed vertex is correct";

			v1 *= 2;
			r = mat * v1;

			check(r.equivalent(Vec3(-10, 0, 0))) << "Transformed vertex is correct";

			//void rotate (const VectorT & rotation_normal, const NumericRealT & angle);
			//LineT rotated_line (const VectorT & rotation_normal, const NumericRealT & angle) const;

			Line3 q = a.rotated_line(norm, R90);

			check(q.direction().equivalent(Vec3(0, 1, 0))) << "Rotated line is correct";
		}

		UNIT_TEST(LineSegment)
		{
			testing("Constructors");

			LineSegment3 a(Vec3(ZERO), Vec3(IDENTITY, 10));
			check(a.offset().length_squared() == (10 * 10) * 3) << "Line is correct length";

			LineSegment3 d(ZERO);
			check(d.is_degenerate()) << "Line segment is degenerate";

			Line3 la(IDENTITY, 1);
			LineSegment3 e(la, 10, 30);
			check(e.offset().equivalent(Vec3(IDENTITY, 20))) << "LineSegment is correct size";
			check(e.start().equivalent(Vec3(IDENTITY, 10))) << "LineSegment starts at correct point";

			testing("Intersections");

			LineSegment3 b(Vec3(0, 10, 0), Vec3(10, 0, 10));

			RealT ta, tb;
			check(a.intersects_with(b, ta, tb)) << "Line segments intersect";
			check(a.point_at_time(ta).equivalent(Vec3(5, 5, 5))) << "Intersection point is correct";

			LineSegment3 c(Vec3(ZERO), Vec3(IDENTITY, 10.1));
			check(!a.intersects_with(c, ta, tb)) << "Line segments don't intersect";
		}
#endif
	}
}
