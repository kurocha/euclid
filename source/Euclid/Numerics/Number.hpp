//
//  Number.Angles.h
//  Euclid
//
//  Created by Samuel Williams on 28/11/12.
//  Copyright (c) 2012 Samuel Williams. All rights reserved.
//

#ifndef _EUCLID_NUMERICS_NUMBER_ANGLES_H
#define _EUCLID_NUMERICS_NUMBER_ANGLES_H

#include "Angle.h"

namespace Euclid
{
	namespace Numerics
	{
		template <typename NumericT>
		auto Number<NumericT>::asin () -> Radians<RealT>
		{
			return Radians<RealT>{std::asin(value)};
		}

		template <typename NumericT>
		auto Number<NumericT>::acos () -> Radians<RealT>
		{
			return Radians<RealT>{std::acos(value)};
		}

		template <typename NumericT>
		auto Number<NumericT>::atan () -> Radians<RealT>
		{
			return Radians<RealT>{std::atan(value)};
		}
	}
}

#endif
