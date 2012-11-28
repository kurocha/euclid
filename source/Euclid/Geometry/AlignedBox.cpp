//
//  Geometry/AlignedBox.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 12/07/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#include "AlignedBox.h"

namespace Euclid {
	namespace Geometry {
		template class AlignedBox<2, RealT>;
		template class AlignedBox<3, RealT>;
	}
}
