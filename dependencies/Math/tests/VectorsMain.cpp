#include "gtest\gtest.h"
#include "Vector3.h"
#define TEST_MODE 1
int main(int argc, char** argv) {
	if(TEST_MODE){
		testing::InitGoogleTest(&argc, argv);
		RUN_ALL_TESTS();
		std::cin.get();
	}
	else{
	//lab exercise
	Vector3 v1 = Vector3(1, 0, 0);
	Vector3 v2 = Vector3(0, 2, 0);
	Vector3 v3 = Vector3(0, 0, 3);
	//case1
	std::cout << "CASE 1 - UP: "<< v1 <<" VIEW: " << v1 << std::endl;
	Vector3 v = v1.normalize();
	Vector3 w;
	Vector3 u;
	
	try {
		 w = (Vector3::crossProduct(v1, v)).normalize();
		 u = Vector3::crossProduct(v, w);
		std::cout << "V:" << v << std::endl;
		std::cout << "U:" << u << std::endl;
		std::cout << "W:" << w << std::endl;

	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}

	

	//case2

	std::cout << "CASE 2 - UP: " << v1 << " VIEW: " << v2 << std::endl;
	v = v2.normalize();
	try{
	w = (Vector3::crossProduct(v1, v)).normalize();
	u = Vector3::crossProduct(v, w);
	std::cout << "V:" << v << std::endl;
	std::cout << "U:" << u << std::endl;
	std::cout << "W:" << w << std::endl;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}

	//case3

	std::cout << "CASE 3 - UP: " << v1 << " VIEW: " << v3 << std::endl;
	v = v3.normalize();
	try {
		w = (Vector3::crossProduct(v1, v)).normalize();
		u = Vector3::crossProduct(v, w);
		std::cout << "V:" << v << std::endl;
		std::cout << "U:" << u << std::endl;
		std::cout << "W:" << w << std::endl;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
	
	//case 4

	std::cout << "CASE 4 - UP: " << v2 << " VIEW: " << v1 << std::endl;
	try{
	v = v1.normalize();
	w = (Vector3::crossProduct(v2, v)).normalize();
	u = Vector3::crossProduct(v, w);
	std::cout << "V:" << v << std::endl;
	std::cout << "U:" << u << std::endl;
	std::cout << "W:" << w << std::endl;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
	//case 5

	std::cout << "CASE 5 - UP: " << v2 << " VIEW: " << v2 << std::endl;
	try{
	v = v2.normalize();
	w = (Vector3::crossProduct(v2, v)).normalize();
	u = Vector3::crossProduct(v, w);
	std::cout << "V:" << v << std::endl;
	std::cout << "U:" << u << std::endl;
	std::cout << "W:" << w << std::endl;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
	//case 6



	std::cout << "CASE 6 - UP: " << v2 << " VIEW: " << v3 << std::endl;
	try{
	v = v3.normalize();
	w = (Vector3::crossProduct(v2, v)).normalize();
	u = Vector3::crossProduct(v, w);
	std::cout << "V:" << v << std::endl;
	std::cout << "U:" << u << std::endl;
	std::cout << "W:" << w << std::endl;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}

	//case 7

	std::cout << "CASE 7 - UP: " << v3 << " VIEW: " << v1 << std::endl;
	try{
	v = v1.normalize();
	w = (Vector3::crossProduct(v3, v)).normalize();
	u = Vector3::crossProduct(v, w);
	std::cout << "V:" << v << std::endl;
	std::cout << "U:" << u << std::endl;
	std::cout << "W:" << w << std::endl;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
	//case 8
	std::cout << "CASE 8 - UP: " << v3 << " VIEW: " << v2 << std::endl;
	try{
	v = v2.normalize();
	w = (Vector3::crossProduct(v3, v)).normalize();
	u = Vector3::crossProduct(v, w);
	std::cout << "V:" << v << std::endl;
	std::cout << "U:" << u << std::endl;
	std::cout << "W:" << w << std::endl;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
	//case 9
	std::cout << "CASE 9 - UP: " << v3 << " VIEW: " << v3 << std::endl;
	try{
	v = v3.normalize();
	w = (Vector3::crossProduct(v3, v)).normalize();
	u = Vector3::crossProduct(v, w);
	std::cout << "V:" << v << std::endl;
	std::cout << "U:" << u << std::endl;
	std::cout << "W:" << w << std::endl;
	}
	catch (std::exception e) {
	std::cout << e.what() << std::endl;
	}


	}
	std::cin.get();
	return 0;
}