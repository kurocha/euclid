//
//  Numerics/Color.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 1/05/10.
//  Copyright (c) 2010 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_COLOR_H
#define _EUCLID_NUMERICS_COLOR_H

#include "Numerics.h"

#include <algorithm>
#include "Vector.h"

// Thanks to the implementation from Python which helped with the implementation
// and testing.

namespace Euclid {
	namespace Numerics {
		namespace {
			template <typename AnyT>
			inline AnyT & max (AnyT & a, AnyT & b, AnyT & c)
			{
				return std::max(a, std::max(b, c));
			}

			template <typename AnyT>
			inline AnyT & min (AnyT & a, AnyT & b, AnyT & c)
			{
				return std::min(a, std::min(b, c));
			}
		}
		
		/// Conversion functions between RGB and other color systems.
		/// These functions are duplicated from the fantastic python module colorsys.
		/// http://en.wikipedia.org/wiki/YIQ
		/// http://en.wikipedia.org/wiki/HLS_color_space
		/// http://en.wikipedia.org/wiki/HSV_color_space

		/// YIQ: used by composite video signals (linear combinations of RGB)
		/// Y: perceived grey level (0.0 == black, 1.0 == white)
		/// I, Q: color components
		template <typename NumericT>
		Vector<3, NumericT> RGB2YIQ (const Vector<3, NumericT> & color) {
			return Vector<3, NumericT>(
			    0.30*color[0] + 0.59*color[1] + 0.11*color[2],
			    0.60*color[0] - 0.28*color[1] - 0.32*color[2],
			    0.21*color[0] - 0.52*color[1] + 0.31*color[2]
			    );
		}

		template <typename NumericT>
		Vector<3, NumericT> YIQ2RGB (const Vector<3, NumericT> & color) {
			Vector<3, NumericT> result (
			    color[0] + 0.948262*color[1] + 0.624013*color[2],
			    color[0] - 0.276066*color[1] - 0.639810*color[2],
			    color[0] - 1.105450*color[1] + 1.729860*color[2]
			    );

			return result.clamp();
		}

		/// HSL: Hue, Saturation, Luminance
		/// H: position in the spectrum
		/// S: color saturation
		/// L: color lightness
		template <typename NumericT>
		Vector<3, NumericT> RGB2HSL (const Vector<3, NumericT> & color) {
			NumericT maxc = max(color[0], color[1], color[2]);
			NumericT minc = min(color[0], color[1], color[2]);

			// Lightness
			NumericT l = (minc + maxc) / 2.0;

			if (minc == maxc)
				return vector(0.0, 0.0, l);

			// Saturation
			NumericT s;

			if (l <= 0.5)
				s = (maxc-minc) / (maxc+minc);
			else
				s = (maxc-minc) / (2.0-maxc-minc);

			Vector<3, NumericT> r (maxc);
			r -= color;
			r /= (maxc - minc);

			// Hue
			NumericT h;

			if (color[0] == maxc)
				h = r[2] - r[1];
			else if (color[1] == maxc)
				h = 2.0 + r[0] - r[2];
			else
				h = 4.0 + r[1] - r[0];

			h = number(h / 6.0).modulo(1.0);

			return vector(h, s, l);
		}

		namespace {
			template <typename NumericT>
			inline NumericT HSL2RGB_mix (const NumericT & m1, const NumericT & m2, NumericT hue)
			{
				const NumericT ONE_SIXTH = 1.0/6.0;
				const NumericT TWO_THIRD = 2.0/3.0;

				assert(hue >= -1.0 && hue <= 1.0);
				if (hue < 0.0) hue += 1.0;

				if (hue < ONE_SIXTH)
					return m1 + (m2 - m1) * hue * 6.0;

				if (hue < 0.5)
					return m2;

				if (hue < TWO_THIRD)
					return m1 + (m2 - m1) * (TWO_THIRD - hue) * 6.0;

				return m1;
			}
		}
		
		template <typename NumericT>
		Vector<3, NumericT> HSL2RGB (const Vector<3, NumericT> & color)
		{
			const NumericT ONE_THIRD = 1.0/3.0;

			// Its gray if saturation is zero...
			if (color[1] == 0.0) return Vector<3, NumericT>(color[2]);

			NumericT m2;
			if (color[2] <= 0.5)
				m2 = color[2] * (1.0 + color[1]);
			else
				m2 = color[2] + color[1] - (color[2] * color[1]);

			NumericT m1 = 2.0 * color[2] - m2;

			return {
			    HSL2RGB_mix(m1, m2, color[0]+ONE_THIRD),
			    HSL2RGB_mix(m1, m2, color[0]),
			    HSL2RGB_mix(m1, m2, color[0]-ONE_THIRD)
			};
		}

		/// HSV: Hue, Saturation, Value
		/// H: position in the spectrum
		/// S: color saturation ("purity")
		/// V: color brightness
		template <typename NumericT>
		Vector<3, NumericT> RGB2HSV (const Vector<3, NumericT> & color)
		{
			NumericT maxc = max(color[0], color[1], color[2]);
			NumericT minc = min(color[0], color[1], color[2]);

			NumericT v = maxc;

			if (minc == maxc)
				return vector(0.0, 0.0, v);

			// Saturation
			NumericT s = (maxc-minc) / maxc;

			Vector<3, NumericT> r (maxc);
			r -= color;
			r /= (maxc - minc);

			// Hue
			NumericT h;

			if (color[0] == maxc)
				h = r[2] - r[1];
			else if (color[1] == maxc)
				h = 2.0 + r[0] - r[2];
			else
				h = 4.0 + r[1] - r[0];

			h = number(h / 6.0).modulo(1.0);

			return vector(h, s, v);
		}

		template <typename NumericT>
		Vector<3, NumericT> HSV2RGB (const Vector<3, NumericT> & color)
		{
			if (color[1] == 0.0) return color[2];

			unsigned i = floor(color[0] * 6.0);

			NumericT f = (color[0] * 6.0) - i;
			NumericT p = color[2] * (1.0 - color[1]);
			NumericT q = color[2] * (1.0 - color[1] * f);
			NumericT t = color[2] * (1.0 - color[1] * (1.0 - f));
			NumericT v = color[2];

			i = i % 6;
			switch (i % 6) {
				case 0: return vector(v, t, p);
				case 1: return vector(q, v, p);
				case 2: return vector(p, v, t);
				case 3: return vector(p, q, v);
				case 4: return vector(t, p, v);
				case 5: return vector(v, p, q);
			}

			return 0;
		}
	}
}

#endif
