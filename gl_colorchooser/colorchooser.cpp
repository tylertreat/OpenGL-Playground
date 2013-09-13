//
// Displays a color square and circle containing the selected color.
// Colors are assigned to each vertex and then the rasterizer interpolates
// those colors across the triangles.  
//
// This version uses separate buffer objects for the position and color attributes.
//

#include <iostream>
#include <Angel.h>
using namespace std;

#define PI 3.14159f

const int SCREEN_SIZE = 512;
const int CIRCLE_SEGMENTS = 48;
const GLfloat LEFT_X = -0.75;
const GLfloat RIGHT_X = 0.0;
const GLfloat TOP_Y = 0.375;
const GLfloat BOTTOM_Y = -0.375;

GLuint square_vao;
GLuint circle_vao;
GLuint square_shaders;
GLuint circle_shaders;

// Get the coordinates for a circle with center (cx, cy) and radius r.
vec2* getCircleVertices(float cx, float cy, float r, int segments) 
{ 
	vec2 *circle = new vec2[segments];
	for(int i = 0; i < segments; i++) 
	{ 
		float theta = i * ((2.0f * PI) / segments);
		float x = r * cosf(theta);
		float y = r * sinf(theta);
		circle[i] = vec2(x + cx, y + cy);
	} 
	
	return circle;
}


// Indicates if the coordinates are in the bounds of the square.
bool isInBounds(float x, float y)
{
	if (x < LEFT_X || x > RIGHT_X)
	{
		return false;
	}
	if (y < BOTTOM_Y || y > TOP_Y)
	{
		return false;
	}

	return true;
}


// Convert the screen x-coordinate to the OpenGL coordinate.
float translateX(int x)
{
	return (float) (2 * x) / SCREEN_SIZE - 1;
}


// Convert the screen y-coordinate to the OpenGL coordinate.
float translateY(int y)
{
	return (float) (-2 * y) / SCREEN_SIZE + 1;
}


// Sets up vertex attributes on the GPU
void init()
{
  // The corners of the square as 2D coordinates in clip space
  vec2 square_points[4]  = {
    vec2( RIGHT_X, BOTTOM_Y ), vec2( RIGHT_X, TOP_Y ),
	vec2( LEFT_X, TOP_Y ), vec2( LEFT_X, BOTTOM_Y )
  };

  // A color attribute for each corner (red, blue, green, and white)
  vec4 colors[4]  = {
    vec4( 1.0, 0.0, 0.0, 1.0 ),
	vec4( 0.0, 0.0, 1.0, 1.0 ),
	vec4( 0.0, 1.0, 0.0, 1.0 ),
	vec4( 1.0, 1.0, 1.0, 1.0 )
  };

  // Load shaders
  square_shaders = InitShader( "vshader31.glsl", "fshader31.glsl" );

  // Create and initialize a buffer object for each attribute
  GLuint square_buffer[2];
  glGenBuffers( 2, &square_buffer[0] );

  glBindBuffer( GL_ARRAY_BUFFER, square_buffer[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(square_points), square_points, GL_STATIC_DRAW );

  glBindBuffer( GL_ARRAY_BUFFER, square_buffer[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER, 0);

  // Create a vertex array object
  glGenVertexArrays( 1, &square_vao );
  glBindVertexArray( square_vao );

  // set up vertex attributes
  GLuint loc_vPosition = glGetAttribLocation( square_shaders, "vPosition" );
  glEnableVertexAttribArray( loc_vPosition );

  GLuint loc_vColor = glGetAttribLocation( square_shaders, "vColor" ); 
  glEnableVertexAttribArray( loc_vColor );

  glBindBuffer( GL_ARRAY_BUFFER, square_buffer[0] );
  glVertexAttribPointer( loc_vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  glBindBuffer( GL_ARRAY_BUFFER, square_buffer[1] );
  glVertexAttribPointer( loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind the VAO
  glBindVertexArray(0);


  // The points of the circle as 2D coordinates in clip space
  vec2 *circle_points = getCircleVertices(0.5f, 0, 0.3f, CIRCLE_SEGMENTS);

  // Load shaders
  circle_shaders = InitShader( "vshader31.glsl", "fshader_circle.glsl" );

  GLuint circle_buffer;
  glGenBuffers( 1, &circle_buffer );

  glBindBuffer( GL_ARRAY_BUFFER, circle_buffer );
  glBufferData( GL_ARRAY_BUFFER, CIRCLE_SEGMENTS * 8, circle_points, GL_STATIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER, 0);

  // Create a vertex array object
  glGenVertexArrays( 1, &circle_vao );
  glBindVertexArray( circle_vao );

  // set up vertex attributes
  GLuint vPosition = glGetAttribLocation( circle_shaders, "vPosition" );
  glEnableVertexAttribArray( vPosition );

  GLuint loc = glGetUniformLocation(circle_shaders, "color");
  glUniform4f(loc, 0.0, 1.0, 0.0, 1.0);

  glBindBuffer( GL_ARRAY_BUFFER, circle_buffer );
  glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

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
  glUseProgram(square_shaders);
  glBindVertexArray( square_vao );

  // draw primitives
  glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
  glFlush();

  // bind the shaders and VAO
  glUseProgram(circle_shaders);
  glBindVertexArray( circle_vao );

  // draw primitives
  glDrawArrays( GL_TRIANGLE_FAN, 0, CIRCLE_SEGMENTS );
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


void mouse( int button, int state, int x, int y )
{
	float scaled_x = translateX(x);
	float scaled_y = translateY(y);
  if ( state == GLUT_DOWN )
  {
    switch( button )
	{
		case GLUT_LEFT_BUTTON:
			cout << "mouse down " << scaled_x << ", " << scaled_y << endl;
			cout << "in bounds " << isInBounds(scaled_x, scaled_y) << endl;
			if (isInBounds(scaled_x, scaled_y))
			{
				// TODO: update circle color
			}
			break;
		case GLUT_MIDDLE_BUTTON: break;
		case GLUT_RIGHT_BUTTON:  break;
    }
  }
}


int main( int argc, char **argv )
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
  glutCreateWindow("Color Chooser");

  // This line must come after creating the OpenGL context (glutCreateWindow)
  // but before anything else
  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);

  glutMainLoop();
  return 0;
}
