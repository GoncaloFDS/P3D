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
#define CAPTION "ray tracer"

#define VERTEX_COORD_ATTRIB 0
#define COLOR_ATTRIB 1

#define MAX_DEPTH 4

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

/* Draw Mode: 0 - point by point; 1 - line by line; 2 - full frame */
int draw_mode=1; 

int WindowHandle = 0;

bool isPointInShadow(Hit &hit, Vector3 lightDir) {
	Ray shadowFeeler(hit.Location, lightDir);
	
	for (auto obj : scene->getObjects()) {
		Hit shadowHit = obj->CalculateIntersection(shadowFeeler);

		if (shadowHit.HasCollided) 
			if ((lightDir - hit.Location).quadrance() > shadowHit.T * shadowHit.T) //ignore objects behind the light
				return true;
		
	}
	return false;
}
double clamp(double x, double upper, double lower){
	return std::min(upper, std::max(x, lower));
}

Hit calculateClossestHit(Ray ray){
	double Tmin = DBL_MAX;
	Hit hit;
	for (auto obj : scene->getObjects()) {
		Hit tempHit = obj->CalculateIntersection(ray);
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

Ray calculateRefractedRay(Hit hit, Ray ray, Material mat, float RefractionIndex) {
	Vector3 Nrefr = hit.Normal.normalize();
	Vector3 I = (hit.Location - ray.Origin).normalize();
	//Vector3 I = -ray.Direction;
	float NdotI = Nrefr * I;
	NdotI = clamp(NdotI, 1, -1);
	float etai = RefractionIndex, etat = mat.refractionIndex;
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
	Hit hit = calculateClossestHit(ray);

	if (!hit.HasCollided) 
		return scene->backgroundColor;
	
	Vector3 viewDir;
	Material mat = hit.Mat;
	Vector3 color = mat.color * 0; // ambient color
	Vector3 difColor, specColor;
	Vector3 rColor = scene->backgroundColor;
	Vector3 lColor(0, 0, 0);

	for (auto light : scene->getLights()) {
		for (int i = 0; i < light->GetSampleSize(); i++) {
			
			Vector3 lightDir = (light->GetPoint() - hit.Location).normalize();

			if(isPointInShadow(hit, lightDir)) 
				continue; // this light doesn't contribute for this point

			float lambertian = std::fmax(lightDir * hit.Normal, 0.0f);
			float specular = 0;

			if (lambertian > 0.0f) {
				viewDir = (-ray.Direction).normalize();
				Vector3 Rr = 2 * (viewDir * hit.Normal)*hit.Normal - viewDir;
				float specAngle = std::fmax(Rr * lightDir, 0.0f);
				specular = pow(specAngle, mat.shininess);

				
				float KdLamb = mat.Kd * lambertian; 
				difColor.r() += mat.color.r() * light->GetColor().r() * KdLamb;
				difColor.g() += mat.color.g() * light->GetColor().g() * KdLamb;
				difColor.b() += mat.color.b() * light->GetColor().b() * KdLamb;

				float ksSpec = mat.Ks * specular;
				specColor.r() += mat.color.r() * light->GetColor().r() * ksSpec;
				specColor.g() += mat.color.g() * light->GetColor().g() * ksSpec;
				specColor.b() += mat.color.b() * light->GetColor().b() * ksSpec;

				

			}
			lColor = difColor + specColor;
			lColor = lColor / light->GetSampleSize();
		}
			
	}

	color += lColor;

	if (depth >= MAX_DEPTH) 
		return color;
		
 	Ray reflected = calculateReflectedRay(hit, -ray.Direction);
 	rColor = rayTracing(reflected, depth + 1, RefrIndex);
	color += mat.Ks*rColor;

	//translucid
	//ray = calculate ray in refracted direction;
	if(mat.isTranslucid){
 		Ray refracted = calculateRefractedRay(hit, ray, mat, RefrIndex);
		if (refracted.Direction != Vector3(0, 0, 0)) {
			Vector3 refrColor = rayTracing(refracted, depth + 1, RefrIndex);
			color += mat.T * refrColor;
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
	int index_pos=0;
	int index_col=0;
	int nSamples = 6;
	int nSquared = nSamples * nSamples;

	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++)
		{	
			Vector3 c = Vector3(0, 0, 0);
			for (int p = 0; p < nSamples; p++) {
				for (int q = 0; q < nSamples; q++) {
					float pEps = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float qEps = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					Ray ray = scene->getCamera()->CalculatePrimaryRay(x + (p + pEps)/nSamples, y + (q + qEps) / nSamples);
					c += rayTracing(ray, 1, 1.0);

					
				}		  
			}
			Vector3 color = c / nSquared;
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

	printf("Terminou!\n"); 	
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

/////////////////////////////////////////////////////////////////////// SETUP
void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
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
	if (!(scene->loadNFF("balls_low.nff"))) {
		std::cout << "Failed to load scene" << std::endl;
		std::cin.get();
		return 0;
	}
	RES_X = scene->getCamera()->GetResX();
	RES_Y = scene->getCamera()->GetResY();
	scene->getCamera()->CalculateOtherStuff();

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