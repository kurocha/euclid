//
//  Numerics/Angle.h
//  Euclid
//
//  Created by Samuel Williams on 21/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_ANGLE_H
#define _EUCLID_NUMERICS_ANGLE_H

#include "Numerics.hpp"
#include "Number.hpp"
#include "Float.hpp"

namespace Euclid
{
	namespace Numerics
	{
		template <typename NumericT>
		struct Number;

		namespace Constants
		{
			/// Radians to degrees multiplier
			constexpr double R2D = (180.0 / M_PI);
			constexpr double D2R = (M_PI / 180.0);
		}

		template <typename FloatT = RealT>
		struct Radians
		{
			FloatT value;

			constexpr Radians() = default;

			explicit constexpr Radians(const FloatT & _value) : value(_value) {}

			// Implicit convertion between Radians<float> and Radians<double>.
			template <typename OtherNumericT>
			constexpr Radians(const Radians<OtherNumericT> angle) : value(angle.value) {}

			constexpr operator FloatT () const {
				return value;
			}

			constexpr Radians operator+ (const Radians & other) const {
				return Radians{value + other.value};
			}

			constexpr Radians operator- (const Radians & other) const {
				return Radians{value - other.value};
			}

			template <typename OtherT>
			constexpr Radians operator* (const OtherT & other) const {
				return Radians{value * other};
			}

			constexpr Radians operator/ (const Radians & other) const {
				return Radians{value / other.value};
			}

			template <typename OtherT>
			constexpr Radians operator/ (const OtherT & other) const {
				return Radians{value / other};
			}

			constexpr Radians operator- () const {
				return Radians{-value};
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
				return Radians{std::atan2(std::sin(x-y), std::cos(x-y))};
			}

			bool equivalent(const Radians & other) const {
				return Numerics::equivalent<FloatT>(this->offset_to(other).value, 0);
			}
		};

		template <typename FloatT>
		constexpr Radians<FloatT> radians(const FloatT & value) {
			return Radians<FloatT>{value};
		}

		template <typename FloatT>
		constexpr Radians<FloatT> degrees(const FloatT & value) {
			return Radians<FloatT>{value * D2R};
		}

		template <typename FloatT>
		inline constexpr Number<FloatT> number(const Radians<FloatT> & angle)
		{
			return {angle.value};
		}

		constexpr Radians<double> operator"" _rad(long double r) { return Radians<double>(r); }
		constexpr Radians<double> operator"" _rad(unsigned long long r) { return Radians<double>((double)r); }
		constexpr Radians<double> operator"" _deg(long double d) { return Radians<double>(d * D2R); }
		constexpr Radians<double> operator"" _deg(unsigned long long d) { return Radians<double>((double)d * D2R); }

		namespace Constants
		{
			constexpr Radians<double> R0{0};
			constexpr Radians<double> R10{M_PI_2 / 9.0};
			constexpr Radians<double> R30{M_PI_2 / 3.0};
			constexpr Radians<double> R45{M_PI_4};
			constexpr Radians<double> R60 = R30 * 2.0;
			constexpr Radians<double> R90{M_PI_2};
			constexpr Radians<double> R180{M_PI};
			constexpr Radians<double> R270 = R90 * 3.0;
			constexpr Radians<double> R360 = R180 * 2.0;
		}
	}
}

#endif
