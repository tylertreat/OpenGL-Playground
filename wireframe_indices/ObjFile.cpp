#include "ObjFile.h"
#include <assert.h>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "Util.h"

using namespace std;

#ifndef MAX
  #define MAX(x,y) (x > y ? x : y)
#endif

/**
 * \brief Class to help with parsing the face definitions in obj files
 */
struct ObjAttribute
{
public:
    ObjAttribute(const string& str)
    {
        if (str.find("//") != string::npos)
        {
            // Format is vertex//normal
            sscanf(str.c_str(), "%d//%d", &vertex, &normal);

            // Account for 1 based indexing
            vertex--;
            normal--;
            
            // No texcoord
            texCoord = -1;
        }
        else if (str.find('/') != string::npos)
        {
            // Either vertex/texCoord/normal or vertex/texCoord
            if (sscanf(str.c_str(), "%d/%d/%d", &vertex, &texCoord, &normal) != 3)
            {
                // If we're here, then we didn't get a normal
                normal = -1;
            }
            else
            {
                normal--;
            }
            vertex--;
            texCoord--;
        }
        else
        {
            // Format is just vertex
            sscanf(str.c_str(), "%d", &vertex);
            vertex--;
            texCoord = -1;
            normal = -1;
        }
    }

    /**
     * \brief Less than operator for sorting
     */
    bool operator<(const ObjAttribute& other) const
    {
        if (vertex != other.vertex)
        {
            return vertex < other.vertex;
        }
        else if (texCoord != other.texCoord)
        {
            return texCoord < other.texCoord;
        }
        else
        {
            return normal < other.normal;
        }
    }

    int vertex, texCoord, normal;
};

/*
 * Constructor
 */
ObjFile::ObjFile(const char* filename)
    : numVertices(0),
    numIndices(0),
    vertices(NULL),
    normals(NULL),
    indices(NULL),
    texCoords(NULL),
    tangents(NULL),
    minXYZ(0,0,0),
    maxXYZ(0,0,0)
{
    ifstream file(filename);

    if (!file.good())
    {
        cerr << "Couldn't open obj file " << filename << " for reading" << endl;
        return;
    }

    // Read everything from the file
    ReadObjFile(file);

    // Close the file
    file.close();
}

/*
 * Copy constructor
 */
ObjFile::ObjFile(const ObjFile& other)
{
    Copy(other);
}

/*
 * Assignment operator
 */
ObjFile& ObjFile::operator=(const ObjFile& other)
{
    // Safety check against self assignment
    if (&other != this)
    {
        // Free memory, same as destructor
        FreeMemory();

        // Copy the other ObjFile over, same as copy constructor
        Copy(other);
    }

    return *this;
}

/*
 * Destructor
 */
ObjFile::~ObjFile()
{
    FreeMemory();
}

/*
 * Copy ObjFile helper method
 */
void ObjFile::Copy(const ObjFile& other)
{
    this->numVertices = other.numVertices;
    this->numIndices  = other.numIndices;

    if (other.vertices)
    {
        // Debug assertions
        assert(numVertices);
        assert(numIndices);
        assert(other.vertices);
        assert(other.indices);
        assert(other.normals);

        // Copy vertices
        this->vertices = new vec3[numVertices];
        memcpy(this->vertices, other.vertices, numVertices * sizeof(vec3));

        // If we have vertices, we must also have normals and indices
        this->normals = new vec3[numVertices];
        memcpy(this->normals, other.normals, numVertices * sizeof(vec3));

        this->indices = new unsigned int[numIndices];
        memcpy(this->indices, other.indices, numIndices * sizeof(unsigned int));

        // Check if we also have texture coordinates
        if (other.texCoords)
        {
            this->texCoords = new vec2[numVertices];
            memcpy(this->texCoords, other.texCoords, numVertices * sizeof(vec2));
            
            // We must also have tangents
            assert(other.tangents);
            this->tangents = new vec3[numVertices];
            memcpy(this->tangents, other.tangents, numVertices * sizeof(vec3));
        }
        else
        {
            this->texCoords = NULL;
            this->tangents  = NULL;
        }
    }
    else
    {
        // Invalid ObjFile from a bad read
        this->vertices  = NULL;
        this->indices   = NULL;
        this->normals   = NULL;
        this->texCoords = NULL;
        this->tangents  = NULL;
    }
}

/*
 * Free Memory helper method
 */
void ObjFile::FreeMemory()
{
    if (vertices) 
    {
        delete[] vertices;
        vertices = NULL;
    }
    if (indices)
    {
        delete[] indices;
        indices = NULL;
    }
    if (normals)
    {
        delete[] normals;
        normals = NULL;
    }
    if (texCoords)
    {
        delete[] texCoords;
        texCoords = NULL;
    }
    if (tangents)
    {
        delete[] tangents;
        tangents = NULL;
    }
}

/*
 * Read from obj file
 */
void ObjFile::ReadObjFile(std::ifstream& file)
{
    // Allocate temporary lists on the heap
    vector<vec3>*                    vertexList   = new vector<vec3>();
    vector<vec3>*                    normalList   = new vector<vec3>();
    vector<vec2>*                    texCoordList = new vector<vec2>();
    vector<ObjAttribute>*            faceList     = new vector<ObjAttribute>();
    map<ObjAttribute, unsigned int>* indexMap     = new map<ObjAttribute, unsigned int>();

    // Read every line
    string line;
    while (file.good())
    {
        getline(file, line);

        stringstream parser(line);

        string identifier;
        parser >> identifier;

        if (identifier.compare(0, 1, "v") == 0 &&
            identifier.length() == 1)
        {
            // Vertex
            float x, y, z;
            parser >> x >> y >> z;
            vertexList->push_back(vec3(x, y, z));
        }
        else if (identifier.compare(0, 2, "vt") == 0 &&
            identifier.length() == 2)
        {
            // Vertex texture coordinate
            float x, y;
            parser >> x >> y;
            texCoordList->push_back(vec2(x, y));
        }
        else if (identifier.compare(0, 2, "vn") == 0 &&
            identifier.length() == 2)
        {
            // Vertex normal
            float x, y, z;
            parser >> x >> y >> z;
            normalList->push_back(normalize(vec3(x, y, z)));
        }
        else if (identifier.compare(0, 1, "f") == 0 &&
            identifier.length() == 1)
        {
            // Face definition, this can come in one of a couple forms:
            // vertex:                 f v1 v2 v3 ...
            // vertex/texcoord:        f v1/vt1 v2/vt2 v3/vt3 ...
            // vertex/texcoord/normal: f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
            // vertex//normal:         f v1//vn1 v2//vn2 v3//vn3 ...
            //
            // If the format is anything other than the simple vertex format,
            // vertices can be repeated with different texture coordinates or normals,
            // which would mean we need to duplicate the vertex.
            // In order to handle this, save each unique combination into a map
            // that we can look through later.
            int startingFaceIndex = faceList->size();
            for (int i = 0; i < 3; i++)
            {
                string word;
                parser >> word;

                ObjAttribute attribute(word);

                faceList->push_back(attribute);

                // We may need to end up duplicating vertices if some
                // vertices, texCoords or normals are reused.
                // For now, just save each unique combination
                if (indexMap->count(attribute) == 0)
                {
                    // Assign each combination a unique number, which will be
                    // used as its index for indexed rendering
                    (*indexMap)[attribute] = indexMap->size();
                }
            }

            // Handle additional face indices as triangle fans
            while (!parser.eof())
            {
                string word;
                parser >> word;

                if (!word.length() == 0)
                {
                    ObjAttribute attribute(word);

                    // Make the triangle fan by taking the first point,
                    // previous point and current point
                    faceList->push_back((*faceList)[startingFaceIndex]);
                    faceList->push_back((*faceList)[faceList->size() - 2]);
                    faceList->push_back(attribute);

                    if (indexMap->count(attribute) == 0)
                    {
                        (*indexMap)[attribute] = indexMap->size();
                    }
                }
            }
        }
        else
        {
            // Ignore everything else
        }
    }

    // Now we can process the data to create our arrays
    bool hasNormals   = normalList->size() > 0;
    bool hasTexCoords = texCoordList->size() > 0;
    numVertices = indexMap->size();
    numIndices  = faceList->size();

    // Make sure the data was good before proceeding
    if (numVertices == 0 || 
        numIndices == 0  ||
        (numIndices % 3) != 0)
    {
        cerr << "Obj file did not have a valid amount of vertices or indices" << endl;

        // Free allocated memory
        delete vertexList;
        delete normalList;
        delete texCoordList;
        delete faceList;
        delete indexMap;
        return;
    }

    // Create the arrays
    vertices    = new vec3[numVertices];
    normals     = new vec3[numVertices];
    texCoords   = hasTexCoords ? new vec2[numVertices] : NULL;
    tangents    = hasTexCoords ? new vec3[numVertices] : NULL;
    indices     = new unsigned int[numIndices];

    // Populate the vertex data arrays by traversing through indexMap
    for (map<ObjAttribute,unsigned int>::iterator iterator = indexMap->begin();
        iterator != indexMap->end();
        iterator++)
    {
        const ObjAttribute& attribute = iterator->first;
        unsigned int index = iterator->second;
        vertices[index] = (*vertexList)[attribute.vertex];
        if (hasNormals)
        {
            if (attribute.normal < 0)
            {
                // We have some geometry with normals and some without.
                // Throw out the normals and calculate all by hand
                hasNormals = false;
            }
            else
            {
                normals[index] = (*normalList)[attribute.normal];
            }
        }
        if (hasTexCoords)
        {
            // If we have some faces without texture coordinates,
            // fix them to use (0,0)
            texCoords[index] = attribute.texCoord >= 0 ?
                               (*texCoordList)[attribute.texCoord] :
                               vec2(0,0);
        }
    }

    // Populate our indices for indexed rendering by traversing through faceList
    for (int i = 0; i < numIndices; i++)
    {
        indices[i] = (*indexMap)[(*faceList)[i]];
    }

    // If we need to calculate our own normals, do that now
    if (!hasNormals)
    {
        CalculateNormals();
    }

    // If we have texture coordinates, we can calculate tangent vectors
    if (hasTexCoords)
    {
        CalculateTangents();
    }

    // Calculate a bounding box so we can translate and scale nicely
    minXYZ = maxXYZ = vertices[0];
    for (int i = 1; i < numVertices; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (vertices[i][j] < minXYZ[j])
            {
                minXYZ[j] = vertices[i][j];
            }
            else if (vertices[i][j] > maxXYZ[j])
            {
                maxXYZ[j] = vertices[i][j];
            }
        }
    }
    
    // Free allocated memory
    delete vertexList;
    delete normalList;
    delete texCoordList;
    delete faceList;
    delete indexMap;
}

/*
 * Get a scale factor
 */
float ObjFile::GetScaleFactor() const
{
    vec3 delta = maxXYZ - minXYZ;
    float max = MAX(delta[0], MAX(delta[1], delta[2]));
    return 2.0f / max;
}

/*
 * Calculate vertex normals
 */
void ObjFile::CalculateNormals()
{
    assert(normals);

    // Clear the normals array to 0's
    for (int i = 0; i < numVertices; i++)
    {
        normals[i] = vec3(0,0,0);
    }

    // Sum up face normals for each vertex
    for (int i = 0; i < numIndices; i += 3)
    {
        vec3 a = vertices[indices[i]];
        vec3 b = vertices[indices[i + 1]];
        vec3 c = vertices[indices[i + 2]];
        
        vec3 normal = normalize(cross(c - b, a - b));

        normals[indices[i]]     += normal;
        normals[indices[i + 1]] += normal;
        normals[indices[i + 2]] += normal;
    }

    // Average the face normals by normalizing thier sums
    for (int i = 0; i < numVertices; i++)
    {
        normals[i] = normalize(normals[i]);
    }
}

/*
 * Calculate tangents
 */
void ObjFile::CalculateTangents()
{
    assert(texCoords);
    assert(tangents);

    // The teapot obj model has some degenerate triangles whose summed tangents
    // come out to 0, but the individual face tangents are still non-zero.
    // If we encounter something like this, use the last good face tangent for
    // the vertex tangent instead of the average of the face tangents
    vec3* fallbackTangents = new vec3[numVertices];

    // Clear the tangents array to 0's
    for (int i = 0; i < numVertices; i++)
    {
        tangents[i]   = vec3(0,0,0);
    }

    // Sum up tangent vectors for each face
    for (int i = 0; i < numIndices; i += 3)
    {
        // Calculate the tangent vectors as described in this article:
        // http://www.terathon.com/code/tangent.html
        int index0 = indices[i];
        int index1 = indices[i+1];
        int index2 = indices[i+2];

        const vec3& p0 = vertices[index0];
        const vec3& p1 = vertices[index1];
        const vec3& p2 = vertices[index2];

        const vec2& tex0 = texCoords[index0];
        const vec2& tex1 = texCoords[index1];
        const vec2& tex2 = texCoords[index2];

        vec3 q1 = p1 - p0;
        vec3 q2 = p2 - p0;

        // Check for degenerate triangles (some appear in the teapot obj file)
        if (LengthSquared(q1) < 0.0001f ||
            LengthSquared(q2) < 0.0001f ||
            LengthSquared(q1 - q2) < 0.0001f)
        {
            continue;
        }

        float s1 = tex1[0] - tex0[0];
        float t1 = tex1[1] - tex0[1];
        float s2 = tex2[0] - tex0[0];
        float t2 = tex2[1] - tex0[1];

        // We are solving the following equations for T and B:
        // q1 = s1*T + t1*B
        // q2 = s2*T + t2*B
        //
        // This is a linear system that can be expressed as a product of matrices:
        // | q1.x q1.y q1.z |   | s1 t1 | | T.x T.y T.z |
        // | q2.x q2.y a2.z | = | s2 t2 | | B.x B.y B.z |
        //
        // Multiply both sides by the inverse of the (s,t) matrix:
        // | T.x T.y T.z |   ______1______ |  t2 -t1 | | q1.x q1.y q1.z |
        // | B.x B.y B.z | = s1*t2 - s2*t1 | -s2  s1 | | q2.x q2.y q2.z |
        //
        // Multiplying this out yields the following equations:
        // r = 1 / (s1*t2 - s2*t1)
        // T.x = r * (t2 * q1.x + -t1 * q2.x)
        // T.y = r * (t2 * q1.y + -t1 * q2.y)
        // T.z = r * (t2 * q1.z + -t1 * q2.z)
        // B.x = r * (-s2 * q1.x + s1 * q2.x)
        // B.y = r * (-s2 * q1.y + s1 * q2.y)
        // B.z = r * (-s2 * q1.z + s1 * q2.z)
        //
        // We don't actually need the bitangents, since they can be derived
        // in the vertex shader from the tangent and normal vectors
        float r = 1.0f / (s1 * t2 - s2 * t1);
        vec3 T = (r*t2) * q1 + (r*-t1) * q2;

        if (LengthSquared(T) < 0.0001f)
        {
            continue;
        }

        // Add the face tangents to the vertex tangents
        tangents[index0] += T;
        tangents[index1] += T;
        tangents[index2] += T;

        // Save the last good face tangent
        fallbackTangents[index0] = T;
        fallbackTangents[index1] = T;
        fallbackTangents[index2] = T;
    }

    // Average the face tangents by normalizing the summed tangents
    for (int i = 0; i < numVertices; i++)
    {
        // Ensure the tangents are orthogonal to the normal vector by
        // performing Gram-Schmidt algorithm.
        // T' = T - (N dot T)*N
        const vec3& tangent = (LengthSquared(tangents[i]) < 0.0001f) ?
                                  fallbackTangents[i] : 
                                  tangents[i];
        tangents[i] = normalize(tangent - dot(normals[i], tangent) * normals[i]);
    }

    delete[] fallbackTangents;
}
