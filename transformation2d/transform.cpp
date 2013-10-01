//
// Displays a color triangle.
// Colors are assigned to each vertex and then the rasterizer interpolates
// those colors across the triangles.  
//
// This version uses separate buffer objects for the position and color attributes.
//


#include <Angel.h>

GLuint vao;
GLuint program;

// Sets up vertex attributes on the GPU
void init()
{
  // The corners of the triangle as 2D coordinates in clip space
  vec2 points[3]  = {
    vec2( 0.75, -0.75 ), vec2( 0.0, 0.75 ), vec2( -0.75, -0.75 )
  };

  // A color attribute for each corner (red, green, blue)
  vec4 colors[3]  = {
    vec4( 1.0, 0.0, 0.0, 1.0 ), vec4( 0.0, 1.0, 0.0, 1.0 ), vec4( 0.0, 0.0, 1.0, 1.0 )
  };

  // Load shaders
  program = InitShader( "vshader31.glsl", "fshader31.glsl" );

  // Create and initialize a buffer object for each attribute
  GLuint buffer[2];
  glGenBuffers( 2, &buffer[0] );

  glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

  glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER, 0);

  // Create a vertex array object
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // set up vertex attributes
  GLuint loc_vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( loc_vPosition );

  GLuint loc_vColor = glGetAttribLocation( program, "vColor" ); 
  glEnableVertexAttribArray( loc_vColor );

  glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
  glVertexAttribPointer( loc_vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
  glVertexAttribPointer( loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

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
  glutCreateWindow("Example 2");

  // This line must come after creating the OpenGL context (glutCreateWindow)
  // but before anything else
  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}
