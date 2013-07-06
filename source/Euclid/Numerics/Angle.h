//
//  Numerics/Angle.h
//  Euclid
//
//  Created by Samuel Williams on 21/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_ANGLE_H
#define _EUCLID_NUMERICS_ANGLE_H

#include "Numerics.h"
#include "Number.h"
#include "Float.h"

namespace Euclid
{
	namespace Numerics
	{
		template <typename NumericT>
		struct Number;

		namespace Constants
		{
			/// Radians to degrees multiplier
			const double R2D = (180.0 / M_PI);
			const double D2R = (M_PI / 180.0);
		}

		template <typename FloatT = RealT>
		struct Radians
		{
			FloatT value;

			Radians() = default;

			constexpr Radians (const FloatT & _value) : value(_value) {}

			// Implicit convertion between Radians<float> and Radians<double>.
			template <typename OtherNumericT>
			Radians(const Radians<OtherNumericT> angle) : value(angle._value) {}

			constexpr operator Radians<float> () const {
				return {value};
			}

			constexpr operator FloatT () const {
				return value;
			}

			constexpr Radians operator+ (const Radians & other) const {
				return {value + other.value};
			}

			constexpr Radians operator- (const Radians & other) const {
				return {value - other.value};
			}

			template <typename OtherT>
			constexpr Radians operator* (const OtherT & other) const {
				return {value * other};
			}

			constexpr Radians operator/ (const Radians & other) const {
				return {value / other.value};
			}

			template <typename OtherT>
			constexpr Radians operator/ (const OtherT & other) const {
				return {value / other};
			}

			constexpr Radians operator- () const {
				return {-value};
			}

			Radians & operator+= (const Radians & other) {
				value += other.value;
				return *this;
			}

			Radians & operator-= (const Radians & other) {
				value -= other.value;
				return *this;
			}

			template <typename OtherT>
			Radians & operator*= (const OtherT & other) {
				value *= other;
				return *this;
			}

			Radians & operator/= (const Radians & other) {
				value /= other.value;
				return *this;
			}

			template <typename OtherT>
			Radians & operator/= (const OtherT & other) {
				value /= other;
				return *this;
			}

			Number<FloatT> sin() const {
				return std::sin(value);
			}

			Number<FloatT> cos() const {
				return std::cos(value);
			}

			Number<FloatT> tan() const {
				return std::tan(value);
			}

			Radians offset_to (const Radians & other) const {
				auto x = this->value, y = other.value;
				return std::atan2(std::sin(x-y), std::cos(x-y));
			}

			bool equivalent(const Radians & other) const {
				return Numerics::equivalent<FloatT>(this->offset_to(other).value, 0);
			}
		};

		template <typename FloatT>
		constexpr Radians<FloatT> radians(const FloatT & value) {
			return value;
		}

		template <typename FloatT>
		constexpr Radians<FloatT> degrees(const FloatT & value) {
			return value * D2R;
		}

		namespace Constants
		{
			/// 10 degree rotation
			constexpr Radians<> R10 = M_PI_2 / 9.0;
			/// 30 degree rotation
			constexpr Radians<> R30 = M_PI_2 / 3.0;
			/// 45 degree rotation
			constexpr Radians<> R45 = M_PI_4;
			/// 60 degree rotation
			constexpr Radians<> R60 = R30 * 2.0;
			/// 90 degree rotation
			constexpr Radians<> R90 = M_PI_2;
			/// 180 degree rotation
			constexpr Radians<> R180 = M_PI;
			/// 270 degree rotation
			constexpr Radians<> R270 = R90 * 3.0;
			/// 360 degree rotation
			constexpr Radians<> R360 = R180 * 2.0;
		}
	}
}

#endif
