//
//  Geometry/Sphere.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 18/10/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_SPHERE_H
#define _EUCLID_GEOMETRY_SPHERE_H

#include "Geometry.h"

namespace Euclid {
	namespace Geometry {
		/**
		 A sphere is defined by its center point and its radius.

		 It is fast for using with collision detection, as the comparison between a sphere and other geometric objects is typically very fast due to the
		 simple nature of a sphere.
		 */
		template <unsigned D, typename NumericT>
		class Sphere {
		protected:
			Vector<D, NumericT> _center;
			NumericT _radius;

			typedef typename RealType<NumericT>::RealT NumericRealT;

		public:
			/// Undefined constructor
			Sphere ();
			/// Zero constructor. Creates a sphere centered at coordinate zero, with radius zero.
			Sphere (const Zero &);
			/// Identity constructor. Creates a sphere centered at coordinate zero, with radius n.
			Sphere (const Identity &, const NumericT & n = 1);

			/// Full constructor. Supply all parameters.
			Sphere(Vector<D, NumericT> center, NumericT radius);

			/// @returns the center of the sphere.
			const Vector<D, NumericT> & center () const;
			/// Sets the center of the sphere.
			void set_center (const Vector<D> & center);

			/// @returns the radius of the sphere.
			const NumericT & radius () const;
			/// Sets the center of the sphere.
			void set_radius (const NumericT & radius);

			/// Displacement returns the vector between the two circles.
			NumericRealT distance_between_edges (const Sphere<D, NumericT> & other, Vector<D, NumericT> & displacement) const;

			/// Displacement returns the vector between the two centers.
			NumericRealT distance_from_point (const Vector<D, NumericT> & point, Vector<D, NumericT> & displacement) const;

			/// Distance is the time along the Line (use point_at_time).
			IntersectionResult intersects_with (const Line<D, NumericT> &line, RealT & entry_time, RealT & exit_time) const;

			/// Distance is the time along the LineSegment (use point_at_time).
			IntersectionResult intersects_with (const LineSegment<D, NumericT> &line, RealT & entry_time, RealT & exit_time) const;

			/// Test between two spheres
			IntersectionResult intersects_with (const Sphere & other, Vector<D, NumericT> & displacement) const;

			/// Test between a point and a sphere
			IntersectionResult intersects_with (const Vector<D, NumericT> & point, Vector<D, NumericT> & displacement) const;
		};

		typedef Sphere<2> Sphere2;
		typedef Sphere<3> Sphere3;
	}
}

#include "Sphere.impl.h"

#endif
