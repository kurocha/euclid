
#include <UnitTest/UnitTest.hpp>

#include <Euclid/Geometry/Eye.hpp>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite EyeTestSuite {
			"Euclid::Geometry::Eye",

			{"Construction",
				[](UnitTest::Examiner & examiner) {
					auto eye = eye_transformation<RealT>(orthographic_projection_matrix(AlignedBox3::from_center_and_size(ZERO, 10)), translate(Vec3(10, 10, 0)));

					auto object_space = eye.convert_from_normalized_space_to_object_space(Vec3(0, 0, -1));

					examiner << "Point was mapped to object space." << std::endl;
					examiner.check(object_space.forward.point().equivalent({-10, -10, 5}));

					examiner << "Looking down negative-z axis." << std::endl;
					examiner.check(object_space.forward.direction().equivalent({0, 0, -1}));

					// Up axis might not be computed correctly, need to do some more unit tests.
				}
			},
		};
	}
}
