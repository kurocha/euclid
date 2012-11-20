//
//  Geometry/Geometry.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 11/11/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#include "Geometry.h"

namespace Euclid
{
	namespace Geometry
	{
// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Shapes)
		{
			testing("Construction");

			typedef Shape<2, 4, int> ShapeT;

			ShapeT s;
			s[0] = {-5, -5};
			s[1] = {5, -5};
			s[2] = {5, 5};
			s[3] = {-5, 5};

			check(s.center() == Vec2i{0, 0}) << "Center is located correctly";
		}
#endif
	}
}
