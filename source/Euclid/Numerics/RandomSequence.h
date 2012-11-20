//
//  Numerics/RandomSequence.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 17/02/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#include "Vector.h"

#include <random>

namespace Euclid {
	namespace Numerics {
		class RandomSequence {
		protected:
			typedef std::uniform_real_distribution<> DistributionT;
			typedef std::minstd_rand GeneratorT;

			DistributionT _distribution;
			GeneratorT _generator;

			Vec3 _current_point;
			Vec3 _scale;
		public:
			RandomSequence (unsigned seed, RealT min = -1.0, RealT max = 1.0);

			template <unsigned D>
			const Vector<D> next_vector () {
				Vector<D> result;

				for (std::size_t i = 0; i < D; i++) {
					result[i] = next_real();
				}

				return result;
			}

			const RealT next_real ();
		};
	}
}
