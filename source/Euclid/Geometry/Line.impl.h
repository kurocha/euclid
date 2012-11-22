//
//  Geometry/Line.impl.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 16/11/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_LINE_H
#error This header should not be included manually. Include Line.h instead.
#endif

#include "Intersection.h"

namespace Euclid
{
	namespace Geometry
	{
// MARK: -
// MARK: class LineTranslationTraits

		template <typename NumericT>
		typename LineTranslationTraits<3, NumericT>::MatrixT LineTranslationTraits<3, NumericT>::transformation_to_mate_with_line (const LineT & other, const VectorT & normal) const
		{
			const LineT * t = static_cast<const LineT*>(this);

			// Optimization
			if (t->direction() == other.direction())
				return translation_to_mate_with_point(other.point());
			else {
				MatrixT translation_to_origin = Mat44::translating_matrix(-t->point());
				MatrixT translation_from_origin = Mat44::translating_matrix(t->point());
				return translation_to_mate_with_point(other.point()) * translation_from_origin * rotation_to_mate_with_direction(other.direction(), normal) * translation_to_origin;
			}
		}

		template <typename NumericT>
		typename LineTranslationTraits<3, NumericT>::MatrixT LineTranslationTraits<3, NumericT>::translation_to_mate_with_point (const VectorT & point) const
		{
			const LineT * t = static_cast<const LineT*>(this);

			return MatrixT::translating_matrix(point - t->point());
		}

		template <typename NumericT>
		typename LineTranslationTraits<3, NumericT>::MatrixT LineTranslationTraits<3, NumericT>::rotation_to_mate_with_direction (const VectorT & direction, const VectorT & normal) const
		{
			const LineT * t = static_cast<const LineT*>(this);

			return MatrixT::rotating_matrix(direction, t->direction(), normal);
		}

		template <typename NumericT>
		void LineTranslationTraits<3, NumericT>::rotate (const VectorT & rotation_normal, const NumericRealT & angle)
		{
			LineT * t = static_cast<LineT*>(this);

			MatrixT rotation = MatrixT::rotating_matrix(angle, rotation_normal);

			t->set_direction(rotation * t->direction());
		}

		template <typename NumericT>
		typename LineTranslationTraits<3, NumericT>::LineT LineTranslationTraits<3, NumericT>::rotated_line (const VectorT & rotation_normal, const NumericRealT & angle) const
		{
			const LineT * t = static_cast<const LineT*>(this);

			LineT result(*t);

			result.rotate (rotation_normal, angle);

			return result;
		}

// MARK: -
// MARK: class Line

		template <dimension D, typename NumericT>
		Line<D, NumericT>::Line ()
		{
		}

		template <dimension D, typename NumericT>
		Line<D, NumericT>::Line (const Zero &) : _point(ZERO), _direction(ZERO)
		{
		}

		template <dimension D, typename NumericT>
		Line<D, NumericT>::Line (const Identity &, const NumericT & n) : _point(ZERO), _direction(IDENTITY, n)
		{
		}

		template <dimension D, typename NumericT>
		Line<D, NumericT>::Line (const VectorT & direction) : _point(ZERO), _direction(direction)
		{
		}

		template <dimension D, typename NumericT>
		Line<D, NumericT>::Line (const VectorT & point, const VectorT & direction) : _point(point), _direction(direction)
		{
		}

		template <dimension D, typename NumericT>
		NumericT Line<D, NumericT>::time_for_closest_point (const VectorT & p3) const
		{
			const VectorT & p1 = _point;
			VectorT p2 = _point + _direction;

			NumericT d = _direction.length2();
			NumericT t = 0;

			for (dimension i = 0; i < D; ++i)
				t += (p3[i] - p1[i]) * (p2[i] - p1[i]);

			return t / d;
		}

		template <typename NumericT>
		bool line_intersection_test (const Line<2, NumericT> & lhs, const Line<2, NumericT> & rhs, NumericT & left_time, NumericT & right_time)
		{
			Vector<2, NumericT> t = lhs.direction();
			Vector<2, NumericT> o = rhs.direction();

			// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
			float denom = (o[Y] * t[X]) - (o[X] * t[Y]);

			// Quick return
			if (denom == (NumericT)0) return false;

			NumericT na = o[X] * (lhs.point()[Y] - rhs.point()[Y]) - o[Y] * (lhs.point()[X] - rhs.point()[X]);
			left_time = na / denom;

			NumericT nb = t[X] * (lhs.point()[Y] - rhs.point()[Y]) - t[Y] * (lhs.point()[X] - rhs.point()[X]);
			right_time = nb / denom;

			return true;
		}

		template <dimension D, typename NumericT>
		bool line_intersection_test (const Line<D, NumericT> & lhs, const Line<D, NumericT> & rhs, NumericT & left_time, NumericT & right_time)
		{
			Vector<2, NumericT> lhs_point, lhs_dir, rhs_point, rhs_dir;
			lhs_point.set(lhs.point());
			lhs_dir.set(lhs.direction());

			rhs_point.set(rhs.point());
			rhs_dir.set(rhs.direction());

			Line<2, NumericT> lhs2d(lhs_point, lhs_dir), rhs2d(rhs_point, rhs_dir);
			if (line_intersection_test(lhs2d, rhs2d, left_time, right_time)) {
				// Collision occurred in 2-space, check in n-space
				Vector<D, NumericT> lhs_pt, rhs_pt;

				lhs_pt = lhs.point_at_time(left_time);
				rhs_pt = rhs.point_at_time(right_time);

				return lhs_pt.equivalent(rhs_pt);
			} else   {
				// No collision occurred.
				return false;
			}
		}

		template <dimension D, typename NumericT>
		bool Line<D, NumericT>::intersects_with (const Line<D, NumericT> & other, NumericT & this_time, NumericT & other_time) const
		{
			return line_intersection_test(*this, other, this_time, other_time);
		}

		template <typename NumericT>
		bool ray_slabs_intersection(NumericT start, NumericT dir, NumericT min, NumericT max, NumericT& tfirst, NumericT& tlast)
		{
			if (Number<NumericT>::equivalent(dir, 0))
				return (start < max && start > min);

			NumericT tmin = (min - start) / dir;
			NumericT tmax = (max - start) / dir;

			if (tmin > tmax) std::swap(tmin, tmax);

			if (tmax < tfirst || tmin > tlast)
				return false;

			if (tmin > tfirst) tfirst = tmin;

			if (tmax < tlast) tlast = tmax;

			return true;
		}

		template <dimension D, typename NumericT>
		bool Line<D, NumericT>::intersects_with(const AlignedBox<D, NumericT> &a, NumericT & t1, NumericT & t2) const {
			t1 = (NumericT)0;
			t2 = (NumericT)1;

			for (dimension i = 0; i < D; i += 1) {
				if (!ray_slabs_intersection(_point[i], _direction[i], a.min()[i], a.max()[i], t1, t2)) return false;
			}

			return true;
		}

		template <dimension D, typename NumericT>
		inline Line<D, NumericT> operator+ (const Line<D, NumericT> &l, const Vector<D, NumericT> &v) {
			return Line<D, NumericT>(l.point() + v, l.direction());
		}

		template <dimension D, typename NumericT>
		inline Line<D, NumericT> operator- (const Line<D, NumericT> &l, const Vector<D, NumericT> &v) {
			return Line<D, NumericT>(l.point() - v, l.direction());
		}

// MARK: -
// MARK: class LineSegment

		template <dimension D, typename NumericT>
		LineSegment<D, NumericT>::LineSegment ()
		{
		}

		template <dimension D, typename NumericT>
		LineSegment<D, NumericT>::LineSegment (const Zero &) : _start(ZERO), _end(ZERO)
		{
		}

		template <dimension D, typename NumericT>
		LineSegment<D, NumericT>::LineSegment (const Identity &, const NumericT & n) : _start(ZERO), _end(IDENTITY, n)
		{
		}

		template <dimension D, typename NumericT>
		LineSegment<D, NumericT>::LineSegment (const Line<D, NumericT> & line, const NumericT & start_time, const NumericT & end_time)
		{
			_start = line.point_at_time(start_time);
			_end = line.point_at_time(end_time);
		}

		template <dimension D, typename NumericT>
		LineSegment<D, NumericT>::LineSegment (const VectorT & start, const VectorT & end) : _start(start), _end(end)
		{
		}

		template <dimension D, typename NumericT>
		bool LineSegment<D, NumericT>::intersects_with (const AlignedBox<D, NumericT> &other, VectorT & at) const
		{
			Vector<D, NumericT> d((end() - start()).normalize());
			Line<D, NumericT> l(start(), d);

			l.intersects_with(other, at);

			return (end() - at).normalize() == d;
		}

		template <typename NumericT>
		bool line_intersection_test (const LineSegment<1, NumericT> & lhs, const LineSegment<1, NumericT> & rhs, LineSegment<1, NumericT> & overlap)
		{
			NumericT lmin, lmax, rmin, rmax;

			lmin = lhs.start()[X];
			lmax = lhs.end()[X];

			if (lmin > lmax) std::swap(lmin, lmax);

			rmin = rhs.start()[X];
			rmax = rhs.end()[X];

			if (rmin > rmax) std::swap(rmin, rmax);

			Vector<2, NumericT> o;
			if (segments_intersect(vector(lmin, lmax), vector(rmin, rmax), o)) {
				overlap = LineSegment<1, NumericT>(vector(o[0]), vector(o[1]));
				return true;
			} else   {
				return false;
			}
		}

		template <typename NumericT>
		bool line_intersection_test (const LineSegment<2, NumericT> & lhs, const LineSegment<2, NumericT> & rhs, NumericT & left_time, NumericT & right_time)
		{
			Vector<2, NumericT> t = lhs.offset();
			Vector<2, NumericT> o = rhs.offset();

			// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
			float denom = (o[Y] * t[X]) - (o[X] * t[Y]);

			// Quick return
			if (denom == 0.0f) return false;

			float na = o[X] * (lhs.start()[Y] - rhs.start()[Y]) - o[Y] * (lhs.start()[X] - rhs.start()[X]);
			left_time = na / denom;

			// Quick return
			if (left_time < 0.0 || left_time > 1.0) return false;

			float nb = t[X] * (lhs.start()[Y] - rhs.start()[Y]) - t[Y] * (lhs.start()[X] - rhs.start()[X]);
			right_time = nb / denom;

			// Quick return
			if (right_time < 0.0 || right_time > 1.0) return false;

			return true;
		}

		template <dimension D, typename NumericT>
		bool line_intersection_test (const LineSegment<D, NumericT> & lhs, const LineSegment<D, NumericT> & rhs, NumericT & left_time, NumericT & right_time)
		{
			Vector<2, NumericT> lhs_start, lhs_end, rhs_start, rhs_end;
			lhs_start.set(lhs.start());
			lhs_end.set(lhs.end());

			rhs_start.set(rhs.start());
			rhs_end.set(rhs.end());

			LineSegment<2, NumericT> lhs2d(lhs_start, lhs_end), rhs2d(rhs_start, rhs_end);
			if (line_intersection_test(lhs2d, rhs2d, left_time, right_time)) {
				// Collision occurred in 2-space, check in n-space
				Vector<D, NumericT> lhs_pt, rhs_pt;

				lhs_pt = lhs.point_at_time(left_time);
				rhs_pt = rhs.point_at_time(right_time);

				return lhs_pt.equivalent(rhs_pt);
			} else   {
				// No collision occurred.
				return false;
			}
		}

		template <dimension D, typename NumericT>
		bool LineSegment<D, NumericT>::intersects_with (const LineSegment<D, NumericT> & other, LineSegment<D, NumericT> & overlap) const
		{
			return line_intersection_test(*this, other, overlap);
		}

		template <dimension D, typename NumericT>
		bool LineSegment<D, NumericT>::intersects_with (const LineSegment<D, NumericT> & other, NumericT & left_time, NumericT & right_time) const
		{
			return line_intersection_test(*this, other, left_time, right_time);
		}
	}
}
