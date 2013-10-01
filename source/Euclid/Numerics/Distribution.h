//
//  Numerics/Distribution.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 1/10/2013.
//  Copyright (c) 2013 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_DISTRIBUTION_H
#define _EUCLID_NUMERICS_DISTRIBUTION_H

#include "Average.h"

namespace Euclid
{
	namespace Numerics
	{
		template <typename NumericT>
		class Distribution : protected Average<NumericT>
		{
		protected:
			NumericT _minimum, _maximum;
			std::vector<NumericT> _samples;

		public:
			Distribution() : _minimum(0), _maximum(0)
			{
			}

			void add_sample(const NumericT & sample)
			{
				if (!this->has_samples() || sample < _minimum)
					_minimum = sample;

				if (!this->has_samples() || sample > _maximum)
					_maximum = sample;

				Average<NumericT>::add_sample(sample);

				_samples.push_back(sample);
			}

			bool has_samples ()
			{
				return Average<NumericT>::has_samples();
			}

			/// The number of samples taken.
			std::size_t number_of_samples () const
			{
				return Average<NumericT>::number_of_samples();
			}

			NumericT average() { return this->value(); }

			const NumericT & minimum() { return _minimum; }
			const NumericT & maximum() { return _maximum; }

			Average<NumericT> variance()
			{
				Average<NumericT> squared_difference;
				NumericT mean = this->value();

				for (auto & sample : _samples) {
					NumericT d = (sample - mean);

					squared_difference.add_sample(d*d);
				}

				return squared_difference;
			}

			Number<NumericT> standard_deviation()
			{
				return number(this->variance().value()).square_root();
			}

			Number<NumericT> standard_error()
			{
				return standard_deviation() / number(this->number_of_samples()).square_root();
			}
		};
	}
}

#endif
