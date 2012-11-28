//
//  Geometry/Generate/Cube.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 9/03/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_GENERATE_CUBE_H
#define _EUCLID_GEOMETRY_GENERATE_CUBE_H

#include "Basic.h"

namespace Euclid {
	namespace Geometry {
		namespace Generate {
			template <typename VertexT>
			void simple_cube(Mesh<VertexT> & mesh, const AlignedBox<3> & box)
			{
				const std::size_t CUBE_3X2[] = {
					1, 0, 3, 2,
					3, 2, 7, 6,
					7, 6, 5, 4,
					4, 6, 6, 6, // Degenerate
					6, 2, 4, 0,
					4, 0, 5, 1,
					5, 1, 7, 3
				};

				const Vec3u K = 2;
				const std::size_t SIZE = sizeof(CUBE_3X2) / sizeof(std::size_t);

				mesh.vertices.resize(SIZE);
				mesh.indices.resize(SIZE);

				for (std::size_t c = 0; c < SIZE; c += 1) {
					//std::cout << "c: " << c << " = "  << K.distribute(c) << std::endl;
					Vector<3, bool> current_corner(K.distribute(c));

					mesh.vertices[c].position = box.corner(current_corner);
					mesh.indices[c] = CUBE_3X2[c];
				}
			}

			template <typename VertexT>
			void simple_cube_shade(Mesh<VertexT> & mesh)
			{
				// Look at each face
				for (std::size_t c = 0; c < mesh.vertices.size(); c += 4) {
					Vec3 normal = surface_normal(mesh.vertices[c].position, mesh.vertices[c+1].position, mesh.vertices[c+2].position);

					mesh.vertices[c+0].normal = normal;
					mesh.vertices[c+1].normal = normal;
					mesh.vertices[c+2].normal = normal;
					mesh.vertices[c+3].normal = normal;

					mesh.vertices[c+0].mapping = Vec2(1, 0);
					mesh.vertices[c+1].mapping = Vec2(0, 0);
					mesh.vertices[c+2].mapping = Vec2(1, 1);
					mesh.vertices[c+3].mapping = Vec2(0, 1);
				}
			}
		}
	}
}

#endif
