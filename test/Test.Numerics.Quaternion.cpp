
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
				[](UnitTest::Examiner & examiner) {
					// Angle axis
					Quaternion<double> q(R90, {1.0, 0.0, 0.0});
					examiner << "Rotation axis is correct" << std::endl;
					examiner.check(q.axis().equivalent({1.0, 0.0, 0.0}));
					examiner << "Rotation angle is correct" << std::endl;
					examiner.check(equivalent(R90, q.angle()));

					Matrix<4, 4, double> a = rotate(R90, vector(1.0, 0.0, 0.0));
					Matrix<4, 4, double> b = q;
					
					examiner << "Rotation matrix from quaternion is correct" << std::endl;
					examiner.check(a.equivalent(b));
				}
			},

			{"Multiplication",
				[](UnitTest::Examiner & examiner) {
					Quat q = rotate(R90, vector(1.0, 0.0, 0.0));
					Mat44 m = rotate(R90, vector(1.0, 0.0, 0.0));

					Vec3 si(15.14, -12.5, 4.55);

					Vec3 r1, r2;

					r1 = q * si;
					r2 = m * si;

					examiner << "Represented rotation is same" << std::endl;
					examiner.check(r1.equivalent(r2));

					Quat a(R90, Vec3(1, 0, 0).normalize());
					Quat b(R90, Vec3(0, 1, 0).normalize());
					Quat c = a.rotation_to(b);

					examiner << "Quaternion rotation is correct" << std::endl;
					examiner.check(a.equivalent(Vec4(0.707107, 0, 0, 0.707107)));
					examiner << "Quaternion rotation is correct" << std::endl;
					examiner.check(b.equivalent(Vec4(0, 0.707107, 0, 0.707107)));
					examiner << "Quaternion rotation is correct" << std::endl;
					examiner.check(c.equivalent(Vec4(-0.5, 0.5, -0.5, 0.5)));

					examiner << "Quaternion conjugate is correct" << std::endl;
					examiner.check(a.conjugate().equivalent(Vec4(-0.707107, 0, 0, 0.707107)));

					examiner << "Rotations are equivalent" << std::endl;
					examiner.check((a * c).equivalent(b));
				}
			},

			{"Rotation Matrix",
				[](UnitTest::Examiner & examiner) {
					Quat identity(IDENTITY);
					Mat44 m1 = identity;

					examiner << "X axis is correct" << std::endl;
					examiner.check((m1 * Vec3(1, 0, 0)).equivalent(Vec3(1, 0, 0)));
					examiner << "Y axis is correct" << std::endl;
					examiner.check((m1 * Vec3(0, 1, 0)).equivalent(Vec3(0, 1, 0)));
					examiner << "Z axis is correct" << std::endl;
					examiner.check((m1 * Vec3(0, 0, 1)).equivalent(Vec3(0, 0, 1)));

					Quat a(R90, Vec3(1, 0, 0).normalize());
					Mat44 m2 = a;

					examiner << "X axis is correct" << std::endl;
					examiner.check((m2 * Vec3(1, 0, 0)).equivalent(Vec3(1, 0, 0)));
					examiner << "Y axis is correct" << std::endl;
					examiner.check((m2 * Vec3(0, 1, 0)).equivalent(Vec3(0, 0, 1)));
					examiner << "Z axis is correct" << std::endl;
					examiner.check((m2 * Vec3(0, 0, 1)).equivalent(Vec3(0, -1, 0)));
				}
			},
			
			{"Transforms",
				[](UnitTest::Examiner & examiner) {
					Quat q1 = rotate(R90, Vec3(1, 0, 0));

					examiner.check(q1.angle().equivalent(R90));
					examiner.check(q1.axis().equivalent({1, 0, 0}));

					Quat q2 = rotate<X>(R90);

					examiner.check(q1.equivalent(q2));

					Quat q3 = rotate(R90, Vec3(1, 0, 0)) << rotate(-R90, Vec3(1, 0, 0));

					examiner.check(q3.equivalent(Quat(IDENTITY)));
				}
			},
		};
	}
}
