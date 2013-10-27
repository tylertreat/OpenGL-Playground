#ifndef CAMERA_H
#define CAMERA_H

#include <Angel.h>

/**
 * \brief Represents a camera in world space
 *
 * Camera class to assist in looking at the world by providing a view matrix
 * to transform from world space to eye space. Logically, a Camera encapsulates 
 * a position and three orthogonal vectors referred to as right, up, and forward 
 * (corresponding to the x, y, and negative z axes of the eye frame).
 * An invariant is maintained that the three vectors returned by getRight(),
 * getUp(), and getForward() are always unit length and are orthogonal.
 * 
 * A Camera also encapsulates a standard perspective projection given by
 * a vertical field of view (fovY), aspect ratio, and near and far clipping
 * planes.  
 * 
 * The camera supports 3 different
 * types of control depending on the methods used to update it.
 *
 * 1. Using the MoveForward/Backward/etc methods along with the LookUp/Down/etc
 *    methods provides a free-look camera in similar style to FPS games.
 *    Looking up and down always takes the world-up vector into account to
 *    ensure the camera never rolls.  That is, the camera's right vector
 *    always remains orthogonal to the world up vector.  This invariant
 *    is only maintained as long as NO calls are made to the Pitch/Head/Roll 
 *    methods.  Otherwise, the results are undefined.
 *
 * 2. Using the Pitch/Head/Roll methods provides a space flight simulator like
 *    camera, in which the camera is free to rotate in any direction.  There is
 *    no feel for which direction is the world up.  These methods should not
 *    be mixed with the MoveForward/Backward methods.
 *
 * 3. Using the Orbit methods provides a trackball like control in which the
 *    camera moves around the scene on a sphere.  Using the OrbitUp/Down methods
 *    corresponds to changing the polar component of the camera's position in
 *    spherical coordinates, while using the OrbitLeft/Right corresponds to
 *    changing the azimuthal component.  Use MoveForward/Backward to bring the
 *    camera closer or further away from the orbit point, essentially increasing
 *    or decreasing the orbit distance, i.e., the radial component in spherical 
 *    coordinates.
 *
 * \author Jonathan Henze
 */
class Camera
{
public:

    /**
     * \brief Creates a new camera
     *
     * The vectors provided to the constructor need not be unit length 
     * nor orthogonal to each other. However, if either vector is approximately
     * zero, the default value is used instead. If the
     * given up vector is parallel to the forward vector, the world up vector
     * (0, 1, 0) will be used.  If both the given up vector and forward vector
     * are in direction (0, 1, 0), the default value (0, 0, -1) is used for the 
     * forward vector. 
     *
     * The camera defaults to a projection with 60 degree field of view, aspect
     * ratio 1.0, and near and far planes set at 0.1 and 1000, respectively.
     * 
     * \param[in] position    - Starting position of the camera in world space
     * \param[in] forward     - Direction the camera is facing
     * \param[in] up          - Direction of the camera's up
     * \param[in] aspectRatio - Aspect ratio of the camera as width/height
     * \param[in] fovY        - Vertical field of view of the camera in degrees
     * \param[in] nearPlane   - How close objects can be before they can no
     *                          longer be seen.  Must be > 0
     * \param[in] farPlane    - How far away object can be before they dissapear.
     *                          Must be > nearPlane
     */
    Camera(
        const vec3& position = vec3(0,0,0),
        const vec3& forward  = vec3(0,0,-1),
        const vec3& up       = vec3(0,1,0));

    /**
     * \brief Creates a camera from an existing instance
     *
     * \param[in] other - Camera to copy
     */
    Camera(const Camera& other);

    /**
     * \brief Camera destructor
     */
    ~Camera();
 
    /**
     * \brief Creates and returns a view matrix corresponding to the camera's
     * current position and orientation
     *
     * \return The camera's view matrix
     */
    const mat4& GetView() const;


    /**
     * \brief Gets the current world space position of the camera
     *
     * \return The current world space position of the camera
     */
    inline const vec3& GetPosition() const
    {
      return position;
    }

    /**
     * \brief Sets the current world space position of the camera
     *
     * Sets the current world space position of the camera without modifying
     * the rotation of the camera.  The camera will still be looking in the
     * same direction, but not at the same point.  If you want the camera to
     * adjust its rotation to still look at the same point, use the LookAt
     * function after setting the new position.
     *
     * \param[in] position - New position for the camera
     */
    inline void SetPosition(const vec3& position)
    {
      this->position = position;
    }

    /**
     * \brief Gets a unit vector in the direction the camera is facing
     *
     * \return A unit vector in the direction the camera is facing
     */
    inline const vec3& GetForward() const
    {
      return forward;
    }
 
    /**
     * \brief Gets a unit vector in the direction of the camera's right
     *
     * \return A unit vector in the direction of the camera's right
     */
    inline const vec3& GetRight() const
    {
      return right;
    }

    /**
     * \brief Gets a unit vector in the direction of the camera's up
     *
     * This vector must be orthogonal to getForward() and getRight().

     * \return A unit vector in the direction of the camera's up
     */
    inline const vec3& GetUp() const
    {
      return up;
    }
 
    /**
     * \brief Moves the camera along its forward vector
     *
     * \param[in] amount - Distance to move the camera by
     */
    void MoveForward(float amount);

    /**
     * \brief Moves the camera along its backward vector
     *
     * \param[in] amount - Distance to move the camera by
     */
    inline void MoveBackward(float amount)
    {
        MoveForward(-amount);
    }

    /**
     * \brief Moves the camera along its right vector
     *
     * \param[in] amount - Distance to move the camera by
     */
    void MoveRight(float amount);

    /**
     * \brief Moves the camera along its left vector
     *
     * \param[in] amount - Distance to move the camera by
     */
    inline void MoveLeft(float amount)
    {
        MoveRight(-amount);
    }

    /**
     * \brief Moves the camera along its up vector
     *
     * \param[in] amount - Distance to move the camera by
     */
    void MoveUp(float amount);
 
    /**
     * \brief Moves the camera along its down vector
     *
     * \param[in] amount - Distance to move the camera by
     */
    inline void MoveDown(float amount)
    {
        MoveUp(-amount);
    }

    /**
     * \brief Turns the camera to face a world space position
     *
     * \param[in] at - Position to look at
     * \param[in] up - Up vector to use; if none is given, will
     *                 attempt to use world up direction
     */
    void LookAt(const vec3& at);
    void LookAt(const vec3& at, const vec3& up);

    /**
     * \brief Tilts the camera to look up with FPS-like movement
     *
     * Tilts the camera to look up while taking the world up vector into account.
     * Precondition: Camera's right vector is orthogonal to world up vector.
     *
     * \param[in] degrees - Degrees to look up by
     */
    void LookUp(float degrees);

    /**
     * \brief Tilts the camera to look down with FPS-like movement
     *
     * Tilts the camera to look down while taking the world up vector into account.
     * Precondition: Camera's right vector is orthogonal to world up vector.
     *
     * \param[in] degrees - Degrees to look down by
     */
    inline void LookDown(float degrees)
    {
        LookUp(-degrees);
    }

    /**
     * \brief Tilts the camera to look right with FPS-like movement
     *
     * Tilts the camera to look right while taking the world up vector into account.
     * Precondition: Camera's right vector is orthogonal to world up vector.
     *
     * \param[in] degrees - Degrees to look right by
     */
    void LookRight(float degrees);

    /**
     * \brief Tilts the camera to look left with FPS-like movement
     *
     * Tilts the camera to look left while taking the world up vector into account.
     * Precondition: Camera's right vector is orthogonal to world up vector.
     *
     * \param[in] degrees - Degrees to look left by
     */
    inline void LookLeft(float degrees)
    {
        LookRight(-degrees);
    }

    /**
     * \brief Pitches the camera up with space flight like movement
     *
     * Pitches the camera up as if it were an airplane by rotating about
     * its right vector.  This can cause some roll
     * to be introduced if the camera changes head angle afterwards.
     *
     * This should not be mixed with the LookUp/Down/Right/Left functions
     * or the Orbit functions.
     *
     * \param[in] degrees - Degrees to pitch up by
     */
    void PitchUp(float degrees);

    /**
     * \brief Pitches the camera down with space flight like movement
     *
     * Pitches the camera down as if it were an airplane by rotating about
     * its right vector.  This can cause some roll
     * to be introduced if the camera yaws afterwards.
     *
     * This should not be mixed with the LookUp/Down/Right/Left functions
     * or the Orbit functions.
     *
     * \param[in] degrees - Degrees to pitch down by
     */
    inline void PitchDown(float degrees)
    {
        PitchUp(-degrees);
    }

    /**
     * \brief Heads the camera right with space flight like movement
     *
     * Heads the camera right as if it were an airplane by rotating about
     * its own up vector. This can cause some roll
     * to be introduced if the camera has previously pitched.
     *
     * This should not be mixed with the LookUp/Down/Right/Left functions
     * or the Orbit functions.
     *
     * \param[in] degrees - Degrees to yaw right by
     */
    void HeadRight(float degrees);

    /**
     * \brief Heads the camera left with space flight like movement
     *
     * Heads the camera left as if it were an airplane by rotating about
     * its own up vector. This can cause some roll
     * to be introduced if the camera has previously pitched.
     *
     * This should not be mixed with the LookUp/Down/Right/Left functions
     * or the Orbit functions.
     *
     * \param[in] degrees - Degrees to yaw left by
     */
    inline void HeadLeft(float degrees)
    {
        HeadRight(-degrees);
    }

    /**
     * \brief Rolls the camera clockwise
     *
     * Rolls the camera clockwise, as if it were an airplane, by rotating about
     * its forward vector.
     *
     * This should not be mixed with the LookUp/Down/Right/Left functions
     * or the Orbit functions.
     *
     * \param[in] degrees - Degrees to rotate clockwise by
     */
    inline void RollCW(float degrees)
    {
        RollCCW(-degrees);
    }

    /**
     * \brief Rolls the camera counter-clockwise
     *
     * Rolls the camera counter-clockwise, as if it were an airplane, by rotating about
     * its forward vector.
     *
     * This should not be mixed with the LookUp/Down/Right/Left functions
     * or the Orbit functions.
     *
     * \param[in] degrees - Degrees to rotate counter-clockwise by
     */
    void RollCCW(float degrees);

    /** 
     * \brief Moves the camera upwards in orbit around a point
     *
     * Moves the camera up a sphere around the point located length units
     * along the forward vector while looking at that point.  The camera
     * rotates about its right vector while maintaining the same distance
     * along the forward vector to the point. 
     *
     * To orbit around world origin, use OrbitUp(length(GetPosition), degrees),
     * (assuming that the world origin lies along forward vector).
     *
     * \param[in] length - distance from camera to point
     * \param[in] degrees - angle by which to orbit up
     */
    void OrbitUp(float length, float degrees);

    /** 
     * \brief Moves the camera downwards in orbit around a point
     *
     * Moves the camera down a sphere around the point located length units
     * along the forward vector while looking at that point.  The camera
     * rotates about its right vector while maintaining the same distance
     * along the forward vector to the point. 
     *
     * To orbit around world origin, use OrbitDown(length(GetPosition), degrees),
     * (assuming that the world origin lies along forward vector).
     *
     * \param[in] length - distance from camera to point
     * \param[in] degrees - angle by which to orbit down
     */
    inline void OrbitDown(float length, float degrees)
    {
        OrbitUp(length, -degrees);
    }

    /** 
     * \brief Moves the camera right in orbit around a point
     *
     * Moves the camera right in a sphere around the point located length units
     * along the forward vector while looking at that point.  The camera
     * rotates about the world's up axis while maintaining the same distance
     * along the forward vector to the point. 
     *
     * To orbit around world origin, use OrbitRight(length(GetPosition), degrees),
     * (assuming that the world origin lies along forward vector).
     *
     * Precondition: Camera's right vector is orthogonal to world up vector.
     *
     * \param[in] length - distance from camera to point
     * \param[in] degrees - angle by which to orbit right
     */
    void OrbitRight(float length, float degrees);

    /** 
     * \brief Moves the camera left in orbit around a point
     *
     * Moves the camera left in a sphere around the point located length units
     * along the forward vector while looking at that point.  The camera
     * rotates about the world up axis while maintaining the same distance
     * along the forward vector to the point. 
     *
     * To orbit around world origin, use OrbitLeft(length(GetPosition), degrees),
     * (assuming that the world origin lies along forward vector).
     *
     * Precondition: Camera's right vector is orthogonal to world up vector.
     *
     * \param[in] length - distance from camera to point
     * \param[in] degrees - angle by which to orbit left
     */
    inline void OrbitLeft(float length, float degrees)
    {
        OrbitRight(length, -degrees);
    }

        /**
     * \brief Assigns this camera to be equal to another instance
     *
     * \param[in] other - Camera to copy
     */
    Camera& operator=(const Camera& other);



 
private:
    const float EPSILON;

    vec3 position;     //!< World space position of the camera
    vec3 forward;      //!< Unit vector in the direction the camera is facing
    vec3 right;        //!< Unit vector in the direction of the camera's right
    vec3 up;           //!< Unit vector in the direction of the camera's up
	mutable mat4 view; //!< Camera view matrix


	// Some utility methods...

    /**
     * \brief Returns true if the given vector is approximately zero
     */
    bool IsZeroVector(vec3 v);

    /**
     * \brief Returns true if the two vectors are approximately parallel
     */
    bool IsParallel(vec3 u, vec3 v);

    /**
     * \brief Returns true if the two points are approximately the same
     */
    bool IsSamePoint(vec3 p, vec3 q);
 
	/**
	* \brief Creates a matrix to rotate around an arbitrary axis
	*
	* \param[in] axis  - Axis of rotation
	* \param[in] theta - Degrees to rotate by
	*
	* \return Rotation matrix
	*/
	mat4 RotateAxis(const vec3& axis, float theta);


	/**
	* \brief Transforms a vec3 using only the linear part of an affine transformation
	*
	* \param[in] matrix - Transformation matrix
	* \param[in] v      - Vector to transform
	*
	* \return Transformed vector
	*/
	vec3 ApplyTransform(const mat4& matrix, const vec3& v);

};

#endif
