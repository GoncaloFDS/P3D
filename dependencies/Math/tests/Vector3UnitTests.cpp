#include "gtest\gtest.h"
#include "Vector3.h"

namespace{

	TEST(Vector3Tests, Sum) {
		Vector3 a = Vector3(1, 0, 0);
		Vector3 b = Vector3(0, 1, 0);
		Vector3 w = Vector3(0, 0, 1);
		Vector3 c = a + b + w;
		ASSERT_TRUE(c.coordinates[0] == 1 && c.coordinates[1] == 1 && c.coordinates[2] == 1);
	}

	TEST(Vector3Tests, Sub) {
		Vector3 a = Vector3(1, 0, 0);
		Vector3 b = Vector3(0, 1, 0);
		Vector3 c = a - b;
		ASSERT_TRUE(c.coordinates[0] == 1 && c.coordinates[1] == -1 && c.coordinates[2] == 0);
	}

	TEST(Vector3Tests, MultiplyScalar) {
		Vector3 a = Vector3(1, 1, 1);
		Vector3 b = a * 3;
		ASSERT_TRUE(b.coordinates[0] == 3 && b.coordinates[1] == 3 && b.coordinates[2] == 3);
	}

	TEST(Vector3Tests, InnerProduct) {
		Vector3 a = Vector3(1, 0, 0);
		Vector3 b = Vector3(0, 1, 0);
		float dotresult = a*b;
		ASSERT_TRUE((dotresult == 0));
	}

	TEST(Vector3Tests, Equal) {
		Vector3 a = Vector3(1, 0, 0);
		Vector3 b = Vector3(1, 0, 0);
		ASSERT_TRUE(a == b);
	}

	TEST(Vector3Tests, CrossProduct) {
		Vector3 a = Vector3(1, 0, 0);
		Vector3 b = Vector3(0, 1, 0);
		Vector3 actual = Vector3::crossProduct(a, b);
		ASSERT_TRUE(actual.coordinates[0] == 0 && actual.coordinates[1] == 0 && actual.coordinates[2] == 1);
	}

	TEST(Vector3Tests, Diff) {
		Vector3 a = Vector3(1, 0, 0);
		Vector3 b = Vector3(0, 1, 0);
		ASSERT_TRUE(a != b);
	}

	TEST(Vector3Tests, DivisionByScalar) {
		Vector3 a = Vector3(1, 1, 1);
		Vector3 result = a / 2;
		ASSERT_TRUE(result.coordinates[0] == 0.5f && result.coordinates[1] == 0.5f && result.coordinates[2] == 0.5f);
	}
	
	TEST(Vector3Tests, Assignment) {
		Vector3 a = Vector3(1, 1, 1);
		Vector3 b = Vector3(0, 0, 0);
		b = a;
		ASSERT_TRUE(b.coordinates[0] == a.coordinates[0] && b.coordinates[1] == a.coordinates[1] && b.coordinates[2] == a.coordinates[2]);
	}

	TEST(Vector3Tests, Output) {
		std::stringstream output;
		Vector3 a = Vector3(1, 1, 1);
		output << a;
		ASSERT_TRUE((output.str()) == "Vector3[1,1,1]");
	}

	TEST(Vector3Tests, Quadrance) {
		Vector3 a = Vector3(3, 4, 5);
		float result = a.quadrance();
		ASSERT_TRUE(result == (3 * 3 + 4 * 4 + 5 * 5));
	}

	TEST(Vector3Tests, Magnitude) {
		Vector3 a = Vector3(3, 4, 5);
		float result = a.magnitude();
		ASSERT_TRUE(result == (float) sqrt(3*3 + 4*4 + 5*5));
	}

	TEST(Vector3Tests, Normalize) {
		Vector3 a = Vector3(2, 2, 2);
		float l = a.magnitude();
		Vector3 normalized = a.normalize();
		ASSERT_TRUE(normalized.coordinates[0] == (2 / l) && normalized.coordinates[1] == (2 / l) && normalized.coordinates[2] == (2 / l));
	}

}