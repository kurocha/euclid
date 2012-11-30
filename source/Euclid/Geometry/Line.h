//
//  Geometry/Line.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 15/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_LINE_H
#define _EUCLID_GEOMETRY_LINE_H

#include "Geometry.h"
#include "../Numerics/Matrix.h"

namespace Euclid {
	namespace Geometry {
		template <dimension D, typename NumericT>
		class Line {
		protected:
			typedef typename RealTypeTraits<NumericT>::RealT RealT;
			typedef Vector<D, NumericT> VectorT;

			VectorT _point;
			VectorT _direction;

		public:
			Line () = default;

			Line (const VectorT & direction) : _point(ZERO), _direction(direction)
			{
			}

			Line (const VectorT & point, const VectorT & direction) : _point(point), _direction(direction)
			{
			}

			const VectorT & point () const { return _point; }
			const VectorT & direction () const { return _direction; }

			void set_point (const VectorT & point) { _point = point; }
			void set_direction (const VectorT & direction) { _direction = direction; }

			VectorT point_at_time (const NumericT & t) const {
				return _point + (_direction * t);
			}

			/// Returns the time on the line where a point is closest to the given point.
			NumericT time_for_closest_point (const VectorT & p3) const {
				auto p1 = _point;
				auto p2 = _point + _direction;

				auto d = _direction.length_squared();
				NumericT t = 0;

				for (dimension i = 0; i < D; ++i)
					t += (p3[i] - p1[i]) * (p2[i] - p1[i]);

				return t / d;
			}

			VectorT point_for_closest_point (const VectorT & p) const {
				return point_at_time(time_for_closest_point(p));
			}

			RealT shortest_distance_to_point (const VectorT &p) const {
				return (p - point_for_closest_point(p)).length();
			}

			// Calculates the factor for line equations
			RealT factor (const RealT & v, dimension i) const {
				return RealT(v + _point[i]) / RealT(_direction[i]);
			}

			Line<D-1, NumericT> reduce() const {
				return Line<D-1, NumericT>(point().reduce(), direction().reduce());
			}

			bool equivalent (const Line<D, NumericT> & other) {
				// Are we pointing in the same direction
				if (!_direction.equivalent(other._direction))
					return false;

				// Is the distance between the parallel lines equivalent to zero?
				return Numerics::equivalent(shortest_distance_to_point(other._point), 0);
			}

			bool intersects_with (const Line<D, NumericT> & other, NumericT & this_time, NumericT & other_time) const;
			bool intersects_with (const AlignedBox<D, NumericT> & other, NumericT & t1, NumericT & t2) const;

			/// Helper function for intersection testing where less information is needed.
			bool intersects_with (const AlignedBox<D, NumericT> & other, VectorT & at) const
			{
				NumericT t1, t2;

				bool result = intersects_with(other, t1, t2);
				if (result) at = point_at_time(t1);

				return result;
			}

			///@todo Implement this function
			/// bool clip_to_box (const AlignedBox<D, NumericT> & other, LineSegment<D, NumericT> & segment) const;

			/// Construct a line given two points
			static Line from (const VectorT & from, const VectorT & to) {
				return Line(from, (to - from).normalize());
			}
		};
		
		template <typename NumericT>
		Vector<2, NumericT> normal(const Line<2, NumericT> & line)
		{
			return normal(line.direction());
		}
		
		typedef Line<2> Line2;
		typedef Line<3> Line3;

		extern template class Line<2, RealT>;
		extern template class Line<3, RealT>;

		template <dimension D, typename NumericT>
		inline Line<D, NumericT> operator+ (const Line<D, NumericT> & l, const Vector<D, NumericT> & v);

		template <dimension D, typename NumericT>
		Line<D, NumericT> operator- (const Line<D, NumericT> & l, const Vector<D, NumericT> & v);

		template <dimension D, typename NumericT>
		class LineSegment {
		public:
			typedef typename RealTypeTraits<NumericT>::RealT RealT;
			typedef Vector<D, NumericT> VectorT;

		protected:
			VectorT _start;
			VectorT _end;

		public:
			LineSegment () = default;

			LineSegment (const VectorT & end) : _start(ZERO), _end(end)
			{
			}

			LineSegment (const VectorT & start, const VectorT & end) : _start(start), _end(end)
			{
			}

			LineSegment (const Line<D, NumericT> & line, const NumericT & start_time, const NumericT & end_time) : _start(line.point_at_time(start_time)), _end(line.point_at_time(end_time))
			{
			}

			VectorT point_at_time(const RealT & t) const {
				return _start + (offset() * t);
			}

			Line<D, NumericT> to_line () const {
				return Line<D, NumericT>(start(), offset().normalize());
			}

			/// Is this segment zero-length?
			bool is_degenerate () {
				return _start.equivalent(_end);
			}

			bool intersects_with (const AlignedBox<D, NumericT> & other, VectorT & at) const;
			bool intersects_with (const LineSegment<D, NumericT> & other, NumericT & this_time, NumericT & other_time) const;
			bool intersects_with (const LineSegment<D, NumericT> & other, LineSegment<D, NumericT> & overlap) const;

			const VectorT & start () const {
				return _start;
			}

			const VectorT & end () const {
				return _end;
			}

			VectorT & start () {
				return _start;
			}

			VectorT & end () {
				return _end;
			}

			VectorT center () {
				return (_start + _end) / 2.0;
			}

			VectorT offset () const {
				return _end - _start;
			}

			Vector<D> direction () const {
				return (_end - _start).normalize();
			}
		};
		
		template <typename NumericT>
		Vector<2, NumericT> normal(const LineSegment<2, NumericT> & line_segment)
		{
			return normal(line_segment.direction());
		}

		typedef LineSegment<2> LineSegment2;
		typedef LineSegment<3> LineSegment3;

		extern template class LineSegment<2, RealT>;
		extern template class LineSegment<3, RealT>;
	}
}

#include "Line.hpp"

#endif
