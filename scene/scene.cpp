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
VertexArray* quadVao;

Texture2D* planetTexture;
Texture2D* moonTexture;

Shader* skyboxShader;
Shader* lightShader;
Shader* texLightShader;
Shader* texShader;

Camera* camera;
CameraControl* cameraControl;

TextureCube* skyboxTexture;

GLfloat alphaAsteroid;
GLfloat alphaPlanet;
GLfloat alphaMoon;

GLuint fbo;
mat4 prevMats[4][3];
int bufferSize = 512;

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

vec3 asteroidTranslation;

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
	skyboxShader    = new Shader("vshader_cube_tex.glsl", "fshader_cube_tex.glsl");
	lightShader     = new Shader("vshader_phong.glsl", "fshader_phong.glsl");
	texLightShader  = new Shader("vshader_phong.glsl", "fshader_phong_tex.glsl");
	texShader       = new Shader("vshader_tex.glsl", "fshader_tex.glsl");
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

	// Vao for quad
	quadVao = new VertexArray();
	vec2 rect[6] = { vec2(-1, 1), vec2(1, 1), vec2(1, -1), vec2(1, -1), vec2(-1, -1), vec2(-1, 1) };
	vec2 texCoords[6] = { vec2(0, 1), vec2(1, 1), vec2(1, 0), vec2(1, 0), vec2(0, 0), vec2(0, 1) };
	quadVao->AddAttribute("vPosition", rect, 6);
	quadVao->AddAttribute("texCoord", texCoords, 6);
}

void init()
{
	initTextures();
	initCamera();
	initShaders();
	initSkybox();
	initModels();

	// Create a framebuffer object
	glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  
    // Allocate memory for a depth buffer for rendering
    GLuint depthbuffer;
    glGenRenderbuffers(1, &depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, bufferSize, bufferSize);

	// Allocate texture memory
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bufferSize, bufferSize, 0, GL_RGBA, GL_FLOAT, NULL);

	// Attach the texture and depth buffer memory to the FBO
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

	  // Basic texture sampling parameters, implicitly using texture unit zero
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );  

    // Done with FBO setup
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

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

void drawAsteroid(vec3 position, vec3 scale, Axis axis, float rotScale, int i)
{
	mat4 view = camera->GetView();
	mat4 rotation;
	if (axis == XAxis) rotation = RotateX(alphaAsteroid + rotScale);
	else if (axis == YAxis) rotation = RotateY(alphaAsteroid + rotScale);
	else rotation = RotateZ(alphaAsteroid + rotScale);

	mat4 model = Scale(scale) * Translate(position) * rotation;
	model = model * Translate(asteroidTranslation);

    mat4 mv = view * model;
    mat3 normalMatrix = mat3(vec3(mv[0][0], mv[0][1], mv[0][2]),
		                     vec3(mv[1][0], mv[1][1], mv[1][2]),
                             vec3(mv[2][0], mv[2][1], mv[2][2]));

	prevMats[i][0] = model;
	prevMats[i][1] = view;
	prevMats[i][2] = camera->GetProjection();

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

	texLightShader->Bind();
	texLightShader->SetUniform("texture", planetTexture->GetTextureUnit());
    texLightShader->SetUniform("model",  model);
    texLightShader->SetUniform("view",  view);
    texLightShader->SetUniform("projection", camera->GetProjection());
	texLightShader->SetUniform("normalMatrix", normalMatrix);
	texLightShader->SetUniform("lightPosition", lightPosition);
	texLightShader->SetUniform("materialProperties", material);
	texLightShader->SetUniform("lightProperties", light);
	texLightShader->SetUniform("shininess", shininess);

    planetVao->Bind(*texLightShader);
    planetVao->Draw(GL_TRIANGLES);
    planetVao->Unbind();
    texLightShader->Unbind();
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

	texLightShader->Bind();
	texLightShader->SetUniform("texture", moonTexture->GetTextureUnit());
    texLightShader->SetUniform("model",  model);
    texLightShader->SetUniform("view",  view);
    texLightShader->SetUniform("projection", camera->GetProjection());
	texLightShader->SetUniform("normalMatrix", normalMatrix);
	texLightShader->SetUniform("lightPosition", lightPosition);
	texLightShader->SetUniform("materialProperties", material);
	texLightShader->SetUniform("lightProperties", light);
	texLightShader->SetUniform("shininess", shininess);

    planetVao->Bind(*texLightShader);
    planetVao->Draw(GL_TRIANGLES);
    planetVao->Unbind();
    texLightShader->Unbind();
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
	drawAsteroid(vec3(4.5, -7.0, 15.0), vec3(0.1, 0.1, 0.1), XAxis, 0.0, 0);
	drawAsteroid(vec3(-15.5, 10.0, -40.0), vec3(0.05, 0.05, 0.05), ZAxis, 0.2, 1);
	drawAsteroid(vec3(50.0, -12.5, 11.0), vec3(0.03, 0.05, 0.03), YAxis, 0.1, 2);
	drawAsteroid(vec3(-5.5, 9.0, 7.5), vec3(0.15, 0.15, 0.15), ZAxis, 0.4, 3);
}

void display( void )
{  
	// Render to the texture FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glClearColor( 0.5, 0.5, 0.5, 0.0 );  // transparent background 
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Set the viewport to the texture size
    glViewport(0, 0, bufferSize, bufferSize);

    drawSkybox();
	drawModels();

	// Render to the default framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Restore viewport to current window size
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, width, height);

	// Bind shader for model and set uniforms
    texShader->Bind();

    // Tell the fragment shader which texture register to use. 
    texShader->SetUniform("tex", 0); 

    // Set other uniforms and draw
    quadVao->Bind(*texShader);
    quadVao->Draw(GL_TRIANGLES);

    glutSwapBuffers();
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
			asteroidTranslation = vec3(asteroidTranslation.x - 0.5, asteroidTranslation.y, asteroidTranslation.z);
			break;
		case ']':
			asteroidTranslation = vec3(asteroidTranslation.x + 0.5, asteroidTranslation.y, asteroidTranslation.z);
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
