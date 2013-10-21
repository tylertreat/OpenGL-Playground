//
// Cube that spins about the y-axis.  The '+' and '-' keys can be
// used to speed up and slow down the rotation.
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
#include "Angel.h"
#include "cube.h"

GLuint vao[3];
GLuint program;


vec3 viewPoint(0.0, 0.0, 3.0);

//(6 faces)(2 triangles/face)(3 vertices/triangle)
int NumVertices;

// Vertices of a unit cube centered at origin, sides aligned with axes
vec4 vertices[8] = {
  vec4( -0.5, -0.5,  0.5, 1.0 ),
  vec4( -0.5,  0.5,  0.5, 1.0 ),
  vec4(  0.5,  0.5,  0.5, 1.0 ),
  vec4(  0.5, -0.5,  0.5, 1.0 ),
  vec4( -0.5, -0.5, -0.5, 1.0 ),
  vec4( -0.5,  0.5, -0.5, 1.0 ),
  vec4(  0.5,  0.5, -0.5, 1.0 ),
  vec4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA olors
vec4 vertex_colors[8] = {
  vec4( 0.0, 0.0, 0.0, 1.0 ),  // black
  vec4( 1.0, 0.0, 0.0, 1.0 ),  // red
  vec4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
  vec4( 0.0, 1.0, 0.0, 1.0 ),  // green
  vec4( 0.0, 0.0, 1.0, 1.0 ),  // blue
  vec4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
  vec4( 1.0, 1.0, 1.0, 1.0 ),  // white
  vec4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};
enum { BLACK, RED, YELLOW, GREEN, BLUE, MAGENTA, WHITE, CYAN };

// use these colors for the 6 faces
int faceColors[6] = {RED, YELLOW, GREEN, BLUE, MAGENTA, CYAN};

// current amount of rotation
GLfloat degrees = 0.0;

// degree change in each frame
GLfloat increment = 0.5;

// elapsed time
int elapsedTime;

// frame rate in millis for 30 frames/sec
const int frameRate = 1000.0 / 60;


// Add data for one face (two triangles) to the points and colors arrays
// Vertex indices a, b, c, d are corners of the face in CCW order
void initOneFace(int a, int b, int c, int d, vec4 points[], vec4 colors[], int face, int index)
{   
  int faceColorIndex = faceColors[face];
  colors[index] = vertex_colors[faceColorIndex]; 
  points[index] = vertices[a]; 
  index++;
  colors[index] = vertex_colors[faceColorIndex]; 
  points[index] = vertices[b]; 
  index++;
  colors[index] = vertex_colors[faceColorIndex]; 
  points[index] = vertices[c]; 
  index++;
  colors[index] = vertex_colors[faceColorIndex]; 
  points[index] = vertices[a]; 
  index++;
  colors[index] = vertex_colors[faceColorIndex]; 
  points[index] = vertices[c]; 
  index++;
  colors[index] = vertex_colors[faceColorIndex]; 
  points[index] = vertices[d]; 
}

void initCube(vec4 points[], vec4 colors[])
{
  initOneFace( 1, 0, 3, 2, points, colors, 0, 0 );     // front face (positive z) is red
  initOneFace( 2, 3, 7, 6, points, colors, 1, 1 * 6);  // face in positive x-direction is yellow
  initOneFace( 3, 0, 4, 7, points, colors, 2, 2 * 6);
  initOneFace( 6, 5, 1, 2, points, colors, 3, 3 * 6);
  initOneFace( 4, 5, 6, 7, points, colors, 4, 4 * 6);
  initOneFace( 5, 4, 0, 1, points, colors, 5, 5 * 6);
}


void init()
{

  // Start by initializing an array of vertices and 
  // a parallel array of colors
  Cube c;
  vec3 * points = c.GetVertices();
  vec4 * colors = c.GetFaceColors();
  NumVertices = c.GetNumVertices();
  int sizeOfPoints = NumVertices * sizeof(vec3);
  int sizeOfColors = NumVertices * sizeof(vec4);
  //initCube(points, colors);

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

  // Create and initialize buffers
  GLuint buffers[2];
  glGenBuffers( 2, buffers );

  glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
  //glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, sizeOfPoints + sizeOfColors, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfPoints, points);
  glBufferSubData(GL_ARRAY_BUFFER, sizeOfPoints, sizeOfColors, colors);

  glBindBuffer( GL_ARRAY_BUFFER, buffers[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(axes) + sizeof(colorsForAxes), NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(axes), axes );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(axes), sizeof(colorsForAxes), colorsForAxes );
  
  program = InitShader( "vshader.glsl", "fshader.glsl" );
  glUseProgram( program );

  // set up vertex array objects
  glGenVertexArrays( 2, vao );

  // VAO for triangles
  glBindVertexArray(vao[0]);
  glBindBuffer( GL_ARRAY_BUFFER, buffers[0]);

  // vPosition is an "in" variable in the vertex shader
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  GLuint vColor = glGetAttribLocation( program, "vColor" ); 
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeOfPoints));

  // VAO for axes
  glBindVertexArray(vao[1]);
  glBindBuffer( GL_ARRAY_BUFFER, buffers[1]);

  // vPosition is an "in" variable in the vertex shader
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(axes)));

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


  glUseProgram(program);
  GLint transform_loc = glGetUniformLocation(program, "transform");

  // second arg means "one 4 x 4 matrix"
  // third arg is true if matrix needs to be transposed into column-major format
  glUniformMatrix4fv(transform_loc, 1, GL_TRUE, projection * view * model); 

  // draw cube
  glBindVertexArray(vao[0]);
  glDrawArrays( GL_TRIANGLES, 0, NumVertices);

  // draw fixed axes *without* model transformation
  glUniformMatrix4fv(transform_loc, 1, GL_TRUE, projection * view); 

  glBindVertexArray(vao[1]);
  glLineWidth(3);
  glDrawArrays( GL_LINES, 0, 6 );

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


int mainSave( int argc, char **argv )
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
