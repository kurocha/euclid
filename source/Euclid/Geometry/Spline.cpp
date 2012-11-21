//
//  Geometry/Spline.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 1/03/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//


#include "Spline.h"

#include "Triangle.h"
#include "Line.h"

namespace Euclid {
	namespace Geometry {
// MARK: -
// MARK: ISpline

		template <dimension D>
		ISpline<D>::~ISpline () {
		}

		template <dimension D>
		Vector<D> ISpline<D>::tangent_at_time(RealT t) const {
			// TODO: Need to write a better function !!
			const RealT f = 0.001;
			RealT t1 = Math::wrap(t-f, 1.0), t2 = Math::wrap(t+f, 1.0);

			assert(t1 >= 0.0 && t1 <= 1.0);
			assert(t2 >= 0.0 && t2 <= 1.0);

			return (point_at_time(t2) - point_at_time(t1)).normalize();
		}

		template <>
		Vec3 ISpline<3>::normal_at_time(RealT t) const {
			Vec3 tg = tangent_at_time(t);
			//Vec3 pt = point_at_time(t);

			Vec4 tg4 = tg << 0.0;
			Vec4 orth (0.0, 0.0, 0.0, 1.0);
			Vec4 ori = (tangent_at_time(t-0.2) - tangent_at_time(t+0.2)) << 1.0;

			Vec4 res (cross_product(tg4, orth, ori));

			return res.reduce().normalize();
		}

		template <>
		Vec2 ISpline<2>::normal_at_time(RealT t) const {
			Vec2 tg = tangent_at_time(t);

			Vec3 tg3 = tg << 0.0;
			Vec3 orth (0.0, 0.0, 1.0);

			return tg3.cross(orth).reduce().normalize();
		}

		template <dimension D>
		std::vector<RealT> ISpline<D>::nominal_times () const {
			RealT d = (RealT)1.0 / this->segments();
			std::vector<RealT> times;

			for (RealT t = 0; t <= (RealT)1.0; t += d)
				times.push_back(t);

			return times;
		}

		template <dimension D>
		std::vector<RealT> ISpline<D>::divide_and_append(int n, RealT res) const {
			// Entry point for the function below.
			// The higher res is, the more detail will be present.
			std::vector<RealT> times;

			divide_and_append(0.0, 1.0, n, 1.0 - (1.0 / res), true, times);

			return times;
		}

		template <dimension D>
		void ISpline<D>::divide_and_append(RealT t, RealT d, int n, RealT res2, bool first, std::vector<RealT> &times) const {
			RealT hd = d / 2.0;

			Vector<D> t1(tangent_at_time(t)), t2(tangent_at_time(t+d));

			bool divide = n > 0 || (t1.dot(t2) < res2 && n > -10) /* Sanity check */;

			// First point of segment
			if (divide) divide_and_append(t, hd, n - 1, res2, true & first, times);

			if (!divide) {
				if (first) times.push_back(t);

				times.push_back(t+d);
			}

			// Last point of segment
			if (divide) divide_and_append(t + hd, hd, n - 1, res2, false, times);
		}

		template <dimension D>
		std::vector<RealT> ISpline<D>::times_at_resolution(RealT resolution, unsigned divisions) const {
			// We must divide at least once per point.
			int d = (int)Math::ceil(Math::sqrt(this->points().size()) + 1);
			divisions = (unsigned)Math::max(d, divisions);

			return divide_and_append(divisions, resolution);
		}

		template <dimension D>
		std::size_t ISpline<D>::segments () const {
			// The number of segments between points
			return this->segment_points().size() - 1;
		}

		template <dimension D>
		std::size_t ISpline<D>::starting_point(RealT t) const {
			// The starting point for a given t in [0.0, 1.0]
			return (unsigned)Math::floor(this->segments() * t);
		}

		template <dimension D>
		RealT ISpline<D>::fractional_component(RealT t) const {
			// The fractional component (ie, in [0.0, 1.0]) of a particular segment.
			RealT m = this->segments() * t;
			return m - Math::floor(m);
		}

		template class ISpline<3>;
		template class ISpline<2>;

// MARK: -
// MARK: SplineWithNormal

		template <dimension D>
		SplineWithNormal<D>::SplineWithNormal(const SplineT * spline, const SplineT * normal_spline) : _spline(spline),
			_normal_spline(normal_spline)
		{
		}

		template <dimension D>
		SplineWithNormal<D>::~SplineWithNormal () {
		}

		template <dimension D>
		Vector<D> SplineWithNormal<D>::point_at_time(RealT t) const {
			return _spline->point_at_time(t);
		}

		template <dimension D>
		Vector<D> SplineWithNormal<D>::tangent_at_time(RealT t) const {
			return _spline->tangent_at_time(t);
		}

		template <dimension D>
		Vector<D> SplineWithNormal<D>::normal_at_time(RealT t) const {
			// We will assume we don't need to normalize this result, but
			// that could be dangerous.
			Vector<D> n1 ((_normal_spline->point_at_time(t) - _spline->point_at_time(t)).normalize());
			Vector<D> t1 (_spline->tangent_at_time(t));

			n1.normalize();

			Vector<D> n2 = t1.cross(n1);
			return n2.cross(t1).normalize();
		}

		template <dimension D>
		const std::vector<Vector<D>> & SplineWithNormal<D>::points() const {
			return _spline->points();
		}

		template <dimension D>
		class _SplineWithNormalGenerator {
		public:
			struct Intersection {
				bool parallel;
				Vector<D> intersection;
				bool tmp;
			};

			struct PlaneAtTime {
				RealT time;
				Plane<D> plane;
				bool interpolated;
			};

			struct NormalAtTime {
				RealT time;
				bool fixed;
				Vector<D> normal;
				bool tmp;
			};

			static std::vector<PlaneAtTime> interpolated_planes_for_spline(const ISpline<D> * spline, const std::vector<RealT> & times) {
				std::list<PlaneAtTime> planes;

				//Generate the plane for each point in the spline
				for (auto t = times.begin(); t != times.end(); t++) {
					PlaneAtTime pat;
					pat.interpolated = false;
					pat.time = *t;
					pat.plane = Plane<D>(spline->point_at_time(*t), spline->tangent_at_time(*t));

					planes.push_back(pat);
				}

				//Subdivide parallel planes that are in opporsite directions
				for (auto pl = planes.begin(); pl != planes.end(); pl++) {
					PlaneAtTime &p1 = *pl; pl++; // Fetch the current plane
					PlaneAtTime &p2 = *pl; // Fetch the next plane

					if (p1.plane.normal() == -p2.plane.normal()) {
						PlaneAtTime p2h;
						p2h.interpolated = true;
						p2h.time = p1.time + p2.time / (RealT)2.0;
						p2h.plane = Plane<D>(spline->point_at_time(p2h.time), spline->tangent_at_time(p2h.time));

						// Insert p2h after p1, before p2 (pl points to p2 atm).
						planes.insert(pl, p2h);
					}

					pl--; // We want to step from the next back to the current
				}

				return std::vector<PlaneAtTime>(planes.begin(), planes.end());
			}

			static void for_spline (const ISpline<D> * spline, const ISpline<D> * normal_spline) {
				std::vector<RealT> nts = spline->nominal_times();
				nts.pop_back();
				std::vector<Intersection> its;

				std::vector<PlaneAtTime> planes = interpolated_planes_for_spline(spline, nts);

				// Calculate plane intersection for non-parallel planes
				for (unsigned i = 0; i < planes.size(); i += 1) {
					PlaneAtTime &pat1 = planes[i];
					PlaneAtTime &pat2 = planes[(i+1) % planes.size()];

					Intersection it;
					it.intersection.zero();

					if (pat1.plane.is_parallel(pat2.plane)) {
						//These points are in sequence and in the same direction, ie '|' straight line
						it.parallel = true;
					} else {
						it.parallel = false;
						RealT t1 = nts[i], t3 = nts[i+1 % nts.size()];
						RealT t2 = (t1 + t3) / 2.0;

						Vector<D> pt1 = spline->point_at_time(t1);
						Vector<D> pt3 = spline->point_at_time(t3);

						// If two of the same points on the line.
						if (pt1 == pt3) continue;

						//Vector<D> pt3 = pt1 + spline->tangent_at_time(nts[i]) + spline->tangent_at_time(nts[i+1 % nts.size()]);
						Vector<D> pt2 = spline->point_at_time(t2);

						Triangle<D> tri (pt1, pt2, pt3);
						Plane<D> p3 (pt1, tri.normal());
						Line<D> line_intersection;

						// There must be an intersection
						assert(pat1.plane.intersects_with(pat2.plane, line_intersection));
						assert(p3.intersects_with(line_intersection, it.intersection));

						// it.intersection = (line_intersection.point_for_closest_point(pt1) + line_intersection.point_for_closest_point(pt2)) / 2.0;
						// This point gives us the best (shortest) normal from pt1 to pt2
					}

					it.tmp = pat1.interpolated;
					its.push_back(it);
				}

				// We need to find all fixed normals
				std::vector<NormalAtTime> normals;
				for (unsigned i = 0; i < its.size(); i += 1) {
					PlaneAtTime pat = planes[i];

					NormalAtTime nat;
					// If the previous intersection was parallel, and the next intersection is parallel, we are not fixed
					nat.fixed = !(its[i-1 % its.size()].parallel & its[i].parallel);
					nat.time = pat.time;

					if (nat.fixed) {
						nat.normal = (its[i].intersection - spline->point_at_time(pat.time)).normalize();
					} else {
						nat.normal.zero();
					}

					normals.push_back(nat);
				}

				// We need to check that there is atleast one non-parallel

				/*/ / Now we need to interpolate any non-fixed normals (ie, parallel)
				for (unsigned i = 0; i < normals.size(); i += 1) {
					unsigned k;

					//Find next non-fixed normal
					for (k = 1; k < normals.size(); k += 1) {
						if (normals[i+k % normals.size()].fixed) break;
					}

					if (k != 1) {
						// We have found 1 or more non-fixed normals
						Vector<D> start(normals[i].normal), end(normals[i+k % normals.size()].normal);

						for (unsigned j = 1; j < k; j += 1) {
							RealT f = ((RealT)j / (RealT)k);
							normals[i+j % normals.size()] = start * f + end * (1.0 - f);
						}
					}
				}

				*/

				// What spline should we use for the new spline???
				for(auto nml = normals.begin(); nml != normals.end(); nml++) {
					if (!(*nml).fixed) continue; // Skip temporary normals

					// This next line is important but doesn't compile right now
					// normal_spline->points().push_back(spline->point_at_time((*nml).time) + ((*nml).normal.normalize() * 100));
				}
			}
		};

		template <dimension D>
		void SplineWithNormal<D>::for_spline (const ISpline<D> * spline, const ISpline<D> * normal_spline) {
			_SplineWithNormalGenerator<D>::for_spline(spline, normal_spline);
		}

		template class SplineWithNormal<3>;

// MARK: -
// MARK: Spline

		template <dimension D>
		Spline<D>::Spline () : _closed(true) {
		}

		template <dimension D>
		Spline<D>::~Spline () {
		}

		template <dimension D>
		void Spline<D>::reset_segment_points_cache () {
			_segment_points.clear();
		}

		template <dimension D>
		bool Spline<D>::closed () const {
			return _closed;
		}

		template <dimension D>
		void Spline<D>::set_closed (bool closed) {
			if (_closed != closed) {
				_closed = closed;
				reset_segment_points_cache();
			}
		}

		template <dimension D>
		const std::vector<Vector<D>>& Spline<D>::segment_points() const {
			if (_segment_points.empty()) {
				_segment_points = generate_segment_points();
			}

			return _segment_points;
		}

		template <dimension D>
		std::vector<Vector<D>> Spline<D>::generate_segment_points() const {
			if (!_closed) {
				return _points;
			}

			// We need to add the first point last so that the line is closed.
			assert(_points.size() > 0);
			PointsT segment_points(_points);
			segment_points.push_back(_points[0]);

			return segment_points;
		}

// MARK: -
// MARK: LinearSpline

		template <dimension D>
		Vector<D> LinearSpline<D>::point_at_time(RealT t) const {
			std::size_t sp = this->starting_point(t);
			RealT fr = this->fractional_component(t);

			if (t >= 1.0) return this->_points[sp];
			else return linear_interpolate(fr, this->_points[sp], this->_points[sp+1]);
		}

// MARK: -
// MARK: CubicSpline

		template <dimension D>
		std::size_t CubicSpline<D>::segments () const {
			// Remove two segments which are used for initial cubic tangent generation
			// (one at each end)
			return Spline<D>::segments() - 2;
		}

		template <dimension D>
		std::vector<Vector<D>> CubicSpline<D>::generate_segment_points() const {
			if (!this->_closed) {
				return Spline<D>::generate_segment_points();
			}

			assert(this->_points.size() > 3);
			PointsT pts;

			pts.push_back(this->_points[this->_points.size() - 1]);
			pts.insert(pts.end(), this->_points.begin(), this->_points.end());
			pts.push_back(this->_points[0]);
			pts.push_back(this->_points[1]);

			return pts;
		}

		template <dimension D>
		Vector<D> CubicSpline<D>::point_at_time(RealT t) const {
			const PointsT &pts = this->segment_points();
			assert(pts.size() > 3 && "CublicSpline<D> requires at least 4 points");

			std::size_t sp = this->starting_point(t) + 1;
			RealT fr = this->fractional_component(t);

			if (t >= 1.0) return pts[sp];

			return cubic_interpolate(fr, pts[sp-1], pts[sp], pts[sp+1], pts[sp+2]);
		}

// MARK: -
// MARK: HermiteSpline

		template <dimension D>
		Vector<D> HermiteSpline<D>::point_at_time(RealT t) const {
			const PointsT &pts = this->segment_points();

			assert(pts.size() > 3 && "HermiteSpline<D> requires at least 4 points");

			std::size_t sp = this->starting_point(t) + 1;
			RealT fr = this->fractional_component(t);

			if (t >= 1.0) return pts[sp];

			Vector<D> t1 (this->tangent(sp));
			Vector<D> t2 (this->tangent(sp+1));

			return hermite_polynomial(fr, pts[sp], t1, pts[sp+1], t2);
		}

		/// Catmull-Rom tangent function
		template <dimension D>
		Vector<D> HermiteSpline<D>::catmull_rom_spline (const ISpline<D> *s, std::size_t n) {
			const PointsT &pts = s->segment_points();

			return ((pts[n+1 % pts.size()] - pts[n-1 % pts.size()]) * 0.5);
		}

		template <dimension D>
		Vector<D> HermiteSpline<D>::four_point_linear_mu (const ISpline<D> *s, std::size_t n) {
			const PointsT &pts = s->segment_points();

			if (n > 1) n += 1;

			std::size_t p1 = n, p2 = n-1;

			Vector<D> mu (pts[p1] - pts[p2]);
			mu.normalize() *= (pts[1] - pts[2]).length();

			return mu;
		}

		template <dimension D>
		Vector<D> HermiteSpline<D>::four_point_exponential_mu (const ISpline<D>* s, std::size_t n) {
			const PointsT &pts = s->segment_points();

			if (n > 1) n += 1;

			std::size_t p1 = n, p2 = n-1;

			Vector<D> mu (pts[p1] - pts[p2]);

			// This weight of 30 is arbitray
			mu.normalize() *= (pts[1] - pts[2]).length2() / 30.0;

			return mu;
		}

		/// Simple linear tangent function
		template <dimension D>
		Vector<D> HermiteSpline<D>::multi_point_linear_mu (const ISpline<D>* s, unsigned n) {
			const PointsT &pts = s->segment_points();

			unsigned p1 = (n + 1) % pts.size(), p2 = (n-1) % pts.size();

			Vector<D> sg(pts[p1] - pts[n]);
			Vector<D> mu(pts[p1] - pts[p2]);

			return mu.normalize(sg.length());
		}

		// MARK: -

		// Explicit template instantiation
		template class Spline<3>;
		template class Spline<2>;

		template class LinearSpline<3>;
		template class LinearSpline<2>;

		template class CubicSpline<3>;
		template class CubicSpline<2>;

		template class HermiteSpline<3>;
		template class HermiteSpline<2>;
	}
}
