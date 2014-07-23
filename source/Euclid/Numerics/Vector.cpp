//
//  Numerics/Vector.cpp
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 19/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "Vector.hpp"

namespace Euclid
{
	namespace Numerics
	{
		template class Vector<2, RealT>;
		template class Vector<3, RealT>;
		template class Vector<4, RealT>;
	}
}
