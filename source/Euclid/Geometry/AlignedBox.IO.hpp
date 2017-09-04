//
//  Geometry/AlignedBox.IO.hpp
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 30/08/2017.
//  Copyright (c) 2017 Samuel Williams. All rights reserved.
//
//

#pragma once

#include "AlignedBox.hpp"

#include <iostream>

#include "../Numerics/Vector.IO.hpp"

namespace Euclid {
	namespace Geometry {
		template <dimension D, typename NumericT>
		std::ostream & operator<<(std::ostream & output, const AlignedBox<D, NumericT> & aligned_box)
		{
			return output << "{" << aligned_box.min() << "->" << aligned_box.max() << "}";
		}
	}
}
