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

const float PI = atanf(1.0) * 4;

Shader * shader;
VertexArray * cubeVao;
VertexArray * axesVao;
VertexArray * lineVao;

GLfloat phi;     // head/yaw
GLfloat theta;   // pitch

// current view point
vec3 viewPoint(2.0, 1.0, 2.0);

// rotate angle
GLfloat alpha;

// degree change in each frame
GLfloat increment = 0.5;

// View volume size
GLfloat size = 1.0;

// elapsed time
int elapsedTime;

// frame rate in millis for 30 frames/sec
const int frameRate = 1000.0 / 60;


mat4 PerspectiveUsingFrustum(float fovy, float aspect, float zNear, float zFar)
{
	float top = tan(fovy * PI / 360.0) * zNear;
    float bottom = -top;
    float left = bottom * aspect;
    float right = top * aspect;
    return Frustum(left, right, bottom, top, zNear, zFar);
}

mat4 RotateAxis(float degrees, float phi, float theta)
{
	return RotateY(degrees) * RotateY(phi) * RotateX(theta);
}

mat4 RotateAxis(float degrees, vec3 direction)
{
	if (direction.x == 0 && direction.z == 0)
	{
		return NULL;
	}

	// Calculate phi
	float phi = atan2(direction.x, direction.z);
	if (phi < 0)
	{
		phi += PI;
	}

	// Convert to degrees
	phi = phi * (180 / PI);

	// Calculate theta
	float r = length(direction);
	float theta = acos(direction.y / r);

	// Convert to degrees
	theta = theta * (180 / PI);

	return RotateAxis(degrees, phi, theta);
}


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

  // Magenta line
  vec3 line[2] = { vec3(0.0, -1.0, 0.0), vec3(0.0, 1.0, 0.0) };
  vec4 lineColor[2] = { vec4(1.0, 0.0, 1.0, 1.0), vec4(1.0, 0.0, 1.0, 1.0) };

  axesVao = new VertexArray();
  axesVao->AddAttribute("vPosition", axes, 6);
  axesVao->AddAttribute("vColor", colorsForAxes, 6);

  lineVao = new VertexArray();
  lineVao->AddAttribute("vPosition", line, 2);
  lineVao->AddAttribute("vColor", lineColor, 2);


  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0, 1.0, 1.0, 1.0 ); 

}


void display( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // increase the rotation angle
  alpha += increment;

  // rotation around magenta line
  mat4 rotate = RotateAxis(alpha, phi, theta);

  // orbit in X-Z plane
  mat4 orbit = RotateY(alpha / 10.0) * Translate(3.0, 0.0, 0.0);

  // earth
  mat4 earth = orbit * rotate;

  // moon orbiting around earth
  mat4 moon = orbit * rotate * RotateY(alpha) * Translate(2.0, 0.0, 0.0) * Scale(0.2, 0.2, 0.2);
  
  mat4 view = LookAt(viewPoint, vec3(0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0));

  mat4 projection = Ortho(-size, size, -size, size, -size, size);

  // bind shader
  shader->Bind();

  // draw cube with MVP transformation
  shader->SetUniform("transform", projection * view * earth);
  cubeVao->Bind(*shader);
  cubeVao->Draw(GL_TRIANGLES);
  cubeVao->Unbind();

  // draw magenta line in same context as cube
  lineVao->Bind(*shader);
  lineVao->Draw(GL_LINES);
  lineVao->Unbind();

  // draw small cube with MVP transformation
  shader->SetUniform("transform", projection * view * moon);
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

  // Zoom
  case 'V':
	size -= 0.1;
	break;
  case 'v':
	size += 0.1;
	break;
  }
}

// Needed to get key events for arrow keys
void keyboardSpecial(int key, int x, int y)
{
  switch( key ) {
  case GLUT_KEY_UP:
    theta += 5;
    break;
  case GLUT_KEY_DOWN:
    theta -= 5;
    break;
  case GLUT_KEY_RIGHT:
    phi += 5;
    break;
  case GLUT_KEY_LEFT:
    phi -= 5;
    break;
  }
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
