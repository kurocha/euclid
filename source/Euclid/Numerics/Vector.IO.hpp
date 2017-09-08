//
//  Vector.IO.h
//  Euclid
//
//  Created by Samuel Williams on 27/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_VECTOR_IO_H
#define _EUCLID_NUMERICS_VECTOR_IO_H

#include "Vector.hpp"

#include <iostream>
#include <iomanip>

namespace Euclid {
	namespace Numerics {
		/// Write a vector to an std::ostream
		template <dimension E, typename NumericT>
		std::ostream & operator<<(std::ostream & output, const Vector<E, NumericT> & vec)
		{
			output << '{';
			for (dimension i = 0; i < E; ++i) {
				if (i != 0) output << ",";
				// We use this '+' operator to ensure that char and unsigned char are printed correctly:
				output << +vec[i];
			}
			output << '}';

			return output;
		}

		template <char C>
		std::istream & expect_character(std::istream & input)
		{
			char c;
			
			if (input >> c && c != C) {
					input.setstate(std::ios_base::failbit);
			}
			
			return input;
		}

		/// Read a vector from a std::istream
		template <dimension E, typename NumericT>
		std::istream & operator>> (std::istream & input, Vector<E, NumericT> & vec)
		{
			input >> expect_character<'{'>;
			
			for (dimension i = 0; i < E; ++i) {
				if (i != 0) input >> expect_character<','>;
				input >> vec[i];
			}
			
			input >> expect_character<'}'>;
			
			return input;
		}
	}
}

#endif
