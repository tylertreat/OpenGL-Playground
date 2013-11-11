//
// Same as 'skybox' project but uses TextureCube from the rendering utilities.
//
// Simple demo of rendering a cube map onto the 6 inside faces of a model
// to create a "skybox" or a projective texture.  Adapted from the OpenGL Superbible 5e (which
// is also where the landscape images came from). No lighting.
//
// Camera controls are standard.
//

#include <Angel.h>
#include <sphere.h>
#include <cube.h>
#include <images.h>
#include <Camera.h>
#include <Shader.h>
#include <VertexArray.h>
#include <CameraControl.h>
#include <ObjFile.h>
#include <TextureCube.h>

VertexArray* skyboxVao;
VertexArray* asteroidVao;
Shader* skyboxShader;
Shader* lightShader;
Camera* camera;
CameraControl* cameraControl;
TextureCube* skyboxTexture;
GLfloat alpha;

// degree change in each frame
GLfloat increment = 0.5;

// elapsed time
int elapsedTime;

// frame rate in millis for 30 frames/sec
const int frameRate = 1000.0 / 60;

int numVertices;

vec4 lightPosition = vec4(-2.0, 1.0, -1.5, 1.0);

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

void initTextures()
{
	// Constructor sets up cube map with default sampling paramaters
	skyboxTexture = new TextureCube(
		"images/pos_x.tga",
        "images/neg_x.tga",
        "images/pos_y.tga",
        "images/neg_y.tga",
        "images/pos_z.tga",
        "images/neg_z.tga");
}

void initCamera()
{
	camera = new Camera(vec3(0.0, 0.0, 5),   // position
		vec3(0.0, 0.0, -1.0),  // forward
		vec3(0.0, 1.0, 0.0),   // up
		1.0f,                  // aspect
		30.0f,                 // fovy
		0.1f,                  // near
		1000.0f);              // far
	cameraControl = new CameraControl(camera);
}

void initShaders()
{
	skyboxShader = new Shader("vshader_cube_tex.glsl", "fshader_cube_tex.glsl");
	lightShader  = new Shader("vshader_phong.glsl", "fshader_phong.glsl");
}

void initSkybox()
{
	// VAO for skybox
	skyboxVao = new VertexArray();

	ObjFile m("../models/cube_tex.obj");   
    skyboxVao->AddAttribute("vPosition", m.GetVertices(), m.GetNumVertices());
    skyboxVao->AddIndices(m.GetIndices(), m.GetNumIndices());
}

void initModels()
{
	// VAO for asteroid
	asteroidVao = new VertexArray();

	ObjFile m("models/asteroid.obj");
	asteroidVao->AddAttribute("vPosition", m.GetVertices(), m.GetNumVertices());
	asteroidVao->AddAttribute("vNormal", m.GetNormals(), m.GetNumVertices());
	asteroidVao->AddIndices(m.GetIndices(), m.GetNumIndices());
}

void init()
{
	initTextures();
	initCamera();
	initShaders();
	initSkybox();
	initModels();

	glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

mat4 RotateAxis(float degrees, float phi, float theta)
{
	return RotateY(degrees) * RotateY(phi) * RotateX(theta);
}

void drawSkybox()
{
	mat4 model;

	// Bind texture to a texture unit
	skyboxTexture->Bind(1);

	skyboxShader->Bind();
    skyboxShader->SetUniform("textureCube", 0);
    skyboxShader->SetUniform("model",  Scale(20.0, 20.0, 20.0));
    skyboxShader->SetUniform("view",  camera->GetView());
    skyboxShader->SetUniform("projection", camera->GetProjection());
    skyboxShader->SetUniform("textureCube", skyboxTexture->GetTextureUnit());
    skyboxVao->Bind(*skyboxShader);
    skyboxVao->Draw(GL_TRIANGLES);

    skyboxVao->Unbind();
    skyboxShader->Unbind();
}

void drawAsteroid(vec3 position, vec3 scale)
{
	mat4 view = camera->GetView();
	mat4 model = Scale(scale) * Translate(position) * RotateAxis(alpha, 0.0, 0.0);

	// We need to transform the normal vectors into eye space along with the cube.  
    // Since we aren't doing any shearing or nonuniform scaling in this case,
    // we can just use the same model-view as for the cube itself 
    // (upper-left 3x3 submatrix of the model-view matrix).  This assumes
    // that the lighting calculation will be done in eye space.
    mat4 mv = view * model;
    mat3 normalMatrix = mat3(vec3(mv[0][0], mv[0][1], mv[0][2]),
		                     vec3(mv[1][0], mv[1][1], mv[1][2]),
                             vec3(mv[2][0], mv[2][1], mv[2][2]));

	lightShader->Bind();
    lightShader->SetUniform("model",  model);
    lightShader->SetUniform("view",  view);
    lightShader->SetUniform("projection", camera->GetProjection());
	lightShader->SetUniform("normalMatrix", normalMatrix);
	lightShader->SetUniform("lightPosition", lightPosition);
	lightShader->SetUniform("materialProperties", material);
	lightShader->SetUniform("lightProperties", light);
	lightShader->SetUniform("shininess", shininess);
	lightShader->SetUniform("useHalfVector", false);

    asteroidVao->Bind(*lightShader);
    asteroidVao->Draw(GL_TRIANGLES);
    asteroidVao->Unbind();
    lightShader->Unbind();
}

void drawModels()
{
    // increase the rotation angle
    alpha += increment;
    while (alpha >= 360.0) alpha -= 360.0;
    while (alpha <= -360.0) alpha += 360.0;

	drawAsteroid(vec3(0.0, -1.0, 0.0), vec3(0.1, 0.1, 0.1));
	drawAsteroid(vec3(-15.5, 10.0, -40.0), vec3(0.05, 0.05, 0.05));
	drawAsteroid(vec3(50.0, -12.5, 11.0), vec3(0.03, 0.05, 0.03));
	drawAsteroid(vec3(-5.5, 9.0, 7.5), vec3(0.15, 0.15, 0.15));
}

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    drawSkybox();
	drawModels();
    glFlush();
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
