
//
// Examples of simple transformations in two dimensions.  See the keyboard() 
// function for options.  Uses 3D homogeneous coordinates working in the plane
// z = 0. 
//


#include <Angel.h>

const float PI = atanf(1.0) * 4;
const float SLOPE = 0;
const float INTERCEPT = 0.0;

GLuint vao;
GLuint program;
GLuint buffer[2];
mat4 transform; 
vec3 vertices[3];

// Returns a 4x4 matrix representing a transformation in which all points are reflected across
// the line with the given slope and y-intercept.
mat4 Reflect(float slope, float intercept)
{
	float theta = slope >= 0 ? atan(slope) : atan(abs(slope)) + PI / 2;

	mat4 reflect = mat4(vec4(cosf(2 * theta), sinf(2 * theta), 0, 0),
		                vec4(sinf(2 * theta), -cosf(2 * theta), 0, 0),
				        vec4(0, 0, 1, 0),
				        vec4(0, 0, 0, 1));

	return Translate(0, intercept, 0) * reflect * Translate(0, -intercept, 0);
}


// Sets up vertex attributes on the GPU
void init()
{
  vec3 points[7]  = {
    // a triangle in the plane z = 1
    vec3( 0.0, 0.0, 1.0 ), vec3( 0.5, 0.0, 1.0 ), vec3( 0.5, 0.25, 1.0 ), 
    // a pair of axes for reference
    vec3( 0.0, -0.9, 0.0 ), vec3( 0.0, 0.9, 0.0 ), vec3( -0.9, 0.0, 0.0 ), vec3( 0.9, 0.0, 0.0 )  
  };

  for (int i = 0; i < 3; ++i)
  {
    vertices[i] = points[i];
  }

  // A color attribute for each point (red triangle, blue axes)
  vec4 colors[7]  = {
    vec4( 1.0, 0.0, 0.0, 1.0 ),  vec4( 1.0, 0.0, 0.0, 1.0 ), vec4(1.0, 0.0, 0.0, 1.0),
        vec4( 0.0, 0.0, 1.0, 1.0 ), vec4( 0.0, 0.0, 1.0, 1.0 ), vec4( 0.0, 0.0, 1.0, 1.0 ), vec4( 0.0, 0.0, 1.0, 1.0 )

  };

  // Load shaders
  program = InitShader( "vshader31.glsl", "fshader31.glsl" );

  // Create and initialize a buffer object for each attribute
 
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
  glVertexAttribPointer( loc_vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

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
  GLint transform_loc = glGetUniformLocation(program, "transform");

  glBindVertexArray( vao );

  // draw triangle with transformation
  glUniformMatrix4fv(transform_loc, 1, GL_TRUE, transform);
  glDrawArrays( GL_TRIANGLES, 0, 3 );

  // use identity transformation for axes
  // (Note this is the same VAO, but the axis vertices start at index 3 in the buffer.)
  glUniformMatrix4fv(transform_loc, 1, GL_TRUE, mat4()); 
  glDrawArrays( GL_LINES, 3, 4 );

  glFlush();

  // unbind
  glUseProgram(0);
  glBindVertexArray(0);
}


void
keyboard( unsigned char key, int x, int y )
{
  mat4 nextTransform;

  switch( key ) {
  case 033:  // Escape key
  case 'q': case 'Q':
    exit( EXIT_SUCCESS );
    break;
    case 'f': // reflect across line
	nextTransform = Reflect(SLOPE, INTERCEPT);
	break;
    case 'r': // rotate 60 degrees
    nextTransform = mat4(vec4(0.5, -0.866, 0.0, 0.0),
                         vec4(0.866, 0.5, 0.0, 0.0),
                         vec4(0.0, 0.0, 1.0, 0.0),
                         vec4(0.0, 0.0, 0.0, 1.0));
    break;
    case 'R': // rotate -60 degrees
    nextTransform = mat4(vec4(0.5, 0.866, 0.0, 0.0),
                         vec4(-0.866, 0.5, 0.0, 0.0),
                         vec4(0.0, 0.0, 1.0, 0.0),
                         vec4(0.0, 0.0, 0.0, 1.0));
    break;
    case 's': // scale by 0.5
      nextTransform = mat4(); // defaults to identity matrix
      nextTransform[0][0] = 0.5;
      nextTransform[1][1] = 0.5;
    break;
    case 'S': // scale by 2
      nextTransform = mat4();
      nextTransform[0][0] = 2.0;
      nextTransform[1][1] = 2.0;
    break;

    case 't': // translate by (.3, .2)
      nextTransform = mat4();
      nextTransform[0][3] = 0.3;
      nextTransform[1][3] = 0.2;
    break;
    case 'T': // translate by (-.3, -.2)
      nextTransform = mat4();
      nextTransform[0][3] = -0.3;
      nextTransform[1][3] = -0.2;
    break;
    case 'x': // x shear by 0.5y
      nextTransform = mat4();
      nextTransform[0][1] = 0.5;
    break;
    case 'X': // x shear by -0.5y
      nextTransform = mat4();
      nextTransform[0][1] = -0.5;
    break;
    case 'y': // y shear by 0.5x
      nextTransform = mat4();
      nextTransform[1][0] = 0.5;
    break;
    case 'Y': // y shear by -0.5x
      nextTransform = mat4();
      nextTransform[1][0] = -0.5;
    break;
    case 'c': // rotate about centroid

    // First, apply the current transformation to find the vertex locations and
	// calculate the centroid
    vec4 temp[3];
    temp[0] = transform * vec4(vertices[0]);
    temp[1] = transform * vec4(vertices[1]);
    temp[2] = transform * vec4(vertices[2]);
    vec2 centroid = vec2((temp[0].x + temp[1].x + temp[2].x) / 3.0,
                         (temp[0].y + temp[1].y + temp[2].y) / 3.0);

	// Use some useful functions from mat.h to compute T * R * T-inverse
    nextTransform = Translate(centroid.x, centroid.y, 0.0) * RotateZ(60) * Translate(-centroid.x, -centroid.y, 0.0);
    break;

  }

  // compose with current transformation
  transform = nextTransform * transform;

  glutPostRedisplay();
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
