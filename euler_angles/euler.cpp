//
// Similar to ortho.cpp but demonstrates Euler angles.  The model
// transformation is updated as follows:
//
//    Left and right arrows: head or yaw angle         (rotation about y axis)
//    Up and down arrows:    pitch angle               (rotation about x axis)
//    Comma and period keys ('<' and '>'): roll angle  (rotation about z-axis)
//
// We use the "intrinsic Y-X-Z" (head, pitch, roll) convention, equivalent to 
// "extrinsic Z-X-Y", i.e., 
//      first you turn your head (rotate about y axis)
//      then you pitch up or down (rotate intrinsically about x-axis)
//      then you roll your head left or right (rotate intrinsically about z-axis)
// 
// The view point is fixed in the first octant.
//
#include <Angel.h>
#include <Shader.h>
#include <VertexArray.h>

Shader * shader;
VertexArray * trianglesVao;
VertexArray * linesVao;
VertexArray * axesVao;

GLfloat phi;     // head/yaw
GLfloat theta;   // pitch
GLfloat psi;     // roll

vec3 viewPoint(2.0, 1.0, 2.0);

void init()
{
  // bunch of data

  // two triangles, yellow one at z = -1/2 and cyan one at z = 1/2
  vec3 vertices[6] = {vec3(.75, -.75, -0.5), vec3(0.0, .75, -0.5), vec3(-.75, -.75, -0.5), 
    vec3(.75, -.75, 0.5), vec3(0.0, .75, 0.5), vec3(-.75, -.75, 0.5) };

  vec4 colors[6] = {vec4(1.0, 1.0, 0.0, 1.0), vec4(1.0, 1.0, 0.0, 1.0), vec4(1.0, 1.0, 0.0, 1.0),
    vec4(0.0, 1.0, 1.0, 1.0), vec4(0.0, 1.0, 1.0, 1.0), vec4(0.0, 1.0, 1.0, 1.0) };

  // lines along the x, y, and z axes
  vec3 lines[6] = {
    vec3(-0.9, 0.0, 0.0), vec3(0.9, 0.0, 0.0),
    vec3(0.0, -0.9, 0.0), vec3(0.0, 0.9, 0.0), 
    vec3(0.0, 0.0, -0.9), vec3(0.0, 0.0, 0.9)};

  // lines colored black
  vec4 colorsForLines[6] = {
    vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), 
    vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), 
    vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0) };

  // nonmoving set of axes
  vec3 axes[6] = {
    vec3(0.0, 0.0, 0.0), vec3(0.6, 0.0, 0.0),
    vec3(0.0, 0.0, 0.0), vec3(0.0, 0.6, 0.0), 
    vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.6)};

  // Axes colored R, G, B
  vec4 colorsForAxes[6] = {
    vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), 
    vec4(0.0, 1.0, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 1.0), 
    vec4(0.0, 0.0, 1.0, 1.0), vec4(0.0, 0.0, 1.0, 1.0) };

  shader  = new Shader("vshader.glsl", "fshader.glsl");

  trianglesVao = new VertexArray();
  trianglesVao->AddAttribute("vPosition", vertices, 6);
  trianglesVao->AddAttribute("vColor", colors, 6);

  linesVao = new VertexArray();
  linesVao->AddAttribute("vPosition", lines, 6);
  linesVao->AddAttribute("vColor", colorsForLines, 6);

  axesVao = new VertexArray();
  axesVao->AddAttribute("vPosition", axes, 6);
  axesVao->AddAttribute("vColor", colorsForAxes, 6);


  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void display( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  std::cout << "roll " << psi << std::endl;
  std::cout << "pitch " << theta << std::endl;
  std::cout << "head " << phi << std::endl << std::endl;

  // Extrinsic rotations Z (roll), then X (pitch), then Y (head)
  mat4 model= RotateY(phi) * RotateX(theta) * RotateZ(psi);

  mat4 view = LookAt(viewPoint, vec3(0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0));

  // distance from view point to origin
  GLfloat r = length(viewPoint);

  // Translate everything by r and then flip the z-axis
  // to move everything into the default view volume
  mat4 projection = Scale(1, 1, -1) * Translate(0, 0, r);

  shader->Bind();

  // draw triangles
  shader->SetUniform("transform", projection * view * model);
  trianglesVao->Bind(*shader);
  trianglesVao->Draw(GL_TRIANGLES);
  trianglesVao->Unbind();
  linesVao->Bind(*shader);
  glLineWidth(1);
  linesVao->Draw(GL_LINES);
  linesVao->Unbind();

  // draw axes without model transformation
  shader->SetUniform("transform", projection * view);
  axesVao->Bind(*shader);
  glLineWidth(3);
  axesVao->Draw(GL_LINES);
  axesVao->Unbind();

  shader->Unbind();

  glFlush();
}

void keyboard( unsigned char key, int x, int y )
{
  switch( key ) {
  case 033: // Escape Key
  case 'q': case 'Q':
    exit( EXIT_SUCCESS );
    break; 
  case ',':  // '<' key unshifted
    psi -= 5;
    break;
  case '.':  // '>' key unshifted
    psi += 5;
    break;
  case 'c':
    psi = 0.0;
    phi = 0.0;
    theta = 0.0;
    break;
  }
  glutPostRedisplay();
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

  glutMainLoop();
  return 0;
}
