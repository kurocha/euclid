//
//  Geometry/Triangle.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 12/07/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#include "Triangle.h"

#include "AlignedBox.h"
#include "Plane.h"
#include "Line.h"

namespace Euclid {
	namespace Geometry {
// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING

		UNIT_TEST(Triangle) {
			testing("Construction");

			Triangle3 t(Vec3(ZERO), Vec3{5.0, 5.0, 5.0}, Vec3{5.0, 5.0, 0.0});

			check(t.normal().dot({-1, 1, 1})) << "Normal is correct";
		}

#endif
	}
}
