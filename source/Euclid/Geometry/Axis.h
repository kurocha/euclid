//
//  Axis.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 12/03/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_GEOMETRY_AXIS_H
#define _EUCLID_GEOMETRY_AXIS_H

#include "Geometry.h"
#include "../Numerics/Matrix.h"
#include "../Numerics/Quaternion.h"

namespace Euclid {
	namespace Geometry {
		template <typename _NumericT = RealT>
		class Axis {
		public:
			typedef _NumericT NumericT;
			typedef Vector<3, _NumericT> Vec3T;
			typedef Matrix<4, 4, _NumericT> MatrixT;
			typedef Quaternion<_NumericT> QuaternionT;

		protected:
			Vec3T _translation;
			QuaternionT _rotation;

		public:
			Axis() {
			}

			Axis(const Identity & identity) : _translation(ZERO), _rotation(IDENTITY) {
			}

			Axis(Vec3T translation, QuaternionT rotation) : _translation(translation), _rotation(rotation) {
			}

			const Vec3T translation() { return _translation; }
			const QuaternionT rotation() { return _rotation; }

			MatrixT to_origin() const {
				return _rotation << translate(-_translation);
			}

			MatrixT from_origin() const {
				return translate(_translation) << _rotation;
			}

			MatrixT mate_with(const Axis & axis) const {
				return axis.from_origin() << to_origin();
			}

			MatrixT mate_with(const Axis & axis, const MatrixT & intermediate_transform) const {
				return axis.from_origin() << intermediate_transform << to_origin();
			}
		};

		extern template class Axis<float>;
		extern template class Axis<double>;
	}
}

#endif
