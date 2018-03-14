#pragma once
///////////////////////////////////////////////////////////////////////
//
// Using quaternions to rotate in 3D.
//
// Assignment: 1. Create a class for Quaternions.
//             2. Create a scene with a camera rotating around an 
//                object at the origin and pointing towards it. 
//                Do NOT use "gluLookAt" to create the ViewMatrix, 
//                use rotation matrices! DONE
//             3. Gimbal lock mode ON: use Translation + Rotation 
//                matrices (e.g. Euler angles, Rodrigues’ formula).
//             4. Gimbal lock mode OFF: use Quaternions to produce a 
//                transformation matrix and avoid gimbal lock.
//             5. Switch between modes by pressing the 'g' key.
//
// (c) 2013-17 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include "Quaternion.h"
#include "gtest/gtest.h"
#include "QuaternionUnitTests.cpp"

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "Shader.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Camera.h"
#include <vector>
#define CAPTION "Hello Modern 3D World"
#include "Colors.h"
#include "Mouse.h"
#include "QuaternionCamera.h"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;
#define PI 3.14159265358979323846
#define ToRadians(X) (X)*PI/180.0f
#define VERTICES 0
#define COLORS 1
#define W 0
#define S 1
#define A 2
#define D 3
#define Q 4
#define E 5
#define CameraSpeed 0.50f

QuaternionCamera qCamera = QuaternionCamera(Vector3(1,1,-5));
float currentTime = 0, deltaTime = 0, previousTime = 0;
bool GimbleLock = true;
GLuint VaoId, VboId[2];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UboId, UniformId, ColorId;
const GLuint UBO_BP = 0;
Shader Program;
float deltaAngle = 0;
bool keyStates[6];
bool MouseState;
float lastX = 0, lastY = 0;
float deltaX = 0, deltaY = 0;
float mouseX = 0, mouseY = 0;
bool FirstClick = false;
float deltaAngleX = 0, deltaAngleY = 0;
float angleX = 0, angleY = 0;
float xOrigin = 0, yOrigin = 0;





Camera camera = Camera(Vector3(0, 0, 5), Vector3(0, 0, 0));
//Matrix4 ViewMatrix = Matrix4::ViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, 5), Vector3(0, 1, 0));
Matrix4 ViewMatrix = Matrix4::TRANSLATE(0, 0, -5) * Matrix4::RodriguesRotation(Vector3(1, 0, 0), 0) * Matrix4::RodriguesRotation(Vector3(0, 1, 0), 0) * Matrix4::RodriguesRotation(Vector3(0, 0, 1), 0);
float x = ViewMatrix.data[12];
float y = ViewMatrix.data[13];
float z = ViewMatrix.data[14];
Matrix4 SavedStateCamera = ViewMatrix;
Matrix4 SavedQuaternionCamera = ViewMatrix;

Quaternion finalQuat = Quaternion(1,0,0,0);
Mouse mouse;
/////////////////////////////////////////////////////////////////////// ERRORS
Matrix4 Translation;
bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs
void createShaderProgram()
{
	Program = Shader();
	Program.LoadShader(GL_VERTEX_SHADER, "Assets/Shaders/Vertex.Shader");
	Program.LoadShader(GL_FRAGMENT_SHADER, "Assets/Shaders/Fragment.Shader");
	Program.Prepare();
	Program.Attach(GL_VERTEX_SHADER);
	Program.Attach(GL_FRAGMENT_SHADER);
	Program.BindAttribute(VERTICES, "in_Position");
	Program.Link();
	Program.AddUniform("ModelMatrix");
	Program.AddUniform("ex_Color");
	Program.AddUniformBlock("SharedMatrices");
	Program.BindUniformBlock(Program.GetUniformBlock("SharedMatrices"), UBO_BP);

	checkOpenGLError("ERROR: Could not create shaders.");
}



void destroyShaderProgram()
{
	Program.Delete();
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs
typedef struct {
	Vector4 XYZW;
} Vertex;

typedef GLfloat Matrix[16];

const Vertex Vertices[] = // no indices?
{
	{ Vector4(0.0f, 0.0f, 0.5f, 1.0f) }, // 0 - FRONT
	{ Vector4(0.5f, 0.0f, 0.5f, 1.0f) }, // 1
	{ Vector4(0.5f, 0.5f, 0.5f, 1.0f) }, // 2

	{ Vector4(0.5f, 0.0f, 0.0f, 1.0f) }, // 4	
	{ Vector4(0.5, 0.5f, 0.0f, 1.0f) }, // 3
	{ Vector4(0.5f, 0.5f, 0.5f, 1.0f) }, // 2
	{ Vector4(0.5f, 0.5f, 0.5f, 1.0f) }, // 2 - RIGHT
	{ Vector4(0.5f, 0.0f, 0.5f, 1.0f) }, // 1
	{ Vector4(0.5f, 0.0f, 0.0f, 1.0f) }, // 4


	{ Vector4(0.5f, 0.5f, 0.5f, 1.0f) }, // 2 - TOP
	{ Vector4(0.5f, 0.5f, 0.0f, 1.0f) }, // 3
	{ Vector4(0.0f, 0.0f, 0.0f, 1.0f) }, // 5
	{ Vector4(0.0f, 0.0f, 0.0f, 1.0f) }, // 5	
	{ Vector4(0.0f, 0.0f, 0.5f, 1.0f) }, // 0
	{ Vector4(0.5f, 0.5f, 0.5f, 1.0f) }, // 2

	{ Vector4(0.5f, 0.5f, 0.0f, 1.0f) }, // 3
	{ Vector4(0.5f, 0.0f, 0.0f, 1.0f) }, // 4 - BACK
	{ Vector4(0.0f, 0.0f, 0.0f, 1.0f) }, // 5

	{ Vector4(0.0f, 0.0f, 0.0f, 1.0f) }, // 5
	{ Vector4(0.5f, 0.0f, 0.0f, 1.0f) }, // 4
	{ Vector4(0.5f, 0.0f, 0.5f, 1.0f) }, // 1
	{ Vector4(0.5f, 0.0f, 0.5f, 1.0f) }, // 1
	{ Vector4(0.0f, 0.0f, 0.5f, 1.0f) }, // 0 - BOTTOM
	{ Vector4(0.0f, 0.0f, 0.0f, 1.0f) }, // 5



};

void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, VboId[1]);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	//glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

//MOUSE AND KEYBOARD INPUTS

void ControlCamera() {

	if (keyStates[W])
	{
		qCamera.MoveCamera(Direction::Forward, deltaTime);
		ViewMatrix = qCamera.GetCamera();
	}
	if (keyStates[S]) {
		qCamera.MoveCamera(Direction::Backward, deltaTime);
		ViewMatrix = qCamera.GetCamera();
	}
	if (keyStates[A]){
		qCamera.MoveCamera(Direction::Left, deltaTime);
		ViewMatrix = qCamera.GetCamera();
	}
	if (keyStates[D]){
		qCamera.MoveCamera(Direction::Right, deltaTime);
		ViewMatrix = qCamera.GetCamera();
	}
	if (keyStates[Q]) {
		
		x = ViewMatrix.data[12];
		y = ViewMatrix.data[13];
		z = ViewMatrix.data[14];
		if (GimbleLock) {
			//Roll = Matrix4::RodriguesRotation(Vector3(0, 0, 1), deltaAngle);
			deltaAngle += CameraSpeed*deltaTime;
			ViewMatrix = Matrix4::TRANSLATE(x, y, z) * Matrix4::RodriguesRotation(Vector3(0, 0, 1), deltaAngle) * Matrix4::RodriguesRotation(Vector3(1, 0, 0), deltaAngleY) * Matrix4::RodriguesRotation(Vector3(0, 1, 0), deltaAngleX);
		}else
		{
			float AngleZ = CameraSpeed*0.25f;
			qCamera.ChangeOrientation(AngleZ, Vector4(0, 0, 1, 1));
			ViewMatrix = qCamera.GetCamera();

		}
	}
	if (keyStates[E]){
		
		x = ViewMatrix.data[12];
		y = ViewMatrix.data[13];
		z = ViewMatrix.data[14];
		if (GimbleLock) {
		deltaAngle -= CameraSpeed*deltaTime;
		ViewMatrix = Matrix4::TRANSLATE(x, y, z) * Matrix4::RodriguesRotation(Vector3(0, 0, 1), deltaAngle) * Matrix4::RodriguesRotation(Vector3(1, 0, 0), deltaAngleY) * Matrix4::RodriguesRotation(Vector3(0, 1, 0), deltaAngleX);
		}else
		{
			float AngleZ = - CameraSpeed*0.25f;
			qCamera.ChangeOrientation(AngleZ, Vector4(0, 0, 1, 1));
			ViewMatrix = qCamera.GetCamera();
		}
	}
}

void rotateCamera() {
	if (mouse.MouseState) {
		deltaAngleX += ToRadians(mouse.MouseX) * CameraSpeed;
		deltaAngleY += ToRadians(mouse.MouseY) * CameraSpeed;
		
		x = ViewMatrix.data[12];
		y = ViewMatrix.data[13];
		z = ViewMatrix.data[14];
		if(GimbleLock){
			
			ViewMatrix = Matrix4::TRANSLATE(x, y, z) * Matrix4::RodriguesRotation(Vector3(0, 0, 1), deltaAngle) * Matrix4::RodriguesRotation(Vector3(1, 0, 0), deltaAngleY) * Matrix4::RodriguesRotation(Vector3(0, 1, 0), deltaAngleX);
		
		}
		else
		{
			float xAngle = mouse.MouseX*CameraSpeed;
			float yAngle = mouse.MouseY*CameraSpeed;
			/*
			Quaternion qtrnX = Quaternion::FromAngleAxis(yAngle, Vector4(1, 0, 0, 1));
			Quaternion qtrnY = Quaternion::FromAngleAxis(xAngle, Vector4(0, 1, 0, 1));
			*/
			//Quaternion temp = qtrnY * qtrnX;
			//finalQuat = temp * finalQuat;
			//ViewMatrix = Matrix4::TRANSLATE(x, y, z) * finalQuat.ToGLMatrix();	
			qCamera.ChangeOrientation(xAngle, Vector4(0, 1, 0, 1));
			qCamera.ChangeOrientation(yAngle, Vector4(1, 0, 0, 1));
			ViewMatrix = qCamera.GetCamera();
		}
		mouse.MouseX = 0;
		mouse.MouseY = 0;
	}
}

void mouseMovement(int x, int y) {
	if (mouse.MouseState) {
		mouse.Movement(x, y);
	}

}





//////////////////////////////////////////

/////////////////////////////////////////////////////////////////////// SCENE
typedef Matrix4 TangramFigure[];
const Matrix ModelMatrix = {
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  1.0f
}; // Column Major


const TangramFigure SleepingCat = {
	Matrix4::TRANSLATE(-0.6f, 0.3f, 0.0f) * Matrix4::ROTATE(AXIS::Z, PI / 4) * Matrix4::SCALE(0.4,0.4,1) * *ModelMatrix,
	Matrix4::TRANSLATE(-0.28f, 0.58f, 0.0f) * Matrix4::ROTATE(AXIS::Z, PI + PI / 4) * Matrix4::SCALE(0.4,0.4,0.9)* *ModelMatrix,
	Matrix4::TRANSLATE(-0.3f, 0.3f, 0.0f) * Matrix4::ROTATE(AXIS::Z, PI - PI / 4) * Matrix4::SCALE(0.4, 0.4, 0.8)* *ModelMatrix, // HEAD PART 1
	Matrix4::TRANSLATE(-0.58f, 0.30f, 0.0f) * Matrix4::ROTATE(AXIS::Z, -PI / 4) * Matrix4::SCALE(0.4,0.4,0.8)* *ModelMatrix, // HEAD PART 2
	Matrix4::TRANSLATE(-0.11f, -0.05f, 0.0f) * Matrix4::ROTATE(AXIS::Z, PI / 2 + PI / 4) * Matrix4::SCALE(0.75f, 0.75f, 0.95)* *ModelMatrix, // Body 1
	Matrix4::TRANSLATE(-0.25f, 0.10f, 0.0f) * Matrix4::ROTATE(AXIS::Z, -PI / 4) * Matrix4::SCALE(0.50f, 0.50f, 1)* *ModelMatrix, // Body 2
	Matrix4::TRANSLATE(0.375f, -0.17f, 0.0f) * Matrix4::ROTATE(AXIS::Z, PI / 2 + PI / 4) * Matrix4::SCALE(0.75f, 0.75f, 0.88)* *ModelMatrix, // Body 3
	Matrix4::TRANSLATE(0.32f, -0.10f, 0.0f) * Matrix4::ROTATE(AXIS::Z, -PI / 4) * Matrix4::SCALE(0.40f, 0.40f, 0.79)* *ModelMatrix, //tail 1
	Matrix4::TRANSLATE(0.74f, -0.24f, 0.0f) * Matrix4::ROTATE(AXIS::Z, PI / 2 + PI / 4) * Matrix4::SCALE(0.40f, 0.40f, 0.99)* *ModelMatrix, //tail 2
};

const Matrix I = {
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
};



// Eye(5,5,5) Center(0,0,0) Up(0,1,0)
const Matrix4 ViewMatrix1 = Matrix4::ViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, 5), Vector3(0, 1, 0));
// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
const Matrix4 ViewMatrix2 = Matrix4::ViewMatrix(Vector3(0, 0, 0), Vector3(-5, -5, -5), Vector3(0, 1, 0));
const Matrix4 ProjectionMatrix1 = Matrix4::OrthoMatrix(-2, 2, 2, -2, 1, 10);

Matrix4 UsingMatrix = ProjectionMatrix1;
//Matrix4 ViewMatrix = ViewMatrix1;



// Orthographic LeftRight(-2,2) TopBottom(-2,2) NearFar(1,10)
const Matrix4 ProjectionMatrix2 = Matrix4::ProjectionMatrix(PI / 6, 640.0f / 480.0f, 1, 10);
// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)




void drawElement(Matrix4 matrix, float* MainColor, float* DarkerColor1, float* DarkerColor2, float* DarkerColor3, float* DarkerColor4) {
	glUniform4fv(Program.GetUniform("ex_Color"), 1, MainColor);
	glUniformMatrix4fv(Program.GetUniform("ModelMatrix"), 1, GL_FALSE, matrix.data);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUniform4fv(Program.GetUniform("ex_Color"), 1, DarkerColor1);
	glDrawArrays(GL_TRIANGLES, 3, 6);
	glUniform4fv(Program.GetUniform("ex_Color"), 1, DarkerColor2);
	glDrawArrays(GL_TRIANGLES, 9, 6);
	glUniform4fv(Program.GetUniform("ex_Color"), 1, DarkerColor3);
	glDrawArrays(GL_TRIANGLES, 15, 3);
	glUniform4fv(Program.GetUniform("ex_Color"), 1, DarkerColor4);
	glDrawArrays(GL_TRIANGLES, 18, 6);
}




void drawScene()
{
	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix), ViewMatrix.data);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix), sizeof(Matrix), UsingMatrix.data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(VaoId);
	Program.Enable();

	drawElement(SleepingCat[0], RED, DARKRED1, DARKRED2, DARKRED3, DARKRED4);
	drawElement(SleepingCat[1], BLUE, DARKBLUE1, DARKBLUE2, DARKBLUE3, DARKBLUE4);
	drawElement(SleepingCat[2], ORANGE, DARKORANGE1, DARKORANGE2, DARKORANGE3, DARKORANGE4);
	drawElement(SleepingCat[3], ORANGE, DARKORANGE1, DARKORANGE2, DARKORANGE3, DARKORANGE4);
	drawElement(SleepingCat[4], CYAN, DARKCYAN1, DARKCYAN2, DARKCYAN3, DARKCYAN4);
	drawElement(SleepingCat[5], PURPLE, DARKPURPLE1, DARKPURPLE2, DARKPURPLE3, DARKPURPLE4);
	drawElement(SleepingCat[6], MAGENTA, DARKMAGENTA1, DARKMAGENTA2, DARKMAGENTA3, DARKMAGENTA4);
	drawElement(SleepingCat[7], YELLOW, DARKYELLOW1, DARKYELLOW2, DARKYELLOW3, DARKYELLOW4);
	drawElement(SleepingCat[8], GREEN, DARKGREEN1, DARKGREEN2, DARKGREEN3, DARKGREEN4);

	Program.Disable();
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentTime - previousTime) / 1000.0f;
	previousTime = currentTime;
}


void idle()
{
	glutMotionFunc(mouseMovement);
	ControlCamera();
	rotateCamera();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

// MOUSE AND KEYBOARD CALL BACKS

void handleKeyUp(unsigned char Key, int x, int y) {
	switch (Key) {
	case 'w':
		keyStates[W] = false;
		break;
	case 'a':
		keyStates[A] = false;
		break;
	case 's':
		keyStates[S] = false;
		break;
	case 'd':
		keyStates[D] = false;
		break;
	case 'q':
		keyStates[Q] = false;
		break;
	case 'e':
		keyStates[E] = false;
		break;
	case 'p':
		if (UsingMatrix == ProjectionMatrix1) {
			UsingMatrix = ProjectionMatrix2;
		}
		else {
			UsingMatrix = ProjectionMatrix1;
		}
		break;
	case 'r':
		ViewMatrix = Matrix4::ViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, 5), Vector3(0, 1, 0));
		break;
	case 'g':
		
		GimbleLock = !GimbleLock;
		if(GimbleLock)
		{
			SavedQuaternionCamera = ViewMatrix;
			ViewMatrix = SavedStateCamera;
			std::cout << "Using Gimble Lock ON" << std::endl;
		}
		else
		{
			SavedStateCamera = ViewMatrix;
			ViewMatrix = SavedQuaternionCamera;

			std::cout << "Using Quaternions" << std::endl;
		}
		break;
	}
}


void mouseHandle(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouse.MouseState = 1;
			
		}
		if (state == GLUT_UP) {
			mouse.MouseState = 0;
			mouse.oldAngleX += x;
			mouse.oldAngleY += y;
			mouse.FirstClick = false;
		}
	}
}


void handleKeyDown(unsigned char Key, int x, int y) {
	switch (Key) {
	case 'w':
		keyStates[W] = true;
		break;
	case 's':
		keyStates[S] = true;
		break;
	case 'a':
		keyStates[A] = true;
		break;
	case 'd':
		keyStates[D] = true;
		break;
	case 'q':
		keyStates[Q] = true;
		break;
	case 'e':
		keyStates[E] = true;
		break;
	}
}

//

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardUpFunc(handleKeyUp);
	glutKeyboardFunc(handleKeyDown);
	glutMouseFunc(mouseHandle);
}

void setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
	previousTime = glutGet(GLUT_ELAPSED_TIME);
	mouse = Mouse();
}


int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}
