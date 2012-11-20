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
		template <unsigned D, typename NumericT>
		class LineTranslationTraits {
		};

		template <typename NumericT>
		class LineTranslationTraits<3, NumericT>{
		public:
			typedef typename RealType<NumericT>::RealT NumericRealT;
			typedef Matrix<4, 4, NumericRealT> MatrixT;
			typedef Vector<3, NumericRealT> VectorT;
			typedef Line<3, NumericT> LineT;

			MatrixT transformation_to_mate_with_line (const LineT & other, const VectorT & normal) const;
			MatrixT translation_to_mate_with_point (const VectorT & point) const;
			MatrixT rotation_to_mate_with_direction (const VectorT & direction, const VectorT & normal) const;

			void rotate (const VectorT & rotation_normal, const NumericRealT & angle);
			LineT rotated_line (const VectorT & rotation_normal, const NumericRealT & angle) const;
		};

		template <unsigned D, typename NumericT>
		class Line : public LineTranslationTraits<D, NumericT>{
		protected:
			typedef typename RealType<NumericT>::RealT NumericRealT;
			typedef Vector<D, NumericT> VectorT;

			VectorT _point;
			VectorT _direction;

		public:
			Line ();
			Line (const Zero &);
			Line (const Identity &, const NumericT & n = 1);

			Line (const VectorT & direction);
			Line (const VectorT & point, const VectorT & direction);

			const VectorT & point () const
			{
				return _point;
			}

			const VectorT & direction () const
			{
				return _direction;
			}

			void set_point (const VectorT & point)
			{
				_point = point;
			}

			void set_direction (const VectorT & direction)
			{
				_direction = direction;
			}

			VectorT point_at_time (const NumericT & t) const
			{
				return point() + (direction() * t);
			}

			/// Returns the time on the line where a point is closest to the given point.
			NumericT time_for_closest_point (const VectorT & p3) const;

			VectorT point_for_closest_point (const VectorT & p) const
			{
				return point_at_time(time_for_closest_point(p));
			}

			NumericRealT shortest_distance_to_point (const VectorT &p) const
			{
				return (p - point_for_closest_point(p)).length();
			}

			// Calculates the factor for line equations
			NumericRealT factor (const NumericRealT & v, std::size_t i) const
			{
				return (v + _point[i]) / (NumericRealT)_direction[i];
			}

			Line<D-1, NumericT> reduce () const
			{
				return Line<D-1, NumericT>(point().reduce(), direction().reduce());
			}

			bool equivalent (const Line<D, NumericT> & other)
			{
				// Are we pointing in the same direction
				if (!_direction.equivalent(other._direction))
					return false;

				// Is the distance between the parallel lines equivalent to zero?
				return Number<NumericT>::equivalent(shortest_distance_to_point(other._point), 0);
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

		typedef Line<2> Line2;
		typedef Line<3> Line3;

		template <unsigned D, typename NumericT>
		inline Line<D, NumericT> operator+ (const Line<D, NumericT> & l, const Vector<D, NumericT> & v);

		template <unsigned D, typename NumericT>
		Line<D, NumericT> operator- (const Line<D, NumericT> & l, const Vector<D, NumericT> & v);

		template <unsigned D, typename NumericT>
		class LineSegment {
		public:
			typedef typename RealType<NumericT>::RealT NumericRealT;
			typedef Vector<D, NumericT> VectorT;

		protected:
			VectorT _start;
			VectorT _end;

		public:
			LineSegment ();
			LineSegment (const Zero &);
			LineSegment (const Identity &, const NumericT & n = 1);

			LineSegment (const Line<D, NumericT> & line, const NumericT & start_time, const NumericT & end_time);
			LineSegment (const VectorT & start, const VectorT & end);

			VectorT point_at_time(const RealT & t) const
			{
				return _start + (offset() * t);
			}

			Line<D, NumericT> to_line () const
			{
				return Line<D, NumericT>(start(), offset().normalize());
			}

			/// Is this segment zero-length?
			bool is_degenerate ()
			{
				return _start.equivalent(_end);
			}

			bool intersects_with (const AlignedBox<D, NumericT> & other, VectorT & at) const;
			bool intersects_with (const LineSegment<D, NumericT> & other, NumericT & this_time, NumericT & other_time) const;
			bool intersects_with (const LineSegment<D, NumericT> & other, LineSegment<D, NumericT> & overlap) const;

			const VectorT & start () const
			{
				return _start;
			}

			const VectorT & end () const
			{
				return _end;
			}

			VectorT & start ()
			{
				return _start;
			}

			VectorT & end ()
			{
				return _end;
			}

			VectorT center ()
			{
				return (_start + _end) / 2.0;
			}

			VectorT offset () const
			{
				return _end - _start;
			}

			Vector<D> direction () const
			{
				return (_end - _start).normalized_vector();
			}
		};

		typedef LineSegment<2> LineSegment2;
		typedef LineSegment<3> LineSegment3;
	}
}

#include "Line.impl.h"

#endif
