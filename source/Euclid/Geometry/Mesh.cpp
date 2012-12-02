//
//  Geometry/Mesh.cpp
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 15/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "Mesh.h"

#include "../Numerics/Matrix.Multiply.h"
#include "Triangle.h"
#include "Line.h"

namespace Euclid {
	namespace Geometry {
		void VertexP3N3M2::apply(const Mat44 & transform) {
			Vec3 new_position = transform * position;
			normal = ((transform * (position + normal)) - new_position).normalize();
			position = new_position;
		}
	}
}
