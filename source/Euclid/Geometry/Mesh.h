//
//  Geometry/Mesh.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 15/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_MESH_H
#define _EUCLID_GEOMETRY_MESH_H

#include "Geometry.h"
#include "AlignedBox.h"

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

		enum Layout {
			POINTS = 0,
			LINES = 1,
			LINE_LOOP = 2,
			LINE_STRIP = 3,
			TRIANGLES = 4,
			TRIANGLE_STRIP = 5,
			TRIANGLE_FAN = 6
		};

		/// A mesh is a list of vertices and an ordered list of indices which make up a set of triangles. We assume that all meshes are made up of triangle strips. These assumptions and limitations are primarily to keep the generation of Mesh objects simple.
		template <typename _VertexT = VertexP3N3M2, typename _IndexT = uint16_t>
		class Mesh {
		public:
			typedef _IndexT IndexT;
			typedef _VertexT VertexT;

			Layout layout;
			std::vector<IndexT> indices;
			std::vector<VertexT> vertices;

			// Apply some kind of tranform to all vertices.
			template <typename TransformT>
			void apply(const TransformT & transform) {
				for (std::size_t i = 0; i < vertices.size(); i++)
					vertices[i].apply(transform);
			}

			inline Mesh& operator<<(IndexT index) {
				indices.push_back(index);

				return *this;
			}

			inline Mesh& operator<<(VertexT vertex) {
				vertices.push_back(vertex);

				return *this;
			}
		};
	}
}

#endif
