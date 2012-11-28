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
		template class Triangle<2>;
		template class Triangle<3>;
	}
}
