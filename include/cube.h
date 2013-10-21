
#include <Angel.h>

// Cube with normal vectors and a color for each face.
class Cube
{
  
  vec3 * vertices;
  vec3 * normals; 
  vec4 * faceColors;
  int numVertices;

  public:

  Cube()
  {
    //(6 faces)(2 triangles/face)(3 vertices/triangle)
    numVertices = 36;
    vertices = new vec3[numVertices];
    normals = new vec3[numVertices];
    faceColors = new vec4[numVertices];
    initCube();
  }

  ~Cube()
  {
    delete[] vertices;
    delete[] normals;
    delete[] faceColors;
  }

  vec3 * GetVertices()
  {
    return vertices;
  }

  vec3 * GetNormals()
  {
    return normals;
  }

  vec4 * GetFaceColors()
  {
    return faceColors;
  }

  int GetNumVertices()
  {
    return numVertices;
  }

private:

// Add data for one face (two triangles) to the vertices, normals, and color arrays.
// Vertex indices a, b, c, d are corners of the face in CCW order as described
// in points[] array.
void initOneFace(int a, int b, int c, int d, vec3 points[], vec4 faceColor, int index)
{   
  vec3 normal = normalize(cross(points[c] - points[b], points[a] - points[b]));
 
  vertices[index] = points[a]; 
  normals[index] = normal;
  faceColors[index] = faceColor;
  index++;
 
  vertices[index] = points[b];
  normals[index] = normal;
  faceColors[index] = faceColor;
  index++;
 
  vertices[index] = points[c];
  normals[index] = normal;
  faceColors[index] = faceColor;
  index++;
 
  vertices[index] = points[a];
  normals[index] = normal;
  faceColors[index] = faceColor;
  index++;
 
  vertices[index] = points[c];
  normals[index] = normal;
  faceColors[index] = faceColor;
  index++;
 
  vertices[index] = points[d];
  normals[index] = normal;
  faceColors[index] = faceColor;
}

void initCube()
{
  // There are 8 vertex positions, but each location is listed 
  // multiple times to create to create the 36 vertices of the model.
  vec3  points[8] = {
  vec3( -0.5, -0.5,  0.5),
  vec3( -0.5,  0.5,  0.5),
  vec3(  0.5,  0.5,  0.5),
  vec3(  0.5, -0.5,  0.5),
  vec3( -0.5, -0.5, -0.5),
  vec3( -0.5,  0.5, -0.5),
  vec3(  0.5,  0.5, -0.5),
  vec3(  0.5, -0.5, -0.5)};

  // RGBA colors
  vec4 vertex_colors[8] = {
  vec4( 0.0, 0.0, 0.0, 1.0 ),  // black
  vec4( 1.0, 0.0, 0.0, 1.0 ),  // red
  vec4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
  vec4( 0.0, 1.0, 0.0, 1.0 ),  // green
  vec4( 0.0, 0.0, 1.0, 1.0 ),  // blue
  vec4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
  vec4( 1.0, 1.0, 1.0, 1.0 ),  // white
  vec4( 0.0, 1.0, 1.0, 1.0 )   // cyan
  };
  enum { BLACK, RED, YELLOW, GREEN, BLUE, MAGENTA, WHITE, CYAN };

  initOneFace( 1, 0, 3, 2, points, vertex_colors[RED],     0 * 6 ); // z  
  initOneFace( 2, 3, 7, 6, points, vertex_colors[YELLOW],  1 * 6);  // x 
  initOneFace( 3, 0, 4, 7, points, vertex_colors[GREEN],   2 * 6);  // -y
  initOneFace( 6, 5, 1, 2, points, vertex_colors[BLUE],    3 * 6);  // y
  initOneFace( 4, 5, 6, 7, points, vertex_colors[MAGENTA], 4 * 6);  // -z
  initOneFace( 5, 4, 0, 1, points, vertex_colors[CYAN],    5 * 6);  // -x
}


};


