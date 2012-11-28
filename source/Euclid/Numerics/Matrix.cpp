//
//  Numerics/Matrix.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 13/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "Matrix.h"

namespace Euclid
{
	namespace Numerics
	{
		template class Matrix<4, 4, RealT>;
		template class Matrix<3, 3, RealT>;
		template class Matrix<2, 2, RealT>;
	}
}
