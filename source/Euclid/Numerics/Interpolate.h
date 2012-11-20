//
//  Numerics/Interpolate.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 29/09/06.
//  Copyright 2006 Samuel WilliamsWilliams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_INTERPOLATE_H
#define _EUCLID_NUMERICS_INTERPOLATE_H

#include "Numerics.h"

namespace Euclid
{
	namespace Numerics
	{
		/// @todo Add tests for these functions.
		/// Linear interpolate between two values
		template <typename InterpolateT, typename AnyT>
		inline AnyT linear_interpolate (const InterpolateT & t, const AnyT & a, const AnyT & b)
		{
			return (a * (1.0 - t)) + (b * t);
		}

		/// Cosine interpolate between two values
		template <typename InterpolateT, typename AnyT>
		inline AnyT cosine_interpolate (const InterpolateT & t, const AnyT & a, const AnyT & b)
		{
			InterpolateT f = (1.0 - cos(t * M_PI)) * 0.5;

			return a*(1-f) + b*f;
		}

		/// Cubic interpolate between four values
		template <typename InterpolateT, typename AnyT>
		inline AnyT cubic_interpolate (const InterpolateT & t, const AnyT & a, const AnyT & b, const AnyT & c, const AnyT & d)
		{
			AnyT p = (d - c) - (a - b);
			AnyT q = (a - b) - p;
			AnyT r = c - a;
			AnyT s = b;

			return p*(t*t*t) + q*(t*t) + r * t + s;
		}

		/// Hermite interpolation polynomial function.
		template <typename InterpolateT, typename AnyT>
		inline AnyT hermite_polynomial (const InterpolateT & t, const AnyT & p0, const AnyT & m0, const AnyT & p1, const AnyT & m1)
		{
			InterpolateT t3 = t*t*t, t2 = t*t;

			InterpolateT h00 = 2*t3 - 3*t2 + 1;
			InterpolateT h10 = t3 - 2*t2 + t;
			InterpolateT h01 = -2*t3 + 3*t2;
			InterpolateT h11 = t3 - t2;

			return p0 * h00 + m0 * h10 + p1 * h01 + m1 * h11;
		}
	}
}

#endif
