//
//  Numerics/Numerics.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 13/05/06.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_NUMERICS_H
#define _EUCLID_NUMERICS_NUMERICS_H

#include "../Euclid.h"

#include <cmath>
#include <cstdint>
#include <type_traits>

namespace Euclid
{
	/** Mathematics and functionality directly associated with numbers.
	 */
	namespace Numerics
	{
		/// Default floating point type
		typedef float RealT;

		/// Used as a template type for size dimension.
		typedef std::size_t dimension;

		/// Zero type. Used in some Numerics constructors.
		enum Zero {
			ZERO = 0
		};

		/// Identity type. Used in some Numerics constructors.
		enum Identity {
			IDENTITY = 1
		};

		/// 4D Vector indices, for use with Vector
		enum {
			X = 0, Y = 1, Z = 2, W = 3
		};

		/// 3D size indicies, for use with Vector
		enum {
			WIDTH = 0, HEIGHT = 1, DEPTH = 2
		};

		template <typename TypeT, typename ResultT>
		using enable_if_floating_point = std::enable_if<std::is_floating_point<TypeT>::value, ResultT>;

		template <typename TypeT, typename ResultT>
		using enable_if_integral = std::enable_if<std::is_integral<TypeT>::value, ResultT>;
	}
}

#endif
