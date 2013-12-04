#include <Angel.h>
#include <sphere.h>
#include <images.h>
#include <Camera.h>
#include <Shader.h>
#include <VertexArray.h>
#include <CameraControl.h>
#include <ObjFile.h>
#include <TextureCube.h>
#include <Texture2D.h>
#include <FrameBuffer.h>
#include "Timer.h"

VertexArray* sphereVao;
VertexArray* quadVao;

Shader* lightShader;
Shader* texShader;
Shader* motionShader;
Shader* blurShader;

Camera* camera;
CameraControl* cameraControl;

CTimer timer;

FrameBuffer* sceneFbo;
FrameBuffer* velocityFbo;

// elapsed time
int elapsedTime;

// frame rate in millis for 30 frames/sec
const int frameRate = 1000.0 / 60;

const int bufferSize = 512;

int numVertices;

vec4 lightPosition = vec4(0.0, 3.0, 2.0, 1.0);
vec4 spherePosition = vec4(0.0, 0.0, 0.0, 1.0);

mat4 prevMvp;
mat4 currMvp;

// Asteroid material
mat3 material = mat3(
  vec3(0.8, 0.0, 0.0),     // redish in ambient light
  vec3(0.67, 0.6, 0.4),    // brown surface
  vec3(0.25, 0.20, 0.15)); // specular highlights similar to diffuse color

GLfloat shininess = 10.0;

// White light
mat3 light = mat3(
  vec3(0.2, 0.2, 0.2),
  vec3(1.0, 1.0, 1.0),
  vec3(1.0, 1.0, 1.0));

void initCamera()
{
	camera = new Camera(vec3(0.0, 0.0, 15),   // position
		vec3(0.0, 0.0, -1.0),  // forward
		vec3(0.0, 1.0, 0.0),   // up
		1.0f,                  // aspect
		30.0f,                 // fovy
		0.1f,                  // near
		1000.0f);              // far
	cameraControl = new CameraControl(camera, 0.3F);
}

void initShaders()
{
	lightShader   = new Shader("vshader_phong.glsl", "fshader_phong.glsl");
	texShader     = new Shader("vshader_tex.glsl", "fshader_tex.glsl");
	motionShader  = new Shader("vshader_motion.glsl", "fshader_motion.glsl");
	blurShader    = new Shader("vshader_blur.glsl", "fshader_blur.glsl");
}

void initModels()
{
	// VAO for sphere
	sphereVao = new VertexArray();
	Sphere m(16, true);
	sphereVao->AddAttribute("vPosition", m.GetVertices(), m.GetNumVertices());
	sphereVao->AddAttribute("vNormal", m.GetNormals(), m.GetNumVertices());

	// VAO for quad
	quadVao = new VertexArray();
	vec2 quad[6] = { vec2(-1.0, 1.0), vec2(1.0, 1.0), vec2(1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, -1.0), vec2(-1.0, 1.0) };
	vec2 texCoords[6] = { vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 0.0), vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0) };
	quadVao->AddAttribute("vPosition", quad, 6);
	quadVao->AddAttribute("texCoord", texCoords, 6);
}

void initFbos()
{
	sceneFbo = new FrameBuffer(
		true,        // create color texture
		true,        // create depth buffer
		bufferSize,  // width
		bufferSize   // height
	);

    velocityFbo = new FrameBuffer(
		true,        // create color texture
		true,        // create depth buffer
		bufferSize,  // width
		bufferSize   // height
	);
}

void init()
{
	initFbos();
	initCamera();
	initShaders();
	initModels();
	timer.Init();

	glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void drawSphere(Shader* shader, bool firstPass)
{
	mat4 view = camera->GetView();

	mat4 model = Translate(spherePosition);

    mat4 mv = view * model;
    mat3 normalMatrix = mat3(vec3(mv[0][0], mv[0][1], mv[0][2]),
		                     vec3(mv[1][0], mv[1][1], mv[1][2]),
                             vec3(mv[2][0], mv[2][1], mv[2][2]));

	currMvp = camera->GetProjection() * mv;

	shader->Bind();

	if (firstPass)
	{
		shader->SetUniform("model", model);
		shader->SetUniform("view", view);
		shader->SetUniform("projection", camera->GetProjection());
		shader->SetUniform("normalMatrix", normalMatrix);
		shader->SetUniform("lightPosition", lightPosition);
		shader->SetUniform("materialProperties", material);
		shader->SetUniform("lightProperties", light);
		shader->SetUniform("shininess", shininess);
		shader->SetUniform("useHalfVector", false);
	}
	else
	{
		//std::cout << currMvp << std::endl;
		//std::cout << prevMvp << std::endl;
		shader->SetUniform("mvp", currMvp);
		shader->SetUniform("prevMvp", prevMvp);
	}

	sphereVao->Bind(*shader);
    sphereVao->Draw(GL_TRIANGLES);
    sphereVao->Unbind();
	shader->Unbind();
}

void display( void )
{
	// Render to the scene FBO
	sceneFbo->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawSphere(lightShader, true);
	sceneFbo->Unbind();

	// Render to the velocity FBO
	velocityFbo->Bind();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawSphere(motionShader, false);

	// Render to the default framebuffer (unbinding restores default)
	velocityFbo->Unbind();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind the scene texture created in the FBO
	Texture2D& sceneTexture = sceneFbo->GetColorTexture();
	sceneTexture.Bind(1);

	// Bind the velocity texture created in the FBO
	Texture2D& velocityTexture = velocityFbo->GetColorTexture();
	velocityTexture.Bind(2);

	blurShader->Bind();

	// Tell the fragment shader which texture register to use. 
	blurShader->SetUniform("uTexInput", sceneTexture.GetTextureUnit());
	blurShader->SetUniform("uTexVelocity", velocityTexture.GetTextureUnit());
	blurShader->SetUniform("uVelocityScale", timer.GetFPS() / 30.f);

	// Draw
	quadVao->Bind(*blurShader);
	quadVao->Draw(GL_TRIANGLES);
	quadVao->Unbind();
	blurShader->Unbind();

	prevMvp = currMvp;

	glutSwapBuffers();
	timer.Update();
}

void keyboard( unsigned char key, int x, int y )
{
	if (!cameraControl->handleKey(key))
	{
		switch( key ) {
		case 033: // Escape Key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break; 
		case '[':
			spherePosition = vec4(spherePosition.x - 0.5, spherePosition.y, spherePosition.z, spherePosition.w);
			break;
		case ']':
			spherePosition = vec4(spherePosition.x + 0.5, spherePosition.y, spherePosition.z, spherePosition.w);
			break;
		case '-':
			spherePosition = vec4(spherePosition.x, spherePosition.y - 0.5, spherePosition.z, spherePosition.w);
			break;
		case '=':
			spherePosition = vec4(spherePosition.x, spherePosition.y + 0.5, spherePosition.z, spherePosition.w);
			break;
		}
	}
	glutPostRedisplay();
}

// Needed to get key events for arrow keys
void keyboardSpecial(int key, int x, int y)
{
	cameraControl->handleKeySpecial(key);
	glutPostRedisplay();

}

void idle( void )
{
  int now = glutGet(GLUT_ELAPSED_TIME);
  if (now - elapsedTime > frameRate)
  {
    elapsedTime = now;
    glutPostRedisplay();
  }
}

int main( int argc, char **argv )
{
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize( 512, 512 );
  glutCreateWindow( " " );

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keyboardSpecial);
  glutIdleFunc( idle );

  glutMainLoop();
  return 0;
}
