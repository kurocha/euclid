//
//  Numerics/Quaternion.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 19/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_QUATERNION_H
#define _EUCLID_NUMERICS_QUATERNION_H

#include "Numerics.h"
#include "Transforms.h"
#include "Vector.h"
#include "Matrix.h"

namespace Euclid
{
	namespace Numerics
	{
		/** Quaternions are used for calculating rotations.

		 Two variations are available Quaternion<float> and Quaternion<double>.

		 Quaternions are a specialization of a 4-component Vector. It provides some additional operators for convenience.
		 */
		template <typename NumericT = RealT>
		class Quaternion : public Vector<4, NumericT> {
		public:
			/// Undefined constructor.
			Quaternion () = default;

			/// Identity constructor.
			Quaternion (const Identity &) : Vector<4, NumericT>{0, 0, 0, 1} {}

			Quaternion (const Vector<4, NumericT> & value) : Vector<4, NumericT>(value) {}

			/// Angle axis constructor.
			Quaternion (const Radians<NumericT> & angle, const Vector<3, NumericT> & axis) {
				auto half_angle = angle / 2.0;
				
				(*this) = (axis * half_angle.sin()).expand(half_angle.cos());
			}

			template <dimension N, typename AngleNumericT, typename AxisNumericT>
			Quaternion(const AngleAxisRotation<N, AngleNumericT, AxisNumericT> & rotation) : Quaternion(rotation.angle, rotation.axis)
			{
			}

			template <dimension AXIS, typename AngleNumericT>
			Quaternion(const FixedAxisRotation<AXIS, AngleNumericT> rotation) : Quaternion(rotation.angle, rotation.template axis<3>()) {
				
			}

			template <typename A, typename B>
			Quaternion (const Transforms<A, B> & transforms) : Quaternion(IDENTITY)
			{
				transforms.apply(*this);
			}

			/// Calculate the angle of rotation.
			Radians<NumericT> angle () const {
				return number((*this)[W]).acos() * 2.0;
			}

			/// Calculate the axis of rotation.
			Vector<3, NumericT> axis () const {
				return this->reduce().normalize();
			}

			/// Return a quaternion that rotates from this rotation to another.
			Quaternion rotation_to (const Quaternion & other) const {
				return this->conjugate() * other;
			}

			/// Conjugate the current quaternion in place.
			Quaternion conjugate () const {
				Quaternion copy(*this);

				copy[X] *= -1;
				copy[Y] *= -1;
				copy[Z] *= -1;

				return copy;
			}

			/// Rotate a point:
			Vector<3, NumericT> operator* (const Vector<3, NumericT> & v) const {
				auto qvec = this->reduce();

				auto uv = cross_product(qvec, v);
				auto uuv = cross_product(qvec, uv);

				uv *= (*this)[W] * 2;
				uuv *= 2;

				return v + uv + uuv;
			}

			template <typename RightT>
			Transforms<Quaternion, RightT> operator<< (const RightT & right) const
			{
				return {*this, right};
			}
		};

		template <typename NumericT>
		Quaternion<NumericT> quaternion (const Vector<4, NumericT> values) {
			return values;
		}
		
		template <typename NumericT>
		Quaternion<NumericT> quaternion (const Matrix<4, 4, NumericT> & m) {
			auto w = number(1.0 + m[0] + m[5] + m[10]).sqrt() / 2;

			Vector<4, NumericT> q;
			q[X] = (m[6] - m[9]) / (4 * w);
			q[Y] = (m[8] - m[2]) / (4 * w);
			q[Z] = (m[1] - m[4]) / (4 * w);
			q[W] = w;

			return q.normalize();
		}

		template <typename NumericT>
		Quaternion<NumericT> quaternion(const Vector<3, Radians<NumericT>> & angles)
		{
			Quaternion<NumericT> x_rotation(angles[X], {1.0, 0.0, 0.0}),
				y_rotation(angles[Y], {0.0, 1.0, 0.0}),
				z_rotation(angles[Z], {0.0, 0.0, 1.0});

			return x_rotation * y_rotation * z_rotation;
		}

		typedef Quaternion<> Quat;
	}
}

#include "Quaternion.Multiply.h"

#endif
