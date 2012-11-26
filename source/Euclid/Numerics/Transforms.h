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

		/// Fairly standard angle-axis transform:
		template <dimension E, typename NumericT>
		struct AngleAxisRotation {
			/// The angle to rotate by:
			Radians<NumericT> angle;

			/// The axis to rotate around:
			Vector<E, NumericT> axis;
		};

		template <dimension E, typename NumericT>
		AngleAxisRotation<E, NumericT> rotation(const Radians<NumericT> angle, const Vector<E, NumericT> & axis) {
			return {angle, axis};
		}

		template <dimension E, typename NumericT>
		struct AngleAxisOriginRotation {
			AngleAxisRotation<E, NumericT> rotation;
			
			Vector<E, NumericT> origin;
		};

		template <dimension E, typename NumericT>
		AngleAxisOriginRotation<E, NumericT> rotation(const Radians<NumericT> angle, const Vector<E, NumericT> & axis, const Vector<E, NumericT> & origin) {
			return {{angle, axis}, origin};
		}

		/// Axis is the axis of rotation, e.g. X, Y, Z.
		template <dimension Axis, typename NumericT>
		struct FixedAxisRotation {
			Radians<NumericT> angle;
		};

		template <dimension RotationAxis, typename NumericT>
		FixedAxisRotation<RotationAxis, NumericT> rotation(const Radians<NumericT> angle) {
			return {angle};
		}

		template <dimension E, typename NumericT>
		struct Translation {
			/// The offset to translate by:
			Vector<E, NumericT> offset;

			bool identity () const { return offset.sum() == 1; }
		};

		template <dimension E, typename NumericT>
		Translation<E, NumericT> translation (const Vector<E, NumericT> & offset) {
			return {offset};
		}

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
	}
}

#endif
