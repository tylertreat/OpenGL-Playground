//
// Examples of event handling in GLUT.
//
#include <GL/glut.h>
#include <iostream>
using namespace std;

void keyboard( unsigned char key, int x, int y )
{
  cout << "key pressed " << key << endl;
  switch ( key ) {
  case 033:  // escape
    exit( EXIT_SUCCESS );
    break;
  }
}

void display( void )
{

}

void mouse( int button, int state, int x, int y )
{
  cout << "mouse down " << x << ", " << y << endl;
  if ( state == GLUT_DOWN ) {
    switch( button ) {
    case GLUT_LEFT_BUTTON:   break;
    case GLUT_MIDDLE_BUTTON: break;
    case GLUT_RIGHT_BUTTON:  break;
    }
  }
}

void idle( void )
{
  //cout << ".";
  //cout.flush();
  //glutPostRedisplay();
}

int main( int argc, char **argv )
{
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA );
  glutInitWindowSize( 512, 512 );
  glutCreateWindow( "GLUT events" );

  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard );
  glutMouseFunc( mouse );
  glutIdleFunc( idle );

  glutMainLoop();
  return 0;
}
