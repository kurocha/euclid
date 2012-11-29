//
//  Geometry/Generate/Planar.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 9/03/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_GENERATE_PLANAR_H
#define _EUCLID_GEOMETRY_GENERATE_PLANAR_H

#include "Basic.h"

namespace Euclid {
	namespace Geometry {
		namespace Generate {
			/// Size is actually a mesure from the center to the outside.
			template <typename MeshT>
			void plane(MeshT & mesh, Vec2 size) {
				// Draw with GL_TRIANGLE_FAN

				// We make a plane out of four triangles.

				// Center point:
				typename MeshT::VertexT vertex;

				vertex.position = ZERO;
				vertex.mapping = Vec2(0.5, 0.5);

				mesh.vertices.push_back(vertex);

				// The four surrounding points:
				RealT coordinates[4][2] = {
					{-1, -1}, {1, -1}, {1, 1}, {-1, 1}
				};

				RealT mappings[4][2] = {
					{0, 0}, {1, 0}, {1, 1}, {0, 1}
				};

				for (std::size_t i = 0; i < 4; i += 1) {
					typename MeshT::VertexT vertex;

					vertex.position = 0;
					vertex.position.set(size * Vec2(coordinates[i][0], coordinates[i][1]));

					vertex.mapping = Vec2(mappings[i][0], mappings[i][1]);

					mesh.vertices.push_back(vertex);
				}

				// The indices for the triangle fan:
				typename MeshT::std::size_t indices[] = {
					0, 1, 2, 3, 4, 1
				};

				std::copy(indices, indices + (sizeof(indices) / sizeof(*indices)), std::back_inserter(mesh.indices));
			}

			template <typename MeshT>
			void plane(MeshT & mesh, AlignedBox2 box, Vec2u divisions) {
				for (std::size_t y = 0; y <= divisions[Y]; y += 1) {
					for (std::size_t x = 0; x <= divisions[X]; x += 1) {
						typename MeshT::VertexT vertex;

						vertex.position = box.min() + ((box.size() / divisions) * Vec2u(x, y));
						vertex.mapping = Vec2(x, y);

						mesh.vertices.push_back(vertex);
					}
				}

				std::size_t width = divisions[Y] + 1;

				for (std::size_t y = 0; y < divisions[Y]; y += 1) {
					if (y != 0) {
						// Create two degenerate triangles for the move from the end of the row back to the start:
						mesh.indices.push_back(row_major_offset(y, divisions[X], width));
						mesh.indices.push_back(row_major_offset(y, 0, width));
					}

					for (std::size_t x = 0; x < divisions[X]; x += 1) {
						mesh.indices.push_back(row_major_offset(y, x, width));
						mesh.indices.push_back(row_major_offset(y+1, x, width));
						mesh.indices.push_back(row_major_offset(y, x+1, width));
						mesh.indices.push_back(row_major_offset(y+1, x+1, width));
					}
				}
			}

			template <typename MeshT>
			void grid(MeshT & mesh, Vec2u count, Vec2 spacing) {
				Vec2 size = (count * spacing) / 2.0;
				AlignedBox2 surface = AlignedBox2::from_center_and_size(ZERO, size * 2.0);

				for (RealT y = -size[Y]; y <= size[Y]; y += spacing[Y]) {
					typename MeshT::VertexT first, second;
					first.position = Vec3(-size[X], y, 0.0);
					first.normal = Vec3(0, 0, 1);
					first.mapping = surface.relative_offset_of(first.position.reduce());
					mesh.vertices.push_back(first);

					second.position = Vec3(size[X], y, 0.0);
					second.normal = Vec3(0, 0, 1);
					second.mapping = surface.relative_offset_of(second.position.reduce());
					mesh.vertices.push_back(second);

					mesh.indices.push_back(mesh.vertices.size() - 1);
					mesh.indices.push_back(mesh.vertices.size() - 2);
				}

				for (RealT x = -size[X]; x <= size[X]; x += spacing[X]) {
					typename MeshT::VertexT first, second;
					first.position = Vec3(x, -size[Y], 0.0);
					first.normal = Vec3(0, 0, 1);
					first.mapping = surface.relative_offset_of(first.position.reduce());
					mesh.vertices.push_back(first);

					second.position = Vec3(x, size[Y], 0.0);
					second.normal = Vec3(0, 0, 1);
					second.mapping = surface.relative_offset_of(second.position.reduce());
					mesh.vertices.push_back(second);

					mesh.indices.push_back(mesh.vertices.size() - 1);
					mesh.indices.push_back(mesh.vertices.size() - 2);
				}
			}
		}
	}
}

#endif
