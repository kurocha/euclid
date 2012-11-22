//
//  Numerics/Matrix.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 13/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "Matrix.h"

namespace Euclid
{
	namespace Numerics
	{
		Mat44 rotation (const RealT & radians, const Vec3 & around_normal, const Vec3 & around_point)
		{
			return Mat44::rotating_matrix(radians, around_normal, around_point);
		}

		Mat44 rotation (const RealT & radians, const Vec3 & around_normal)
		{
			return Mat44::rotating_matrix(radians, around_normal);
		}

		Mat44 rotation (const Vec3 & from_unit_vector, const Vec3 & to_unit_vector, const Vec3 & around_normal)
		{
			return Mat44::rotating_matrix(from_unit_vector, to_unit_vector, around_normal);
		}

// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Matrix)
		{
			testing("Identity Multiplication");

			{
				Mat44 a, I(IDENTITY);
				a.load_test_pattern();
				
				check(a == a * I) << "Matricies are equivalent after multiplication by identity";
				check(I * a == a) << "Matricies are equivalent after multiplication by identity";
			}

			{
				testing("Matrix Rotation");

				// These tests assume column basis, column major order (traditional mathematical notation):
				Mat44 xa = Mat44::rotating_matrix(R90, Vec3(1.0, 0.0, 0.0));
				Mat44 xb = Mat44::rotating_matrix_around_x(R90);

				check(xa.equivalent(xb));

				Mat44 xc(IDENTITY);

				xc.at(1, 1) = 0;
				xc.at(1, 2) = -1;
				xc.at(2, 1) = 1;
				xc.at(2, 2) = 0;

				check(xa.equivalent(xc)) << "Matrix rotation is correct";

				Mat44 ya = Mat44::rotating_matrix(R90, Vec3(0.0, 1.0, 0.0));
				Mat44 yb = Mat44::rotating_matrix_around_y(R90);

				check(ya.equivalent(yb));

				Mat44 yc(IDENTITY);

				yc.at(0, 0) = 0;
				yc.at(0, 2) = 1;
				yc.at(2, 0) = -1;
				yc.at(2, 2) = 0;

				check(ya.equivalent(yc)) << "Matrix rotation is correct";

				Mat44 za = Mat44::rotating_matrix(R90, Vec3(0.0, 0.0, 1.0));
				Mat44 zb = Mat44::rotating_matrix_around_z(R90);

				check(za.equivalent(zb));

				Mat44 zc(IDENTITY);

				zc.at(0, 0) = 0;
				zc.at(0, 1) = -1;
				zc.at(1, 0) = 1;
				zc.at(1, 1) = 0;

				check(za.equivalent(zc)) << "Matrix rotation is correct";

				auto matrix_product = xa * ya * za;

				RealT sample_data[16] = {0, 0, 1, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
				Mat44 sample(sample_data);

				check(matrix_product.equivalent(sample));

				// Check that the rotation between two vectors is correct:
				Mat44 rk = Mat44::rotating_matrix(Vec3(1.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0), Vec3(0.0, 0.0, 1.0));

				Vec3 pk = rk * Vec3(1.0, 0.0, 0.0);

				check(pk.equivalent(Vec3(0.0, 1.0, 0.0))) << "Rotation between vectors is correct";
			}

			{
				testing("Vector Multiplication");

				Mat44 b, c;

				Vec4 r, pt(10.0, 0.0, 0.0, 1.0);
				b = rotation(R90, vector(0.0, 1.0, 0.0));

				r = b * pt;

				check(r.equivalent(vector(0.0, 0.0, -10.0, 1.0))) << "Rotation was successful";

				b = rotation(R180, vector(0.0, 1.0, 0.0), vector(0.0, 10.0, 10.0));

				r = b * pt;

				check(r.equivalent(vector(-10.0, 0.0, 20.0, 1.0))) << "Rotation was successful";
			}

			{
				testing("Composite Transforms");

				Mat44 a(IDENTITY);
				a << Mat44::translating_matrix(Vec3(1.0, 2.0, 3.0));
				a << Mat44::rotating_matrix(R90, Vec3(1.0, 0.0, 0.0));
				
				Vec4 r, pt(0.0, 0.0, 0.0, 1.0);

				r = a * pt;

				check(r.equivalent(Vec4(1.0, -3.0, 2.0, 1.0))) << "Rotation was correct";
			}
		}

		UNIT_TEST(MatrixVector)
		{
			testing("Vector Assignment");

			Mat44 a(ZERO);
			Mat44 mat_ident(IDENTITY);
			Vec4 vec_ident(IDENTITY);

			a.set(0, 0, vec_ident, 5);

			check(a == mat_ident) << "Vector was copied correctly";

			Vec4 c1(0, 1, 2, 3), c2(4, 5, 6, 7), c3(8, 10, 12, 14), c4(9, 11, 13, 15);
			a.zero();

			a.set(0, 0, c1, 1);
			a.set(0, 1, c2, 1);
			a.set(0, 2, c3, 2);
			a.set(1, 2, c4, 2);

			Mat44 test_pattern;
			test_pattern.load_test_pattern();

			check(a == test_pattern) << "Vector was copied correctly";
		}
#endif
	}
}
