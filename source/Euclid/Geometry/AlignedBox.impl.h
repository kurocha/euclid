//
//  Geometry/AlignedBox.impl.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 2/12/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_ALIGNEDBOX_H
#error This header should not be included manually. Include AlignedBox.h instead.
#endif

#include "Line.h"
#include "Sphere.h"

namespace Euclid
{
	namespace Geometry
	{
		template <unsigned D, typename NumericT>
		void AlignedBox<D, NumericT>::set_corner (const BoolVectorT & cnr, const VectorT & adj)
		{
			for (std::size_t axis = 0; axis < D; axis += 1)
				set_corner(cnr, axis, adj[axis]);
		}

		template <unsigned D, typename NumericT>
		void AlignedBox<D, NumericT>::set_corner (const BoolVectorT & cnr, const unsigned & axis, const NumericT & amnt)
		{
			if (cnr[axis])
				_max[axis] = amnt;
			else
				_min[axis] = amnt;
		}

// MARK: Line Intersection Tests

		template <unsigned D, typename NumericT>
		Sphere<D, NumericT> AlignedBox<D, NumericT>::bounding_sphere () const
		{
			return Sphere<D, NumericT>(center(), (center() - _min).length());
		}

		template <unsigned D, typename NumericT>
		void AlignedBox<D, NumericT>::subtract_in_order (const AlignedBox & other, const Vector<D, unsigned> & order) {
			subtract_in_order(other, order, Vector<D, SubtractResolution>(SUBTRACT_SMALLEST, SUBTRACT_SMALLEST, SUBTRACT_SMALLEST));
		}

		template <unsigned D, typename NumericT>
		void AlignedBox<D, NumericT>::subtract_in_order (const AlignedBox & other, const Vector<D, unsigned> & order, const Vector<D, SubtractResolution> & cuts)
		{
			// This function is fairly complex, for a good reason - it does a fairly complex geometric operation.
			// This operation can be summarised as subtracting one box from another. The reason why this is complex is because there are many edge cases to
			// consider, and this function works for boxes in n dimensions.

			Vector<D, unsigned> k(IDENTITY, 2);

			// Total number of corners for a given D.
			const std::size_t CORNERS = 1 << D;

			// We consider every corner and its opporsite.
			// We do this because we need to consider if the corner intersects the other shape.
			for (std::size_t c = 0; c < CORNERS; c += 1) {
				// We consider both the given corner and its opporsite
				BoolVectorT current_corner(k.distribute(c));
				BoolVectorT opporsite_corner(!current_corner);

				VectorT this_current_corner(corner(current_corner)), this_opporsite_corner(corner(opporsite_corner));
				VectorT other_current_corner(other.corner(current_corner)), other_opporsite_corner(other.corner(opporsite_corner));

				bool intersects = contains_point(other_current_corner, true) || other.contains_point(this_opporsite_corner, true);

				// We consider each axis for these corners
				for (std::size_t axis_index = 0; axis_index < D; axis_index += 1) {
					// We pick out the current axis
					unsigned axis = order[axis_index];

					// We consider the lines on this axis
					NumericT a1 = this_current_corner[axis];
					NumericT a2 = this_opporsite_corner[axis];
					NumericT b1 = other_current_corner[axis];
					NumericT b2 = other_opporsite_corner[axis];

					// We copy the corner vectors just in case we need to swap them
					BoolVectorT c1(current_corner), c2(opporsite_corner);

					// We need to compare things relatively, so a1 should be smaller than a2.
					// if not, we swap everything around.
					if (a1 > a2) {
						std::swap(a1, a2);
						std::swap(b1, b2);
						std::swap(c1, c2);
					}

					if (b1 > a1 && b2 >= a2 && intersects) {
						// Remove the right hand segment a2 = b1
						set_corner(c2, axis, b1);
						break;
					}

					if (b1 <= a1 && b2 < a2 && intersects) {
						// Remove the left hand segment a1 = b2
						set_corner(c1, axis, b2);
						break;
					}

					if (a1 < b1 && a2 > b2 && intersects_with(other)) {
						// The line is being split in two by the subtracted segment.
						// We will choose the larger segment
						RealT left_side = b1 - a1;
						RealT right_side = a2 - b2;

						// We use cuts to determine how to resolve these cases with more than one result
						if (cuts[axis] == SUBTRACT_POSITIVE || (left_side > right_side && cuts[axis] == SUBTRACT_SMALLEST)) {
							// remove right side (positive side)
							set_corner(c2, axis, b1);
						} else {
							// remove left side (negative side)
							set_corner(c1, axis, b2);
						}

						break;
					}

					if (a1 > b1 && a2 < b2) {
						// This case is when we are subtracting more than we have available,
						// ie, both ends of the line are within the subtracting bounds
						// There is nothing we can do reasonably within this case, so we just ignore it
						// and hope for the best ^_^.
					}
				}
			}
		}

		template <unsigned D, typename NumericT>
		bool AlignedBox<D, NumericT>::subtract_edge (const AlignedBox<D, NumericT> & other, unsigned axis, const BoxEdge & edge, const NumericT & offset)
		{
			NumericT a, b, c;

			// Offset indicates the distance the edge must be from the other box if they are on top of each other, ie a == b, then a -> a + offset

			if (edge == NEGATIVE_EDGE) {
				a = _min[axis];
				b = other._max[axis];
				c = _max[axis];

				if ((b + offset) < c && compare_edge(a, b, !Numerics::equal_within_tolerance(offset, (NumericT)0.0))) {
					//std::cout << "Adjusting [" << axis << "] from " << a << " to " << b - offset << std::endl;
					_min[axis] = b + offset;
					return true;
				}
			} else {
				a = _max[axis];
				b = other._min[axis];
				c = _min[axis];

				if ((b + offset) > c && compare_edge(b, a, !Numerics::equal_within_tolerance(offset, (NumericT)0.0))) {
					//std::cout << "Adjusting [" << axis << "] from " << a << " to " << b - offset << std::endl;
					_max[axis] = b - offset;
					return true;
				}
			}

			//std::cout << "Did not adjust [" << axis << "] from " << a << " to " << b - offset << " other " << c << std::endl;
			return false;
		}

		template <unsigned D, typename NumericT>
		AlignedBox<D, NumericT> AlignedBox<D, NumericT>::subtract_using_translation (const AlignedBox<D, NumericT> & from, const AlignedBox<D, NumericT> & to,
		                                                                             const NumericT & offset)
		{
			VectorT orientation = from.orientation_of(to);
			AlignedBox translation = from;

			for (unsigned i = 0; i < D; ++i) {
				AlignedBox to_copy = to;
				bool result = false;

				if (orientation[i] == 0.0) {
					result = to_copy.subtract_edge(*this, i, NEGATIVE_EDGE, offset);
				} else if (orientation[i] == 1.0) {
					result = to_copy.subtract_edge(*this, i, POSITIVE_EDGE, offset);
				}

				if (result) {
					to_copy.union_with_box(translation);

					result = to_copy.intersects_with(*this, false);

					if (!result)
						translation.union_with_box(to_copy);
				}
			}

			return translation;
		}
	}
}
