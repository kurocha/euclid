//
//  Numerics/Number.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 31/10/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_NUMBER_H
#define _EUCLID_NUMERICS_NUMBER_H

#include "Numerics.h"
#include "Integer.h"
#include "Float.h"

#include <algorithm>

namespace Euclid
{
	namespace Numerics
	{
		// Private base implementation of a variety of common numerical operations:
		namespace {
			template <typename NumericT>
			typename std::enable_if<std::is_floating_point<NumericT>::value, NumericT>::type
			/* NumericT */ _truncate (const NumericT & value, bool up = false) {
				return up ? std::ceil(value) : std::floor(value);
			}

			template <typename NumericT>
			typename std::enable_if<std::is_integral<NumericT>::value, NumericT>::type
			/* NumericT */ _truncate (const NumericT & value, bool) {
				return value;
			}

			template <typename ValueT, typename ModulusT>
			typename std::enable_if<std::is_floating_point<ValueT>::value || std::is_floating_point<ModulusT>::value, ModulusT>::type
			/* NumericT */ _modulo (const ValueT & value, const ModulusT & modulus) {
				return std::fmod(value, modulus);
			}

			template <typename ValueT, typename ModulusT>
			typename std::enable_if<std::is_integral<ValueT>::value && std::is_integral<ModulusT>::value, ValueT>::type
			/* NumericT */ _modulo (const ValueT & value, const ModulusT & modulus) {
				return value % modulus;
			}

			template <typename BaseT, typename ExponentT>
			typename std::enable_if<std::is_floating_point<BaseT>::value || std::is_signed<ExponentT>::value, BaseT>::type
			/* BaseT */ _raise (const BaseT & base, const ExponentT & exponent) {
				return std::pow(base, exponent);
			}

			template <typename BaseT, typename ExponentT>
			typename std::enable_if<std::is_integral<BaseT>::value && std::is_unsigned<ExponentT>::value, BaseT>::type
			/* BaseT */ _raise (BaseT base, ExponentT exponent) {
				BaseT result = 1;

				while (exponent != 0) {
					if ((exponent & 1) == 1)
						result *= base;
					
					base *= base;
					exponent >>= 1;
				}
				
				return result;
			}
		}

		/** Functionality common to all numeric types.
		 */
		template <typename NumericT>
		struct Number {
			static_assert(std::is_arithmetic<NumericT>::value, "Number can only work with arithmetic data-types!");
			typedef typename RealTypeTraits<NumericT>::RealT RealT;

			template <typename ValueT>
			static ValueT value_of(const ValueT & value) {
				return value;
			}

			template <typename ValueT>
			static ValueT value_of(const Number<ValueT> & number) {
				return number.value;
			}

			NumericT value;

			constexpr Number (const NumericT & value_) : value(value_) {
			}

			template <typename OtherNumericT>
			constexpr Number (const OtherNumericT & value_) : value(value_) {
			}

			constexpr operator NumericT () {
				return value;
			}

			constexpr Number clamp (const Number & lower = 0, const Number & upper = 1) const {
				return value < lower.value ? lower : (value > upper.value ? upper : *this);
			}

			constexpr Number absolute() {
				return value * (value < 0 ? -1 : 1);
			}

			Number truncate(bool up = false) {
				return _truncate(value, up);
			}

			Number modulo(const Number & modulus) {
				return _modulo(value, (NumericT)modulus);
			}

			Number wrap(const Number & modulus) {
				return modulo(modulus) + (value < 0 ? modulus : (Number)0);
			}

			template <typename ExponentT>
			Number raise(const ExponentT & exponent) {
				return _raise(value, value_of(exponent));
			}

			bool equivalent(const Number & other) {
				return Numerics::equivalent(value, other.value);
			}

			Number<RealT> square_root() const {
				return std::sqrt(value);
			}

			Number fractional_part() const {
				return value - _truncate(value);
			}

			template <typename OtherNumericT>
			Number operator+ (const OtherNumericT & other) { return value + other; };

			template <typename OtherNumericT>
			Number operator- (const OtherNumericT & other) { return value - other; };

			template <typename OtherNumericT>
			Number operator* (const OtherNumericT & other) { return value * other; };

			template <typename OtherNumericT>
			Number operator/ (const OtherNumericT & other) { return value / other; };

			template <typename OtherNumericT>
			Number operator%(const OtherNumericT & modulus) { return modulo(modulus); }

			Number max (const Number & other) {
				return std::max(value, other.value);
			}

			Number min (const Number & other) {
				return std::min(value, other.value);
			}

			template <typename OtherNumericT>
			Number<OtherNumericT> as() {
				return value;
			}
		};

		template <typename NumericT>
		inline constexpr Number<NumericT> number(const Number<NumericT> & value) {
			return value;
		}

		template <typename NumericT>
		inline constexpr Number<NumericT> number(const NumericT & value) {
			return {value};
		}
	}
}
#endif
