//
//  Geometry/Intersection.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 17/11/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#include "Intersection.h"

namespace Euclid
{
	namespace Geometry
	{
// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Intersection)
		{
			testing("Segment Intersection");

			Vec2i overlap;
			//Vector<2, int> s1{0, 5}, s2(2, 4), s3(3, 10), s4(-5, 10), s5(-10, -5), s6(5, 6), o;

			check(segments_intersect({0, 5}, {2, 4}, overlap)) << "Segments overlap";
			check(overlap == Vec2i{2, 4}) << "Segment overlap is correct";

			check(!segments_intersect({0, 5}, {-10, -5}, overlap)) << "Segments don't overlap";

			check(segments_intersect({0, 5}, {3, 10}, overlap)) << "Segments overlap";
			check(overlap == Vec2i{3, 5}) << "Segment overlap is correct";

			check(segments_intersect({0, 5}, {-5, 10}, overlap)) << "Segments overlap";
			check(overlap == Vec2i{0, 5}) << "Segment overlap is correct";

			check(segments_intersect({0, 5}, {5, 6}, overlap)) << "Segments overlap";
			check(overlap == Vec2i{5, 5}) << "Segment overlap is correct";
		}
#endif
	}
}
