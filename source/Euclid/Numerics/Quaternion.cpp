//
//  Numerics/Quaternion.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 26/07/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#include "Quaternion.h"

namespace Euclid
{
	namespace Numerics
	{
		template <typename _NumericT>
		Quaternion<_NumericT> Quaternion<_NumericT>::spherical_linear_interpolate (_NumericT t, const Quaternion<_NumericT> & q0, const Quaternion<_NumericT> & q1)
		{
			// v0 and v1 should be unit length or else
			// something broken will happen.

			const Vec4T & v0 = q0.vector();
			const Vec4T & v1 = q1.vector();

			// Compute the cosine of the angle between the two vectors.
			NumericT dot = v0.dot(v1);

			const RealT DOT_THRESHOLD = 0.9995;
			if (dot > DOT_THRESHOLD) {
				// If the inputs are too close for comfort, linearly interpolate
				// and normalize the result.
				Vec4T result(v0 + (v1 - (Vec4)v0) * t);

				result.normalize();
				return Quaternion(result);
			}

			// Robustness: Stay within domain of acos():
			Number<NumericT>::clamp(dot);
			// theta_0 = angle between input vectors:
			RealT theta_0 = acos(dot);
			// theta = angle between v0 and result:
			RealT theta = theta_0 * t;
			Vec4T v2 = v1 - v0 * dot;

			v2.normalize();
			// { v0, v2 } is now an orthonormal basis.

			return Quaternion(v0*cos(theta) + v2*sin(theta));
		}

		template <typename _NumericT>
		Quaternion<_NumericT>::Quaternion (const Identity &)
		{
			_vector.zero();
			_vector[W] = 1;
		}

		template <typename _NumericT>
		Quaternion<_NumericT>::Quaternion (const Vec4T & other)
		{
			_vector.set(other);
		}

		template <typename _NumericT>
		Quaternion<_NumericT>::Quaternion (const _NumericT & angle, const Vec3T & axis)
		{
			set_to_angle_axis_rotation(angle, axis);
		}

		template <typename _NumericT>
		Quaternion<_NumericT>::Quaternion (const Vec3T & point)
		{
			_vector.set(point);
			_vector[W] = 0;
		}

		template <typename _NumericT>
		Quaternion<_NumericT>::Quaternion (const Vec3T & from, const Vec3T & to, _NumericT factor)
		{
			Vec3T axis = from.cross(to).normalize();
			RealT angle = from.angle_between(to) * factor;

			set_to_angle_axis_rotation(angle, axis);
		}

		// Rotation from A to B
		template <typename _NumericT>
		Quaternion<_NumericT>::Quaternion (const Vec3T & from, const Vec3T & to)
		{
			Vec3T axis = from.cross(to).normalize();
			RealT angle = from.angle_between(to);

			set_to_angle_axis_rotation(angle, axis);
		}

		template <typename _NumericT>
		Quaternion<_NumericT> Quaternion<_NumericT>::from_matrix (const Mat44 & m) {
			RealT w = Math::sqrt(1 + m[0] + m[5] + m[10]) / 2.0;

			Vec4 e;
			e[X] = (m[6] - m[9]) / (4 * w);
			e[Y] = (m[8] - m[2]) / (4 * w);
			e[Z] = (m[1] - m[4]) / (4 * w);
			e[W] = w;

			return Quaternion(e.normalized_vector(1));
		}

		template <typename _NumericT>
		Quaternion<_NumericT> Quaternion<_NumericT>::from_euler(Vec3T angles) {
			Quaternion x_rotation(angles[X], Vec3T(1.0, 0.0, 0.0));
			Quaternion y_rotation(angles[Y], Vec3T(0.0, 1.0, 0.0));
			Quaternion z_rotation(angles[Z], Vec3T(0.0, 0.0, 1.0));

			return x_rotation * y_rotation * z_rotation;
		}

		template <typename _NumericT>
		Quaternion<_NumericT> Quaternion<_NumericT>::from_1ijk(Vec4T r4) {
			Vec4T vector(r4[1], r4[2], r4[3], r4[0]);

			return Quaternion(vector);
		}

		template <typename _NumericT>
		const _NumericT & Quaternion<_NumericT>::operator[] (unsigned i) const
		{
			return _vector[i];
		}

		template <typename _NumericT>
		_NumericT & Quaternion<_NumericT>::operator[] (unsigned i)
		{
			return _vector[i];
		}

		template <typename _NumericT>
		typename Quaternion<_NumericT>::Vec3T Quaternion<_NumericT>::rotate (const Vec3T & v) const
		{
			Vec3T qvec = _vector.reduce();

			Vec3T uv = qvec.cross(v);
			Vec3T uuv = qvec.cross(uv);

			uv *= _vector[W] * 2;
			uuv *= 2;

			return v + uv + uuv;
		}

		template <typename _NumericT>
		inline typename Quaternion<_NumericT>::Vec3T Quaternion<_NumericT>::operator* (const Vec3T & other) const
		{
			return rotate(other);
		}

		template <typename _NumericT>
		Quaternion<_NumericT> Quaternion<_NumericT>::multiply_with (const Quaternion<_NumericT> & q2) const
		{
			const Quaternion & q1 = *this;

			Quaternion result;

			result[X] = (q1[W]*q2[X] + q1[X]*q2[W] + q1[Y]*q2[Z]) - q1[Z]*q2[Y];
			result[Y] = (q1[W]*q2[Y] + q1[Y]*q2[W] + q1[Z]*q2[X]) - q1[X]*q2[Z];
			result[Z] = (q1[W]*q2[Z] + q1[X]*q2[Y] + q1[Z]*q2[W]) - q1[Y]*q2[X];
			result[W] =  q1[W]*q2[W] - q1[X]*q2[X] - q1[Y]*q2[Y]  - q1[Z]*q2[Z];

			return result;
		}

		template <typename _NumericT>
		Quaternion<_NumericT> & Quaternion<_NumericT>::operator*= (const Quaternion & q2)
		{
			Quaternion r = multiply_with(q2);
			_vector = r._vector;

			return *this;
		}

		template <typename _NumericT>
		Quaternion<_NumericT> Quaternion<_NumericT>::operator* (const Quaternion & q2) const
		{
			return multiply_with(q2);
		}

		template <typename _NumericT>
		Quaternion<_NumericT> & Quaternion<_NumericT>::operator= (const Vec4T & other)
		{
			_vector.set(other);

			return *this;
		}

		template <typename _NumericT>
		typename Quaternion<_NumericT>::Vec3T Quaternion<_NumericT>::rotation_axis () const
		{
			return _vector.reduce().normalize();
		}

		template <typename _NumericT>
		typename Quaternion<_NumericT>::MatrixT Quaternion<_NumericT>::rotating_matrix () const
		{
			Matrix<4, 4, NumericT> matrix(IDENTITY);

			assert(Numerics::equal_within_tolerance(_vector.length2(), (NumericT)1.0)
			             && "Quaternion.rotating_matrix magnitude must be 1");

			NumericT x = _vector[X], y = _vector[Y], z = _vector[Z], w = _vector[W];

			matrix.at(0, 0) = 1 - 2 * (y*y + z*z);
			matrix.at(0, 1) =     2 * (x*y - w*z);
			matrix.at(0, 2) =     2 * (x*z + w*y);

			matrix.at(1, 0) =     2 * (x*y + w*z);
			matrix.at(1, 1) = 1 - 2 * (x*x + z*z);
			matrix.at(1, 2) =     2 * (y*z - w*x);

			matrix.at(2, 0) =     2 * (x*z - w*y);
			matrix.at(2, 1) =     2 * (y*z + w*x);
			matrix.at(2, 2) = 1 - 2 * (x*x + y*y);

			return matrix;
		}

		template <typename _NumericT>
		typename Quaternion<_NumericT>::Vec3T Quaternion<_NumericT>::extract_axis (std::size_t a) const
		{
			Vec3T result;

			if (a == X) {
				result = Vec3(1, 0, 0);
			} else if (a == Y) {
				result = Vec3(0, 1, 0);
			} else if (a == Z) {
				result = Vec3(0, 0, 1);
			}

			return rotating_matrix() * result;
		}

		template <typename _NumericT>
		void Quaternion<_NumericT>::set_to_angle_axis_rotation (const RealT & angle, const Vec3T & axis)
		{
			_vector.set(axis * Number<NumericT>::sin(angle / 2.0));
			_vector[W] = Math::cos(angle / 2.0);
		}

		template <typename _NumericT>
		_NumericT Quaternion<_NumericT>::rotation_angle () const
		{
			return Number<NumericT>::acos(_vector[W]) * 2;
		}

		template <typename _NumericT>
		void Quaternion<_NumericT>::conjugate ()
		{
			_vector[X] *= -1;
			_vector[Y] *= -1;
			_vector[Z] *= -1;
		}

		template <typename _NumericT>
		Quaternion<_NumericT> Quaternion<_NumericT>::conjugated_quaternion () const
		{
			Quaternion result(*this);
			result.conjugate();
			return result;
		}

		template <typename _NumericT>
		Quaternion<_NumericT> Quaternion<_NumericT>::rotation_to (const Quaternion & other) const
		{
			return this->conjugated_quaternion() * other;
		}

		template <typename _NumericT>
		Vector<4, _NumericT> & Quaternion<_NumericT>::vector ()
		{
			return _vector;
		}

		template <typename _NumericT>
		const Vector<4, _NumericT> & Quaternion<_NumericT>::vector () const
		{
			return _vector;
		}

		template class Quaternion<float>;
		template class Quaternion<double>;

// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Quaternion)
		{
			testing("Construction");

			// Angle axis
			Quat q(R90, vec(1.0, 0.0, 0.0));
			check(q.rotation_axis().equivalent(Vec3(1.0, 0.0, 0.0))) << "Rotation axis is correct";
			check(equivalent((RealT)R90, (RealT)q.rotation_angle())) << "Rotation angle is correct";

			Mat44 m = Mat44::rotating_matrix(R90, vec(1.0, 0.0, 0.0));
			check(q.rotating_matrix().equivalent(m)) << "Rotation matrix from quaternion is correct";

			testing("Multiplication");

			Vec3 si(15.14, -12.5, 4.55);

			Vec3 r1, r2;

			r1 = q * si;
			r2 = m * si;

			check(r1.equivalent(r2)) << "Represented rotation is same";

			Quat a(R90, Vec3(1, 0, 0).normalized_vector());
			Quat b(R90, Vec3(0, 1, 0).normalized_vector());
			Quat c = a.rotation_to(b);

			check(a.vector().equivalent(Vec4(0.707107, 0, 0, 0.707107))) << "Quaternion rotation is correct";
			check(b.vector().equivalent(Vec4(0, 0.707107, 0, 0.707107))) << "Quaternion rotation is correct";
			check(c.vector().equivalent(Vec4(-0.5, 0.5, -0.5, 0.5))) << "Quaternion rotation is correct";

			check(a.conjugated_quaternion().vector().equivalent(Vec4(-0.707107, 0, 0, 0.707107))) << "Quaternion conjugate is correct";

			check((a * c).vector().equivalent(b.vector())) << "Rotations are equivalent";

			testing("Axis Extraction");

			Quat identity(IDENTITY);

			check(identity.extract_axis(X).equivalent(Vec3(1, 0, 0))) << "X axis is correct";
			check(identity.extract_axis(Y).equivalent(Vec3(0, 1, 0))) << "Y axis is correct";
			check(identity.extract_axis(Z).equivalent(Vec3(0, 0, 1))) << "Z axis is correct";

			check(a.extract_axis(X).equivalent(Vec3(1, 0, 0))) << "X axis is correct";
			check(a.extract_axis(Y).equivalent(Vec3(0, 0, 1))) << "Y axis is correct";
			check(a.extract_axis(Z).equivalent(Vec3(0, -1, 0))) << "Z axis is correct";

			testing("Rotation Matrix");

			Quat d(R360 * 0.34, Vec3(0.52, 0.1, -0.9).normalized_vector());
			Vec3 va(1, 0, 0), vb(0, 1, 0), vc(0, 0, 1);

			Mat44 t = d.rotating_matrix();

			check(d.extract_axis(X).equivalent(t * va)) << "X axis is correct";
			check(d.extract_axis(Y).equivalent(t * vb)) << "Y axis is correct";
			check(d.extract_axis(Z).equivalent(t * vc)) << "Z axis is correct";

			// Transformation by matrix and transformation by multiplication is not correct
			// check((d * va).equivalent(t * va)) << "X rotation is correct";
			// check((d * vb).equivalent(t * vb)) << "Y rotation is correct";
			// check((d * vc).equivalent(t * vc)) << "Z rotation is correct";
		}
#endif
	}
}
