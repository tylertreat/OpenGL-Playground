//
// Same as 'skybox' project but uses TextureCube from the rendering utilities.
//
// Simple demo of rendering a cube map onto the 6 inside faces of a model
// to create a "skybox" or a projective texture.  Adapted from the OpenGL Superbible 5e (which
// is also where the landscape images came from). No lighting.
//
// Camera controls are standard.
//

#include <Angel.h>
#include <sphere.h>
#include <cube.h>
#include <images.h>
#include <Camera.h>
#include <Shader.h>
#include <VertexArray.h>
#include <CameraControl.h>
#include <ObjFile.h>
#include <TextureCube.h>

VertexArray * vao;
Shader * cubeShader;
Camera * camera;
CameraControl * cameraControl;
TextureCube * cubeTexture;
mat4 model;

int numVertices;

void init()
{
  // Constructor sets up cube map with default sampling paramaters
  cubeTexture = new TextureCube(
	"../images/pos_x.tga",
    "../images/neg_x.tga",
    "../images/pos_y.tga",
    "../images/neg_y.tga",
    "../images/pos_z.tga",
    "../images/neg_z.tga");

  camera = new Camera(vec3(0.0, 0.0, 0.0),   // position
              vec3(0.0, 0.0, -1.0),  // forward
              vec3(0.0, 1.0, 0.0),   // up
              1.0f,                  // aspect
              30.0f,                 // fovy
              0.1f,                  // near
              1000.0f);              // far
  cameraControl = new CameraControl(camera);

  cubeShader = new Shader("vshader_cube_tex.glsl", "fshader_cube_tex.glsl");

  // VAO for model
  vao = new VertexArray();

  //ObjFile m("../models/teapot.obj"); model = Scale(m.GetScaleFactor());
  ObjFile m("../models/cube_tex.obj");   
  vao->AddAttribute("vPosition", m.GetVertices(), m.GetNumVertices());
  vao->AddIndices(m.GetIndices(), m.GetNumIndices());

  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void display( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Bind texture to a texture unit
  cubeTexture->Bind(1);

  cubeShader->Bind();
  cubeShader->SetUniform("textureCube", 0);
  cubeShader->SetUniform("model",  model * Scale(20.0, 20.0, 20.0));
  cubeShader->SetUniform("view",  camera->GetView());
  cubeShader->SetUniform("projection", camera->GetProjection());
  cubeShader->SetUniform("textureCube", cubeTexture->GetTextureUnit());
  vao->Bind(*cubeShader);
  vao->Draw(GL_TRIANGLES);

  vao->Unbind();
  cubeShader->Unbind();
  glFlush();
}

void keyboard( unsigned char key, int x, int y )
{
	if (!cameraControl->handleKey(key))
	{
		switch( key ) {
		case 033: // Escape Key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break; 
		}
	}
	glutPostRedisplay();
}

// Needed to get key events for arrow keys
void keyboardSpecial(int key, int x, int y)
{
	cameraControl->handleKeySpecial(key);
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
