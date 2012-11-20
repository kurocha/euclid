//
//  Axis.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 12/03/12.
//  Copyright (c) 2012 Orion Transfer Ltd. All rights reserved.
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

			Axis(Identity & identity) : _translation(ZERO), _rotation(IDENTITY) {
			}

			Axis(Vec3T translation, QuaternionT rotation) : _translation(translation), _rotation(rotation) {
			}

			const Vec3T translation() { return _translation; }
			const QuaternionT rotation() { return _rotation; }

			MatrixT to_origin() const {
				MatrixT transform = IDENTITY;

				transform = transform * _rotation.conjugated_quaternion().rotating_matrix();
				transform = transform * MatrixT::translating_matrix(-_translation);

				return transform;
			}

			MatrixT from_origin() const {
				MatrixT transform = IDENTITY;

				transform = transform * MatrixT::translating_matrix(_translation);
				transform = transform * _rotation.rotating_matrix();

				//return transform.translated_matrix(_translation);
				return transform;
			}

			MatrixT mate_with(const Axis & axis) const {
				MatrixT transform = to_origin();
				return axis.from_origin() * transform;
			}

			MatrixT mate_with(const Axis & axis, const MatrixT & intermediate_transform) const {
				MatrixT transform = to_origin();
				transform = intermediate_transform * transform;
				transform = axis.from_origin() * transform;

				return transform;
			}
		};
	}
}

#endif
