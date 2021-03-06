 ///////////////////////////////////////////////////////////////////////
//
// P3D Course
// (c) 2016 by Jo�o Madeiras Pereira
// TEMPLATE: Whitted Ray Tracing NFF scenes and drawing points with Modern OpenGL
//
//You should develop your rayTracing( Ray ray, int depth, float RefrIndex) which returns a color and
// to develop your load_NFF function
//
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <algorithm> 
#include <cmath>
#include "math.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "stdlib.h"
#include "Scene.h"
#include "Ray.h"
#include <cstdlib>
#include "Grid.h"
#include <time.h>
#include "MathUtils.h"
#define CAPTION "ray tracer"

#define VERTEX_COORD_ATTRIB 0
#define COLOR_ATTRIB 1
#define M_PI 3.14159265358979323846
#define MAX_DEPTH 4
#define SAMPLE_SIZE 32

// Points defined by 2 attributes: positions which are stored in vertices array and colors which are stored in colors array
float *colors;
float *vertices;

int size_vertices;
int size_colors;

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

GLfloat m[16];  //projection matrix initialized by ortho function

GLuint VaoId;
GLuint VboId[2];

GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

Scene* scene = nullptr;
int RES_X, RES_Y;
bool glossy = true;

/* Draw Mode: 0 - point by point; 1 - line by line; 2 - full frame */
int draw_mode=1; 

int WindowHandle = 0;

bool isPointInShadow(Hit &hit, Vector3 lightDir) {
	Ray shadowFeeler(hit.Location, lightDir);
	
	Hit shadowHit = scene->calculateClossestHit(shadowFeeler);
	return shadowHit.HasCollided;

// 	for (auto obj : *scene->getObjects()) {
// 		Hit shadowHit = obj->calculateIntersection(shadowFeeler);
// 
// 		if (shadowHit.HasCollided) 
// 			if ((lightDir - hit.Location).quadrance() > shadowHit.T * shadowHit.T) //ignore objects behind the light
// 				return true;
// 		
// 	}
// 	return false;
}

Hit calculateClossestHit(Ray ray){
	double Tmin = DBL_MAX;
	Hit hit;
	for (auto obj : *scene->getObjects()) {
		Hit tempHit = obj->calculateIntersection(ray);
		if ((tempHit.HasCollided && tempHit.T < Tmin)) {
			Tmin = tempHit.T;
			hit = tempHit;
			hit.Mat = obj->material;
		}
	}	return hit;
}

Ray calculateReflectedRay(Hit hit, Vector3 ViewDir) {
	Vector3 rr = 2 * (ViewDir * hit.Normal)*hit.Normal - ViewDir;
	return Ray(hit.Location, rr);
}

Ray calculateGlossyReflectedRay(Hit hit, Vector3 ViewDir, float& phong_lobe, float& pdf) {
	Vector3 rr = calculateReflectedRay(hit, ViewDir).Dir;
	float epson_1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float epson_2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//float theta = pow(acosf(1 - epson_1), hit.Mat->glosiness);
	//float phi = 2 * M_PI*epson_2;

	//float x = sinf(phi)*cosf(theta);
	//float y = sinf(phi)*sinf(theta);
	//float z = cosf(phi);

	float phi = epson_1 * 2.0 * M_PI;
	float cosTheta = sqrt(1.0 - epson_2);
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	Vector3 w = rr;
	Vector3 u = (Vector3::crossProduct(Vector3(0.00424, 1, 0.00764), w)).normalize();
	//Vector3 u = Vector3::crossProduct(w, hit.Normal);
	Vector3 v = Vector3::crossProduct(w, u);

	Vector3 S = Vector3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);

	Vector3 jittered_dir = S.x() * u + S.y() * v + S.z() * w;

	if (hit.Normal * jittered_dir < 0.0f) {
		jittered_dir = -S.x() * u - S.y() * v + S.z() * w;
	}
	phong_lobe = pow(rr*jittered_dir, hit.Mat->shininess);
	pdf = phong_lobe * (hit.Normal*jittered_dir);
	return Ray(hit.Location, jittered_dir);
}



Ray calculateRefractedRay(Hit hit, Ray ray, Material *mat, float RefractionIndex) {
	Vector3 Nrefr = hit.Normal.normalize();
	Vector3 I = (hit.Location - ray.O).normalize();
	float NdotI = Nrefr * I;
	NdotI = MathUtils::clamp(NdotI, 1, -1);
	float etai = RefractionIndex, etat = mat->refractionIndex;
	if (NdotI < 0) {
		//outside the surface, cos theta should be positive
		NdotI = -NdotI;
	}
	else {
		//inside the surface, reverse normal direction
		Nrefr = -hit.Normal.normalize();
		std::swap(etai, etat);
	}

	float eta = etai / etat;
	float k = 1 - ((eta * eta)*(1 - NdotI * NdotI));
	if (k < 0) {
		return Ray(hit.Location, Vector3(0,0,0));
	}
	else
		return Ray(hit.Location, eta * (I) + (eta * NdotI - sqrtf(k))*Nrefr);
		
}

///////////////////////////////////////////////////////////////////////  RAY-TRACE SCENE

Vector3 rayTracing(Ray ray, int depth, float RefrIndex)
{
	Hit hit;
	if (scene->validGrid())
		hit = scene->calculateClossestHit(ray);
	else
		hit = calculateClossestHit(ray);

	if (!hit.HasCollided)
		return scene->backgroundColor;

	Vector3 viewDir;
	Material *mat = hit.Mat;
	Vector3 color = mat->color * 0; // ambient color
	Vector3 difColor, specColor;
	Vector3 rColor = scene->backgroundColor;
	Vector3 lColor(0, 0, 0);
	Vector3 sum(0, 0, 0);
	Ray reflected_jit;
	float pdf = 0;
	float phong_lobe = 0;
	for (auto light : *scene->getLights()) {
		difColor = Vector3(0, 0, 0);
		specColor = Vector3(0, 0, 0);
		for (int i = 0; i < light->getSampleSize(); i++) {

			Vector3 lightDir = (light->getPoint() - hit.Location).normalize();

			if (isPointInShadow(hit, lightDir))
				continue; // this light doesn't contribute for this point

			float lambertian = std::fmax(lightDir * hit.Normal, 0.0f);
			float specular = 0;

			if (lambertian > 0.0f) {
				viewDir = (-ray.Dir).normalize();
				Vector3 Rr = 2 * (viewDir * hit.Normal)*hit.Normal - viewDir;
				float specAngle = std::fmax(Rr * lightDir, 0.0f);
				specular = pow(specAngle, mat->shininess);
				if (mat->glosiness > 0.0f) {

					reflected_jit = calculateGlossyReflectedRay(hit, -ray.Dir, phong_lobe, pdf);
					specular = pow(std::fmax(reflected_jit.Dir * lightDir, 0.0f), mat->shininess);
				}

				float KdLamb = mat->Kd * lambertian;
				difColor.r() += mat->color.r() * light->getColor().r() * KdLamb;
				difColor.g() += mat->color.g() * light->getColor().g() * KdLamb;
				difColor.b() += mat->color.b() * light->getColor().b() * KdLamb;
		
				float ksSpec = mat->Ks * specular;
				specColor.r() += mat->color.r() * light->getColor().r() * ksSpec;
				specColor.g() += mat->color.g() * light->getColor().g() * ksSpec;
				specColor.b() += mat->color.b() * light->getColor().b() * ksSpec;

			}
		}
		sum += (difColor + specColor) / light->getSampleSize();

	}

	color += sum;

	if (depth >= MAX_DEPTH)
		return color;


	Ray reflected;
	if (mat->glosiness <= 0)
		reflected = calculateReflectedRay(hit, -ray.Dir);
	else
		reflected = reflected_jit;

	rColor = rayTracing(reflected, depth + 1, RefrIndex);
	color += mat->Ks*rColor;
//mat->Ks*rColor;
							//translucid
							//ray = calculate ray in refracted direction;
	if (mat->isTranslucid) {
		Ray refracted = calculateRefractedRay(hit, ray, mat, RefrIndex);
		if (refracted.Dir != Vector3(0, 0, 0)) {
			Vector3 refrColor = rayTracing(refracted, depth + 1, RefrIndex);
			color += mat->T * refrColor;
		}

	}

	return color;

}


/////////////////////////////////////////////////////////////////////// ERRORS

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
	if(isOpenGLError()) {
		std::cerr << error << std::endl;
		std::cin.get();
		exit(EXIT_FAILURE);
	}
}
/////////////////////////////////////////////////////////////////////// SHADERs
const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec2 in_Position;\n"
	"in vec3 in_Color;\n"
	"uniform mat4 Matrix;\n"
	"out vec4 color;\n"

	"void main(void)\n"
	"{\n"
	"	vec4 position = vec4(in_Position, 0.0, 1.0);\n"
	"	color = vec4(in_Color, 1.0);\n"
	"	gl_Position = Matrix * position;\n"

	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = color;\n"
	"}\n"
};

void createShaderProgram()
{
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");
	glBindAttribLocation(ProgramId, COLOR_ATTRIB, "in_Color");
	
	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}
/////////////////////////////////////////////////////////////////////// VAOs & VBOs
void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	glGenBuffers(2, VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

	/* N�o � necess�rio fazer glBufferData, ou seja o envio dos pontos para a placa gr�fica pois isso 
	� feito na drawPoints em tempo de execu��o com GL_DYNAMIC_DRAW */

	glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(VERTEX_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glEnableVertexAttribArray(COLOR_ATTRIB);
	glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
	
// unbind the VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	checkOpenGLError("HELP");
	glDisableVertexAttribArray(COLOR_ATTRIB);
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(COLOR_ATTRIB);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, VboId);
	glDeleteVertexArrays(1, &VaoId);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

void drawPoints()
{
	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, size_colors, colors, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(UniformId, 1, GL_FALSE, m);
	glDrawArrays(GL_POINTS, 0, RES_X*RES_Y);
	glFinish();

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

// Render function by primary ray casting from the eye towards the scene's objects

void renderScene()
{	

	clock_t start = clock();
	Camera* cam = scene->getCamera();
	int index_pos = 0;
	int index_col = 0;
	int sampleSize = SAMPLE_SIZE;

	for (int y = 0; y < RES_Y; y++)	{
		for (int x = 0; x < RES_X; x++)	{	
			Vector3 c = Vector3(0, 0, 0);

			if (!cam->isAAenabled() && !cam->isDOFenabled())
				sampleSize = 1;
			
			for (int n = 0; n < sampleSize; n++) {
				Ray ray = cam->calculatePrimaryRay(x, y);
				c += rayTracing(ray, 1, 1.0);
			}

			Vector3 color = c / (sampleSize);
			vertices[index_pos++] = (float)x;
			vertices[index_pos++] = (float)y;
			colors[index_col++] = color.r();
			colors[index_col++] = color.g();
			colors[index_col++] = color.b();

			if (draw_mode == 0) {  // desenhar o conte�do da janela ponto a ponto
				drawPoints();
				index_pos = 0;
				index_col = 0;
			}
		}
		printf("line %d", y);
		if(draw_mode == 1) {  // desenhar o conte�do da janela linha a linha
				drawPoints();
				index_pos=0;
				index_col=0;
		}
	}

	if(draw_mode == 2) //preenchar o conte�do da janela com uma imagem completa
		 drawPoints();

	clock_t finish = clock() - start;
	printf("Terminou!\n");
	std::cout << "Running time: " << finish/1000 << "seconds" << std::endl;
}

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void ortho(float left, float right, float bottom, float top, 
			float nearp, float farp)
{
	m[0 * 4 + 0] = 2 / (right - left);
	m[0 * 4 + 1] = 0.0;
	m[0 * 4 + 2] = 0.0;
	m[0 * 4 + 3] = 0.0;
	m[1 * 4 + 0] = 0.0;
	m[1 * 4 + 1] = 2 / (top - bottom);
	m[1 * 4 + 2] = 0.0;
	m[1 * 4 + 3] = 0.0;
	m[2 * 4 + 0] = 0.0;
	m[2 * 4 + 1] = 0.0;
	m[2 * 4 + 2] = -2 / (farp - nearp);
	m[2 * 4 + 3] = 0.0;
	m[3 * 4 + 0] = -(right + left) / (right - left);
	m[3 * 4 + 1] = -(top + bottom) / (top - bottom);
	m[3 * 4 + 2] = -(farp + nearp) / (farp - nearp);
	m[3 * 4 + 3] = 1.0;
}

void reshape(int w, int h)
{
    glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	ortho(0, (float)RES_X, 0, (float)RES_Y, -1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'd':
		scene->getCamera()->toggleDOF();
		std::cout << "Depth of Field: " << (scene->getCamera()->isDOFenabled() ? "Enabled" : "Disabled") << std::endl;
		break;
	case 'a':
		scene->getCamera()->toggleAA();
		std::cout << "Anti-Aliasing: " << (scene->getCamera()->isAAenabled() ? "Enabled" : "Disabled") << std::endl;
		break;
	case 'g':
		scene->toggleGrid();
		std::cout << "Grid: " << (scene->isGridEnabled() ? "Enabled" : "Disabled") << std::endl;
		break;
	default:
		break;
	}
	renderScene();
	
}
/////////////////////////////////////////////////////////////////////// SETUP
void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	GLenum err_code = glGetError();
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowPosition(640,100);
	glutInitWindowSize(RES_X, RES_Y);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glDisable(GL_DEPTH_TEST);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
    //INSERT HERE YOUR CODE FOR PARSING NFF FILES
	scene = new Scene();
	if (!(scene->loadNFF("random_balls.nff"))) {
		std::cout << "Failed to load scene" << std::endl;
		std::cin.get();
		return 0;
	}

	RES_X = scene->getCamera()->getResX();
	RES_Y = scene->getCamera()->getResY();
	scene->getCamera()->computeParams();
	scene->setupGrid();
	//scene->disableGrid();
	//scene->getCamera()->toggleDOF();
	//scene->getCamera()->toggleAA();

	std::cout << "Grid: " << (scene->isGridEnabled() ? "Enabled" : "Disabled") << std::endl;
	std::cout << "Anti-Aliasing: " << (scene->getCamera()->isAAenabled() ? "Enabled" : "Disabled") << std::endl;
	std::cout << "Depth of Field: " << (scene->getCamera()->isDOFenabled	() ? "Enabled" : "Disabled") << std::endl;


	if(draw_mode == 0) { // desenhar o conte�do da janela ponto a ponto
		size_vertices = 2*sizeof(float);
		size_colors = 3*sizeof(float);
		printf("DRAWING MODE: POINT BY POINT\n");
	}
	else if(draw_mode == 1) { // desenhar o conte�do da janela linha a linha
		size_vertices = 2*RES_X*sizeof(float);
		size_colors = 3*RES_X*sizeof(float);
		printf("DRAWING MODE: LINE BY LINE\n");
	}
	else if(draw_mode == 2) { // preencher o conte�do da janela com uma imagem completa
		size_vertices = 2*RES_X*RES_Y*sizeof(float);
		size_colors = 3*RES_X*RES_Y*sizeof(float);
		printf("DRAWING MODE: FULL IMAGE\n");
	}
	else {
		printf("Draw mode not valid \n");
		exit(0);
	}
	printf("resx = %d  resy= %d.\n", RES_X, RES_Y);

	vertices = (float*)malloc(size_vertices);
    if (vertices==NULL) exit (1);

	colors = (float*)malloc(size_colors);
    if (colors==NULL) exit (1);

	init(argc, argv);
	glutMainLoop();	
	exit(EXIT_SUCCESS);
}
///////////////////////////////////////////////////////////////////////