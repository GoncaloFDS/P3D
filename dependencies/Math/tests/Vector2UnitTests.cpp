#include "gtest\gtest.h"
#include "Vector2.h"

namespace {

	TEST(Vector2Tests, Sum) {
		Vector2 a = Vector2(1, 0);
		Vector2 b = Vector2(0, 1);
		Vector2 c = a + b;
		ASSERT_TRUE(c.coordinates[0] == 1 && c.coordinates[1] == 1);
	}

	TEST(Vector2Tests, Sub) {
		Vector2 a = Vector2(1, 1);
		Vector2 b = Vector2(1, 1);
		Vector2 c = a - b;
		ASSERT_TRUE(c.coordinates[0] == 0 && c.coordinates[1] == 0);
	}

	TEST(Vector2Tests, MultiplyByScalar) {
		Vector2 a = Vector2(1, 1);
		Vector2 b = a * 2;
		ASSERT_TRUE(b.coordinates[0] == 2 && b.coordinates[1] == 2);
	}

	TEST(Vector2Tests, DotProduct) {
		Vector2 a = Vector2(1, 0);
		Vector2 b = Vector2(0, 1);
		ASSERT_TRUE((a*b) == 0);
	}

	TEST(Vector2Tests, Equal) {
		Vector2 a = Vector2(1, 0);
		Vector2 b = Vector2(1, 0);
		ASSERT_TRUE(a == b);
	}

	TEST(Vector2Tests, Dif) {
		Vector2 a = Vector2(1, 0);
		Vector2 b = Vector2(0, 1);
		ASSERT_TRUE(a != b);
	}

	TEST(Vector2Tests, DivisionByScalar) {
		Vector2 a = Vector2(1, 1);
		Vector2 b = a / 2;
		ASSERT_TRUE(b.coordinates[0] == 0.5f && b.coordinates[0] == 0.5f);
		ASSERT_TRUE(a.coordinates[0] == 1.0f && a.coordinates[1] == 1.0f);
	}

	TEST(Vector2Tests, Assignment) {
		Vector2 a = Vector2(1, 0);
		Vector2 b = Vector2(0, 1);
		a = b;
		ASSERT_TRUE(a.coordinates[0] == 0.0f && a.coordinates[1] == 1.0f);
	}

	TEST(Vector2Tests, Quadrance) {
		Vector2 a = Vector2(2, 2);
		ASSERT_TRUE(a.quadrance() == 8);
	}

	TEST(Vector2Tests, Magnitude) {
		Vector2 a = Vector2(3, 4);
		ASSERT_TRUE(a.magnitude() == 5);
	}

	TEST(Vector2Tests, Normalize) {
		Vector2 a = Vector2(3, 4);
		float magnitude = a.magnitude();
		Vector2 normalized = a.normalize();
		ASSERT_TRUE(normalized.coordinates[0] == (3 / magnitude) && normalized.coordinates[1] == (4 / magnitude));
	}

	TEST(Vector2Tests, Output) {
		Vector2 a = Vector2(1, 1);
		std::stringstream output;
		output << a;
		std::string expected = "Vector2[x:1, y:1]";
		std::string actual = output.str();

		ASSERT_TRUE(expected == actual);
	}


}