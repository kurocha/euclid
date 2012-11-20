//
//  Numerics/Quaternion.h
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 19/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#ifndef _EUCLID_NUMERICS_QUATERNION_H
#define _EUCLID_NUMERICS_QUATERNION_H

#include "Numerics.h"
#include "Vector.h"
#include "Matrix.h"

namespace Euclid
{
	namespace Numerics
	{
		/** Quaternions are used for calculating rotations.

		 Two variations are available Quaternion<float> and Quaternion<double>.

		 Quaternions are a specialization of a 4-component Vector. It provides some additional operators for convenience.
		 */
		template <typename _NumericT = RealT>
		class Quaternion {
		protected:
			Vector<4, _NumericT> _vector;
		public:
			typedef _NumericT NumericT;
			typedef Vector<4, _NumericT> Vec4T;
			typedef Vector<3, _NumericT> Vec3T;
			typedef Matrix<4, 4, _NumericT> MatrixT;

			/// Undefined constructor.
			Quaternion () {
			}

			/// Identity constructor.
			Quaternion (const Identity &);

			/// Construct from 4-component Vector.
			Quaternion (const Vec4T & other);

			/// Assignment constructor.
			Quaternion & operator= (const Vec4T & other);

			/// Equality
			bool operator== (const Quaternion & other) {
				return vector() == other.vector();
			}

			/// Angle Axis constructor.
			Quaternion (const NumericT & angle, const Vec3T & axis);

			/// Rotation from one vector to another.
			Quaternion (const Vec3T & from, const Vec3T & to);
			/// Rotation from one vector to another. Multiply the rotation angle by factor.
			Quaternion (const Vec3T & from, const Vec3T & to, NumericT factor);

			/// Point constructor.
			Quaternion (const Vec3T & point);

			/// Get the rotation from a matrix
			static Quaternion from_matrix (const Mat44 & m);

			/// Calculate the rotation from Euler angles
			static Quaternion from_euler(Vec3T angles);

			static Quaternion from_1ijk(Vec4T);

			/// Proxy function for convenience
			const NumericT & operator[] (unsigned i) const;
			/// Proxy function for convenience
			NumericT & operator[] (unsigned i);

			/// Apply the rotation to a vector.
			Vec3T rotate (const Vec3T &) const;

			/// Multiplication function
			Quaternion multiply_with (const Quaternion & other) const;

			/// Convenience operator for rotate function.
			Vec3T operator* (const Vec3T & other) const;

			/// Multiply a quaternion in-place.
			Quaternion & operator*= (const Quaternion & q1);

			/// Multiply a quaternion rotation by another - essentially add the two rotations together.
			Quaternion operator* (const Quaternion & q2) const;

			/// Build a quaternion from an angle-axis rotation.
			void set_to_angle_axis_rotation (const RealT & angle, const Vec3T & axis);

			/// Calculate the angle of rotation.
			NumericT rotation_angle () const;

			/// Calculate the axis of rotation.
			Vec3T rotation_axis () const;

			/// Return a matrix that represents the rotation.
			MatrixT rotating_matrix () const;

			/// Return a quaternion that rotates from this rotation to another.
			Quaternion rotation_to (const Quaternion & other) const;

			/// Conjugate the current quaternion in place.
			void conjugate ();

			/// Return the conjugated quaternion.
			Quaternion conjugated_quaternion () const;

			/// Extract axis information
			Vec3T extract_axis (std::size_t a) const;

			Vector<4, NumericT> & vector ();
			const Vector<4, NumericT> & vector () const;

			static Quaternion spherical_linear_interpolate (_NumericT t, const Quaternion & v0, const Quaternion & v1);
		};

		typedef Quaternion<RealT> Quat;
	}
}

#endif
