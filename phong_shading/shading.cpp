//
// Demonstrates Gourad or Phong shading using the ADS (Phong) lighting model. 
//
// For Gouraud shading (lighting calculation is per-vertex), use shaders 
// "vshader_gouraud.glsl" and "fshader_gouraud.glsl".
//
// For Phong shading (lighting calculation is per-fragment) use shaders
// "vshader_phong.glsl" and "fshader_phong.glsl".
//
// Edit the init() function to try a cube, sphere, or teapot.  For sphere
// or teapot, a 'true' argument will use true normals, default is to 
// use the calculated polygon normals.
//
// Edit the 'material' and 'light' matrices to change surface and light 
// properties. Use 'e' and 'E' to increase/decrease the
// specular exponent.  Lighting calculation can be done using (R dot V)
// or using the "half" vector (N dot H, the Blinn-Phong version of the 
// lighting model).  Use the 'h' key to toggle between them; this will
// change the effect of the specular exponent.
//
// Model can spin about any of the coordinate axes.  Use 'x', 'y', or 
// 'z' keys to change the axis. New rotation will be composed with whatever is
// the existing orientation. The '+' and '-' keys can be
// used to speed up and slow down the rotation.  Use spacebar to pause
// rotation.
//
// The view transformation is set using the LookAt
// function and is always oriented towards the origin. The view point is initially 
// at (1, 1, 3) and can be moved within the plane z = 3
// using the arrow keys.  By default a perspective projection is used.
// 

#include <Angel.h>
#include <cube.h>
#include <sphere.h>
#include <teapot.h>
#include <Shader.h>
#include <VertexArray.h>

Shader * solidColorShader;
Shader * lightShader;
VertexArray * cubeVao;
VertexArray * axesVao;

// current view point
vec3 viewPoint(1.0, 1.0, 3.0);

// current amount of rotation
GLfloat degrees = 0.0;

// degree change in each frame
GLfloat increment = 0.5;

// uses space bar to pause
bool paused = 0;

// transformation of cube into position that is currently being rotated
mat4 currentOrientation;

// current axis of rotation
enum {XAxis, YAxis, ZAxis};
int currentAxis = XAxis;

// elapsed time
int elapsedTime;

// frame rate in millis for 30 frames/sec
const int frameRate = 1000.0 / 60;

// use the half-vector version of the specular calculation,
// to compare effects of the exponent
bool useHalfVector = 0;

// Some surface properties and light properties to try.  Rows are
// ambient, diffuse, specular.  Columns are red, green, blue.

// Dull clay-like material
//mat3 material = mat3(
//  vec3(0.75, 0.38, 0.26),  // 
//  vec3(0.75, 0.38, 0.26),  // terracotta color
//  vec3(0.25, 0.20, 0.15) ); // specular highlight similar to diffuse color
//GLfloat shininess = 10.0;

// Shiny green plastic?
mat3 material = mat3(
  vec3(0.0, 0.3, 0.3),  // blue-green in ambient light
  vec3(0.0, 0.8, 0.0),  // green surface
  vec3(0.8, 0.8, 0.8)); // specular highlights reflect light color
GLfloat shininess = 30.0;

// Brass-like material
//mat3 material = mat3(
//  vec3(0.33, 0.22, 0.03),  // 
//  vec3(0.78, 0.57, 0.11),  // 
//  vec3(0.99, 0.91, 0.81)); // 
//GLfloat shininess = 28.0;

// White light
mat3 light = mat3(
  vec3(0.2, 0.2, 0.2),
  vec3(1.0, 1.0, 1.0),
  vec3(1.0, 1.0, 1.0));

vec4 lightPosition = vec4(0.0, 2.0, 0.0, 1.0);

void init()
{  

  solidColorShader  = new Shader("vshader.glsl", "fshader.glsl");
  //lightShader  = new Shader("vshader_gouraud.glsl", "fshader_gouraud.glsl");
  //lightShader  = new Shader("vshader_phong.glsl", "fshader_phong.glsl");
  lightShader  = new Shader("vshader_phong.glsl", "fshader_phong_spotlight.glsl");

  // Choose a model...
  //Cube m;
  //Sphere m; currentOrientation = Scale(0.75);
  Sphere m(6, true); currentOrientation = Scale(0.75);
  //Teapot m(true); currentOrientation = Scale(0.02);

  cubeVao = new VertexArray();
  cubeVao->AddAttribute("vPosition", m.GetVertices(), m.GetNumVertices());
  cubeVao->AddAttribute("vNormal", m.GetNormals(), m.GetNumVertices());

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

  axesVao = new VertexArray();
  axesVao->AddAttribute("vPosition", axes, 6);
  axesVao->AddAttribute("vColor", colorsForAxes, 6);

  glEnable( GL_DEPTH_TEST );
  glLineWidth(3);
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

  mat4 view = LookAt(viewPoint, vec3(0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0));
  
  // We need to transform the normal vectors into eye space along with the cube.  
  // Since we aren't doing any shearing or nonuniform scaling in this case,
  // we can just use the same model-view as for the cube itself 
  // (upper-left 3x3 submatrix of the model-view matrix).  This assumes
  // that the lighting calculation will be done in eye space.
  mat4 mv = view * model;
  mat3 normalMatrix = mat3(vec3(mv[0][0], mv[0][1], mv[0][2]),
                           vec3(mv[1][0], mv[1][1], mv[1][2]),
                           vec3(mv[2][0], mv[2][1], mv[2][2]));

  // distance from view point to origin
  GLfloat r = length(viewPoint);

  // perspective projection
  mat4 projection = Perspective(30, 1.0, r - 1, r + 1);

  // bind shader and set uniforms
  lightShader->Bind();
  lightShader->SetUniform("model", model);
  lightShader->SetUniform("view", view);
  lightShader->SetUniform("projection", projection);
  lightShader->SetUniform("normalMatrix", normalMatrix);
  lightShader->SetUniform("D", view * vec3(0.0, -1.0, 0.0));
  lightShader->SetUniform("lightPosition", lightPosition);
  lightShader->SetUniform("materialProperties", material);
  lightShader->SetUniform("lightProperties", light);
  lightShader->SetUniform("s", shininess);

  // draw cube 
  cubeVao->Bind(*lightShader);
  cubeVao->Draw(GL_TRIANGLES);
  cubeVao->Unbind();
  lightShader->Unbind();

  // draw fixed axes *without* model transformation
  solidColorShader->Bind();
  solidColorShader->SetUniform("transform", projection * view);
  axesVao->Bind(*solidColorShader);
  axesVao->Draw(GL_LINES);
  axesVao->Unbind();
  solidColorShader->Unbind();

  // instead of glFlush, display the back buffer now
  glutSwapBuffers();

}

void keyboard( unsigned char key, int x, int y )
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
  // pause
  case ' ':
    paused = 1 - paused;
    break;
  // speed up or slow down
  case '+':
    increment += 0.1;
    break;
  case '-':
    increment -= 0.1;
    break;
      case 'e':
    shininess += 1;
    std::cout << shininess << std::endl;
    break;
  case 'E':
    shininess -= 1;
    std::cout << shininess << std::endl;
    break;
  case 'h':
    useHalfVector = 1 - useHalfVector;
    std::cout << (useHalfVector ? "" : "not ") << "using half vector" << std::endl;
    break;
  case 'w':
	lightPosition = vec4(lightPosition.x, lightPosition.y + 0.2, lightPosition.z, lightPosition.w);
	break;
  case 's':
	lightPosition = vec4(lightPosition.x, lightPosition.y - 0.2, lightPosition.z, lightPosition.w);
	break;
  case 'a':
	lightPosition = vec4(lightPosition.x - 0.2, lightPosition.y, lightPosition.z, lightPosition.w);
	break;
  case 'd':
	lightPosition = vec4(lightPosition.x + 0.2, lightPosition.y, lightPosition.z, lightPosition.w);
	break;
  }
}

// Needed to get key events for arrow keys
void keyboardSpecial(int key, int x, int y)
{
  switch( key ) {
  case GLUT_KEY_UP:
    viewPoint[1] += 1;
    break;
  case GLUT_KEY_DOWN:
    viewPoint[1] -= 1;
    break;
  case GLUT_KEY_RIGHT:
    viewPoint[0] += 1;
    break;
  case GLUT_KEY_LEFT:
    viewPoint[0] -= 1;
    break;
  }
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
