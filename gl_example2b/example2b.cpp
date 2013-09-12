//
// Displays a color triangle.
// Colors are assigned to each vertex and then the rasterizer interpolates
// those colors across the triangles.  
//
// This version is the same as example2a, but the position and color
// data are interleaved in the VBO.
//


#include <Angel.h>

GLuint vao;
GLuint program;

// Sets up vertex attributes on the GPU
void init()
{
  // each vertex has 6 values, x and y coordinates followed by 4 values for color
  GLfloat all[18] = {
    0.75, -0.75, 1.0, 0.0, 0.0, 1.0,
    0.0, 0.75, 0.0, 1.0, 0.0, 1.0,
    -0.75, -0.75, 0.0, 0.0, 1.0, 1.0
  };

  // Load shaders
  program = InitShader( "vshader31.glsl", "fshader31.glsl" );

  // Create and initialize a buffer object that stores both the position data and
  // the color data
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(all), all, GL_STATIC_DRAW );

  // Create a vertex array object
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // set up vertex attributes
  GLuint loc_vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( loc_vPosition );

  GLuint loc_vColor = glGetAttribLocation( program, "vColor" ); 
  glEnableVertexAttribArray( loc_vColor );

  // the "stride" is 6 floats
  glVertexAttribPointer( loc_vPosition, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), BUFFER_OFFSET(0) );

  // color data starts after first pair of position values, stride is still 6 floats
  glVertexAttribPointer( loc_vColor, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), BUFFER_OFFSET(2 * sizeof(GLfloat)) );
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind the VAO
  glBindVertexArray(0);

  glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}


void display( void )
{
  // clear the window
  glClear( GL_COLOR_BUFFER_BIT );     

  // bind the shaders and VAO
  glUseProgram(program);
  glBindVertexArray( vao );

  // draw primitives
  glDrawArrays( GL_TRIANGLES, 0, 3 );
  glFlush();

  // unbind
  glUseProgram(0);
  glBindVertexArray(0);
}


void
  keyboard( unsigned char key, int x, int y )
{
  switch( key ) {
  case 033:  // Escape key
  case 'q': case 'Q':
    exit( EXIT_SUCCESS );
    break;
  }
}

int main( int argc, char **argv )
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Example 2b");

  // This line must come after creating the OpenGL context (glutCreateWindow)
  // but before anything else
  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}
