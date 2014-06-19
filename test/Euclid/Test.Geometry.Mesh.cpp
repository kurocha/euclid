
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
				[](UnitTest::Examiner & examiner) {
					Mesh<> mesh;
					Generate::simple_cube(mesh, AlignedBox3::from_center_and_size(0, 10));

					/// There is one degenerate face (6+1):
					examiner.check_equal(mesh.vertices.size(), 4 * (6+1));
				}
			},

			{"Size and Alignment",
				[](UnitTest::Examiner & examiner) {
					examiner << "Vector size doens't include padding." << std::endl;
					examiner.check_equal(sizeof(Vec1), sizeof(RealT) * 1);
					examiner.check_equal(sizeof(Vec2), sizeof(RealT) * 2);
					examiner.check_equal(sizeof(Vec3), sizeof(RealT) * 3);
					examiner.check_equal(sizeof(Vec4), sizeof(RealT) * 4);

					examiner << "The default vertex doesn't include padding." << std::endl;
					examiner.check_equal(sizeof(RealT) * 8, sizeof(VertexP3N3M2));
				}
			},
		};
	}
}
