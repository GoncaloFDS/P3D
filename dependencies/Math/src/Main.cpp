#include "Matrix3.h"
#include "Matrix2.h"
#include "Matrix4.h"
#include <ctime>
#include "gtest\gtest.h"
#include "MatrixUnitTests.cpp"
#include "QuaternionUnitTests.cpp"
#include "Vector2UnitTests.cpp"
#include "Vector3UnitTests.cpp"

#define TEST_MODE 1
int main(int argc, char** argv) {

	if (TEST_MODE) {
		testing::InitGoogleTest(&argc, argv);
		RUN_ALL_TESTS();
	
		std::cin.get();
		return 0;
	}
	//lab exercise
	else {
		srand(static_cast <unsigned> (time(0)));
		float random[20];

		for(int l = 0; l < 10;  l++){
		for (int i = 0; i < 18; i++) {
			random[i] = static_cast <float> (rand()%18-9);
		}

		Matrix3 M1 = Matrix3(random[0], random[1], random[2], random[3], random[4], random[5], random[6], random[7], random[8]);
		Matrix3 M2 = Matrix3(random[9], random[10], random[11], random[12], random[13], random[14], random[15], random[16] ,random[17]);
		try {
			Matrix3 M3 = (M1 * M2).inverse();
			Matrix3 M3Expected = (M2.inverse())* (M1.inverse());
			if (M3 == M3Expected) {
				std::cout << "INVERSE OK!";
			}
			else {
				std::cout << "(M1*M2)^1 is: " << std::endl;
				std::cout << M3 << std::endl;
				std::cout << "(M2)^1 * (M1)^-1 is: " << std::endl;
				std::cout << M3Expected << std::endl;
			}


			Matrix3 MT = (M1 * M2).transpose();

			Matrix3 MTExpected = (M2.transpose()) * (M1.transpose());

			if (MT == MTExpected) {
				std::cout << "TRANPOSE OK!" << std::endl;
				continue;
			}
			else {
				std::cout << "(M1*M2)T is: " << std::endl;
				std::cout << MT << std::endl;
				std::cout << "(M2)T * (M1)T is: " << std::endl;
				std::cout << MTExpected << std::endl;
			}

		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
			continue;
			
		}

		
		
		}

		try {
			Matrix3 M1 = Matrix3(1, 2, 3, 4, 5, 6, 7, 8, 9);
			Matrix3 M3Null = Matrix3::ZERO;

			Matrix3 M3Error = (M1*M3Null).inverse();
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}

		std::cin.get();
		return 0;
	}
}