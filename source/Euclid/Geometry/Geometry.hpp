//
//  Geometry/Geometry.h
//  This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 11/11/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_GEOMETRY_GEOMETRY_H
#define _EUCLID_GEOMETRY_GEOMETRY_H

#include "../Euclid.hpp"
#include "../Numerics/Vector.hpp"
#include "../Numerics/Vector.Geometry.hpp"

namespace Euclid {
	/// Basic geometric constructs.
	namespace Geometry {
		using namespace Numerics;

		/// A general abstraction of a shape in D-space with P points. Used as a base class for several shapes. Provides access to points and some general functions.
		template <dimension D, dimension P, typename NumericT>
		struct Shape {
		protected:
			Vector<D, NumericT> _points[P];

		public:
			typedef Vector<D, NumericT> VectorT;

			Shape () = default;

			template <typename... ArgumentsT>
			Shape (ArgumentsT... arguments) : _points{(VectorT)arguments...}
			{
				static_assert(sizeof...(ArgumentsT) == P, "Incorrect number of points!");
			}

			VectorT center () const
			{
				VectorT total(_points[0]);
				for (dimension i = 1; i < P; i++)
					total += _points[i];

				return total / (NumericT)P;
			}

			VectorT & operator[] (std::size_t i) { return _points[i]; }
			const VectorT & operator[] (std::size_t i) const { return _points[i]; }
		};

		/// Surface normal for any 2-dimensional shape:
		template <dimension P, typename NumericT>
		Vector<3, NumericT> surface_normal (const Shape<2, P, NumericT> & shape)
		{
			return {0, 0, 1};
		}

		/// Surface normal for 3-dimensional triangles:
		template <typename NumericT>
		Vector<3, NumericT> surface_normal (const Shape<3, 3, NumericT> & shape)
		{
			return surface_normal(shape[0], shape[1], shape[2]);
		}

		namespace Constants
		{
			/// An intersection test generally has three kinds of results which are distinct. There was no intersection, the edges touched, or the shapes overlapped.
			enum Intersection
			{
				/// There is no geometric intersection.
				DISJOINT = 0,

				/// Edges of the shapes touch, but the shapes themselves do not overlap.
				TOUCH = 16,

				/// The shapes intersect.
				OVERLAP = 32,

				/// The shape being tested is completely embedded.
				ENCLOSED = 64
			};

			enum Direction
			{
				LEFT   = 1 << 0, // - x-axis
				RIGHT  = 1 << 1, // + x-axis
				BOTTOM = 1 << 2, // - y-axis
				TOP    = 1 << 3, // + y-axis
				NEAR   = 1 << 4, // - z-axis
				FAR    = 1 << 5 // + z-axis
			};
		}

		using namespace Constants;
		
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
