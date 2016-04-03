
#include <UnitTest/UnitTest.hpp>

#include <Euclid/Geometry/AlignedBox.hpp>

#include <Euclid/Numerics/Matrix.hpp>
#include <Euclid/Numerics/Matrix.IO.hpp>
#include <Euclid/Numerics/Matrix.Projections.hpp>

#include <Euclid/Numerics/Vector.IO.hpp>

namespace Euclid
{
	namespace Numerics
	{
		UnitTest::Suite MatrixProjectionsTestSuite {
			"Euclid::Numerics::Matrix",

			{"Clip Space Orthographic Projection",
				[](UnitTest::Examiner & examiner) {
					// This is the natural clip space box:
					auto unit_box = Geometry::AlignedBox3::from_origin_and_size(Vec3{-1, -1, 0}, Vec3{2, 2, 1});
					
					auto projection = orthographic_projection_matrix(unit_box);
					
					examiner << "The unit projection should be the identity" << std::endl;
					examiner.expect(projection) == Mat44(IDENTITY);
				}
			},
			
			{"Unit Space Orthographic Projection",
				[](UnitTest::Examiner & examiner) {
					// This is the natural clip space box:
					auto unit_box = Geometry::AlignedBox3::from_center_and_size(ZERO, Vec3{2, 2, 2});
					
					auto projection = orthographic_projection_matrix(unit_box);
					
					examiner.expect(projection * unit_box.corner({false, false, false})) == Vec3{-1, -1, 0};
					examiner.expect(projection * unit_box.corner({true, true, true})) == Vec3{1, 1, 1};
				}
			},
		};
	}
}
