//
//  Geometry/Generate/Basic.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 14/4/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_GENERATE_BASIC_H
#define _EUCLID_GEOMETRY_GENERATE_BASIC_H

#include "../Mesh.hpp"

namespace Euclid {
	namespace Geometry {
		namespace Generate {
			template <typename MeshT, typename ColorT>
			void solid_color(MeshT & mesh, const ColorT & color)
			{
				for (auto & vertex : mesh.vertices) {
					vertex.color = color;
				}
			}

			template <typename MeshT, typename NormalT>
			void solid_normal(MeshT & mesh, const NormalT & normal) {
				for (auto & vertex : mesh.vertices) {
					vertex.normal = normal;
				}
			}
		}
	}
}

#endif
