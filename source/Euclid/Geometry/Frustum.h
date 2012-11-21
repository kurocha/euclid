//
//  Geometry/Frustum.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 18/10/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_FRUSTUM_H
#define _EUCLID_GEOMETRY_FRUSTUM_H

#include "Plane.h"
#include "../Numerics/Matrix.h"

namespace Euclid {
	namespace Geometry {
		template <typename NumericT = RealT>
		class Frustum {
		public:
			typedef Plane<3, NumericT> PlaneT;
			typedef Vector<3, NumericT> Vec3T;

		protected:
			PlaneT _planes[6];
			Vec3T _corners[8];

			Vec3T _near_center, _far_center;

			void build_frustrum_from_matrix (const Matrix<4, 4, NumericT> &);

		public:
			Frustum(const Matrix<4, 4, NumericT> & view_matrix) {
				build_frustrum_from_matrix(view_matrix);
			}

			const PlaneT & operator[] (dimension i) const {
				return _planes[i];
			}

			bool intersects_with (const Sphere<3, NumericT> & s) const;
			bool intersects_with (const AlignedBox<3, NumericT> & b) const;

			AlignedBox<3, NumericT> bounding_box() const;

			//bool intersects_with (const Triangle &t) const;

			//bool contains_point (const Vec3 &p) const;

			bool visible(Vec3T planar_normal);
		};

		typedef Frustum<> FrustumT;
	}
}

#include "Frustrum.impl.h"

#endif
