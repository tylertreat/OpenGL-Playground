//
// Similar to ortho.cpp but adds camera controls.  
// See keyboard() and keyboardSpecial() for details.
// Summary:
//   Basic motion: w, a, s, d, plus r and f for up/down
//   FPS-like rotations: i, j, k, l
//   Spacecraft-like rotations: I, J, K, L, <, >
//   Orbiting: arrow keys
//   Look at origin: o
//   Move to origin: O
//
#include <Angel.h>
#include "Camera.h"


GLuint vao;
GLuint program; 


Camera * camera;

void init()
{
	// start out looking along the z-axis as before
  camera = new Camera(vec3(0.0, 0.0, 3.0),   // position
              vec3(0.0, 0.0, -1.0),  // forward
              vec3(0.0, 1.0, 0.0));   // up

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


  // Create and initialize buffer
  GLuint buffer;
  glGenBuffers( 1, &buffer );

  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(lines) + sizeof(colors) + sizeof(colorsForLines),
    NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(vertices), sizeof(lines), lines);
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(lines), sizeof(colors), colors );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(lines) + sizeof(colors), sizeof(colorsForLines), colorsForLines );
  
  program = InitShader( "vshader.glsl", "fshader.glsl" );
  glUseProgram( program );

  // set up vertex array objects
  glGenVertexArrays( 1, &vao );

  // VAO for triangles
  glBindVertexArray(vao);
  glBindBuffer( GL_ARRAY_BUFFER, buffer);

  // vPosition is an "in" variable in the vertex shader
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  GLuint vColor = glGetAttribLocation( program, "vColor" ); 
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
    BUFFER_OFFSET(sizeof(vertices) + sizeof(lines)));


  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}


void display( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  mat4 view = camera->GetView();

  mat4 projection = Perspective(30, 1.0, 0.1, 1000);

  glUseProgram(program);
  GLint transform_loc = glGetUniformLocation(program, "transform");

  glUniformMatrix4fv(transform_loc, 1, GL_TRUE, projection * view); 

  // draw triangles and lines
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDrawArrays(GL_LINES, 6, 6);

  glFlush();
}

void keyboard( unsigned char key, int x, int y )
{
  switch( key ) {
  case 033: // Escape Key
  case 'q': case 'Q':	
    exit( EXIT_SUCCESS );
    break; 
   case 'w':   // forward
    camera->MoveForward(0.5);
    break;
  case 'a':
    camera->MoveLeft(0.5);
    break;
  case 's':   
    camera->MoveBackward(0.5);
    break;
  case 'd':
    camera->MoveRight(0.5);
    break;
  case 'r':
    camera->MoveUp(0.5);
    break;
  case 'f':
    camera->MoveDown(0.5);
    break;
  case 'i':
    camera->LookUp(5);
    break;
  case 'j':
    camera->LookLeft(5);
    break;
  case 'k':
    camera->LookDown(5);
    break;
  case 'l':
    camera->LookRight(5);
    break;
  case 'I':
    camera->PitchUp(5);
    break;
  case 'J':
    camera->HeadLeft(5);
    break;
  case 'K':
    camera->PitchDown(5);
    break;
  case 'L':
    camera->HeadRight(5);
    break;
  case '<':
    camera->RollCCW(5);
    break;
  case '>':
    camera->RollCW(5);
    break;
  case 'o':
	// look at origin
    camera->LookAt(vec3(0, 0, 0));
    break;
  case 'O':
	// move to origin
    camera->SetPosition(vec3(0, 0, 0));
    break;
  }
  glutPostRedisplay();
}

// Needed to get key events for arrow keys
void keyboardSpecial(int key, int x, int y)
{
  switch( key ) {
  case GLUT_KEY_UP:
    camera->OrbitUp(length(camera->GetPosition()), 5);
    break;
  case GLUT_KEY_DOWN:
    camera->OrbitDown(length(camera->GetPosition()), 5);
    break;
  case GLUT_KEY_RIGHT:
    camera->OrbitRight(length(camera->GetPosition()), 5);
    break;
  case GLUT_KEY_LEFT:
    camera->OrbitLeft(length(camera->GetPosition()), 5);
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
