//
//  Numerics/Integer.h
//  Euclid
//
//  Created by Samuel Williams on 21/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_INTEGER_H
#define _EUCLID_NUMERICS_INTEGER_H

#include <cstdint>

namespace Euclid
{
	namespace Numerics
	{
		/// These traits provide a mapping from byte size to integer types.
		template <unsigned Bytes>
		struct IntegerSizeTraits {
		};

		template <>
		struct IntegerSizeTraits<1> {
			typedef std::uint8_t UnsignedT;
			typedef std::int8_t SignedT;
		};

		template <>
		struct IntegerSizeTraits<2> {
			typedef std::uint16_t UnsignedT;
			typedef std::int16_t SignedT;
		};

		template <>
		struct IntegerSizeTraits<4> {
			typedef std::uint32_t UnsignedT;
			typedef std::int32_t SignedT;
		};

		template <>
		struct IntegerSizeTraits<8> {
			typedef std::uint64_t UnsignedT;
			typedef std::int64_t SignedT;
		};

		/// If the supplied value is a power of two, it is returned, otherwise the next highest power of 2 is calculated and returned. The integral must be
		// http://acius2.blogspot.com/2007/11/calculating-next-power-of-2.html
		template <typename IntegralT>
		IntegralT power_of_2(IntegralT value)
		{
			static_assert(std::is_unsigned<IntegralT>::value, "IntegralT must be unsigned!");
			const std::size_t BITS = (sizeof(IntegralT) * 8) / 2;

			if (value < 2) return 2;

			value--;

			for (std::size_t i = 1; i <= BITS; i *= 2)
				value |= value >> i;

			return value + 1;
		}

		template <typename IntegralT>
		inline bool is_power_of_2 (const IntegralT & value)
		{
			static_assert(std::is_unsigned<IntegralT>::value, "IntegralT must be unsigned!");
			
			return (value & (value-1)) == 0;
		}

		/// Proportional equivalence of floating point numbers.
		template <typename IntegralT>
		inline bool equivalent (const IntegralT & a, const IntegralT & b)
		{
			return a == b;
		}

		// If we increase row by 1, the offset will increase by sz (number of elements per row i.e. number of columns)
		// If we increase col by 1, the offset will increase by 1
		inline std::size_t row_major_offset(std::size_t row, std::size_t column, std::size_t size)
		{
			return column + (row * size);
		}

		// If we increase col by 1, the offset will increase by sz (number of elements per column i.e. number of rows)
		// If we increase row by 1, the offset will increase by 1
		inline std::size_t column_major_offset(std::size_t row, std::size_t column, std::size_t size)
		{
			return row + (column * size);
		}

		/// @returns the number of elements in a statically defined array.
		template <typename ElementT, dimension N>
		std::size_t array_count(const ElementT(&)[N]) {
			return N;
		}
	}
}

#endif
