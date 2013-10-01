//
//  Numerics/Average.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
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
		/// This class can assist in the calculation of mathematical averages.
		template <typename NumericT>
		class Average
		{
		protected:
			/// The number of samples taken so far.
			std::size_t _samples;
			/// The current total.
			/// @todo This value might overflow. Is there a way to fix this behaviour?
			NumericT _total;

			/// Add a set of samples to the average.
			void add_samples (const NumericT & value, const std::size_t & samples)
			{
				_samples += samples;
				_total += value;
			}

		public:
			/// Default constructor
			Average () : _samples(0), _total(0)
			{
			}

			/// Add a single sample to the average.
			void add_sample (const NumericT & value)
			{
				_samples += 1;
				_total += value;
			}

			/// Add samples from another instance of Average.
			void add_samples (const Average<NumericT> & other)
			{
				add_samples(other._total, other._samples);
			}

			/// Calculate the average value.
			/// @returns The average value.
			NumericT value ()
			{
				if (_samples == 0) return 0;

				return _total / (NumericT)_samples;
			}

			/// Check if any samples have been added.
			/// @returns true if there are samples.
			bool has_samples ()
			{
				return _samples != 0;
			}

			/// The number of samples taken.
			std::size_t number_of_samples () const
			{
				return _samples;
			}

			/// Helper for adding a sample. Same as add_sample.
			void operator+= (const NumericT & sample)
			{
				add_sample(sample);
			}

			/// Helper for adding a samples. Same as add_samples.
			void operator+= (const Average<NumericT> & other)
			{
				add_samples(other);
			}
		};
	}
}

#endif
