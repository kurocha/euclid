//
//  Ratio.h
//  Euclid
//
//  Created by Samuel Williams on 26/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#pragma once

#include "Vector.hpp"
#include "Number.hpp"

namespace Euclid {
	namespace Numerics {
		template <typename NumericT>
		struct Ratio : public Vector<2, NumericT>, public RealTypeTraits<NumericT> {
			template <typename... ArgumentsT>
			Ratio(ArgumentsT... arguments) : Vector<2, NumericT>{arguments...} {
				
			}

			constexpr operator RealT () const {
				return static_cast<RealT>((*this)[WIDTH]) / static_cast<RealT>((*this)[HEIGHT]);
			}

			/// Calculate a vector that is smaller than the current vector in one dimension such that it has the given aspect ratio.
			Ratio shrink_to_fit_aspect_ratio(const RealT & aspect_ratio) const
			{
				Ratio result = (*this);

				result[WIDTH] = aspect_ratio * result[HEIGHT];

				if (result[WIDTH] <= (*this)[WIDTH]) {
					return result;
				}

				result = (*this);
				result[HEIGHT] = (static_cast<RealT>(1) / aspect_ratio) * result[WIDTH];

				return result;
			}

			Ratio shrink_to_fit_aspect_ratio(const Ratio & other) const
			{
				return this->shrink_to_fit_aspect_ratio(other.aspect_ratio());
			}
			
			/// Calculate a vector that is larger than the current vector in one dimension such that it has the given aspect ratio.
			Ratio expand_to_fit_aspect_ratio(const NumericT & aspect_ratio) const
			{
				Ratio result = (*this);

				result[WIDTH] = aspect_ratio * result[HEIGHT];

				if (result[WIDTH] >= (*this)[WIDTH]) {
					return result;
				}

				result = (*this);
				result[HEIGHT] = (static_cast<RealT>(1) / aspect_ratio) * result[WIDTH];

				return result;
			}

			Ratio expand_to_fit_aspect_ratio(const Ratio & other) const
			{
				return this->expand_to_fit_aspect_ratio(other.aspect_ratio());
			}
		};

		template <typename NumericT>
		Ratio<NumericT> ratio(const Vector<2, NumericT> size) {
			return size;
		}
	}
}
