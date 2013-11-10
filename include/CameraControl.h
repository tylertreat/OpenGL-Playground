#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H
/**
 * \brief Key handler for controlling a Camera
 *
 * Summary:
 *   Basic motion: w, a, s, d, plus r and f for up/down
 *   FPS-like rotations: i, j, k, l
 *   Spacecraft-like rotations: I, J, K, L, <, >
 *   Orbiting: arrow keys
 *   Look at origin: o
 *   Move to origin: O
 *   Increase field of view: V
 *   Decrease field of view: v
 */

class CameraControl
{
public:

    CameraControl(Camera * camera, 
		          const float motionIncrement = 0.5f, 
				  const float rotationIncrement = 5.0f):
	camera(camera), motionIncrement(motionIncrement), rotationIncrement(rotationIncrement)
	{

	}

    /**
     * \brief Creates a camera control from an existing instance
     *
     * \param[in] other - Camera control to copy
     */
    CameraControl(const CameraControl& other):
		camera(other.camera), 
		motionIncrement(other.motionIncrement),
		rotationIncrement(rotationIncrement)
	{

	}

    /**
     * \brief CameraControl destructor
     */
    ~CameraControl()
	{
		// nothing to do
	}

	bool handleKey(const unsigned char key)
	{
		switch(key)
		{
			   case 'w':   // forward
				   camera->MoveForward(motionIncrement);
				   return true;
			   case 'a':
				   camera->MoveLeft(motionIncrement);
				   return true;
			   case 's':   
				   camera->MoveBackward(motionIncrement);
				   return true;
			   case 'd':
				   camera->MoveRight(motionIncrement);
				   return true;
			   case 'r':
				   camera->MoveUp(motionIncrement);
				   return true;
			   case 'f':
				   camera->MoveDown(motionIncrement);
				   return true;
			   case 'i':
				   camera->LookUp(rotationIncrement);
				   return true;
			   case 'j':
				   camera->LookLeft(rotationIncrement);
				   return true;
			   case 'k':
				   camera->LookDown(rotationIncrement);
				   return true;
			   case 'l':
				   camera->LookRight(rotationIncrement);
				   return true;
			   case 'I':
				   camera->PitchUp(rotationIncrement);
				   return true;
			   case 'J':
				   camera->HeadLeft(rotationIncrement);
				   return true;
			   case 'K':
				   camera->PitchDown(rotationIncrement);
				   return true;
			   case 'L':
				   camera->HeadRight(rotationIncrement);
				   return true;
			   case '<':
				   camera->RollCCW(rotationIncrement);
				   return true;
			   case '>':
				   camera->RollCW(rotationIncrement);
				   return true;
			   case 'o':
				   // look at origin
				   camera->LookAt(vec3(0, 0, 0));
				   return true;
			   case 'O':
				   // move to origin
				   camera->SetPosition(vec3(0, 0, 0));
				   return true;
			   case 'v':
				   // decrease field of view
				   {
				   float fovy = camera->GetFieldOfView();
				   fovy -= rotationIncrement;
				   fovy = fovy >= rotationIncrement ? fovy : rotationIncrement;
				   camera->SetFieldOfView(fovy);
				   return true;
				   }
				case 'V':
				   // increase field of view
					{
				   float fovy = camera->GetFieldOfView();
				   fovy += rotationIncrement;
				   fovy = fovy <= 90 - rotationIncrement ? fovy : 90 - rotationIncrement;
				   camera->SetFieldOfView(fovy);
				   return true;
					}

			   default:
				   return false;
		}

	}

	bool handleKeySpecial(int key)
	{
		switch(key)
		{
			   case GLUT_KEY_UP:
				   camera->OrbitUp(length(camera->GetPosition()), rotationIncrement);
				   return true;
			   case GLUT_KEY_DOWN:
				   camera->OrbitDown(length(camera->GetPosition()), rotationIncrement);
				   return true;
			   case GLUT_KEY_RIGHT:
				   camera->OrbitRight(length(camera->GetPosition()), rotationIncrement);
				   return true;
			   case GLUT_KEY_LEFT:
				   camera->OrbitLeft(length(camera->GetPosition()), rotationIncrement);
				   return true;
			   default:
				   return false;
		}
	}

private:
	Camera * camera;
	const float motionIncrement;
	const float rotationIncrement;
	
};


#endif
