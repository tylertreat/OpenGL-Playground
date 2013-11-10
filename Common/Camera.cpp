#include "Camera.h"

#define WORLD_UP Y
//#define WORLD_UP Z

#if WORLD_UP == Y
  const vec3 worldUp(0.0f, 1.0f, 0.0f);
  #define RotateUp(theta) RotateY(theta)
#elif WORLD_UP == Z
  const vec3 worldUp(0.0f, 0.0f, 1.0f);
  #define RotateUp(theta) RotateZ(theta)
#else
  #error Bad world up vector
#endif


/*
 * Main constructor
 */
Camera::Camera(
    const vec3& position,
    const vec3& forward,
    const vec3& up,
    float aspectRatio,
    float fovY,
    float nearPlane,
    float farPlane)
    : position(position),
    forward(normalize(forward)),
    aspectRatio(aspectRatio),
    fovY(fovY),
    nearPlane(nearPlane),
    farPlane(farPlane),
    viewStale(false),
    projStale(false),
    EPSILON(0.0000001)
{
  // if either vector is zero, use the default value
  vec3 tempUp = ( IsZeroVector(up) ? vec3(0.0, 1.0, 0.0) : normalize(up) );
  vec3 tempForward = ( IsZeroVector(forward) ? vec3(0.0, 0.0, -1.0) : normalize(forward) );

  // if the given forward and up are parallel, use the default up,
  // (unless both are parallel to default up)
  if (IsParallel(tempUp, tempForward))
  {
    if (IsParallel(tempForward, vec3(0.0, 1.0, 0.0)))
    {
      // revert to defaults
      tempForward = vec3(0.0, 0.0, -1.0);
      tempUp = vec3(0.0, 1.0, 0.0);
    }
    else
    {
      tempUp = vec3(0.0, 1.0, 0.0);
    }
  }

  // Ensure the basis vectors are orthogonal and normalized
  this->forward = tempForward;
  this->right = normalize(cross(tempForward, tempUp));
  this->up    = normalize(cross(this->right, this->forward));

  RecalculateView();
  RecalculateProjection();
}

/**
 * Copy constructor
 */
Camera::Camera(const Camera& other)
    : position(  other.position),
    forward(     other.forward),
    right(       other.right),
    up(          other.up),
    aspectRatio( other.aspectRatio),
    fovY(        other.fovY),
    nearPlane(   other.nearPlane),
    farPlane(    other.farPlane),
    view(        other.view),
    projection(  other.projection),
    viewStale(   other.viewStale),
    projStale(   other.projStale),
    EPSILON(     other.EPSILON)
{
    // Nothing more to do
}

/*
 * Destructor
 */
Camera::~Camera()
{
    // Do nothing
}


/*
 * IsZeroVector
 */
bool Camera::IsZeroVector(vec3 v)
{
  // length squared less than EPSILON
  return dot(v, v) < EPSILON;
}

/*
 * IsParallel
 */
bool Camera::IsParallel(vec3 u, vec3 v)
{
  // cosine of angle between u and v is within EPSILON of 1 or -1
  float cosine = dot(normalize(u), normalize(v));
  return abs(cosine - 1) < EPSILON || abs(cosine + 1) < EPSILON;
}

/*
 * IsSamePoint
 */
bool Camera::IsSamePoint(vec3 p, vec3 q)
{
  // length squared of difference less than EPSILON
  vec3 v = (q - p);
  return dot(v, v) < EPSILON;
}

/*
 * Rotation matrix for arbitrary axis.
 */
mat4 Camera::RotateAxis(const vec3& axis, float theta)
{
	mat4 ret; // identity

	float radians    = theta * DegreesToRadians;
	float cosine     = cos(radians);
	float cosineComp = 1 - cosine;
	float sine       = sin(radians);

	ret[0][0] = axis[0] * axis[0] + (1 - axis[0] * axis[0]) * cosine;
	ret[0][1] = cosineComp * axis[0] * axis[1] - axis[2] * sine;
	ret[0][2] = cosineComp * axis[0] * axis[2] + axis[1] * sine;
	ret[1][0] = cosineComp * axis[0] * axis[1] + axis[2] * sine;
	ret[1][1] = axis[1] * axis[1] + (1 - axis[1] * axis[1]) * cosine;
	ret[1][2] = cosineComp * axis[1] * axis[2] - axis[0] * sine;
	ret[2][0] = cosineComp * axis[0] * axis[2] - axis[1] * sine;
	ret[2][1] = cosineComp * axis[1] * axis[2] + axis[0] * sine;
	ret[2][2] = axis[2] * axis[2] + (1 - axis[2] * axis[2]) * cosine;

	return ret;
}

/*
 * Apply transformation to vector
 */
vec3 Camera::ApplyTransform(const mat4& matrix, const vec3& v)
{
	vec4 temp = matrix * vec4(v, 0.0f);
	return vec3(temp.x, temp.y, temp.z);
}


/*
 * Assignment operator
 */
Camera& Camera::operator=(const Camera& other)
{
    // Copy everything
    this->aspectRatio = other.aspectRatio;
    this->farPlane    = other.farPlane;
    this->forward     = other.forward;
    this->fovY        = other.fovY;
    this->nearPlane   = other.nearPlane;
    this->position    = other.position;
    this->projection  = other.projection;
    this->projStale   = other.projStale;
    this->right       = other.right;
    this->up          = other.up;
    this->view        = other.view;
    this->viewStale   = other.viewStale;

    return *this;
}

/*
 * Look at position
 */
void Camera::LookAt(const vec3& position)
{
    if (IsSamePoint(position, this->position))
    {
        // The position we are trying to look at is roughly equal to our
        // current position, we can't look at it.  Keep our view unchanged
        return;
    }

    // Re-derive our basis vectors using the world up
    forward = normalize(position - this->position);

    // If the new forward direction is parallel to the world up,
    // the cross product will be a zero vector, which makes finding
    // a good up vector a little more complicated
    if (IsParallel(forward, worldUp))
    {
        // Our new forward is parallel to the world up.
        // Unless we have been using the pitch/yaw/roll functions,
        // our right vector will always be perpendicular to the world up.
        // Handle this case by re-using the old right vector

        // This works so long as the old right vector is not parallel to
        // the world up vector.  If it was, then re-use the old up vector
        // which then cannot be parallel to the world up vector
        if (IsParallel(right, forward))
        {
            right = normalize(cross(forward, up));
            up    = normalize(cross(right, forward));
        }
        else
        {
            // The above calculation always returns the up vector above the
            // xz plane (y-up) or xy plane (z-up).  If we are just crossing the
            // vertical, we want the up vector to be able to go under the plane
            // to prevent sudden jumps.
            // We can accomplish this by checking the sign of the dot product
            // between the old up vector and the newly calculated up vector.
            // If the angle between them is greater than 90, the dot product
            // will be negative.  In this case, we want the negation of the
            // calculated up vector to prevent the 90+ degree jump.
            vec3 upTemp = cross(right, forward);
            up    = normalize(upTemp) * (dot(up, upTemp) > 0.0f ? 1.0f : -1.0f);
            right = normalize(cross(forward, up));
        }
    }
    else
    {
        // Typical case.
        // If we just crossed the vertical, the right vector will flip.
        // We can prevent this using the dot product, as described above
        vec3 rightTemp = cross(forward, worldUp);
        right = normalize(rightTemp) * (dot(right, rightTemp) > 0.0f ? 1.0f : -1.0f);
        up    = normalize(cross(right, forward));
    }

    // Our view matrix is now out of date if it wasn't already
    viewStale = true;
}

/*
 * Look at position with up
 */
void Camera::LookAt(const vec3& position, const vec3& up)
{
    // Take the new forward vector if we can, otherwise keep the old one
    if (!IsSamePoint(position, this->position))
    {
        forward = normalize(position - this->position);
    }

    // If the given up vector is parallel to the forward vector,
    // try reusing the existing right vector to deduce an up direction
    if (IsParallel(forward, up))
    {
        // If our old right is parallel to our new forward, this will fail,
        // but we can instead reuse the old up
        if (IsParallel(forward, right))
        {
            right    = normalize(cross(this->up, forward));
            this->up = normalize(cross(right, forward));
        }
        else
        {
            this->up = normalize(cross(right, forward));
            right    = normalize(cross(forward, this->up));
        }
    }
    else
    {
        // Typical case
        right    = normalize(cross(forward, up));
        this->up = normalize(cross(right, forward));
    }

    // The current view matrix is now outdated
    viewStale = true;
}

/*
 * Look up
 */
void Camera::LookUp(float degrees)
{
    // Look up by rotating around the right vector, which is assumed to be
    // orthogonal to the world up vector
    forward = ApplyTransform(RotateAxis(right, degrees), forward);
    up      = normalize(cross(right, forward));

    // Minimize the impact of floating point rounding error by updating right
    right   = normalize(cross(forward, up));

    // Our view matrix needs updating
    viewStale = true;
}

/*
 * Move forward
 */
void Camera::MoveForward(float amount)
{
  position += amount * forward;
  viewStale = true;
}

/*
 * Move right
 */
void Camera::MoveRight(float amount)
{
  position += amount * right;
  viewStale = true;
}

/*
 * Move up
 */
void Camera::MoveUp(float amount)
{
  position += amount * up;
  viewStale = true;
}

/*
 * Look right
 */
void Camera::LookRight(float degrees)
{
    // Look right by rotating around the world up vector
    mat4 rotate = RotateUp(-degrees);
    forward     = ApplyTransform(rotate, forward);
    up          = ApplyTransform(rotate, up);
    right       = normalize(cross(forward, up));

    // Minimize floating point rounding error by recalculating up to ensure
    // all the basis vectors are orthogonal
    up            = normalize(cross(right, forward));

    // Our view matrix needs updating
    viewStale     = true;
}

/*
 * Pitch up
 */
void Camera::PitchUp(float degrees)
{
    // Pitch up by rotating around the right vector
    forward = ApplyTransform(RotateAxis(right, degrees), forward);
    up      = normalize(cross(right, forward));

    // Minimize floating point rounding error by recalculating right
    right   = normalize(cross(forward, up));

    // Our view matrix needs updating
    viewStale = true;
}

/*
 * Yaw right
 */
void Camera::HeadRight(float degrees)
{
    // Yaw right by rotating around our current up vector
    forward = ApplyTransform(RotateAxis(up, -degrees), forward);
    right   = normalize(cross(forward, up));

    // Minimize floating point rounding error by recalculating up
    up      = normalize(cross(right, forward));

    // Our view matrix needs updating
    viewStale = true;
}

/*
 * Roll counter-clockwise
 */
void Camera::RollCCW(float degrees)
{
    // Roll by rotating around our current forward vector
    right = ApplyTransform(RotateAxis(forward, degrees), right);
    up    = normalize(cross(right, forward));

    // Minimize floating point rounding error by recalculating forward
    forward = normalize(cross(up, right));

    // Our view matrix needs updating
    viewStale = true;
}

/*
 * Orbit up
 */
void Camera::OrbitUp(float length, float degrees)
{
  //vec3 center = position + length * forward;
  MoveForward(length);
  PitchDown(degrees);
  MoveBackward(length);
}

/*
 * Orbit right
 */
void Camera::OrbitRight(float length, float degrees)
{
  MoveForward(length);
  LookLeft(degrees);
  MoveBackward(length);
}

/**
 * GetView
 */
const mat4& Camera::GetView() const
{
  if (viewStale)
  {
    RecalculateView();
  }
  return view;
}

/*
 * Recalculate view
 */
void Camera::RecalculateView() const
{
  // The basis vectors of the view coordinate system are
  // 
  //   x = right
  //   y = up
  //   z = -forward
  //
  // so the transformation into the view frame is M = Translate(position) * [x y z 0]
  // (x, y, and z are the columns and 0 denotes the point (0, 0, 0, 1) written
  // as a column vector). Hence the change-of-frames matrix is the inverse of M, or
  // [x y z 0]^T * Translate(-position), where "^T" denotes the transpose.
  // (Since x, y, and z are othornormal, the inverse of [x y z 0] is its transpose.)
  // Hence all we need is to write right, up, and -forward as rows, and then
  // multiply on the right by Translate(-position).

  mat4 inverseOfRotations(vec4(right, 0),
                          vec4(up, 0),
                          vec4(-forward, 0),
                          vec4(0, 0, 0, 1));
  view = inverseOfRotations * Translate(-position);
  viewStale = false;
}

/*
 * Recalculate projection
 */
void Camera::RecalculateProjection() const
{
    projection = Perspective(fovY, aspectRatio, nearPlane, farPlane);//nearPlane, farPlane, fovY, aspectRatio);
}
