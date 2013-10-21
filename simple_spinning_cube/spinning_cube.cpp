//
// Cube that spins about the y-axis.  The '+' and '-' keys can be
// used to speed up and slow down the rotation.
//
// Try the alternate shader fshader_depth.glsl.  This is the one that 
// uses the depth value in the fragment shader to modulate a solid
// color.  The depth value is available in the shader as gl_FragCoord.z
// and by default has a range from 0.0 (vertices at near clipping plane)
// to 1.0 (vertices at far clipping plane).
// 
// This version uses an orthographic projection similar to ortho.cpp.
// The view transformation is set using the LookAt
// function and is always oriented towards the origin. The view point is initially 
// at (0, 0, 3) and can be moved within the plane z = 3
// using the arrow keys.
//
// Since this is an animation, we use double-buffering and a fixed frame rate of 30 frames/sec;
// note that the idle callback does nothing unless at least 1000 / 30 milliseconds have 
// elapsed since the previous display callback.  The display callback updates the
// animation state, which is really just the current angle of rotation.
//
#include <Angel.h>
#include <cube.h>
#include <Shader.h>
#include <VertexArray.h>

Shader * shader;
VertexArray * cubeVao;
VertexArray * axesVao;

// current view point
vec3 viewPoint(0.0, 0.0, 3.0);

// current amount of rotation
GLfloat degrees = 0.0;

// degree change in each frame
GLfloat increment = 0.5;

// elapsed time
int elapsedTime;

// frame rate in millis for 30 frames/sec
const int frameRate = 1000.0 / 60;



void init()
{  

  shader  = new Shader("vshader.glsl", "fshader.glsl");
  //shader  = new Shader("vshader.glsl", "fshader_depth.glsl");

  // data only needed in local scope
  Cube m;

  cubeVao = new VertexArray();
  cubeVao->AddAttribute("vPosition", m.GetVertices(), m.GetNumVertices());
  cubeVao->AddAttribute("vColor", m.GetFaceColors(), m.GetNumVertices());

  // nonmoving set of axes
  vec3 axes[6] = {
    vec3(0.0, 0.0, 0.0), vec3(0.9, 0.0, 0.0),
    vec3(0.0, 0.0, 0.0), vec3(0.0, 0.9, 0.0), 
    vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.9)};

  // Axes colored black
  vec4 colorsForAxes[6] = {
    vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), 
    vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), 
    vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0) };

  axesVao = new VertexArray();
  axesVao->AddAttribute("vPosition", axes, 6);
  axesVao->AddAttribute("vColor", colorsForAxes, 6);

  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0, 1.0, 1.0, 1.0 ); 

}


void display( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // increase the rotation angle
  degrees += increment;
  while (degrees >= 360.0) degrees -= 360.0;
  while (degrees <= -360.0) degrees += 360.0;

  mat4 model = RotateY(degrees);
  
  mat4 view = LookAt(viewPoint, vec3(0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0));

  // distance from view point to origin
  GLfloat r = length(viewPoint);

  // Translate everything by r and then flip the z-axis
  // to move everything into the default view volume
  // (see ortho.cpp for some alternative projection matrices...)
  mat4 projection = Scale(1, 1, -1) * Translate(0, 0, r);

  // bind shader
  shader->Bind();

  // draw cube with MVP transformation
  shader->SetUniform("transform", projection * view * model);
  cubeVao->Bind(*shader);
  cubeVao->Draw(GL_TRIANGLES);
  cubeVao->Unbind();

  // draw fixed axes *without* model transformation
  shader->SetUniform("transform", projection * view);
  axesVao->Bind(*shader);
  axesVao->Draw(GL_LINES);
  axesVao->Unbind();

  shader->Unbind();

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

  // speed up or slow down
  case '+':
    increment += 0.1;
    break;
  case '-':
    increment -= 0.1;
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
