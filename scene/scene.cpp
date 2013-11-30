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

VertexArray* skyboxVao;
VertexArray* asteroidVao;
VertexArray* planetVao;
VertexArray* starcruiserVao;

Texture2D* planetTexture;
Texture2D* moonTexture;

Shader* skyboxShader;
Shader* lightShader;
Shader* texShader;

Camera* camera;
CameraControl* cameraControl;

TextureCube* skyboxTexture;

GLfloat alphaAsteroid;
GLfloat alphaPlanet;
GLfloat alphaMoon;

enum Axis{XAxis, YAxis, ZAxis};

// Degree change in each frame for asteroids
GLfloat incrementAsteroid = 0.2;

// Degree of change in each frame for planets
GLfloat incrementPlanet = 0.025;

// Degree of change in each frame for moons
GLfloat incrementMoon = 0.05;

// elapsed time
int elapsedTime;

// frame rate in millis for 30 frames/sec
const int frameRate = 1000.0 / 60;

int numVertices;

vec4 lightPosition = vec4(2.0, 1.0, 1.5, 1.0);

// Asteroid material
mat3 material = mat3(
  vec3(0.8, 0.0, 0.0),     // redish in ambient light
  vec3(0.67, 0.6, 0.4),    // brown surface
  vec3(0.25, 0.20, 0.15)); // specular highlights similar to diffuse color

GLfloat shininess = 10.0;

// Starcruiser material
mat3 cruiserMaterial = mat3(
  vec3(0.3, 0.3, 0.3),
  vec3(0.8, 0.8, 0.8),
  vec3(0.8, 0.8, 0.8));
GLfloat cruiserShininess = 30.0;

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

	planetTexture = new Texture2D("images/planet.tga");
	moonTexture   = new Texture2D("images/moon.tga");
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
	texShader    = new Shader("vshader_phong.glsl", "fshader_phong_tex.glsl");
}

void initSkybox()
{
	// VAO for skybox
	skyboxVao = new VertexArray();
	ObjFile m("models/cube_tex.obj");   
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

	// Vao for planet
	planetVao = new VertexArray();
	Sphere s(16, true);
	planetVao->AddAttribute("vPosition", s.GetVertices(), s.GetNumVertices());
	planetVao->AddAttribute("vTexCoord", s.GetTexCoords(), s.GetNumVertices());
	planetVao->AddAttribute("vNormal", s.GetNormals(), s.GetNumVertices());

	// Vao for starcruiser
	starcruiserVao = new VertexArray();
	ObjFile starcruiser("models/starcruiser.obj");
	starcruiserVao->AddAttribute("vPosition", starcruiser.GetVertices(), starcruiser.GetNumVertices());
	starcruiserVao->AddAttribute("vNormal", starcruiser.GetNormals(), starcruiser.GetNumVertices());
	starcruiserVao->AddIndices(starcruiser.GetIndices(), starcruiser.GetNumIndices());
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

void drawAsteroid(vec3 position, vec3 scale, Axis axis, float rotScale)
{
	mat4 view = camera->GetView();
	mat4 rotation;
	if (axis == XAxis) rotation = RotateX(alphaAsteroid + rotScale);
	else if (axis == YAxis) rotation = RotateY(alphaAsteroid + rotScale);
	else rotation = RotateZ(alphaAsteroid + rotScale);

	mat4 model = Scale(scale) * Translate(position) * rotation;

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

void drawPlanet()
{
	mat4 view = camera->GetView();
	mat4 rotation = Scale(1.0, 1.1, 1.0) * RotateY(alphaPlanet) * RotateX(90);

	mat4 model = rotation;

    mat4 mv = view * model;
    mat3 normalMatrix = mat3(vec3(mv[0][0], mv[0][1], mv[0][2]),
		                     vec3(mv[1][0], mv[1][1], mv[1][2]),
                             vec3(mv[2][0], mv[2][1], mv[2][2]));

	// Bind texture to a texture unit
    planetTexture->Bind(1);

	texShader->Bind();
	texShader->SetUniform("texture", planetTexture->GetTextureUnit());
    texShader->SetUniform("model",  model);
    texShader->SetUniform("view",  view);
    texShader->SetUniform("projection", camera->GetProjection());
	texShader->SetUniform("normalMatrix", normalMatrix);
	texShader->SetUniform("lightPosition", lightPosition);
	texShader->SetUniform("materialProperties", material);
	texShader->SetUniform("lightProperties", light);
	texShader->SetUniform("shininess", shininess);

    planetVao->Bind(*texShader);
    planetVao->Draw(GL_TRIANGLES);
    planetVao->Unbind();
    texShader->Unbind();
}

void drawMoon()
{
	mat4 view = camera->GetView();
	mat4 rotation = RotateY(alphaMoon) * RotateX(90);

	mat4 model = rotation * Translate(2.0, -2.0, -1.0) * Scale(0.3, 0.3, 0.3);

    mat4 mv = view * model;
    mat3 normalMatrix = mat3(vec3(mv[0][0], mv[0][1], mv[0][2]),
		                     vec3(mv[1][0], mv[1][1], mv[1][2]),
                             vec3(mv[2][0], mv[2][1], mv[2][2]));

	// Bind texture to a texture unit
    moonTexture->Bind(1);

	texShader->Bind();
	texShader->SetUniform("texture", moonTexture->GetTextureUnit());
    texShader->SetUniform("model",  model);
    texShader->SetUniform("view",  view);
    texShader->SetUniform("projection", camera->GetProjection());
	texShader->SetUniform("normalMatrix", normalMatrix);
	texShader->SetUniform("lightPosition", lightPosition);
	texShader->SetUniform("materialProperties", material);
	texShader->SetUniform("lightProperties", light);
	texShader->SetUniform("shininess", shininess);

    planetVao->Bind(*texShader);
    planetVao->Draw(GL_TRIANGLES);
    planetVao->Unbind();
    texShader->Unbind();
}

void drawStarcruiser(vec3 position, vec3 scale)
{
	mat4 view = camera->GetView();
	mat4 rotation = RotateX(alphaMoon) * RotateZ(15.0);

	mat4 model = Scale(scale) * Translate(position) * rotation;

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
	lightShader->SetUniform("materialProperties", cruiserMaterial);
	lightShader->SetUniform("lightProperties", light);
	lightShader->SetUniform("shininess", cruiserShininess);
	lightShader->SetUniform("useHalfVector", false);

    starcruiserVao->Bind(*lightShader);
	starcruiserVao->Draw(GL_TRIANGLES);
	starcruiserVao->Unbind();
    lightShader->Unbind();
}

void drawModels()
{
    alphaAsteroid += incrementAsteroid;
    while (alphaAsteroid >= 360.0) alphaAsteroid -= 360.0;
    while (alphaAsteroid <= -360.0) alphaAsteroid += 360.0;

	alphaPlanet += incrementPlanet;
    while (alphaPlanet >= 360.0) alphaPlanet -= 360.0;
    while (alphaPlanet <= -360.0) alphaPlanet += 360.0;

	alphaMoon += incrementMoon;
    while (alphaMoon >= 360.0) alphaMoon -= 360.0;
    while (alphaMoon <= -360.0) alphaMoon += 360.0;

	drawPlanet();
	drawMoon();
	drawStarcruiser(vec3(-5.0, 0.0, 50.0), vec3(0.03, 0.03, 0.03));
	drawAsteroid(vec3(4.5, -7.0, 15.0), vec3(0.1, 0.1, 0.1), XAxis, 0.0);
	drawAsteroid(vec3(-15.5, 10.0, -40.0), vec3(0.05, 0.05, 0.05), ZAxis, 0.2);
	drawAsteroid(vec3(50.0, -12.5, 11.0), vec3(0.03, 0.05, 0.03), YAxis, 0.1);
	drawAsteroid(vec3(-5.5, 9.0, 7.5), vec3(0.15, 0.15, 0.15), ZAxis, 0.4);
}

void drawScene()
{
	drawSkybox();
	drawModels();
}

const int n = 5;
int i = 0;
bool blur;

void postProcessBlur()
{
	if (i == 0)
	{
		glAccum(GL_LOAD, 1.0 / n);
	}
	else
	{
		glAccum(GL_ACCUM, 1.0 / n);
	}

	i++;
	if (i >= n)
	{
		i = 0;
        glAccum(GL_RETURN, 1.0);
        glFlush();
	}
}

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    drawScene();

	if (blur)
	{
		postProcessBlur();
	}
	else
	{
        glFlush();
	}
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
		case 'b':
			blur = !blur;
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
