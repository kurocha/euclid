//
//  Numerics/RandomSequence.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 17/02/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#include "RandomSequence.h"

namespace Euclid {
	namespace Numerics {
		RandomSequence::RandomSequence (unsigned seed, RealT min, RealT max) : _distribution(min, max), _generator(seed) {
		}

		const RealT RandomSequence::next_real () {
			return _distribution(_generator);
		}
	}
}
