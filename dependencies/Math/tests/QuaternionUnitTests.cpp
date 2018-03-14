#include "Quaternion.h"
#include "gtest/gtest.h"


namespace
{
	TEST(QuaternionTests, Test1) {
		Vector4 axis = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		Quaternion q = Quaternion::FromAngleAxis(90.0f, axis);
		std::cout << "  q" << q << std::endl;

		Quaternion vi = { 0.0f, 7.0f, 0.0f, 0.0f };
		std::cout << "  vi" << vi << std::endl;

		Quaternion q1 = Quaternion(0.0f, 7.0f, 0.0f, 0.0f);
		std::cout << "  q1" << q1 << std::endl;

		Quaternion qe = Quaternion(0.0f, 0.0f, 0.0f, -7.0f);
		std::cout << "  qe" << qe << std::endl;

		Quaternion qInv = q.InverseQuaternion();
		qInv.Clean();
		std::cout << "  qinv" << qInv << std::endl;

		Quaternion qf = (q*vi)*qInv;
		std::cout << "  qf" << qf << std::endl;

		ASSERT_TRUE(qf == qe);
	}
	TEST(QuaternionTests, Test2)
	{
		Vector4 axis = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		Quaternion q = Quaternion::FromAngleAxis(90.0f, axis);
		std::cout << "  q " << q << std::endl;
		Vector4 vi = Vector4(7.0f, 0.0f, 0.0f, 1.0f);
		std::cout << "  vi " << vi << std::endl;

		Vector4 ve = Vector4(0.0f, 0.0f, -7.0f, 1.0f);
		std::cout << "  ve " << ve << std::endl;

		Matrix4 m = q.ToGLMatrix();
		std::cout << "  M " << m << std::endl;

		Vector4 vf = m * vi;
		std::cout << " vf " << vf << std::endl;

		assert(vf == ve);
	}

	TEST(QuaternionTests, Test3)
	{
		std::cout << "Yaw 900 = Roll 180 + Pitch 180" << std::endl;

		Vector4 axis_x = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		Vector4 axis_y = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		Vector4 axis_z = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		Quaternion qyaw900 = Quaternion::FromAngleAxis(900.0f, axis_y);
		std::cout << "  qyaw900 " << qyaw900 << std::endl;
		Quaternion qroll180 = Quaternion::FromAngleAxis(180.0f, axis_x);
		std::cout << "  qroll180 " << qroll180 << std::endl;
		Quaternion qpitch180 = Quaternion::FromAngleAxis(180.0f, axis_z);
		std::cout << "  qpitch180 " << qpitch180 << std::endl;
		Quaternion qrp = qpitch180 * qroll180;
		std::cout << "  qrp " << qrp << std::endl;
		Quaternion qpr = qroll180 * qpitch180;
		std::cout << "  qpr " << qpr << std::endl;

		Quaternion qi = Quaternion(0.0f, 1.0f, 0.0f, 0.0f);
		std::cout << "  qi " << qi << std::endl;
		Quaternion qe = Quaternion(0.0f, -1.0f, 0.0f, 0.0f);
		std::cout << "  qe " << qe << std::endl;

		Quaternion qyaw900inv = qyaw900.InverseQuaternion();
		Quaternion qfy = (qyaw900 * qi) * qyaw900inv;
		std::cout << "  qfy " << qfy << std::endl;
		ASSERT_TRUE((qe == qfy));

		Quaternion qrpinv = qrp.InverseQuaternion();
		Quaternion qfrp = (qrp*qi) * qrpinv;
		std::cout << "  qfrp " << qfrp << std::endl;
		ASSERT_TRUE((qe == qfrp));

		Quaternion qprinv = qpr.InverseQuaternion();
		Quaternion qfpr = (qpr*qi)*qprinv;
		std::cout << "  qpfr " << qfpr << std::endl;
		ASSERT_TRUE(qe == qfpr);
	}

	TEST(QuaternionTests, Test4)
	{
		float thetai = 45.0f;
		Vector4 axis_i = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		Quaternion q = Quaternion::FromAngleAxis(thetai, axis_i);
		std::cout << thetai << " ";
		std::cout << "axis_i " << axis_i << std::endl;

		float thetaf;
		Vector4 axis_f;
		q.QuaternionToAngleAxis(thetaf, axis_f);
		std::cout << thetaf << " ";
		std::cout << "axis_f " << axis_f << std::endl;

		ASSERT_TRUE((fabs(thetai - thetaf) < THRESHOLD) && (axis_i == axis_f));
	}

	TEST(QuaternionTests, Test5)
	{
		Vector4 axis = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		Quaternion q0 = Quaternion::FromAngleAxis(0.0f, axis);
		std::cout << "  q0 " << q0 << std::endl;
		Quaternion q1 = Quaternion::FromAngleAxis(90.0f, axis);
		std::cout << "  q1 " << q1 << std::endl;
		Quaternion qe = Quaternion::FromAngleAxis(30.0f, axis);
		std::cout << "  qe " << qe << std::endl;

		Quaternion qLerp0 = Quaternion::Lerp(q0, q1, 0.0f);
		std::cout << "  lerp(0) " << qLerp0 << std::endl;
		ASSERT_TRUE(qLerp0 == q0);

		Quaternion qLerp1 = Quaternion::Lerp(q0, q1, 1.0f);
		std::cout << "  lerp(1) " << qLerp1 << std::endl;
		ASSERT_TRUE(qLerp1 == q1);

		Quaternion qLerp = Quaternion::Lerp(q0, q1, 1.0f/3.0f);
		std::cout << "  lerp(1/3) " << qLerp << std::endl;

		ASSERT_TRUE((qLerp == qe) == 0);

	}

	TEST(QuaternionTests, Test6)
	{
		Vector4 axis = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		Quaternion q0 = Quaternion::FromAngleAxis(0.0f, axis);
		std::cout << "  q0 " << q0 << std::endl;
		Quaternion q1 = Quaternion::FromAngleAxis(90.0f, axis);
		std::cout << "  q1 " << q1 << std::endl;
		Quaternion qe = Quaternion::FromAngleAxis(30.0f, axis);
		std::cout << "  qe " << qe << std::endl;

		Quaternion qSlerp0 = Quaternion::Slerp(q0, q1, 0.0f);
		std::cout << "  slerp(0) " << qSlerp0 << std::endl;
		ASSERT_TRUE(qSlerp0 == q0);

		Quaternion qSlerp1 = Quaternion::Slerp(q0, q1, 1.0f);
		std::cout << "  slerp(1) " << qSlerp1 << std::endl;
		ASSERT_TRUE(qSlerp1 == q1);

		Quaternion qSlerp = Quaternion::Slerp(q0, q1, 1.0f / 3.0f);
		std::cout << "  slerp(1/3) " << qSlerp << std::endl;

		ASSERT_TRUE(qSlerp == qe);
	}
}
