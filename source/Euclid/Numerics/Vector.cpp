//
//  Numerics/Vector.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 19/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "Vector.h"

namespace Euclid
{
	namespace Numerics
	{
		extern template class Vector<2, RealT>;
		extern template class Vector<3, RealT>;
		extern template class Vector<4, RealT>;
	}
}
