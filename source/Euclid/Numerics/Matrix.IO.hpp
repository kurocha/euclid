//
//  Matrix.IO.h
//  Euclid
//
//  Created by Samuel Williams on 27/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_MATRIX_IO_H
#define _EUCLID_NUMERICS_MATRIX_IO_H

#include "Matrix.hpp"

#include <iostream>
#include <iomanip>

namespace Euclid {
	namespace Numerics {
		template <dimension R, dimension C, typename NumericT>
		inline std::ostream & operator<< (std::ostream & out, const Matrix<R, C, NumericT> & m)
		{
			using namespace std;

			dimension k = 0;
			for (dimension c = 0; c < C; ++c)
				for (dimension r = 0; r < R; ++r) {
					out << setw(10) << setprecision(4) << m.at(r, c);

					if (k % R == (R-1))
						out << endl;
					else
						out << " ";

					k += 1;
				}

			return out;
		}
	}
}

#endif
