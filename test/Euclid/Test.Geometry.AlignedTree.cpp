
#include <UnitTest/UnitTest.h>

#include <Euclid/Geometry/AlignedTree.h>
#include <Euclid/Numerics/Vector.IO.h>

namespace Euclid
{
	namespace Geometry
	{
		UnitTest::Suite AlignedTreeTestSuite {
			"Euclid::Geometry::AlignedTree",

			{"Construction",
				[](UnitTest::Examiner & examiner) {
					AlignedTree<Quadrants, AlignedBox2> tree(0, 20);
					
					examiner << "Tree has correct size.";
					examiner.check_equal(tree.top()->origin(), Vec2{0, 0});
					examiner.check_equal(tree.top()->size(), Vec2{20, 20});
					examiner.check_equal(tree.top()->level(), 0);
					
					for (unsigned i = 0; i < 20; i += 1)
						tree.insert(AlignedBox2{i, i+1});
					
					examiner << "The tree has been redistributed.";
					examiner.check_equal(tree.top()->objects().size(), 0);
					examiner.check_equal(tree.top()->child(Quadrants::BottomLeft)->objects().size(), 10);
					examiner.check_equal(tree.top()->child(Quadrants::TopRight)->objects().size(), 10);
					examiner.check_equal(tree.top()->child(Quadrants::BottomRight)->objects().size(), 0);
					examiner.check_equal(tree.top()->child(Quadrants::TopLeft)->objects().size(), 0);
				}
			},
		};
	}
}
