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
		Mat44 rotation (const RealT & radians, const Vec3 & around_normal, const Vec3 & around_point)
		{
			return Mat44::rotating_matrix(radians, around_normal, around_point);
		}

		Mat44 rotation (const RealT & radians, const Vec3 & around_normal)
		{
			return Mat44::rotating_matrix(radians, around_normal);
		}

		Mat44 rotation (const Vec3 & from_unit_vector, const Vec3 & to_unit_vector, const Vec3 & around_normal)
		{
			return Mat44::rotating_matrix(from_unit_vector, to_unit_vector, around_normal);
		}
	}
}
