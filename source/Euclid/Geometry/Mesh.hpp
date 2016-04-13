//
//  Geometry/Mesh.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 15/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_MESH_H
#define _EUCLID_GEOMETRY_MESH_H

#include "Geometry.hpp"
#include "AlignedBox.hpp"

#include <set>
#include <vector>

namespace Euclid {
	namespace Geometry {
		struct VertexP3N3M2 {
			Vec3 position;
			Vec3 normal;
			Vec2 mapping;

			void apply(const Mat44 & transform);
		};

		struct VertexP3N3M2C4 : public VertexP3N3M2 {
			Vec4 color;
		};

		enum class Layout : uint32_t {
			POINTS = 0,
			LINES = 1,
			LINE_LOOP = 2,
			LINE_STRIP = 3,
			TRIANGLES = 4,
			TRIANGLE_STRIP = 5,
			TRIANGLE_FAN = 6
		};

		/// A mesh is a list of vertices and an ordered list of indices which make up a set of triangles. We assume that all meshes are made up of triangle strips. These assumptions and limitations are primarily to keep the generation of Mesh objects simple.
		template <typename VertexType = VertexP3N3M2, typename IndexType = uint16_t>
		class Mesh {
		public:
			typedef IndexType Index;
			typedef VertexType Vertex;
			typedef std::vector<Index> Indices;
			typedef std::vector<Vertex> Vertices;

			Layout layout;
			Indices indices;
			Vertices vertices;

			// Apply some kind of tranform to all vertices.
			template <typename TransformT>
			void apply(const TransformT & transform) {
				for (std::size_t i = 0; i < vertices.size(); i++)
					vertices[i].apply(transform);
			}

			inline Mesh& operator<<(Index index) {
				indices.push_back(index);

				return *this;
			}

			inline Mesh& operator<<(Vertex vertex) {
				vertices.push_back(vertex);

				return *this;
			}
		};
	}
}

#endif
