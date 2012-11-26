//
//  Ratios.h
//  Euclid
//
//  Created by Samuel Williams on 26/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_RATIOS_H
#define _EUCLID_NUMERICS_RATIOS_H

#include <Vector.h>
#include "Number.h>

namespace Euclid {
	namespace Numerics {
		template <typename NumericT>
		struct Ratios : public Vector<2, NumericT> {
			Number<NumericT> aspect_ratio () const
			{
				return (*this)[WIDTH] / (*this)[HEIGHT];
			}

			Ratios shrink_to_fit_aspect_ratio(const NumericT & aspect_ratio) const
			{
				Vector<2, NumericT> result;

				result = (*this);
				result[WIDTH] = aspect_ratio * result[HEIGHT];

				if (result[WIDTH] <= (*this)[WIDTH]) {
					return result;
				}

				result = (*this);
				result[HEIGHT] = (1.0 / aspect_ratio) * result[WIDTH];

				return result;
			}

			Ratios shrink_to_fit_aspect_ratio(const Ratios & other) const
			{
				return this->shrink_to_fit_aspect_ratio(other.aspect_ratio());
			}

			Ratios expand_to_fit_aspect_ratio(const NumericT & aspect_ratio) const
			{
				Ratios result;

				result = (*this);
				result[WIDTH] = aspect_ratio * result[HEIGHT];

				if (result[WIDTH] >= (*this)[WIDTH]) {
					return result;
				}

				result = (*this);
				result[HEIGHT] = (1.0 / aspect_ratio) * result[WIDTH];

				return result;
			}

			Ratios expand_to_fit_aspect_ratio(const Ratios & other) const
			{
				return this->expand_to_fit_aspect_ratio(other.aspect_ratio());
			}
		};

		template <typename NumericT>
		Ratios<NumericT> ratios(const Vector<2, NumericT> size) {
			return size;
		}
	}
}

#endif
