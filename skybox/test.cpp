//
// Simple demo of rendering a cube map onto the 6 inside faces of a model
// to create a "skybox" or a projective texture.  Adapted from the OpenGL Superbible 5e (which
// is also where the landscape images came from).  No lighting.
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
mat4 model;

int numVertices;

void init()
{
	
	// names of landscape image files 
    char * cubeImages[6] = {
    "../images/pos_x.tga",
    "../images/neg_x.tga",
    "../images/pos_y.tga",
    "../images/neg_y.tga",
    "../images/pos_z.tga",
    "../images/neg_z.tga"
    };

  // OpenGL enum types for loading the 6 images
  GLenum cubeEnums[6] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
  };
  
  // Set up texture and set parameters (using texture unit 1)
  glActiveTexture(GL_TEXTURE1);
  GLuint cubeTexture;
  glGenTextures(1, &cubeTexture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);       

  // load 6 images
  for (int i = 0; i < 6; ++i)
  {
    GLubyte *imageBytes;
    int nWidth, nHeight, nComponents;
    GLenum eFormat;
    imageBytes = loadFile(cubeImages[i], &nWidth, &nHeight, &nComponents, &eFormat);

	// Other possibilities for images...
	//imageBytes = checkerboard2(&nWidth, &nHeight, &nComponents, &eFormat);
    //imageBytes = loadFile("../images/stone.tga", &nWidth, &nHeight, &nComponents, &eFormat);

	glTexImage2D(cubeEnums[i], 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, imageBytes);
    free(imageBytes);
  }


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

  // Draw cube - we scale it up by 20 to ensure our view point is inside it

  cubeShader->Bind();
  cubeShader->SetUniform("textureCube", 0);
  cubeShader->SetUniform("model",  model * Scale(20.0, 20.0, 20.0));
  cubeShader->SetUniform("view",  camera->GetView());
  cubeShader->SetUniform("projection", camera->GetProjection());
  cubeShader->SetUniform("textureCube", 1); // texture unit GL_TEXTURE1
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
