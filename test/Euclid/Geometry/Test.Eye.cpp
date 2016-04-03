
#include <UnitTest/UnitTest.hpp>

#include <Euclid/Geometry/Eye.hpp>
#include <Euclid/Numerics/Vector.IO.hpp>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite EyeTestSuite {
			"Euclid::Geometry::Eye",

			{"Clip Box Projection",
				[](UnitTest::Examiner & examiner) {
					auto eye = eye_transformation<RealT>(IDENTITY, IDENTITY);
					
					auto object_space = eye.convert_from_normalized_space_to_object_space(Vec3(0, 0, 0));
					
					examiner << "Origin is not transformed." << std::endl;
					examiner.expect(object_space.origin) == Vec3{0, 0, 0};
					
					examiner << "Forward is along Z axis." << std::endl;
					examiner.expect(object_space.forward.point()) == Vec3{0, 0, 0};
					examiner.expect(object_space.forward.direction()) == Vec3{0, 0, 1};

					examiner << "Up is along -Y axis." << std::endl;
					examiner.expect(object_space.up) == Vec3{0, -1, 0};
				}
			},

			{"Orthographic Projection",
				[](UnitTest::Examiner & examiner) {
					auto box = AlignedBox3::from_center_and_size(ZERO, 10);

					auto eye = eye_transformation<RealT>(orthographic_projection_matrix(box), translate(Vec3(10, 10, 0)));

					auto object_space = eye.convert_from_normalized_space_to_object_space(Vec3(0, 0, 0));

					examiner << "Point was mapped to object space." << std::endl;
					examiner.expect(object_space.forward.point()) == Vec3{-10, -10, -5};

					examiner << "Looking along z axis." << std::endl;
					examiner.expect(object_space.forward.direction()) == Vec3{0, 0, 1};
				}
			},
			
			{"Perspective Projection",
				[](UnitTest::Examiner & examiner) {
					auto box = AlignedBox3::from_center_and_size(ZERO, 10);

					auto eye = eye_transformation<RealT>(orthographic_projection_matrix(box), translate(Vec3(10, 10, 0)));

					auto object_space = eye.convert_from_normalized_space_to_object_space(Vec3(0, 0, 0));

					examiner << "Point was mapped to object space." << std::endl;
					examiner.expect(object_space.forward.point()) == Vec3{-10, -10, -5};

					examiner << "Looking down negative-z axis." << std::endl;
					examiner.expect(object_space.forward.direction()) == Vec3{0, 0, 1};
				}
			},
		};
	}
}
