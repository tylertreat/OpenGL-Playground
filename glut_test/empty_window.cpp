//
// Simplest possible demonstration of GLUT.  Creates an empty window.
//
#include <GL/glut.h>

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );
    glutCreateWindow( "Hello, GLUT!" );
    glutMainLoop();
    return 0;	
}
