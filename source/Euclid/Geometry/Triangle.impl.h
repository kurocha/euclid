//
//  Geometry/Triangle.impl.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 2/12/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_TRIANGLE_H
#error This header should not be included manually. Include Plane.h instead.
#endif

#include "Plane.h"

namespace Euclid
{
	namespace Geometry
	{
		/// http://www.blackpawn.com/texts/pointinpoly/default.html
		template <typename NumericT>
		bool same_side(const Vector<3, NumericT> &p1, const Vector<3, NumericT> &p2, const Vector<3, NumericT> &a, const Vector<3, NumericT> &b) {
			Vector<3, NumericT> cp1 = (b-a).cross(p1-a);
			Vector<3, NumericT> cp2 = (b-a).cross(p2-a);

			if (cp1.dot(cp2) >= 0) return true;

			return false;
		}

		template <unsigned D, typename NumericT>
		Triangle<D, NumericT>::Triangle (const Vector<D, NumericT> & p1, const Vector<D, NumericT> & p2, const Vector<D, NumericT> & p3)
		{
			this->_points[0] = p1;
			this->_points[1] = p2;
			this->_points[2] = p3;
		}

		template <unsigned D, typename NumericT>
		Vector<D, NumericT> Triangle<D, NumericT>::normal () const
		{
			return this->_points[0].normal(this->_points[1], this->_points[2]);
		}

		template <unsigned D, typename NumericT>
		IntersectionResult Triangle<D, NumericT>::intersects_with (const Line<3, NumericT> & line, Vector<D, NumericT> & at) const
		{
			Plane<D, NumericT> p (*this);

			IntersectionResult result = p.intersects_with(line, at);
			if (!result) return NO_INTERSECTION;

			if (same_side(at, this->_points[0], this->_points[1], this->_points[2]) &&
			    same_side(at, this->_points[1], this->_points[0], this->_points[2]) &&
			    same_side(at, this->_points[2], this->_points[0], this->_points[1]))
				return SHAPES_INTERSECT;

			return NO_INTERSECTION;
		}

		template <unsigned D, typename NumericT>
		AlignedBox<D, NumericT> Triangle<D, NumericT>::bounding_box ()
		{
			AlignedBox<D, NumericT> box(this->_points[0], this->_points[1]);

			box.union_with_point(this->_points[2]);

			return box;
		}
	}
}
