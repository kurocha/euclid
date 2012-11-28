
#include <UnitTest/UnitTest.h>

#include <Euclid/Numerics/Vector.h>
#include <Euclid/Numerics/Vector.Distribution.h>
#include <Euclid/Numerics/Vector.Geometry.h>
#include <Euclid/Numerics/Vector.Ratios.h>
#include <Euclid/Numerics/Vector.IO.h>

#include <set>
#include <tuple>

namespace Euclid
{
	namespace Numerics
	{
		UnitTest::Suite VectorTestSuite {
			"Euclid::Numerics::Vector",

			{"Construction",
				[](UnitTest::Examiner * examiner) {
					Vector<1, int> b1 = IDENTITY;
					Vector<2, int> b2 = {1, 2};
					Vector<3, int> b3 = {1, 2, 3};
					Vector<4, int> b4 = {1, 2, 3, 4};

					examiner->check(b1.sum() == 1) << "Vector components constructed correctly";
					examiner->check(b2.sum() == 3) << "Vector components constructed correctly";
					examiner->check(b3.sum() == 6) << "Vector components constructed correctly";
					examiner->check(b4.sum() == 10) << "Vector components constructed correctly";

					b1 = vector(5);
					b2 = vector(5, 9);
					b3 = vector(5, 9, 17);
					b4 = vector(5, 9, 17, 29);

					examiner->check(b1.sum() == 5) << "Vector components constructed correctly";
					examiner->check(b2.sum() == 5+9) << "Vector components constructed correctly";
					examiner->check(b3.sum() == 5+9+17) << "Vector components constructed correctly";
					examiner->check(b4.sum() == 5+9+17+29) << "Vector components constructed correctly";

					b2 = b4;

					examiner->check(b2[0] == b4[0]) << "Vector components are equal";
					examiner->check(b2[1] == b4[1]) << "Vector components are equal";

					Vector<4, float> f4;

					f4 = b4;

					examiner->check(b4.sum() == f4.sum()) << "Vector components copied correctly";

					f4 = ZERO;

					examiner->check(f4.sum() == 0.0f) << "Vector is zeroed";

					f4 = IDENTITY;

					examiner->check(f4.sum() == 4.0f) << "Vector is identity";

					f4 = 2.5;

					examiner->check(f4.sum() == 10.0f) << "Vector was assigned to correctly.";

					f4 = b4;

					examiner->check(b4.sum() == f4.sum()) << "Vector components copied correctly";

					examiner->check(f4.equivalent(b4));
				}
			},

			{"Cross Product",
				[](UnitTest::Examiner * examiner) {
					Vec3 a{1, 0, 0}, b{0, 1, 0};

					examiner->check(cross_product(a, b).equivalent({0, 0, 1}));
				}
			},

			{"Geometric Comparisons",
				[](UnitTest::Examiner * examiner) {
					examiner->check(vector(4, 5).less_than(vector(6, 6))) << "Vector comparison correct";
					examiner->check(!vector(4, 5).less_than(vector(4, 4))) << "Vector comparison correct";

					examiner->check(vector(4, 5).less_than_or_equal(vector(6, 6))) << "Vector comparison correct";
					examiner->check(vector(4, 4).less_than_or_equal(vector(4, 4))) << "Vector comparison correct";
					examiner->check(!vector(4, 5).less_than_or_equal(vector(4, 4))) << "Vector comparison correct";

					examiner->check(!vector(4, 5).greater_than(vector(6, 6))) << "Vector comparison correct";
					examiner->check(!vector(4, 5).greater_than(vector(4, 4))) << "Vector comparison correct";
					examiner->check(vector(5, 5).greater_than(vector(4, 4))) << "Vector comparison correct";

					examiner->check(!vector(4, 5).greater_than_or_equal(vector(6, 6))) << "Vector comparison correct";
					examiner->check(vector(4, 5).greater_than_or_equal(vector(4, 4))) << "Vector comparison correct";
					examiner->check(vector(4, 4).greater_than_or_equal(vector(4, 4))) << "Vector comparison correct";
				}
			},

			{"Reflection",
				[](UnitTest::Examiner * examiner) {
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

						examiner->check(c.equivalent(std::get<2>(record)));

						k += 1;
					}
				}
			},

			{"Comparisons",
				[](UnitTest::Examiner * examiner) {
					Vec2 v1 = {1, 1}, v2 = {1, 2}, v3 = {2, 1}, v4 = {2, 2};

					// Equality
					examiner->check(v1 == v1) << "==";
					examiner->check(v1 != v2) << "!=";

					// Lesser than
					examiner->check(v1 < v2) << "<";
					examiner->check(!v4 < v1) << "<";
					examiner->check(v1 <= v2) << "<=";
					examiner->check(v1 <= v1) << "<=";
					examiner->check(!v2 <= v1) << "<=";

					// Greater than
					examiner->check(v2 > v1) << ">";
					examiner->check(v3 > v2) << ">";
					examiner->check(!(v2 >= v3)) << ">=";
					examiner->check(v1 >= v1) << ">=";
					examiner->check(v3 >= v2) << ">=";
				}
			},

			{"Arithmetic",
				[](UnitTest::Examiner * examiner) {
					Vec2 v1 = {1, 1}, v2 = {1, 2}, v3 = {2, 1}, v4 = {2, 2};

					// Basic Arithmetic
					examiner->check(v2+v3 == v1+v4) << "+ == +";
					examiner->check((v3/v2) * v2 == v3) << "/ * ==";
					examiner->check(vector(2, 1) % 2 == vector(0, 1)) << "% ==";

					// Advanced Arithemtic
					examiner->check(v2.sum() == 3) << "sum";
					examiner->check(v4.product() == 4) << "product";
					examiner->check(vector(2.5, 5.5).truncate() == vector(2.0, 5.0)) << "floor";
					examiner->check(vector(2.5, 5.5).truncate(true) == vector(3.0, 6.0)) << "ceil";
					examiner->check(vector(2.5, 5.25).fraction() == vector(0.5, 0.25)) << "frac";
				}
			},

			{"Sets",
				[](UnitTest::Examiner * examiner) {
					Vec2 v1 = {1, 1}, v2 = {1, 2}, v3 = {2, 1}, v4 = {2, 2};

					std::set<Vec2> set1, set2, set3;
					set1.insert(v1);
					set1.insert(v2);
					set1.insert(v3);

					set2.insert(v1);
					set2.insert(v2);

					set3.insert(v1);
					set3.insert(v4);

					examiner->check(set1.size() == 3) << "Set has correct number of items";
					examiner->check(set2.size() == 2) << "Set has correct number of items";
					examiner->check(set3.size() == 2) << "Set has correct number of items";

					examiner->check(includes(set1.begin(), set1.end(), set2.begin(), set2.end())) << "Set functions work correctly";
					examiner->check(!includes(set1.begin(), set1.end(), set3.begin(), set3.end())) << "Set functions work correctly";
				}
			},

			{"Normalization",
				[](UnitTest::Examiner * examiner) {
					examiner->check_equal(vector(0.0, 0.0, 2.5).normalize(), vector(0.0, 0.0, 1.0));
					examiner->check_equal(vector(0.0, 0.0, 2.5).normalize(2), vector(0.0, 0.0, 2.0));
				}
			},

			{"Distribution",
				[](UnitTest::Examiner * examiner) {
					using namespace Euclid::Numerics;

					std::mt19937 generator(41234234);
					UniformVectorDistribution<3> distribution({0.2, 0.2, 0.2}, {0.6, 0.6, 0.6});

					examiner->check(distribution(generator) > Vec3{0.2, 0.2, 0.2});
					examiner->check(distribution(generator) < Vec3{0.8, 0.8, 0.8});
				}
			},

			{"Expansion",
				[](UnitTest::Examiner * examiner) {
					using namespace Euclid::Numerics;

					auto v1 = vector(1, 2, 3, 4, 5);
					auto v2 = vector(1).expand(2, 3, 4, 5);

					examiner->check_equal(v1, v2);
				}
			},

			{"Ratios",
				[](UnitTest::Examiner * examiner) {
					using namespace Euclid::Numerics;

					auto size = ratios(Vec2{10.0, 20.0});
					examiner->check_equal(size.aspect_ratio(), 0.5);

					auto resized_smaller = ratios(Vec2{20.0, 10.0}).shrink_to_fit_aspect_ratio(0.5);
					examiner->check_equal(resized_smaller, Vec2{5.0, 10.0});

					auto resized_larger = ratios(Vec2{20.0, 10.0}).expand_to_fit_aspect_ratio(0.5);
					examiner->check_equal(resized_larger, Vec2{20.0, 40.0});
				}
			},
		};
	}
}