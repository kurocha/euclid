//
//  Geometry/Viewport.cpp
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 5/06/11.
//  Copyright (c) 2011 Samuel Williams. All rights reserved.
//

#include "Eye.hpp"
#include "../Numerics/Matrix.Multiply.hpp"

namespace Euclid
{
	namespace Geometry
	{
		template <typename NumericT>
		Vector<4, NumericT> Eye<NumericT>::Transformation::convert_from_projection_space_to_object_space(Vector<4, NumericT> coordinate) const
		{
			return inverse_view_matrix * (inverse_projection_matrix * coordinate);
		}
		
		template <typename NumericT>
		Eye<NumericT> Eye<NumericT>::Transformation::convert_from_viewport_space_to_object_space (const AlignedBox<2, NumericT> & viewport, const Vector<2, NumericT> & point) const
		{
			// Reverse the viewport transformation
			Vector<2, NumericT> normalized_point;

			// Calculate position in normal projection box
			normalized_point[X] = ((point[X] - viewport.origin()[X]) / viewport.size()[X]) * 2.0 - 1.0;
			normalized_point[Y] = ((point[Y] - viewport.origin()[Y]) / viewport.size()[Y]) * 2.0 - 1.0;

			return convert_from_normalized_space_to_object_space(normalized_point << near);
		}

		template <typename NumericT>
		Eye<NumericT> Eye<NumericT>::Transformation::convert_from_normalized_space_to_object_space (Vector<3, NumericT> normalized_point) const
		{
			// Reverse the viewport transformation from normalized device coordinates back into object space
			Eye<NumericT> result;

			result.origin = vector(inverse_view_matrix.at(3, 0), inverse_view_matrix.at(3, 1), inverse_view_matrix.at(3, 2));

			auto p1 = (inverse_view_matrix * (inverse_projection_matrix * (normalized_point << 1.0)));

			normalized_point[Z] *= -1;
			auto p2 = (inverse_view_matrix * (inverse_projection_matrix * (normalized_point << 1.0)));

			p1 /= p1[W];
			p2 /= p2[W];

			result.forward = Line<3, NumericT>{p1.reduce(), (p2 - p1).reduce().normalize()};

			// Calculate up direction
			Vector<3, NumericT> eye_up = {0.0, 1.0, 0.0};
			result.up = inverse_view_matrix * eye_up;

			return result;
		}
		
		template <typename NumericT>
		Matrix<4, 4, NumericT> Eye<NumericT>::look_at(const Vector<3, NumericT> & origin, const Vector<3, NumericT> & direction, const Vector<3, NumericT> & up)
		{
			auto s = cross_product(direction, up);
			auto u = cross_product(s, direction);

			Matrix<4, 4, NumericT> m = ZERO;
			m.set(0, 0, s, 4);
			m.set(1, 0, u, 4);
			m.set(2, 0, -direction, 4);
			m.at(3, 3) = 1;

			Matrix<4, 4, NumericT> result = translate(-origin);

			return m.transpose() * result;
		}
		
		template <typename NumericT>
		Matrix<4, 4, NumericT> Eye<NumericT>::look_at() const
		{
			return look_at(origin, forward.direction(), up);
		}
		
		template struct Eye<float>;
		template struct Eye<double>;
	}
}
