//
//  Geometry/Triangle.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 15/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_TRIANGLE_H
#define _EUCLID_GEOMETRY_TRIANGLE_H

#include "Geometry.h"
#include "Vector.Geometry.h"
#include "Plane.h"

namespace Euclid {
	namespace Geometry {
		template <dimension D, typename NumericT>
		class Triangle : public Shape<D, 3, NumericT> {
		public:
			template <typename... ArgumentsT>
			Triangle(ArgumentsT... arguments) : Shape<D, 3, NumericT>(arguments...) {}

			AlignedBox<D, NumericT> bounding_box ();
		};

		template <dimension D, typename NumericT>
		AlignedBox<D, NumericT> Triangle<D, NumericT>::bounding_box ()
		{
			AlignedBox<D, NumericT> box(this->_points[0], this->_points[1]);

			box.union_with_point(this->_points[2]);
			
			return box;
		}

		typedef Triangle<2, RealT> Triangle3;
		typedef Triangle<3, RealT> Triangle2;

		extern template class Triangle<2, RealT>;
		extern template class Triangle<3, RealT>;
	}
}

#endif
