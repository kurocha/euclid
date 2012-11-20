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

namespace Euclid {
	namespace Geometry {
		template <unsigned D, typename NumericT>
		class Triangle : public Shape<D, 3, NumericT>{
		public:
			Triangle (const Vector<D, NumericT> & p1, const Vector<D, NumericT> & p2, const Vector<D, NumericT> & p3);

			Vector<D, NumericT> normal () const;

			/// @todo Improve method of intersection test as per URI in cpp.
			IntersectionResult intersects_with (const Line<3, NumericT> & line, Vector<D, NumericT> & at) const;

			AlignedBox<D, NumericT> bounding_box ();
		};

		typedef Triangle<3, RealT> Triangle3;
		typedef Triangle<2, RealT> Triangle2;
	}
}

#include "Triangle.impl.h"

#endif
