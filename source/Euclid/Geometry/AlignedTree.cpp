//
//  Euclid/Geomtry/AlignedTree.cpp
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 2/01/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#include "AlignedTree.hpp"

namespace Euclid {
	namespace Geometry {

		unsigned Quadrants::index_for_partition(const Vec2 &point, const Vec2 &center) {
			if (point[Y] < center[Y]) {
				if (point[X] < center[X])
					return 0;
				else
					return 1;
			} else {
				if (point[X] < center[X])
					return 2;
				else
					return 3;
			}
		}

		Vec2 Quadrants::normal_origin_for_partition_index(const std::size_t &i) {
			assert(i < Q && "Invalid partition index!");

			switch (i) {
			case 0:
				return Vec2 (0.0, 0.0);
			case 1:
				return Vec2 (0.5, 0.0);
			case 2:
				return Vec2 (0.0, 0.5);
			case 3:
				return Vec2 (0.5, 0.5);
			}

			assert(false && "Invalid partition index!");

			return ZERO;
		}

		Vec2 Quadrants::offset_origin_for_partition_index(const std::size_t &i) {
			return ((normal_origin_for_partition_index(i) + 0.25) * 2.0) - 0.5;
		}

		Quadrants::PartitionLocation Quadrants::location_for_direction (const Direction &dir) {
			switch ((unsigned)dir) {
			case (BOTTOM | LEFT):
				return BottomLeft;
			case (BOTTOM | RIGHT):
				return BottomRight;
			case (TOP | LEFT):
				return TopLeft;
			case (TOP | RIGHT):
				return TopRight;
			default:
				break;
			}

			assert(false && "Invalid partition direction!");

			return BottomLeft;
		}

		unsigned Octants::index_for_partition(const Vec3 &point, const Vec3 &center) {
			unsigned v = point[Z] < center[Z] ? 0 : 4;

			if (point[Y] < center[Y]) {
				if (point[X] < center[X])
					return v + 0;
				else
					return v + 1;
			} else {
				if (point[X] < center[X])
					return v + 2;
				else
					return v + 3;
			}
		}

		Vec3 Octants::normal_origin_for_partition_index(const std::size_t &i) {
			switch (i) {
			case 0:
				return Vec3 (0.0, 0.0, 0.0);
			case 1:
				return Vec3 (0.5, 0.0, 0.0);
			case 2:
				return Vec3 (0.0, 0.5, 0.0);
			case 3:
				return Vec3 (0.5, 0.5, 0.0);
			case 4:
				return Vec3 (0.0, 0.0, 0.5);
			case 5:
				return Vec3 (0.5, 0.0, 0.5);
			case 6:
				return Vec3 (0.0, 0.5, 0.5);
			case 7:
				return Vec3 (0.5, 0.5, 0.5);
			}

			assert(false && "Invalid partition index!");

			return ZERO;
		}

		Vec3 Octants::offset_origin_for_partition_index(const std::size_t &i) {
			return ((normal_origin_for_partition_index(i) + 0.25) * 2.0) - 0.5;
		}

		Octants::PartitionLocation Octants::location_for_direction (const Direction &dir) {
			switch ((unsigned)dir) {
			case (BOTTOM | LEFT | NEAR):
				return BottomLeftNear;
			case (BOTTOM | RIGHT | NEAR):
				return BottomRightNear;
			case (TOP | LEFT | NEAR):
				return TopLeftNear;
			case (TOP | RIGHT | NEAR):
				return TopRightNear;

			case (BOTTOM | LEFT | FAR):
				return BottomLeftFar;
			case (BOTTOM | RIGHT | FAR):
				return BottomRightFar;
			case (TOP | LEFT | FAR):
				return TopLeftFar;
			case (TOP | RIGHT | FAR):
				return TopRightFar;

			default:
				break;
			}

			assert(false && "Invalid partition direction!");

			return BottomLeftNear;
		}
	}
}
