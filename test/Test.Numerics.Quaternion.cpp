
#include <UnitTest/UnitTest.h>

#include <Euclid/Numerics/Quaternion.h>
#include <Euclid/Numerics/Transforms.h>
#include <Euclid/Numerics/Quaternion.Multiply.h>
#include <Euclid/Numerics/Matrix.Multiply.h>

namespace Euclid
{
	namespace Numerics
	{
		UnitTest::Suite QuaternionTestSuite {
			"Euclid::Numerics::Quaternion",

			{"Construction",
				[](UnitTest::Examiner * examiner) {
					// Angle axis
					Quaternion<double> q(R90, {1.0, 0.0, 0.0});
					examiner->check(q.rotation_axis().equivalent({1.0, 0.0, 0.0})) << "Rotation axis is correct";
					examiner->check(equivalent(R90, q.rotation_angle())) << "Rotation angle is correct";

					Matrix<4, 4, double> m = rotate(R90, vector(1.0, 0.0, 0.0));
					examiner->check(q.rotation_matrix<4>().equivalent(m)) << "Rotation matrix from quaternion is correct";
				}
			},

			{"Multiplication",
				[](UnitTest::Examiner * examiner) {
					Quat q(R90, vector(1.0, 0.0, 0.0));
					Mat44 m = rotate(R90, vector(1.0, 0.0, 0.0));

					Vec3 si(15.14, -12.5, 4.55);

					Vec3 r1, r2;

					r1 = q * si;
					r2 = m * si;

					examiner->check(r1.equivalent(r2)) << "Represented rotation is same";

					Quat a(R90, Vec3(1, 0, 0).normalize());
					Quat b(R90, Vec3(0, 1, 0).normalize());
					Quat c = a.rotation_to(b);

					examiner->check(a.equivalent(Vec4(0.707107, 0, 0, 0.707107))) << "Quaternion rotation is correct";
					examiner->check(b.equivalent(Vec4(0, 0.707107, 0, 0.707107))) << "Quaternion rotation is correct";
					examiner->check(c.equivalent(Vec4(-0.5, 0.5, -0.5, 0.5))) << "Quaternion rotation is correct";

					examiner->check(a.conjugate().equivalent(Vec4(-0.707107, 0, 0, 0.707107))) << "Quaternion conjugate is correct";

					examiner->check((a * c).equivalent(b)) << "Rotations are equivalent";
				}
			},

			{"Rotation Matrix",
				[](UnitTest::Examiner * examiner) {
					Quat identity(IDENTITY);
					auto m1 = identity.rotation_matrix<4>();

					examiner->check((m1 * Vec3(1, 0, 0)).equivalent(Vec3(1, 0, 0))) << "X axis is correct";
					examiner->check((m1 * Vec3(0, 1, 0)).equivalent(Vec3(0, 1, 0))) << "Y axis is correct";
					examiner->check((m1 * Vec3(0, 0, 1)).equivalent(Vec3(0, 0, 1))) << "Z axis is correct";

					Quat a(R90, Vec3(1, 0, 0).normalize());
					auto m2 = a.rotation_matrix<4>();

					examiner->check((m2 * Vec3(1, 0, 0)).equivalent(Vec3(1, 0, 0))) << "X axis is correct";
					examiner->check((m2 * Vec3(0, 1, 0)).equivalent(Vec3(0, 0, 1))) << "Y axis is correct";
					examiner->check((m2 * Vec3(0, 0, 1)).equivalent(Vec3(0, -1, 0))) << "Z axis is correct";
				}
			},
			
			{"Transforms",
				[](UnitTest::Examiner * examiner) {
					Quat q1 = rotate(R90, Vec3(1, 0, 0));

					examiner->check(q1.rotation_angle().equivalent(R90));
					examiner->check(q1.rotation_axis().equivalent({1, 0, 0}));

					Quat q2 = rotate<X>(R90);

					examiner->check(q1.equivalent(q2));
				}
			},
		};
	}
}
