//
//  Numerics/PerlinNoise.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 13/05/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_IMAGING_PERLINNOISE_H
#define _EUCLID_IMAGING_PERLINNOISE_H

#include "Numerics.h"
#include "Vector.h"

namespace Euclid {
	namespace Numerics {
		class PerlinNoise {
		protected:
			RealT _table[256];
			unsigned char _indicies[256];

		public:
			typedef std::uint32_t SeedT;
			
			PerlinNoise(SeedT seed);

			RealT sample (const Vec3 &v) const;

			RealT turbulence (const Vec3 &v) const;
			RealT marble (const RealT &strength, const Vec3 &v) const;

			/* roct -> reciprocal octave, ie 1/oct, rscale is 1/scale */
			RealT sample(const Vec3 &at, RealT roct, RealT rscale) const {
				return sample(at * roct) * rscale;
			}

		protected:
			RealT lattice_noise(std::size_t i, std::size_t j, std::size_t k) const;
			RealT spline (RealT x) const;
		};
	}
}

#endif
