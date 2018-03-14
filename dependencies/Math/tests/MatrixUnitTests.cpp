#include "Matrix3.h"
#include "gtest\gtest.h"
#include "Matrix2.h"
#include "Matrix4.h"
namespace {

	TEST(Matrix2Tests, Constructor) {
		Matrix2 m2 = Matrix2(0, 0, 0, 0);
		ASSERT_TRUE(m2.data[0] == 0 && m2.data[1] == 0 && m2.data[2] == 0 && m2.data[3] == 0);
	}


	TEST(Matrix2Tests, Identity) {
		Matrix2 m2 = Matrix2::IDENTITY;
		ASSERT_TRUE(m2.data[0] == 1 && m2.data[1] == 0 && m2.data[2] == 0 && m2.data[3] == 1);
	}

	TEST(Matrix2Tests, Zero) {
		Matrix2 m2 = Matrix2::ZERO;
		ASSERT_TRUE(m2.data[0] == 0 && m2.data[1] == 0 && m2.data[2] == 0 && m2.data[3] == 0);
	}

	TEST(Matrix2Tests, Scale) {
		Matrix2 m2 = Matrix2::SCALE(2, 1);
		ASSERT_TRUE(m2.data[0] == 2 && m2.data[1] == 0 && m2.data[2] == 0 && m2.data[3] == 1);
	}

	TEST(Matrix2Tests, Rotation) {
		Matrix2 m2 = Matrix2::ROTATE(M_PI);
		Matrix2 m3 = Matrix2(cos(M_PI), sin(M_PI), -sin(M_PI), cos(M_PI));
		ASSERT_TRUE((m2 == m3));
	}

	TEST(Matrix2Tests, Translation) {
		Matrix3 m3 = Matrix2::TRANSLATION(2, 2);
		ASSERT_TRUE((m3.data[0] == 1 && m3.data[1] == 0 && m3.data[2] == 0 && m3.data[3] == 0 && m3.data[4] == 1 && m3.data[5] == 0	&& m3.data[6] == 2 && m3.data[7] == 2 && m3.data[8] == 1));
	}

	TEST(Matrix2Tests, Sum) {
		Matrix2 m1 = Matrix2(1, 1, 1, 1);
		Matrix2 m2 = Matrix2(1, 1, 1, 1);
		Matrix2 res = m1 + m2;

		ASSERT_TRUE((res.data[0] == 2 && res.data[1] == 2 && res.data[2] == 2 && res.data[3] == 2));

	}

	TEST(Matrix2Tests, Sub) {
		Matrix2 m1 = Matrix2(1, 1, 1, 1);
		Matrix2 m2 = Matrix2(1, 1, 1, 1);
		Matrix2 res = m1 - m2;
		ASSERT_TRUE((res == Matrix2::ZERO));
	}

	TEST(Matrix2Tests, MultiplyByScalar) {
		Matrix2 m1 = Matrix2(1, 1, 1, 1);
		Matrix2 res = m1 * 2;
		ASSERT_TRUE((res.data[0] == 2 && res.data[1] == 2 && res.data[2] == 2 && res.data[3] == 2));
	}

	TEST(Matrix2Tests, MultiplyByVector) {
		Matrix2 m2 = Matrix2::IDENTITY;
		Vector2 v2 = Vector2(4, 3);
		Vector2 res  = m2 * v2;
		Matrix2 m3 = Matrix2(4, 3, 0, 1);
		ASSERT_TRUE((res == v2));
		Vector2 res2 = m3*v2;
		ASSERT_TRUE((res2.coordinates[0] == 16 && res2.coordinates[1] == 15));
	}

	TEST(Matrix2Tests, MultiplyByMatrix2) {
		Matrix2 m1 = Matrix2(4, 3, 0, 1);
		Matrix2 m2 = Matrix2(5, 4, 3, 1);
		Matrix2 res = m1 * m2;
		ASSERT_TRUE((res.data[0] == 20 && res.data[1] == 19 && res.data[2] == 12 && res.data[3] == 10));
	}

	TEST(Matrix2Tests, ConvertToMatrix3) {
		Matrix2 m1 = Matrix2(4, 3, 0, 1);
		Matrix3 m3 = Matrix2::ConvertToMatrix3(m1);
		ASSERT_TRUE((m3.data[0] == 4 && m3.data[1] == 3 && m3.data[2] == 0 && m3.data[3] == 0 && m3.data[4] == 1 && m3.data[5] == 0 && m3.data[6] == 0 && m3.data[7] == 0 && m3.data[8] == 1));

	}

	TEST(Matrix2Tests, ConvertFromMatrix3) {
		Matrix3 m3 = Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);
		Matrix2 m2 = Matrix2::ConvertFromMatrix3(m3);
		ASSERT_TRUE((m2 == Matrix2::IDENTITY));
	}

	TEST(Matrix2Tests, Equality) {
		Matrix2 m1 = Matrix2(1, 2, 3, 4);
		Matrix2 m2 = Matrix2(1, 2, 3, 4);
		ASSERT_TRUE((m1 == m2));
	}

	TEST(Matrix2Tests, Inequality) {
		Matrix2 m1 = Matrix2(1, 2, 3, 4);
		Matrix2 m2 = Matrix2(1, 3, 3, 4);
		ASSERT_TRUE((m1 != m2));
	}

	TEST(Matrix2Tests, Assignment) {
		Matrix2 m1 = Matrix2::ZERO;
		Matrix2 m2 = Matrix2(1, 2, 3, 4);
		m1 = m2;
		ASSERT_TRUE((m1 == m2));
	}

	TEST(Matrix2Tests, Tranpose) {
		Matrix2 m1 = Matrix2(1, 2, 3, 4);
		Matrix2 m1t = m1.tranpose();
		ASSERT_TRUE((m1t.data[0] == m1.data[0] && m1t.data[1] == m1.data[2] && m1t.data[2] == m1.data[1] && m1t.data[3] == m1.data[3]));
	}

	TEST(Matrix2Tests, Determinant) {
		Matrix2 m1 = Matrix2::IDENTITY;
		ASSERT_TRUE((m1.determinant() == 1));
	}

	TEST(Matrix2Tests, Inverse) {
		Matrix2 m1 = Matrix2(1, 2, 3, 4);
		Matrix2 m2 = m1.inverse();
		ASSERT_TRUE(m2.data[0] == -2 && m2.data[1] == 1 && m2.data[2] == 1.5f && m2.data[3] == -0.5f);
	}

	TEST(Matrix2Tests, InvalidInverse) {
		Matrix2 m1 = Matrix2::ZERO;
		ASSERT_ANY_THROW(m1.inverse());
	}

	TEST(Matrix3Tests, Constructor) {
		Matrix3 m3 = Matrix3(1, 2, 3, 4, 5, 6, 7, 8, 9);
		ASSERT_TRUE((m3.data[0] == 1 && m3.data[1] == 2 && m3.data[2] == 3 && m3.data[3] == 4 && m3.data[4] == 5 && m3.data[5] == 6 && m3.data[6] == 7 && m3.data[7] == 8 && m3.data[8] == 9));
	}

	TEST(Matrix3Tests, Zero) {
		Matrix3 m3 = Matrix3::ZERO;
		ASSERT_TRUE((m3.data[0] == 0 && m3.data[1] == 0 && m3.data[2] == 0 && m3.data[3] == 0 && m3.data[4] == 0 && m3.data[5] == 0 && m3.data[6] == 0 && m3.data[7] == 0 && m3.data[8] == 0));
	}
	
	TEST(Matrix3Tests, Identity) {
		Matrix3 m3 = Matrix3::IDENTITY;
		ASSERT_TRUE((m3.data[0] == 1 && m3.data[1] == 0 && m3.data[2] == 0 && m3.data[3] == 0 && m3.data[4] == 1 && m3.data[5] == 0 && m3.data[6] == 0 && m3.data[7] == 0 && m3.data[8] == 1));

	}

	TEST(Matrix3Tests, Equality) {
		Matrix3 m3 = Matrix3(1, 2, 3, 4, 5, 6, 7, 8, 9);
		Matrix3 m3new = Matrix3(1, 2, 3, 4, 5, 6, 7, 8, 9);
		ASSERT_TRUE((m3 == m3new));
	}

	TEST(Matrix3Tests, Rotation) {
		Matrix3 m3 = Matrix3::ROTATE(AXIS::X, M_PI);
		Matrix3 mxrot = Matrix3(1, 0, 0, 0, cos(M_PI), sin(M_PI), 0, -sin(M_PI), cos(M_PI));
		ASSERT_TRUE(m3 == mxrot);
		Matrix3 m4 = Matrix3::ROTATE(AXIS::Y, M_PI);
		Matrix3 myrot = Matrix3(cos(M_PI), 0, -sin(M_PI), 0, 1, 0, sin(M_PI), 0, cos(M_PI));
		ASSERT_TRUE((m4 == myrot));
		Matrix3 m5 = Matrix3::ROTATE(AXIS::Z, M_PI);
		Matrix3 mzrot = Matrix3(cos(M_PI), sin(M_PI), 0, -sin(M_PI), cos(M_PI), 0, 0, 0, 1);
		ASSERT_TRUE((m5 == mzrot));
	}

	TEST(Matrix3Tests, Scale) {
		Matrix3 m3 = Matrix3::SCALE(2, 2, 2);
		Matrix3 scale = Matrix3(2, 0, 0, 0, 2, 0, 0, 0, 2);
		ASSERT_TRUE((m3 == scale));
	}

	TEST(Matrix3Tests, Translate) {
		Matrix4 m3translation = Matrix3::TRANSLATE(1, 2, 3);
		Matrix4 m3real = Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 2, 3, 1);
		ASSERT_TRUE(m3translation == m3real);
	}

	TEST(Matrix3Tests, ConvertFromMatrix4) {
		Matrix4 toConvert = Matrix4(1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 4);
		Matrix3 converted = Matrix3::ConvertFromMatrix4(toConvert);
		Matrix3 expected = Matrix3(1, 2, 3, 1, 2, 3, 1, 2, 3);
		ASSERT_TRUE(converted == expected);
	}

	TEST(Matrix3Tests, ConvertToMatrix4) {
		Matrix3 toConvert = Matrix3(1, 2, 3, 1, 2, 3, 1, 2, 3);
		Matrix4 converted = Matrix3::ConvertToMatrix4(toConvert);
		Matrix4 expected = Matrix4(1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 0, 0, 0, 1);
		ASSERT_TRUE(converted == expected);
	}
	TEST(Matrix3Tests, Inequality) {
		Matrix3 m3 = Matrix3::IDENTITY;
		Matrix3 m3fake = Matrix3(1, 2, 3, 4, 5, 6, 7, 8, 9);
		ASSERT_TRUE(m3 != m3fake);
	}

	TEST(Matrix3Tests, Assignment) {
		Matrix3 m3 = Matrix3::IDENTITY;
		Matrix3 m4 = Matrix3::ZERO;
		m3 = m4;
		ASSERT_TRUE(m3 == m4);
	}

	TEST(Matrix3Tests, Sum) {
		Matrix3 m3 = Matrix3::IDENTITY;
		Matrix3 m4 = Matrix3::IDENTITY;
		Matrix3 m2 = m3 + m4;
		Matrix3 expected = (Matrix3::IDENTITY);
		expected = expected * 2;
		ASSERT_TRUE(m2 == expected);
	}

	TEST(Matrix3Tests, Sub) {
		Matrix3 m3 = Matrix3::IDENTITY;
		Matrix3 m4 = Matrix3::IDENTITY;
		Matrix3 m2 = m3 - m4;
		ASSERT_TRUE(m2 == Matrix3::ZERO);
	}

	TEST(Matrix3Tests, MultiplyByScalar) {
		Matrix3 m3 = Matrix3::IDENTITY;
		Matrix3 m4 = m3 * 2;
		ASSERT_TRUE(m4.data[0] == 2 && m4.data[1] == 0 && m4.data[2] == 0 && m4.data[3] == 0 && m4.data[4] == 2 && m4.data[5] == 0 && m4.data[6] == 0 && m4.data[7] == 0 && m4.data[8] == 2);
	}

	TEST(Matrix3Tests, MultiplyByVector) {
		Matrix3 m3 = Matrix3::IDENTITY;
		Vector3 v3 = Vector3(1, 2, 3);
		ASSERT_TRUE((m3 * v3) == v3);
		Matrix3 m4 = Matrix3(2, 0, 0, 0, 2, 0, 0, 0, 2);
		Vector3 expected = Vector3(2, 4, 6);
		ASSERT_TRUE((m4*v3) == expected);
	}

	TEST(Matrix3Tests, MultiplyByMatrix) {
		Matrix3 m3 = Matrix3(1, 2, 3, 4, 5, 6, 7, 8, 9);
		Matrix3 m4 = Matrix3(9, 8, 7, 6, 5, 4, 3, 2, 1);
		Matrix3 expected = Matrix3(90, 114, 138, 54, 69, 84, 18, 24, 30);
		ASSERT_TRUE((m3 * m4) == expected);
	}

	TEST(Matrix3Tests, DivisionByScalar) {
		float scalar = 2;
		Matrix3 m3 = Matrix3(2, 2, 2, 2, 2, 2, 2, 2, 2);
		Matrix3 expected = Matrix3(1, 1, 1, 1, 1, 1, 1, 1, 1);
		ASSERT_TRUE((m3 / scalar) == expected);
	}

	TEST(Matrix3Tests, Determinant) {
		Matrix3 m3 = Matrix3(8, 3, 1, 7, 4, 4, 6, 2, 3);
		float expected = 31;
		ASSERT_TRUE(m3.determinant() == expected);
	}

	TEST(Matrix3Tests, Tranpose) {
		Matrix3 m3 = Matrix3(8, 3, 1, 7, 4, 4, 6, 2, 3);
		Matrix3 expected = Matrix3(8, 7, 6, 3, 4, 2, 1, 4, 3);
		ASSERT_TRUE((m3.transpose()) == expected);
	}

	TEST(Matrix3Tests, Inverse) {
		Matrix3 m3 = Matrix3(1, 0, 5, 2, 1, 6, 3, 4, 0);
		Matrix3 expected = Matrix3(-24, 20, -5, 18, -15, 4, 5, -4, 1);
		ASSERT_TRUE((m3.inverse()) == expected);
	}


	TEST(Matrix4Tests, Constructor) {
		Matrix4 m4 = Matrix4(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
		ASSERT_TRUE(m4.data[0] == 1 && m4.data[1] == 1 && m4.data[2] == 1 && m4.data[3] == 1 && m4.data[4] == 1 && m4.data[5] == 1 && m4.data[6] == 1
			&& m4.data[7] == 1 && m4.data[8] == 1 && m4.data[9] == 1 && m4.data[10] == 1 && m4.data[11] == 1 && m4.data[12] == 1 && m4.data[13] == 1 &&
			m4.data[14] == 1 && m4.data[15] == 1);
	}

	TEST(Matrix4Tests, Identity) {
		Matrix4 m4 = Matrix4::IDENTITY;
		ASSERT_TRUE(m4.data[0] == 1 && m4.data[1] == 0 && m4.data[2] == 00 && m4.data[3] == 0 && m4.data[4] == 0 && m4.data[5] == 1 && m4.data[6] == 0
			&& m4.data[7] == 0 && m4.data[8] == 0 && m4.data[9] == 0 && m4.data[10] == 1 && m4.data[11] == 0 && m4.data[12] == 0 && m4.data[13] == 0 &&
			m4.data[14] == 0 && m4.data[15] == 1);
	}

	TEST(Matrix4Tests, Zero) {
		Matrix4 m4 = Matrix4::ZERO;
		ASSERT_TRUE(m4.data[0] == 0 && m4.data[1] == 0 && m4.data[2] == 00 && m4.data[3] == 0 && m4.data[4] == 0 && m4.data[5] == 0 && m4.data[6] == 0
			&& m4.data[7] == 0 && m4.data[8] == 0 && m4.data[9] == 0 && m4.data[10] == 0 && m4.data[11] == 0 && m4.data[12] == 0 && m4.data[13] == 0 &&
			m4.data[14] == 0 && m4.data[15] == 0);
	}

	TEST(Matrix4Tests, Rotation) {
		Matrix4 m4 = Matrix4::ROTATE(AXIS::X, M_PI);
		Matrix4 m4rot = Matrix3::ConvertToMatrix4(Matrix3::ROTATE(AXIS::X, M_PI));
		ASSERT_TRUE(m4 == m4rot);
	}

	TEST(Matrix4Tests, Scale) {
		Matrix4 m4 = Matrix4::SCALE(1, 2, 3);
		Matrix4 m4scale = Matrix3::ConvertToMatrix4(Matrix3::SCALE(1, 2, 3));
		ASSERT_TRUE(m4 == m4scale);
	}

	TEST(Matrix4Tests, Sum) {
		Matrix4 m4 = Matrix4::IDENTITY;
		Matrix4 m5 = Matrix4::IDENTITY;
		Matrix4 expected = Matrix4(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2);
		ASSERT_TRUE((m4 + m5) == expected);
	}
	
	TEST(Matrix4Tests, Sub) {
		Matrix4 m4 = Matrix4::IDENTITY;
		Matrix4 m5 = Matrix4::IDENTITY;
		Matrix4 expected = Matrix4::ZERO;
		ASSERT_TRUE((m4 - m5) == expected);
	}

	TEST(Matrix4Tests, MultiplyByScalar) {
		Matrix4 m4 = Matrix4::IDENTITY;
		Matrix4 expected = Matrix4(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2);
		ASSERT_TRUE((m4 * 2) == expected);
	}

	TEST(Matrix4Tests, MultiplyByVector4) {
		Matrix4 m4 = Matrix4(2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5);
		Vector4 v4 = Vector4(1, 2, 3, 4);
		Vector4 expected = Vector4(40, 40, 40, 40);
		ASSERT_TRUE((m4*v4) == expected);
	}

	TEST(Matrix4Tests, MultiplyByMatrix4) {
		Matrix4 m4 = Matrix4(2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5);
		Matrix4 m5 = Matrix4(2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5);
		Matrix4 expected = Matrix4(28, 28, 28, 28, 42, 42, 42, 42, 56, 56, 56, 56, 70, 70, 70, 70);
		ASSERT_TRUE((m4*m5) == expected);
	}

	TEST(Matrix4Tests, DivisionByScalar) {
		float scalar = 2;
		Matrix4 m4 = Matrix4(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2);
		Matrix4 expected = Matrix4::IDENTITY;
		ASSERT_TRUE((m4 / scalar) == expected);
	}

	TEST(Matrix4Tests, Equality) {
		Matrix4 m1 = Matrix4(1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5,6,7);
		Matrix4 m2 = Matrix4(1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7);
		ASSERT_TRUE(m1 == m2);
	}

	TEST(Matrix4Tests, Inequality) {
		Matrix4 m1 = Matrix4::ZERO;
		Matrix4 m2 = Matrix4::IDENTITY;
		ASSERT_TRUE(m1 != m2);
	}

	TEST(Matrix4Tests, Transpose) {
		Matrix4 m1 = Matrix4(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16);
		Matrix4 expected = Matrix4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
		ASSERT_TRUE((m1.transpose()) == expected);
	}

	TEST(CleanMatrixes, CleanMatrix2) {
		Matrix2 m2 = Matrix2(0.0004, 0.0004, 0.0004, 0.0004);
		Matrix2 expected = Matrix2::ZERO;
		ASSERT_TRUE((m2.clean()) == expected);
	}

	TEST(CleanMatrixes, CleanMatrix3) {
		Matrix3 m3 = Matrix3(0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004);
		Matrix3 expected = Matrix3::ZERO;
		ASSERT_TRUE((m3.clean()) == expected);
	}

	TEST(CleanMatrixes, CleanMatrix4) {
		Matrix4 m4 = Matrix4(0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004);
		Matrix4 expected = Matrix4::ZERO;
		ASSERT_TRUE((m4.clean()) == expected);
	}
}