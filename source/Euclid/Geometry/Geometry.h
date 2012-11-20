//
//  Geometry/Geometry.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 11/11/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_GEOMETRY_H
#define _EUCLID_GEOMETRY_GEOMETRY_H

#include "../Euclid.h"
#include "../Numerics/Vector.h"

namespace Euclid {
	/**
	 Geometry groups a set of mathematical constructs which model geometrical phenomenon, such as lines, spheres and boxes. These classes try to be as = 0
	 as possible, therefore, most classes provide template elements for the number of dimensions, and the type of number to use.
	 */
	namespace Geometry {
		using namespace Numerics;

		/**
		 A general = 0ion of a shape in D-space with P points. Used as a base class for several shapes. Provides access to points and some general
		 functions.
		 */
		template <unsigned D, unsigned P, typename NumericT>
		class Shape {
		public:
			typedef Vector<D, NumericT> VectorT;

		protected:
			Vector<D, NumericT> _points[P];

		public:
			VectorT center () const
			{
				VectorT total(_points[0]);
				for (unsigned i = 1; i < P; i++)
					total += _points[i];

				return total / (NumericT)P;
			}

			const VectorT & operator[] (unsigned i) const
			{
				return _points[i];
			}

			VectorT & operator[] (unsigned i)
			{
				return _points[i];
			}
		};

		/**
		 An intersection test generally has three kinds of results which are distinct. There was no intersection, the edges touched, or the shapes overlapped.
		 */
		enum IntersectionResult {
			/// There is no geometric intersection.
			NO_INTERSECTION = 0,

			/// Edges of the shapes touch, but the shapes themselves do not overlap.
			EDGES_INTERSECT = 16,

			/// The shapes intersect.
			SHAPES_INTERSECT = 32,

			/// The shape being tested is completely embedded.
			SHAPE_EMBEDDED = 64
		};

		enum Direction {
			LEFT   = 1 << 0, // - x-axis
			    RIGHT  = 1 << 1, // + x-axis
			    BOTTOM = 1 << 2, // - y-axis
			    TOP    = 1 << 3, // + y-axis
			    NEAR   = 1 << 4, // - z-axis
			    FAR    = 1 << 5 // + z-axis
		};

		/*
		    Geometry is very inter-dependant, for example, a plane can be constructed from a triangle, but a triangle also relies on planes for intersection
		    tests. Therefore, we predefine all general geometry classes here.
		 */

		template <unsigned D, typename NumericT = RealT>
		class Triangle;

		template <unsigned D, typename NumericT = RealT>
		class Sphere;

		template <unsigned D, typename NumericT = RealT>
		class Plane;

		template <unsigned D, typename NumericT = RealT>
		class Line;

		template <unsigned D, typename NumericT = RealT>
		class LineSegment;

		template <unsigned D, typename NumericT = RealT>
		class AlignedBox;
	}
}

#endif
