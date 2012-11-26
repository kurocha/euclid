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
		 A general abstraction of a shape in D-space with P points. Used as a base class for several shapes. Provides access to points and some general
		 functions.
		 */
		template <dimension D, dimension P, typename NumericT>
		class Shape {
		public:
			typedef Vector<D, NumericT> VectorT;

		protected:
			Vector<D, NumericT> _points[P];

		public:
			template <typename... ArgumentsT>
			Shape (ArgumentsT... arguments) : _points{(VectorT)arguments...} {
				static_assert(sizeof...(ArgumentsT) == P, "Incorrect number of points!");
			}

			VectorT center () const
			{
				VectorT total(_points[0]);
				for (dimension i = 1; i < P; i++)
					total += _points[i];

				return total / (NumericT)P;
			}

			const VectorT & operator[] (dimension i) const
			{
				return _points[i];
			}

			VectorT & operator[] (dimension i)
			{
				return _points[i];
			}
		};

		/// Surface normal for any 2-dimensional shape:
		template <dimension P, typename NumericT>
		Vector<3, NumericT> surface_normal (const Shape<2, P, NumericT> & shape) {
			return {0, 0, 1};
		}

		/// Surface normal for 3-dimensional triangles:
		template <typename NumericT>
		Vector<3, NumericT> surface_normal (const Shape<3, 3, NumericT> & shape) {
			return surface_normal(shape[0], shape[1], shape[2]);
		}

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
		    Geometry is very inter-dependant, for example, a plane can be constructed from a triangle, but a triangle also relies on planes for intersection tests. Therefore, we predefine all general geometry classes here.
		 */

		template <dimension D, typename NumericT = RealT>
		class Triangle;

		template <dimension D, typename NumericT = RealT>
		class Sphere;

		template <dimension D, typename NumericT = RealT>
		class Plane;

		template <dimension D, typename NumericT = RealT>
		class Line;

		template <dimension D, typename NumericT = RealT>
		class LineSegment;

		template <dimension D, typename NumericT = RealT>
		class AlignedBox;
	}
}

#endif
