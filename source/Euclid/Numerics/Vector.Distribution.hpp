//
//  Vector.Distribution.h
//  Euclid
//
//  Created by Samuel Williams on 22/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_VECTOR_DISTRIBUTION_H
#define _EUCLID_NUMERICS_VECTOR_DISTRIBUTION_H

#include "Vector.hpp"

#include <random>

namespace Euclid
{
	namespace Numerics
	{
		template <unsigned E, typename NumericT = RealT>
		class UniformVectorDistribution {
		public:
			typedef Vector<E, NumericT> VectorT;
			
		protected:
			VectorT _offset, _scale;

		public:
			UniformVectorDistribution(const VectorT & offset, const VectorT & scale) : _offset(offset), _scale(scale) {
			}

			template <typename RandomT>
			VectorT operator() (RandomT & random) {
				VectorT result;

				for (dimension i = 0; i < E; i += 1) {
					std::uniform_real_distribution<> distribution(_offset[i], _offset[i] + _scale[i]);

					result[i] = distribution(random);
				}

				return result;
			}
		};

	}
}

#endif
