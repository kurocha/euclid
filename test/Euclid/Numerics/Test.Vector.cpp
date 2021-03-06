
#include <UnitTest/UnitTest.hpp>

#include <Euclid/Numerics/Vector.hpp>
#include <Euclid/Numerics/Vector.Distribution.hpp>
#include <Euclid/Numerics/Vector.Geometry.hpp>
#include <Euclid/Numerics/Vector.IO.hpp>

#include <set>
#include <tuple>

namespace Euclid
{
	namespace Numerics
	{
		UnitTest::Suite VectorTestSuite {
			"Euclid::Numerics::Vector",

			{"Construction",
				[](UnitTest::Examiner & examiner) {
					Vector<1, int> b1 = IDENTITY;
					Vector<2, int> b2 = {1, 2};
					Vector<3, int> b3 = {1, 2, 3};
					Vector<4, int> b4 = {1, 2, 3, 4};

					examiner << "Vector components constructed correctly" << std::endl;
					examiner.check(b1.sum() == 1);
					examiner << "Vector components constructed correctly" << std::endl;
					examiner.check(b2.sum() == 3);
					examiner << "Vector components constructed correctly" << std::endl;
					examiner.check(b3.sum() == 6);
					examiner << "Vector components constructed correctly" << std::endl;
					examiner.check(b4.sum() == 10);

					b1 = vector(5);
					b2 = vector(5, 9);
					b3 = vector(5, 9, 17);
					b4 = vector(5, 9, 17, 29);

					examiner << "Vector components constructed correctly" << std::endl;
					examiner.check(b1.sum() == 5);
					examiner << "Vector components constructed correctly" << std::endl;
					examiner.check(b2.sum() == 5+9);
					examiner << "Vector components constructed correctly" << std::endl;
					examiner.check(b3.sum() == 5+9+17);
					examiner << "Vector components constructed correctly" << std::endl;
					examiner.check(b4.sum() == 5+9+17+29);

					b2 = b4;

					examiner << "Vector components are equal" << std::endl;
					examiner.check(b2[0] == b4[0]);
					examiner << "Vector components are equal" << std::endl;
					examiner.check(b2[1] == b4[1]);

					Vector<4, float> f4;

					f4 = b4;

					examiner << "Vector components copied correctly" << std::endl;
					examiner.check(b4.sum() == f4.sum());

					f4 = ZERO;

					examiner << "Vector is zeroed" << std::endl;
					examiner.check(f4.sum() == 0.0f);

					f4 = IDENTITY;

					examiner << "Vector is identity" << std::endl;
					examiner.check(f4.sum() == 4.0f);

					f4 = 2.5;

					examiner << "Vector was assigned to correctly." << std::endl;
					examiner.check(f4.sum() == 10.0f);

					f4 = b4;

					examiner << "Vector components copied correctly" << std::endl;
					examiner.check(b4.sum() == f4.sum());

					examiner.check(f4.equivalent(b4));
				}
			},

			{"Data Construction",
				[](UnitTest::Examiner & examiner) {
					examiner << "Construct from fixed sized array" << std::endl;
					float fv[4] = {1.0, 2.0, 3.0, 4.0};
					Vec4 v1 = fv;

					examiner.check(v1.equivalent({1, 2, 3, 4}));

					examiner << "Construct from data pointer" << std::endl;
					
					unsigned char * data = (unsigned char *)"Hello World";
					Vector<5, unsigned char> v2 = (data + 6);

					examiner.check(v2.equivalent({'W', 'o', 'r', 'l', 'd'}));
				}
			},

			{"Conversion",
				[](UnitTest::Examiner & examiner) {
					Vec1 f = 10.0;

					examiner << "Converting 1-dimentional vector." << std::endl;
					RealT v = f;

					examiner.check_equal(v, 10.0);
				}
			},

			{"Cross Product",
				[](UnitTest::Examiner & examiner) {
					Vec3 a{1, 0, 0}, b{0, 1, 0};

					examiner.check(cross_product(a, b).equivalent({0, 0, 1}));
				}
			},

			{"Geometric Comparisons",
				[](UnitTest::Examiner & examiner) {
					examiner << "Vector comparison correct" << std::endl;
					examiner.check(vector(4, 5).less_than(vector(6, 6)));
					examiner << "Vector comparison correct" << std::endl;
					examiner.check(!vector(4, 5).less_than(vector(4, 4)));

					examiner << "Vector comparison correct" << std::endl;
					examiner.check(vector(4, 5).less_than_or_equal(vector(6, 6)));
					examiner << "Vector comparison correct" << std::endl;
					examiner.check(vector(4, 4).less_than_or_equal(vector(4, 4)));
					examiner << "Vector comparison correct" << std::endl;
					examiner.check(!vector(4, 5).less_than_or_equal(vector(4, 4)));

					examiner << "Vector comparison correct" << std::endl;
					examiner.check(!vector(4, 5).greater_than(vector(6, 6)));
					examiner << "Vector comparison correct" << std::endl;
					examiner.check(!vector(4, 5).greater_than(vector(4, 4)));
					examiner << "Vector comparison correct" << std::endl;
					examiner.check(vector(5, 5).greater_than(vector(4, 4)));

					examiner << "Vector comparison correct" << std::endl;
					examiner.check(!vector(4, 5).greater_than_or_equal(vector(6, 6)));
					examiner << "Vector comparison correct" << std::endl;
					examiner.check(vector(4, 5).greater_than_or_equal(vector(4, 4)));
					examiner << "Vector comparison correct" << std::endl;
					examiner.check(vector(4, 4).greater_than_or_equal(vector(4, 4)));
				}
			},

			{"Reflection",
				[](UnitTest::Examiner & examiner) {
					// 0:Wall Normal, 1:I, 2:R
					typedef std::tuple<Vec2, Vec2, Vec2> ReflectTestRecord;
					std::vector<ReflectTestRecord> reflection_tests;

					reflection_tests.push_back(std::make_tuple(Vec2{1.0, 0.0}, Vec2{-1.0, -1.0}, Vec2{1.0, -1.0}));
					reflection_tests.push_back(std::make_tuple(Vec2{1.0, 0.0}, Vec2{-1.0, 0.0}, Vec2{1.0, 0.0}));
					reflection_tests.push_back(std::make_tuple(Vec2{1.0, 1.0}.normalize(), Vec2{-1.0, -1.0}, Vec2{1.0, 1.0}));
					reflection_tests.push_back(std::make_tuple(Vec2{1.0, 1.0}.normalize(), Vec2{-1.0, 0.0}, Vec2{0.0, 1.0}));

					/*
					 (Vec2(1.0, 0.0), Vec2(-0.417897, 0.908494), Vec2(0.417897, 0.908494))
					 (Vec2(0.0, 1.0), Vec2(-0.593251, -0.805017), Vec2(-0.593251, 0.805017))
					 (Vec2(0.0, 1.0), Vec2(-1, 0), Vec2(-1, 0))
					 (Vec2(0.106533, 0.994309), Vec2(-0.593248, -0.80502), Vec2(-0.409235, 0.912429)
					 };
					 */

					unsigned k = 0;
					for (auto & record : reflection_tests) {
						Vec2 c = std::get<1>(record).reflect(std::get<0>(record));

						examiner.check(c.equivalent(std::get<2>(record)));

						k += 1;
					}
				}
			},

			{"Comparisons",
				[](UnitTest::Examiner & examiner) {
					Vec2 v1 = {1, 1}, v2 = {1, 2}, v3 = {2, 1}, v4 = {2, 2};
					
					examiner.expect(v1) == v1;
					examiner.expect(v1) != v2;

					// Lesser than
					examiner.expect(v1) < v2;
					examiner.expect(v4).to_not() < v1;
					examiner.expect(v1) <= v2;
					examiner.expect(v1) <= v1;
					examiner.expect(v2).to_not() <= v1;

					// Greater than
					examiner.expect(v2) > v1;
					examiner.expect(v3) > v2;
					examiner.expect(v2).to_not() >= v3;
					examiner.expect(v1) >= v1;
					examiner.expect(v3) >= v2;
				}
			},

			{"Arithmetic",
				[](UnitTest::Examiner & examiner) {
					Vec2 v1 = {1, 1}, v2 = {1, 2}, v3 = {2, 1}, v4 = {2, 2};

					// Basic Arithmetic
					examiner << "+ == +" << std::endl;
					examiner.check(v2+v3 == v1+v4);
					examiner << "/ * ==" << std::endl;
					examiner.check((v3/v2) * v2 == v3);
					examiner << "% ==" << std::endl;
					examiner.check(vector(2, 1) % 2 == vector(0, 1));

					// Advanced Arithemtic
					examiner << "sum" << std::endl;
					examiner.check(v2.sum() == 3);
					examiner << "product" << std::endl;
					examiner.check(v4.product() == 4);
					examiner << "floor" << std::endl;
					examiner.check(vector(2.5, 5.5).truncate() == vector(2.0, 5.0));
					examiner << "ceil" << std::endl;
					examiner.check(vector(2.5, 5.5).truncate(true) == vector(3.0, 6.0));
					examiner << "frac" << std::endl;
					examiner.check(vector(2.5, 5.25).fraction() == vector(0.5, 0.25));
				}
			},

			{"Sets",
				[](UnitTest::Examiner & examiner) {
					Vec2 v1 = {1, 1}, v2 = {1, 2}, v3 = {2, 1}, v4 = {2, 2};

					std::set<Vec2> set1, set2, set3;
					set1.insert(v1);
					set1.insert(v2);
					set1.insert(v3);

					set2.insert(v1);
					set2.insert(v2);

					set3.insert(v1);
					set3.insert(v4);

					examiner << "Set has correct number of items" << std::endl;
					examiner.check(set1.size() == 3);
					examiner << "Set has correct number of items" << std::endl;
					examiner.check(set2.size() == 2);
					examiner << "Set has correct number of items" << std::endl;
					examiner.check(set3.size() == 2);

					examiner << "Set functions work correctly" << std::endl;
					examiner.check(includes(set1.begin(), set1.end(), set2.begin(), set2.end()));
					examiner << "Set functions work correctly" << std::endl;
					examiner.check(!includes(set1.begin(), set1.end(), set3.begin(), set3.end()));
				}
			},

			{"Normalization",
				[](UnitTest::Examiner & examiner) {
					examiner.check_equal(vector(0.0, 0.0, 2.5).normalize(), vector(0.0, 0.0, 1.0));
					examiner.check_equal(vector(0.0, 0.0, 2.5).normalize(2), vector(0.0, 0.0, 2.0));
				}
			},

			{"Constraints",
				[](UnitTest::Examiner & examiner) {
					examiner << "Checking interpolated constraints";
					Vec3 x(0, 0, 0);
					auto y = x.constrain({10, 10, 10}, {0.5, 0.5, 0.5});
					examiner.check(y.equivalent({5, 5, 5}));

					examiner << "Checking min/max constraints";
					Vec3 a(10, -10, 0), b(0, 10, 10);
					auto c = a.constrain(b, false), d = a.constrain(b, true);
					examiner.check(c.equivalent({0, -10, 0}));
					examiner.check(d.equivalent({10, 10, 10}));
				}
			},

			{"Distribution",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Numerics;

					std::mt19937 generator(41234234);
					UniformVectorDistribution<3> distribution({0.2, 0.2, 0.2}, {0.6, 0.6, 0.6});

					examiner.check(distribution(generator) > Vec3{0.2, 0.2, 0.2});
					examiner.check(distribution(generator) < Vec3{0.8, 0.8, 0.8});
				}
			},

			{"Expansion",
				[](UnitTest::Examiner & examiner) {
					using namespace Euclid::Numerics;

					auto v1 = vector(1, 2, 3, 4, 5);
					auto v2 = vector(1).expand(2, 3, 4, 5);

					examiner.check_equal(v1, v2);
				}
			},
		};
	}
}
