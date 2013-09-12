//
// Demonstrates drawing various primitives using OpenGL.  Press a key
// to cycle between GL_POINTS, GL_LINES, etc.
//
// Adapted from Example 1, chapter 2, of Angel
//
#include <Angel.h>

const int NumPoints = 8;
int count = 0;
GLuint vao;
GLuint program;

// Sets up the vertex data on the GPU
void init( void )
{
  // Some points, given as 2D coordinates in clip space.  This array is temporary
  // since the data will shortly be stored on the GPU
  vec2 points[NumPoints]  = {
    vec2( 0.75, 0.0 ), vec2( 0.5, 0.5 ), vec2( 0.0, 0.75 ), vec2(-0.5, 0.5),
    vec2( -0.75, 0.0 ), vec2( -0.5, -0.5 ), vec2( 0.0, -0.75 ), vec2(0.5, -0.5)
  };

  // Load shaders
  program = InitShader("vshader21.glsl", "fshader21.glsl");

  // Create a buffer
  GLuint buffer;
  glGenBuffers(1, &buffer);

  // Bind it to GL_ARRAY_BUFFER and pass the data to the GPU
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  // Create a vertex array object (VAO)
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Initialize the vertex vPosition attribute defined in the vertex shader

  // Get an index for the attribute
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);

  // Associate the attribute with the data in the buffer
  // This implicitly refers to the currently bound GL_ARRAY_BUFFER
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  // Unbind the buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind the VAO
  glBindVertexArray(0);

  glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
  glPointSize(4.0);  // make points show up better
}


void display( void )
{
  // clear the window
  glClear( GL_COLOR_BUFFER_BIT );     

  // bind the shaders
  glUseProgram(program);

  // bind the VAO
  glBindVertexArray( vao );

  // draw primitives
  switch (count % 6) {
  case 0: glDrawArrays( GL_POINTS, 0, NumPoints ); break; 
  case 1: glDrawArrays( GL_LINES, 0, NumPoints );  break;
  case 2: glDrawArrays( GL_LINE_STRIP, 0, NumPoints );  break;
  case 3: glDrawArrays( GL_TRIANGLES, 0, NumPoints );  break;
  case 4: glDrawArrays( GL_TRIANGLE_STRIP, 0, NumPoints );  break;
  case 5: glDrawArrays( GL_TRIANGLE_FAN, 0, NumPoints );  break;
  }
  glFlush();

  // unbind
  glUseProgram(0);
  glBindVertexArray(0);
}

void keyboard( unsigned char key, int x, int y )
{
  count++;
  switch (key) {
  case 033:  // Escape key
    exit(EXIT_SUCCESS);
    break;
  }
  glutPostRedisplay();
}


int main( int argc, char **argv )
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Example 1");

  // This line must come after creating the OpenGL context (glutCreateWindow)
  // but before anything else
  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}
