
#include <UnitTest/UnitTest.h>

#include <Euclid/Geometry/Mesh.h>
#include <Euclid/Geometry/Generate/Cube.h>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite MeshTestSuite {
			"Euclid::Geometry::Mesh",
			
			{"Construction",
				[](UnitTest::Examiner * examiner) {
					Mesh<> mesh;
					Generate::simple_cube(mesh, AlignedBox3::from_center_and_size(0, 10));

					/// There is one degenerate face (6+1):
					examiner->check_equal(mesh.vertices.size(), 4 * (6+1));
				}
			},
		};
	}
}
