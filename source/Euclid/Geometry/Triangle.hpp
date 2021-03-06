//
//  Geometry/Triangle.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 15/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_TRIANGLE_H
#define _EUCLID_GEOMETRY_TRIANGLE_H

#include "Geometry.hpp"
#include "Plane.hpp"

namespace Euclid {
	namespace Geometry {
		template <dimension D, typename NumericT>
		class Triangle : public Shape<D, 3, NumericT> {
		public:
			template <typename... ArgumentsT>
			Triangle(ArgumentsT... arguments) : Shape<D, 3, NumericT>(arguments...)
			{
			}

			AlignedBox<D, NumericT> bounding_box ()
			{
				AlignedBox<D, NumericT> box((*this)[0], (*this)[1]);

				box.union_with_point((*this)[2]);

				return box;
			}
		};

		typedef Triangle<2, RealT> Triangle3;
		typedef Triangle<3, RealT> Triangle2;

		extern template class Triangle<2, RealT>;
		extern template class Triangle<3, RealT>;
	}
}

#endif
