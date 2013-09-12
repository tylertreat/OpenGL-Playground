//
// Creates a simple window using GLUT and clears it using
// a white background.  
//
#include <GL/glut.h>

void display( void )
{
  // Clear the framebuffer
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGBA );
  glutInitWindowSize(512, 512);
  glutCreateWindow("Example 0");

  // set the window background color
  glClearColor(1.0, 1.0, 0.0, 1.0);

  glutDisplayFunc(display);

  glutMainLoop();
  return 0;	
}

