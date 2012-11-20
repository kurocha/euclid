//
//  Geometry/Generate/Cylinder.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 18/08/12.
//  Copyright (c) 2012 Orion Transfer Ltd. All rights reserved.
//

#include "Cylinder.h"

namespace Euclid {
	namespace Geometry {
		namespace Generate {
			RealT calculate_rotation_length (const std::size_t rotation, const std::vector<Vec3> &left, const std::vector<Vec3> &right) {
				RealT length = 0;

				Vec3 centers[2];
				centers[0].zero(); centers[1].zero();

				for (std::size_t i = 0; i < left.size(); i += 1) {
					centers[0] += left[i];
					centers[1] += right[i];
				}

				centers[0] /= left.size();
				centers[1] /= right.size();

				Vec3 segment_direction = (centers[1] - centers[0]).normalize();

				for (std::size_t i = 0; i < left.size(); i += 1) {
					RealT cosl = segment_direction.dot((right[(i + rotation) % right.size()] - left[i]).normalize());
					length += 1.0 - cosl;
				}

				return length;
			}

			std::size_t calculate_rotation_offset (const std::vector<Vec3> &left, const std::vector<Vec3> &right) {
				assert(left.size() == right.size());
				std::vector<RealT> lengths(left.size());

				for (std::size_t i = 0; i < right.size(); i++) {
					lengths.at(i) = calculate_rotation_length(i, left, right);
				}

				std::size_t index = 0;
				RealT maximum = lengths[0];
				for (std::size_t i = 1; i < lengths.size(); ++i) {
					if (maximum > lengths[i]) {
						index = i;
						maximum = lengths[i];
					}
				}

				return index;
			}
		}
	}
}
