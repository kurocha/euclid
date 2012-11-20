//
//  Geometry/Sphere.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 18/10/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "Sphere.h"
#include "../Numerics/Number.h"
#include "../Numerics/Vector.h"

namespace Euclid {
	namespace Geometry {
// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING

		UNIT_TEST(Sphere) {
			testing("Sphere-Sphere Intersection");

			Sphere<2> a({-10, 0}, 10);
			Sphere<2> b(Vec2{  0, 0}, 10);
			Sphere<2> c(Vec2{ 20, 0}, 10);

			Vec2 d;
			check(a.intersects_with(b, d) == SHAPES_INTERSECT) << "Spheres overlap";
			check(d.length() == 10.0) << "Spheres displaced by the distance between their centers";

			check(b.intersects_with(c, d) == EDGES_INTERSECT) << "Sphere's edges overlap";
			check(d.length() == 20.0) << "Spheres displaced by the distance between their centers";

			testing("Sphere-Point Intersection");

			check(b.intersects_with(Vec2{5.0, 0.0}, d) == SHAPES_INTERSECT) << "Point lies within sphere";
			check(a.intersects_with(Vec2{5.0, 0.0}, d) == NO_INTERSECTION) << "Point lies outside of sphere";

			testing("Sphere-Line Intersection");

			RealT t1, t2;
			LineSegment<2> segmentA({-50, 0}, {50, 0});
			LineSegment<2> segmentB({-50, -10}, {50, -10});

			check(b.intersects_with(segmentA, t1, t2) == SHAPES_INTERSECT) << "Line and sphere overlap at two points";
			check(segmentA.point_at_time(t1).equivalent(Vec2{-10.0, 0})) << "Line intersects surface of sphere";
			check(segmentA.point_at_time(t2).equivalent(Vec2{10.0, 0})) << "Line intersects surface of sphere";

			check(b.intersects_with(segmentB, t1, t2) == EDGES_INTERSECT) << "Line and sphere touch at one point";
			check(segmentB.point_at_time(t1).equivalent(Vec2{0, -10.0})) << "Line intersects surface of sphere";
		}

#endif
	}
}
