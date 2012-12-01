//
//  Geometry/AlignedBox.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 3/01/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_ALIGNEDBOX_H
#define _EUCLID_GEOMETRY_ALIGNEDBOX_H

#include "Geometry.h"
#include "../Numerics/Matrix.h"
#include "../Numerics/Matrix.Projections.h"
#include "../Numerics/Interpolate.h"

namespace Euclid {
	namespace Geometry {
		enum SubtractResolution {
			SUBTRACT_SMALLEST = 0,
			SUBTRACT_BIGGEST = 1,
			SUBTRACT_NEGATIVE = 2,
			SUBTRACT_POSITIVE = 3
		};

		enum BoxEdge {
			NEGATIVE_EDGE = 2,
			POSITIVE_EDGE = 3
		};

		/// An Axis Aligned Bounding Box.
		template <dimension D, typename NumericT>
		class AlignedBox {
		public:
			typedef Vector<D, NumericT> VectorT;
			typedef Vector<D, bool> BoolVectorT;

		protected:
			VectorT _min, _max;

			// This could be optimised by using a function ptr rather than a bool, but need to profile!
			inline bool compare_edge (const NumericT &a, const NumericT &b, bool allow_equality) const {
				if (allow_equality)
					return a <= b;
				else
					return a < b;
			}

		public:
// MARK: Constructors
			AlignedBox ()
			{
			}

			AlignedBox (const Zero &) : _min(ZERO), _max(ZERO)
			{
			}

			AlignedBox (const Identity &, const NumericT &n = 1) : _min(ZERO), _max(n)
			{
			}

			AlignedBox (const VectorT & size) : _min(ZERO), _max(size) {
			}

			AlignedBox (const VectorT &min, const VectorT &max) : _min(min), _max(max)
			{
			}

			static AlignedBox from_center_and_size (const VectorT &center, const VectorT &size)
			{
				AlignedBox box;
				box.set_center_and_size(center, size);
				return box;
			}

			static AlignedBox from_origin_and_size (const VectorT &origin, const VectorT &size)
			{
				AlignedBox box;
				box.set_origin_and_size(origin, size);
				return box;
			}

			// Bounds _min and _max must infact be minima and maxima.
			// This function establishes this condition.
			void normalize_bounds () {
				VectorT t(ZERO);
				VectorT c(_min);

				_min.constrain(_max, t - 1);
				_max.constrain(c, t + 1);

				for (dimension i = 0; i < D; i += 1) {
					_min[i] = std::min(_min[i], _max[i]);
					_max[i] = std::max(_min[i], _max[i]);
				}
			}

			// Copy Semantics
			template <typename M>
			AlignedBox (const AlignedBox<D, M> &other) : _min(other.min()), _max(other.max()) {}

			template <typename M>
			AlignedBox & operator= (const AlignedBox<D, M> &other) {
				_min = other.min(); _max = other.max();
			}

			/// Returns true if the box does not enclose any space.
			bool is_degenerate () const { return _min == _max; }

			/// Returns true if the box is in normal form - i.e. the minimum point is lesser than the maximum point.
			bool is_normal () const { return _min.less_than(_max); }

			/// Test for exact equivalence
			bool operator== (const AlignedBox & other) const
			{
				return min() == other.min() && max() == other.max();
			}

			bool operator!= (const AlignedBox & other) const
			{
				return !(*this == other);
			}

			bool equivalent (const AlignedBox & other) const
			{
				return min().equivalent(other.min()) && max().equivalent(other.max());
			}

			/// Returns the minimum point of the box.
			const VectorT &min () const { return _min; }

			/// Returns the maximum point of the box.
			VectorT &min () { return _min; }

			/// Returns the minimum point of the box.
			const VectorT &max () const { return _max; }

			/// Returns the maximum point of the box.
			VectorT &max () { return _max; }

			/// Returns the origin of the box.
			/// @sa min()
			const VectorT& origin () const { return _min; }

			/// Returns the center of the box.
			VectorT center () const { return (_min + _max) / NumericT(2); }

			/// Returns the size of the box.
			VectorT size () const { return _max - _min; }

			/// Set a particular corner.
			void set_corner (const BoolVectorT & cnr, const VectorT & adj);

			/// Set the value for a particular axis for a given corner.
			void set_corner (const BoolVectorT & cnr, const dimension & axis, const NumericT & amnt);

			/// Set the center and size of the box.
			/// @sa from_center_and_size()
			void set_center_and_size (const VectorT &center, const VectorT &size)
			{
				_min = center - (size/2);
				_max = center + (size/2);
			}

			/// Resize the box but keep the same origin.
			void set_size_from_origin (const VectorT & size)
			{
				_max = _min + size;
			}

			/// Set a new origin and size.
			void set_origin_and_size (const VectorT &origin, const VectorT &size)
			{
				_min = origin;
				this->set_size_from_origin(size);
			}

			/// Set a new origin, maintain the current size.
			/// @sa set_origin_and_size
			void set_origin (const VectorT & origin) {
				this->set_origin_and_size(origin, this->size());
			}

			/// Recenter the box, maintaining current size.
			AlignedBox& recenter_at (const VectorT & center)
			{
				Vector<D> half_current_size = size() / 2;
				_min = center - half_current_size;
				_max = center + half_current_size;

				return *this;
			}

			/// Translate the box in-place, maintaining the current size.
			AlignedBox& translate_by (const VectorT & offset)
			{
				_min += offset;
				_max += offset;

				return *this;
			}

			/// Return a copy of this box, translated by the given offset.
			AlignedBox translated_by (const VectorT & offset) const {
				return AlignedBox(*this).translate_by(offset);
			}

			/// Adjust an individual axis of the box by the given amount
			void shift_axis (dimension axis, const NumericT &amount) {
				_min[axis] += amount;
				_max[axis] += amount;
			}

			/// Return a particular corner of the box, given by an index vector
			/// The components of d must be either false or true, and this will select the value from either min or max respectively.
			inline VectorT corner (const BoolVectorT & d) const {
				VectorT result;

				for (std::size_t i = 0; i < D; i += 1)
					result[i] = d[i] ? _max[i] : _min[i];

				return result;
			}

			/// Expand the box to include the other point.
			AlignedBox& union_with_point (const VectorT & point) {
				Vector<D> t(ZERO);

				_min.constrain(point, t - 1);
				_max.constrain(point, t + 1);

				return *this;
			}

			/// Expand the box to include the other box.
			AlignedBox& union_with_box (const AlignedBox & other) {
				Vector<D> t(ZERO);

				_min.constrain(other.min(), t - 1);
				_max.constrain(other.max(), t + 1);

				return *this;
			}

			/// Clip the current box to the given box. This function is very simple.
			/// Case 1: this and other do not intersect; result is box the same as other
			/// Case 2: this and other intersect partially; the result is that this will be resized to fit completely within other
			/// Case 3: this is completely within other; no change will occur.
			/// @sa intersects_with()
			AlignedBox& clip_to_box (const AlignedBox & other)
			{
				Vector<D> t(ZERO);

				_min.constrain(other._min, t + 1);
				_max.constrain(other._max, t - 1);

				return *this;
			}

			/// Given an orientation, aligns this box within a superbox.
			void align_within_super_box (const AlignedBox & super_box, const Vector<D> & orientation)
			{
				for (dimension i = 0; i < D; ++i) {
					RealT width = _max[i] - _min[i];
					RealT super_width = super_box._max[i] - super_box._min[i];
					RealT scale = super_width - width;
					RealT offset = orientation[i] * scale;
					RealT distance = (super_box._min[i] + offset) - _min[i];

					shift_axis(i, distance);
				}
			}

			/// Returns the orientation of one box relative to another.
			Vector<D> orientation_of (const AlignedBox &other) const {
				Vector<D> o;

				for (dimension i = 0; i < D; ++i) {
					if (other._min[i] < _min[i])
						o[i] = 0;
					else if (other._max[i] > _max[i])
						o[i] = 1;
					else {
						RealT min_width = other._max[i] - other._min[i];
						RealT this_width = _max[i] - _min[i];
						RealT s = this_width - min_width;
						RealT offset = other._min[i] - _min[i];

						if (s == 0)
							o[i] = 0.5;
						else
							o[i] = offset / s;
					}
				}

				return o;
			}

			/// Find the absolute offset of a point within the box.
			Vector<D> offset_of(const Vector<D> & point) const {
				return point - _min;
			}

			/// Find the relative position of a point inside the box.
			Vector<D> relative_offset_of(const Vector<D> &point) const {
				return offset_of(point) / size();
			}

			/// Calculate an absolute point based on a relative offset.
			Vector<D> absolute_position_of(const Vector<D> &offset) const {
				return _min + (size() * offset);
			}

			/// Returns a sphere that encloses the entire box.
			Sphere<D, NumericT> bounding_sphere () const;

			/// Tests whether this is completely within other.
			/// @returns true when this is within other, depending on the includes_edges parameter.
			/// @sa contains_point()
			bool contains_box (const AlignedBox<D, NumericT> & other, bool includes_edges = true) const {
				return contains_point(other.min(), includes_edges) && contains_point(other.max(), includes_edges);
			}

			/// Tests whether the box contains a point.
			/// @returns true when the point is within the box, or on an edge, depending on the includes_edges parameter.
			bool contains_point (const Vector<D>& point, bool includes_edges = true) const
			{
				bool result = false;

				if (!includes_edges)
					result = point.less_than(max()) && point.greater_than(min());
				else
					result = point.less_than_or_equal(max()) && point.greater_than_or_equal(min());

				return result;
			}

			/// Tests whether this box intersects with another box.
			/// @returns true when the two boxes overlap, or edges touch, depending on includes_edges parameter.
			bool intersects_with (const AlignedBox<D, NumericT> & other, bool includes_edges = true) const
			{
				for (dimension i = 0; i < D; ++i) {
					if (compare_edge(_max[i], other._min[i], !includes_edges) || compare_edge(other._max[i], _min[i], !includes_edges))
						return false;
				}

				return true;
			}

			// Ordered subtraction methods
			void subtract_in_order (const AlignedBox & other, const Vector<D, dimension> & order);
			void subtract_in_order (const AlignedBox & other, const Vector<D, dimension> & order, const Vector<D, unsigned> & cuts);

			// Translation based subtraction methods
			// These methods assume that only the edges specified by the orientation may overlap. For a more general
			// approach, the old methods may be more useful.
			// These methods remove the need for a lot of complex maths, and thus are faster. However, subtract_in_order
			// Is guaranteed to work in ALL cases.
			AlignedBox subtract_using_translation (const AlignedBox & from, const AlignedBox & to, const NumericT & offset = 0);

			// This just subtracts a single edge from another box, essentially a helper for subtract_using_translation
			bool subtract_edge (const AlignedBox & other, dimension axis, const BoxEdge & edge, const NumericT & offset = 0);
		};

		typedef AlignedBox<2, RealT> AlignedBox2;
		typedef AlignedBox<3, RealT> AlignedBox3;
		typedef AlignedBox<2, int> AlignedBox2i;
		typedef AlignedBox<3, int> AlignedBox3i;
		typedef AlignedBox<2, unsigned> AlignedBox2u;
		typedef AlignedBox<3, unsigned> AlignedBox3u;

		/// Return an ortographic projection as described by the given AlignedBox.
		template <typename NumericT>
		Matrix<4, 4, NumericT> orthographic_projection_matrix (const AlignedBox<3, NumericT> & box) {
			Vector<3, NumericT> translation = (box.max() + box.min()) / (box.max() - box.min());
			return orthographic_projection_matrix(translation, box.size());
		}
	}
}

#include "AlignedBox.hpp"

#endif
