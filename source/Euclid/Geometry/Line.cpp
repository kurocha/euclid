//
//  Geometry/Line.cpp
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 15/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "Line.h"

namespace Euclid {
	namespace Geometry {
		template class Line<2, RealT>;
		template class Line<3, RealT>;

		template class LineSegment<2, RealT>;
		template class LineSegment<3, RealT>;
	}
}
