
//
// Texture and lighting examples.  Edit the init method to try different
// images. The shader fshader_phong_tex includes several
// options for how to use the sampled texture value, see the comments
// and edit to try different things.  
//
// There are also a couple of other shaders to try, fshader_phong_tex_stripes
// is a procedural texture that makes stripes. fshader_toon is 
// an example of another effect, though unrelated to texture coordinates.
// 


#include <Angel.h>
#include <sphere.h>

#include <images.h>
#include <Camera.h>
#include <Shader.h>
#include <VertexArray.h>
#include <CameraControl.h>
#include <ObjFile.h>
#include <Texture2D.h>

char* objFileName = "../models/teapot.obj";

Texture2D * texture;

Camera * camera;
CameraControl * cameraControl;
Shader* vertexColorShader;
Shader* phongShader;
Shader* texShader;

VertexArray* modelVao;
VertexArray* axesVao;

// current amount of rotation
GLfloat degrees = 0.0;

// degree change in each frame
GLfloat increment = 0.5;

// uses space bar to pause
bool paused = 1;

// transformation of cube into position that is currently being rotated
mat4 currentOrientation;

// current axis of rotation
enum {XAxis, YAxis, ZAxis};
int currentAxis = XAxis;

// elapsed time
int elapsedTime;

// frame rate in millis for 30 frames/sec
const int frameRate = 1000.0 / 30;

// Some material and light properties to try

// Shiny green plastic?
//mat3 material = mat3(
//  vec3(0.0, 0.3, 0.3),  // blue-green in ambient light
//  vec3(0.0, 0.8, 0.0),  // green surface
//  vec3(0.8, 0.8, 0.8)); // specular highlights reflect light color
//GLfloat shininess = 30.0;

// Brass-like material
mat3 material = mat3(
  vec3(0.33, 0.22, 0.03),  // 
  vec3(0.78, 0.57, 0.11),  // 
  vec3(0.99, 0.91, 0.81)); // 
GLfloat shininess = 28.0;

//mat3 material = mat3(
//  vec3(0.33, 0.22, 0.50),  // 
//  vec3(0.18, 0.07, 0.51),  // 
//  vec3(0.09, 0.91, 0.81)); // 
//GLfloat shininess = 28.0;

// White light
mat3 light = mat3(
  vec3(0.4, 0.4, 0.4),
  vec3(1.0, 1.0, 1.0),
  vec3(1.0, 1.0, 1.0));


// nonmoving set of axes
vec3 axes[6] = {
  vec3(0.0, 0.0, 0.0), vec3(0.9, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0), vec3(0.0, 0.9, 0.0), 
  vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.9)};

// Axes colored R, G, B
vec4 colorsForAxes[6] = {
  vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), 
  vec4(0.0, 1.0, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 1.0), 
  vec4(0.0, 0.0, 1.0, 1.0), vec4(0.0, 0.0, 1.0, 1.0) };

void init()
{
  // Load texture from an image file (using default sampling parameters)
  texture = new Texture2D("../images/stone.tga");
  //texture = new Texture2D("../images/brick.png");
  //texture = new Texture2D("../images/clover.jpg");

  // Try this one with alpha blending (transparent background)
  //texture = new Texture2D("../images/steve.png");
  
  //texture = new Texture2D("../images/tarnish.tga");


  // Alternatively, a texture can be loaded from a byte stream.
  // Here we get the checkerboard texture (from images.h) and load it
  // using default sampling parameters
  //int nWidth, nHeight, nComponents;
  //GLenum eFormat;
  //GLubyte *imageBytes = checkerboard(&nWidth, &nHeight, &nComponents, &eFormat);
  //texture = new Texture2D(imageBytes, GL_RGB, nWidth, nHeight);
  //delete[] imageBytes;

  vertexColorShader  = new Shader("vshader.glsl", "fshader.glsl");
  phongShader      = new Shader("vshader_phong.glsl", "fshader_phong_tex.glsl");
  //phongShader      = new Shader("vshader_phong.glsl", "fshader_phong_tex_stripes.glsl");
  //phongShader      = new Shader("vshader_phong.glsl", "fshader_toon.glsl");


  // Load model from obj file
  ObjFile m(objFileName);

  //Cube m;
  //Sphere m;
  //Teapot m;
  modelVao = new VertexArray();
  modelVao->AddAttribute("vPosition", m.GetVertices(), m.GetNumVertices());
  modelVao->AddAttribute("vTexCoord", m.GetTexCoords(), m.GetNumVertices());
  modelVao->AddAttribute("vNormal", m.GetNormals(), m.GetNumVertices());
  modelVao->AddIndices((const unsigned int*) m.GetIndices(), m.GetNumIndices());

  currentOrientation = Scale(m.GetScaleFactor()); // initial scale for model

  axesVao = new VertexArray();
  axesVao->AddAttribute("vPosition", axes, 6);
  axesVao->AddAttribute("vColor", colorsForAxes, 6);

  camera = new Camera (vec3(0.0, 0.0, 4.0),   // position
              vec3(0.0, 0.0, -1.0),  // forward
              vec3(0.0, 1.0, 0.0),   // up
              1.0f,                  // aspect
              30.0f,                 // fovy
              0.1f,                  // near
              1000.0f);              // far

  cameraControl = new CameraControl(camera);


  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.0, 0.0, 0.0, 1.0 ); 

}

mat4 getCurrentRotation()
{
  // rotate around the current axis
  mat4 rotation;
  if (currentAxis == XAxis) rotation = RotateX(degrees);
  else if (currentAxis == YAxis) rotation = RotateY(degrees);
  else rotation = RotateZ(degrees);
  return rotation;
}

void display( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // increase the rotation angle
  if (!paused)
  {
    degrees += increment;
    while (degrees >= 360.0) degrees -= 360.0;
    while (degrees <= -360.0) degrees += 360.0;
  }

  // apply the rotation to the current orientation of the cube
  mat4 model = getCurrentRotation() * currentOrientation;

  mat4 view = camera->GetView();
  mat4 projection = camera->GetProjection();

  // Create normal matrix
  mat4 mv = view * model;
  mat3 normalMatrix = mat3(vec3(mv[0][0], mv[0][1], mv[0][2]),
                           vec3(mv[1][0], mv[1][1], mv[1][2]),
                           vec3(mv[2][0], mv[2][1], mv[2][2]));

  // Bind texture to a texture unit
  texture->Bind(1);

  // Bind shader and set uniforms
  phongShader->Bind();
  phongShader->SetUniform("texture", texture->GetTextureUnit());
  phongShader->SetUniform("normalMatrix", normalMatrix);
  phongShader->SetUniform("lightPosition", vec4(0.0, 2.0, 0.0, 1.0));
  phongShader->SetUniform("model", model);
  phongShader->SetUniform("view", view);
  phongShader->SetUniform("projection", projection);
  phongShader->SetUniform("materialProperties", material);
  phongShader->SetUniform("lightProperties", light);
  phongShader->SetUniform("shininess", shininess);

  // Bind model VAO and draw.  The Draw() function will automatically 
  // use indices if they were added to the VAO
  modelVao->Bind(*phongShader);
  modelVao->Draw(GL_TRIANGLES);

  // Bind shader for axes and set uniforms
  vertexColorShader->Bind();
  vertexColorShader->SetUniform("transform", projection * view);

  // Bind VAO for axes and draw
  axesVao->Bind(*vertexColorShader);
  glLineWidth(2);
  axesVao->Draw(GL_LINES);

  vertexColorShader->Unbind();
  axesVao->Unbind();

  // instead of glFlush, display the back buffer now
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
			// when we change the axis of rotation, want to take its current
			// position and start rotating about the new axis, so we compose
			// the current rotation with the current orientation matrix
		case 'x':
			currentOrientation = getCurrentRotation() * currentOrientation;
			currentAxis = XAxis;
			degrees = 0;
			break;
		case 'y':
			currentOrientation = getCurrentRotation() * currentOrientation;
			currentAxis = YAxis;
			degrees = 0;
			break;
		case 'z':
			currentOrientation = getCurrentRotation() * currentOrientation;
			currentAxis = ZAxis;
			degrees = 0;
			break;		
		case '+':
			increment += 0.1;
			break;
		case '-':
			increment -= 0.1;
			break;
		case ' ':   // pause
			paused = 1 - paused;
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
  // Allow for models to be specified on the command line
  if (argc >= 2)
  {
      objFileName = argv[1];
  }
  glutInit( &argc, argv );

  // Set up the graphics context with double-buffering
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

  glutInitWindowSize( 512, 512 );
  glutCreateWindow( " " );

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keyboardSpecial);
  glutIdleFunc( idle );

  elapsedTime = glutGet(GLUT_ELAPSED_TIME);

  glutMainLoop();
  return 0;
}
