//
//  Geometry/Plane.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 23/09/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "Plane.h"

#include "Line.h"
#include "Triangle.h"

#include <iostream>

namespace Euclid {
	namespace Geometry {
// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Plane)
		{
			testing("Intersection");

			Plane3 p1(10.0, {1.0, 0.0, 0.0});

			check(p1.closest_point(ZERO) == Vec3{10, 0, 0}) << "Closest point is correct";

			Plane3 p2(10.0, Vec3(0.0, 1.0, 0.0));
			Line3 l1, l2(Vec3(10, 10, 0), Vec3(0, 0, 1));

			check(p1.intersects_with(p2, l1)) << "Planes intersect";
			check(l1.equivalent(l2)) << "Planes intersect at correct line";
		}
#endif
	}
}
