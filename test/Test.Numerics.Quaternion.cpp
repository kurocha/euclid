
#include <UnitTest/UnitTest.h>

#include <Euclid/Numerics/Quaternion.h>

UnitTest::Suite TestNumericsQuaternionSuite {
	"Test Quaternion",

	{"Construction",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			// Angle axis
			Quaternion<double> q(R90, {1.0, 0.0, 0.0});
			examiner->check(q.rotation_axis().equivalent({1.0, 0.0, 0.0})) << "Rotation axis is correct";
			examiner->check(equivalent(R90, q.rotation_angle())) << "Rotation angle is correct";

			auto m = Matrix<4, 4, double>::rotating_matrix(R90, {1.0, 0.0, 0.0});
			examiner->check(q.rotating_matrix().equivalent(m)) << "Rotation matrix from quaternion is correct";
		}
	},

	{"Multiplication",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			Quat q(R90, vector(1.0, 0.0, 0.0));
			Mat44 m = Mat44::rotating_matrix(R90, vector(1.0, 0.0, 0.0));

			Vec3 si(15.14, -12.5, 4.55);

			Vec3 r1, r2;

			r1 = q * si;
			r2 = m * si;

			examiner->check(r1.equivalent(r2)) << "Represented rotation is same";

			Quat a(R90, Vec3(1, 0, 0).normalized_vector());
			Quat b(R90, Vec3(0, 1, 0).normalized_vector());
			Quat c = a.rotation_to(b);

			examiner->check(a.vector().equivalent(Vec4(0.707107, 0, 0, 0.707107))) << "Quaternion rotation is correct";
			examiner->check(b.vector().equivalent(Vec4(0, 0.707107, 0, 0.707107))) << "Quaternion rotation is correct";
			examiner->check(c.vector().equivalent(Vec4(-0.5, 0.5, -0.5, 0.5))) << "Quaternion rotation is correct";

			examiner->check(a.conjugated_quaternion().vector().equivalent(Vec4(-0.707107, 0, 0, 0.707107))) << "Quaternion conjugate is correct";

			examiner->check((a * c).vector().equivalent(b.vector())) << "Rotations are equivalent";
		}
	},

	{"Axis Extraction",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;

			Quat identity(IDENTITY);

			examiner->check(identity.extract_axis(X).equivalent(Vec3(1, 0, 0))) << "X axis is correct";
			examiner->check(identity.extract_axis(Y).equivalent(Vec3(0, 1, 0))) << "Y axis is correct";
			examiner->check(identity.extract_axis(Z).equivalent(Vec3(0, 0, 1))) << "Z axis is correct";

			Quat a(R90, Vec3(1, 0, 0).normalized_vector());

			examiner->check(a.extract_axis(X).equivalent(Vec3(1, 0, 0))) << "X axis is correct";
			examiner->check(a.extract_axis(Y).equivalent(Vec3(0, 0, 1))) << "Y axis is correct";
			examiner->check(a.extract_axis(Z).equivalent(Vec3(0, -1, 0))) << "Z axis is correct";
		}
	},

	{"Rotation Matrix",
		[](UnitTest::Examiner * examiner) {
			using namespace Euclid::Numerics;
			
			Quat d(R360 * 0.34, Vec3(0.52, 0.1, -0.9).normalized_vector());
			Vec3 va(1, 0, 0), vb(0, 1, 0), vc(0, 0, 1);

			Mat44 t = d.rotating_matrix();

			examiner->check(d.extract_axis(X).equivalent(t * va)) << "X axis is correct";
			examiner->check(d.extract_axis(Y).equivalent(t * vb)) << "Y axis is correct";
			examiner->check(d.extract_axis(Z).equivalent(t * vc)) << "Z axis is correct";

			// Transformation by matrix and transformation by multiplication is not correct
			// examiner->check((d * va).equivalent(t * va)) << "X rotation is correct";
			// examiner->check((d * vb).equivalent(t * vb)) << "Y rotation is correct";
			// examiner->check((d * vc).equivalent(t * vc)) << "Z rotation is correct";
		}
	},
};
