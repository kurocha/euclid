//
//  Numerics/Vector.cpp
// This file is part of the "Euclid" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 19/03/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "Vector.h"

#include <iostream>
#include <vector>

#include <set>
#include <tuple>

namespace Euclid
{
	namespace Numerics
	{
// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING
		UNIT_TEST(Vector)
		{
			testing("Construction");

			Vector<1, int> b1 = vec(1);
			Vector<2, int> b2 = vec(1, 2);
			Vector<3, int> b3 = vec(1, 2, 3);
			Vector<4, int> b4 = vec(1, 2, 3, 4);

			check(b1.sum() == 1) << "Vector components constructed correctly";
			check(b2.sum() == 3) << "Vector components constructed correctly";
			check(b3.sum() == 6) << "Vector components constructed correctly";
			check(b4.sum() == 10) << "Vector components constructed correctly";

			b1 = vec(5);
			b2 = vec(5, 9);
			b3 = vec(5, 9, 17);
			b4 = vec(5, 9, 17, 29);

			check(b1.sum() == 5) << "Vector components constructed correctly";
			check(b2.sum() == 5+9) << "Vector components constructed correctly";
			check(b3.sum() == 5+9+17) << "Vector components constructed correctly";
			check(b4.sum() == 5+9+17+29) << "Vector components constructed correctly";

			b2.set(b4);

			check(b2[0] == b4[0]) << "Vector components are equal";
			check(b2[1] == b4[1]) << "Vector components are equal";

			Vector<4, float> f4;

			f4.set(b4);

			check(b4.sum() == f4.sum()) << "Vector components copied correctly";

			f4.zero();

			check(f4.sum() == 0.0f) << "Vector is zeroed";

			f4.load_identity();

			check(f4.sum() == 4.0f) << "Vector is identity";

			f4 = 2.5;

			check(f4.sum() == 10.0f) << "Vector was assigned to correctly.";

			f4 = b4;

			check(b4.sum() == f4.sum()) << "Vector components copied correctly";

			check(f4 == b4) << "Vectors are equal";

			testing("Geometric Comparisons");

			check(vec(4, 5).less_than(vec(6, 6))) << "Vector comparison correct";
			check(!vec(4, 5).less_than(vec(4, 4))) << "Vector comparison correct";

			check(vec(4, 5).less_than_or_equal(vec(6, 6))) << "Vector comparison correct";
			check(vec(4, 4).less_than_or_equal(vec(4, 4))) << "Vector comparison correct";
			check(!vec(4, 5).less_than_or_equal(vec(4, 4))) << "Vector comparison correct";

			check(!vec(4, 5).greater_than(vec(6, 6))) << "Vector comparison correct";
			check(!vec(4, 5).greater_than(vec(4, 4))) << "Vector comparison correct";
			check(vec(5, 5).greater_than(vec(4, 4))) << "Vector comparison correct";

			check(!vec(4, 5).greater_than_or_equal(vec(6, 6))) << "Vector comparison correct";
			check(vec(4, 5).greater_than_or_equal(vec(4, 4))) << "Vector comparison correct";
			check(vec(4, 4).greater_than_or_equal(vec(4, 4))) << "Vector comparison correct";
		}

		UNIT_TEST(VectorReflection)
		{
			// 0:Wall Normal, 1:I, 2:R
			typedef std::tuple<Vec2, Vec2, Vec2> ReflectTestRecord;
			std::vector<ReflectTestRecord> reflection_tests;

			reflection_tests.push_back(std::make_tuple(Vec2(1.0, 0.0), Vec2(-1.0, -1.0), Vec2(1.0, -1.0)));
			reflection_tests.push_back(std::make_tuple(Vec2(1.0, 0.0), Vec2(-1.0, 0.0), Vec2(1.0, 0.0)));
			reflection_tests.push_back(std::make_tuple(Vec2(1.0, 1.0).normalize(), Vec2(-1.0, -1.0), Vec2(1.0, 1.0)));
			reflection_tests.push_back(std::make_tuple(Vec2(1.0, 1.0).normalize(), Vec2(-1.0, 0.0), Vec2(0.0, 1.0)));

			/*
			    (Vec2(1.0, 0.0), Vec2(-0.417897, 0.908494), Vec2(0.417897, 0.908494))
			    (Vec2(0.0, 1.0), Vec2(-0.593251, -0.805017), Vec2(-0.593251, 0.805017))
			    (Vec2(0.0, 1.0), Vec2(-1, 0), Vec2(-1, 0))
			    (Vec2(0.106533, 0.994309), Vec2(-0.593248, -0.80502), Vec2(-0.409235, 0.912429)
			     };
			*/

			testing("Vector Reflection");

			unsigned k = 0;
			foreach (r, reflection_tests)
			{
				Vec2 c = std::get<1>(*r).reflect(std::get<0>(*r));

				check(c.equivalent(std::get<2>(*r))) << k << ": R <" << std::get<2>(*r) << "> is reflected around N <" << std::get<1>(*r) << ">";

				k += 1;
			}
		}

		UNIT_TEST(VectorArithmetic)
		{
			using namespace std;
			Vec2 v1(1, 1), v2(1, 2), v3(2, 1), v4(2, 2);

			testing ("Equality");
			check(v1 == v1) << "==";
			check(v1 != v2) << "!=";

			testing ("Lesser Than");
			check(v1 < v2) << "<";
			check(!v4 < v1) << "<";
			check(v1 <= v2) << "<=";
			check(v1 <= v1) << "<=";
			check(!v2 <= v1) << "<=";

			testing("Greater Than");
			check(v2 > v1) << ">";
			check(v3 > v2) << ">";
			check(!(v2 >= v3)) << ">=";
			check(v1 >= v1) << ">=";
			check(v3 >= v2) << ">=";

			testing("Basic Arithmetic");
			check(v2+v3 == v1+v4) << "+ == +";
			check((v3/v2) * v2 == v3) << "/ * ==";
			check(vec(2, 1) % 2 == vec(0, 1)) << "% ==";

			testing("Advanced Arithemtic");
			check(v2.sum() == 3) << "sum";
			check(v4.product() == 4) << "product";
			check(vec(2.5, 5.5).floor() == vec(2.0, 5.0)) << "floor";
			check(vec(2.5, 5.5).ceil() == vec(3.0, 6.0)) << "ceil";
			check(vec(2.5, 5.25).frac() == vec(0.5, 0.25)) << "frac";

			testing("Sets");
			std::set<Vec2> set1, set2, set3;
			set1.insert(v1);
			set1.insert(v2);
			set1.insert(v3);

			set2.insert(v1);
			set2.insert(v2);

			set3.insert(v1);
			set3.insert(v4);

			check(set1.size() == 3) << "Set has correct number of items";
			check(set2.size() == 2) << "Set has correct number of items";
			check(set3.size() == 2) << "Set has correct number of items";

			//std::cout << set1 << " " << set2 << " " << set3 << std::endl;

			check(includes(set1.begin(), set1.end(), set2.begin(), set2.end())) << "Set functions work correctly";
			check(!includes(set1.begin(), set1.end(), set3.begin(), set3.end())) << "Set functions work correctly";

			Vec3 a(1, 0, 0), b(0, 1, 0);

			//std::cout << "a.dot(b) = " << a.dot(b) << std::endl;
			//std::cout << "b.dot(a) = " << b.dot(a) << std::endl;

			testing("Normalization");
			check(vec(0.0, 0.0, 2.5).normalize() == vec(0.0, 0.0, 1.0)) << "Normalized correctly";
			check(vec(0.0, 0.0, 2.5).normalize(2) == vec(0.0, 0.0, 2.0)) << "Normalized correctly";

			/*
			 Vector<4, unsigned> t1(0x13, 0x23, 0xAF, 0x53);
			 unsigned t2 = 0; t1.pack(8, t2);
			 Vector<4, unsigned> t3;
			 t3.unpack(8, t2);

			 std::cout << "Original: " << t1 << " Packed/Unpacked: " << t3 << std::endl;
			 */
		}

		UNIT_TEST(VectorConversion)
		{
			Vec1 v1 = 5.5;
			RealT r1 = v1;

			check(r1 == 5.5) << "Type conversion successful";
		}
#endif
	}
}
