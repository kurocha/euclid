//
//  Numerics/Average.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 31/10/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_AVERAGE_H
#define _EUCLID_NUMERICS_AVERAGE_H

#include "Numerics.h"

namespace Euclid
{
	namespace Numerics
	{
		/** This class can assist in the calculation of mathematical averages.
		 */
		template <typename type_t>
		class Average {
		protected:
			/// The number of samples taken so far.
			uint32_t _times;
			/// The current total.
			/// @todo This value might overflow. Is there a way to fix this behaviour?
			type_t _value;

			/// Add a set of samples to the average.
			void add_samples (const type_t & v, const uint32_t & t)
			{
				_times += t;
				_value += v;
			}

		public:
			/// Default constructor
			Average () : _times (0), _value (0)
			{
			}

			/// Add a single sample to the average.
			void add_sample (const type_t & v)
			{
				_times += 1;
				_value += v;
			}

			/// Add samples from another instance of Average.
			void add_samples (const Average<type_t> & other)
			{
				add_samples(other._value, other._times);
			}

			/// Calculate the average value.
			/// @returns The average value.
			type_t average ()
			{
				if (_times == 0) return 0;

				return _value / (type_t)_times;
			}

			/// Check if any samples have been added.
			/// @returns true if there are samples.
			bool has_samples ()
			{
				return _times != 0;
			}

			/// The number of samples taken.
			uint32_t number_of_samples () const
			{
				return _times;
			}

			/// Helper for adding a sample. Same as add_sample.
			void operator+= (const type_t & sample)
			{
				add_sample(sample);
			}

			/// Helper for adding a samples. Same as add_samples.
			void operator+= (const Average<type_t> & other)
			{
				add_samples(other);
			}
		};
	}
}

#endif
