//
//  Numerics/Transforms.h
//  Euclid
//
//  Created by Samuel Williams on 24/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_TRANSFORMS_H
#define _EUCLID_NUMERICS_TRANSFORMS_H

#include "Angle.h"
#include "Vector.h"

namespace Euclid {
	namespace Numerics {

		// MARK: -
		// MARK: Translation

		template <dimension E, typename NumericT>
		struct Translation {
			/// The offset to translate by:
			Vector<E, NumericT> offset;

			bool identity () const { return offset.sum() == 1; }
		};

		template <dimension E, typename NumericT>
		Translation<E, NumericT> translate (const Vector<E, NumericT> & offset) {
			return {offset};
		}

		// MARK: -
		// MARK: Scale

		template <dimension E, typename NumericT>
		struct Scale {
			Vector<E, NumericT> factor;

			bool identity () const { return factor == 1; }
		};

		template <dimension E, typename NumericT>
		Scale<E, NumericT> scale (const Vector<E, NumericT> & factor) {
			return {factor};
		}

		template <typename NumericT>
		struct UniformScale {
			NumericT factor;

			bool identity () const { return factor == 1; }
		};

		template <dimension E, typename NumericT>
		UniformScale<NumericT> scale (const NumericT & factor) {
			return {factor};
		}

		// MARK: -
		// MARK: Fixed Axis Rotation

		/// A fixed rotation around a specific unit axis, e.g. X, Y, Z.
		template <dimension AXIS, typename NumericT>
		struct FixedAxisRotation {
			Radians<NumericT> angle;

			bool identity () const { return angle == 0; }

			template <dimension E>
			Vector<E, NumericT> axis () const
			{
				Vector<E, NumericT> result = 0;

				result[AXIS] = 1;

				return result;
			}
		};

		/// Rotation around a fixed axis: rotation<X>(R90)
		template <dimension AXIS, typename NumericT>
		FixedAxisRotation<AXIS, NumericT> rotate(const Radians<NumericT> & angle) {
			return {angle};
		}

		// MARK: -
		// MARK: Angle Axis Rotation

		/// Fairly standard angle-axis transform:
		template <dimension E, typename AngleNumericT, typename AxisNumericT>
		struct AngleAxisRotation {
			/// The angle to rotate by:
			Radians<AngleNumericT> angle;

			/// The axis to rotate around:
			Vector<E, AxisNumericT> axis;

			bool identity () const { return angle == 0; }
		};

		template <dimension E, typename AngleNumericT, typename AxisNumericT>
		AngleAxisRotation<E, AngleNumericT, AxisNumericT> rotate(const Radians<AngleNumericT> & angle, const Vector<E, AxisNumericT> & axis) {
			return {angle, axis};
		}

		template <dimension E, typename NumericT>
		AngleAxisRotation<E, NumericT, NumericT> rotate(const Vector<E, NumericT> & from, const Vector<E, NumericT> & to, const Vector<E, NumericT> & normal) {
			auto angle = to.angle_between(from);

			if (angle.equivalent(0)) {
				return {angle, normal};
			} else if (angle.equivalent(R180)) {
				return {angle, cross_product(from, normal).normalize()};
			} else {
				return {angle, cross_product(from, to).normalize()};
			}
		}

		template <dimension E, typename AngleNumericT, typename AxisNumericT>
		struct OffsetAngleAxisRotation {
			AngleAxisRotation<E, AngleNumericT, AxisNumericT> rotation;
			Vector<E, AxisNumericT> origin;

			bool identity () const { return rotation.identity(); }
		};

		template <dimension E, typename AngleNumericT, typename AxisNumericT>
		OffsetAngleAxisRotation<E, AngleNumericT, AxisNumericT> rotate(const Radians<AngleNumericT> & angle, const Vector<E, AxisNumericT> & axis, const Vector<E, AxisNumericT> & origin) {
			return {{angle, axis}, origin};
		}
	}
}

#endif
