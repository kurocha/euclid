//
//  Geometry/Viewport.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 5/06/11.
//  Copyright (c) 2011 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_GEOMETRY_EYE_H
#define _EUCLID_GEOMETRY_EYE_H

#include "Line.hpp"
#include "AlignedBox.hpp"
#include "../Numerics/Matrix.hpp"
#include "../Numerics/Matrix.Inverse.hpp"

namespace Euclid
{
	namespace Geometry
	{
		template <typename NumericT = RealT>
		struct Eye {
			Vector<3, NumericT> origin;
			Line<3, NumericT> forward;
			Vector<3, NumericT> up;
			
			struct Transformation {
				Matrix<4, 4, NumericT> inverse_projection_matrix;
				Matrix<4, 4, NumericT> inverse_view_matrix;
				NumericT near;
				
				Vector<4, NumericT> convert_from_projection_space_to_object_space(Vector<4, NumericT> coordinate) const;
				
				/// Calculate the object-space coordinates when given the window's viewport and a point in the viewport.
				Eye convert_from_viewport_space_to_object_space (const AlignedBox<2, NumericT> & viewport, const Vector<2, NumericT> & point) const;

				/// Calculate the object-space coordinates when given a projection-space coordinate on the near plane.
				Eye convert_from_normalized_space_to_object_space (Vector<3, NumericT> normalized_point) const;
			};
			
			/// Basically an implementation of gluLookAt, but slightly simpler due to the following constraints:
			/// - direction is already normalized and points from _origin in the direction we are currently looking in
			/// - up is already normalized
			static Matrix<4, 4, NumericT> look_at(const Vector<3, NumericT> & origin, const Vector<3, NumericT> & direction, const Vector<3, NumericT> & up);
			
			Matrix<4, 4, NumericT> look_at() const;
		};
		
		template <typename NumericT>
		typename Eye<NumericT>::Transformation eye_transformation(const Matrix<4, 4, NumericT> & projection_matrix, const Matrix<4, 4, NumericT> & view_matrix, NumericT near = -1) {
			return typename Eye<NumericT>::Transformation {
				inverse(projection_matrix),
				inverse(view_matrix),
				near
			};
		}
		
		extern template struct Eye<float>;
		extern template struct Eye<double>;
	}
}

#endif
