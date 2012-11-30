//
//  Geometry/Generate/Sphere.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 9/03/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_GENERATE_SPHERE_H
#define _EUCLID_GEOMETRY_GENERATE_SPHERE_H

#include "Basic.h"

namespace Euclid {
	namespace Geometry {
		namespace Generate {
			template <typename MeshT>
			void sphere(MeshT & mesh, RealT radius, std::size_t stacks = 8, std::size_t slices = 8) {
				for (std::size_t stack = 0; stack <= stacks; stack += 1) {
					for (std::size_t slice = 0; slice < slices; slice += 1) {
						RealT theta = stack * (R180 / stacks);
						RealT phi = slice * (R360 / (slices - 1));

						RealT sin_theta = std::sin(theta);
						RealT sin_phi = std::sin(phi);
						RealT cos_theta = std::cos(theta);
						RealT cos_phi = std::cos(phi);

						typename MeshT::VertexT vertex;
						vertex.position = Vec3(radius * cos_phi * sin_theta, radius * sin_phi * sin_theta, radius * cos_theta);
						vertex.normal = vertex.position.normalize();
						vertex.mapping = Vec2(RealT(slice) / RealT(slices - 1), RealT(stack) / RealT(stacks));

						mesh.vertices.push_back(vertex);
					}
				}

				for (std::size_t stack = 0; stack < stacks; stack += 1) {
					for (std::size_t slice = 0; slice <= slices; slice += 1) {
						mesh.indices.push_back((stack * slices) + (slice % slices));
						mesh.indices.push_back(((stack + 1) * slices) + (slice % slices));
					}
				}
			}
		}
	}
}

#endif
