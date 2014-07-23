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

namespace Euclid {
	namespace Numerics {
		/// Write a vector to an std::ostream
		template <dimension E, typename NumericT>
		std::ostream & operator<<(std::ostream & output, const Vector<E, NumericT> & vec)
		{
			for (dimension i = 0; i < E; ++i) {
				// We use this helper to ensure that char and unsigned char are printed correctly.
				output << (typename NumericOutputTraits<NumericT>::NumericT)(vec[i]) << ((i + 1) != E ? " " : "");
			}

			return output;
		}

		/// Read a vector from a std::istream
		template <dimension E, typename NumericT>
		std::istream & operator>> (std::istream & input, Vector<E, NumericT> & vec)
		{
			for (dimension i = 0; i < E; ++i) {
				input >> vec[i];
			}
			
			return input;
		}
	}
}

#endif
