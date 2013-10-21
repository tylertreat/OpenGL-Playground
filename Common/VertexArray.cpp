#include <cassert>
#include "VertexArray.h"

int VertexArray::activeVertexArrayId = 0;
int VertexArray::vertexArrayIdCounter = 1;

/**
 * \brief Comparator to sort c-strings by lexicographical order
 *
 * \param[in] str1 - First string
 * \param[in] str2 - Second string
 * \return Whether the first c-string comes first in lexicographical order
 */
bool LexicographicalOrder(const char* str1, const char* str2)
{
    assert(str1 != NULL && str2 != NULL);

    int cmp = strcmp(str1, str2);
    return cmp < 0;
}

/*
 * Default constructor
 */
VertexArray::VertexArray()
    : numVertices(0), 
    numIndices(0),
    indicesId(0), 
    id(vertexArrayIdCounter++),
    attributes(),//LexicographicalOrder)
    vertexArrayIds()
{
}

/*
 * Destructor
 */
VertexArray::~VertexArray()
{
    // Make sure we are unbound
    if (IsBound())
    {
        Unbind();
    }

    // Delete the vertex arrays
    for (VertexArrayIdMap::iterator it = vertexArrayIds.begin();
         it != vertexArrayIds.end();
         it++)
    {
        glDeleteVertexArrays(1, &(it->second.vaoId));
    }
    vertexArrayIds.clear();

    // Delete the attribute buffers
    for(AttributeMap::iterator it = attributes.begin();
        it != attributes.end();
        it++)
    {
        glDeleteBuffers(1, &(it->second.bufferId));
    }
    attributes.clear();

    // Delete the indices buffer, if we have one
    if (indicesId != 0)
    {
        glDeleteBuffers(1, &indicesId);
    }
}

/*
 * Bind
 */
void VertexArray::Bind(const Shader& shader)
{
    assert(shader.IsBound());

    // Check if we have a current VAO for this shader
    if (vertexArrayIds.count(shader.GetProgramId()) == 0 ||
        vertexArrayIds[shader.GetProgramId()].stale)
    {
        // Create or update the VAO for the shader
        GenerateVAO(shader);
    }

    // Bind the VAO for this shader
    glBindVertexArray(vertexArrayIds[shader.GetProgramId()].vaoId);
    activeVertexArrayId = id;
}

/*
 * Unbind
 */
void VertexArray::Unbind()
{
    glBindVertexArray(0);
    activeVertexArrayId = 0;
}

/*
 * IsBound
 */
bool VertexArray::IsBound() const
{
    return activeVertexArrayId == id;
}

/*
 * Add Attribute vec2
 */
void VertexArray::AddAttribute(const char* name, const vec2* data, int length)
{
    AddAttributeCommon(
        name, 
        (float*)&(data[0].x), // Point to the first component of the first vector
        2, 
        length, 
        sizeof(vec2) - 2*sizeof(float), // Ideally 0
        GL_FLOAT);
}

/*
 * Add Attribute vec3
 */
void VertexArray::AddAttribute(const char* name, const vec3* data, int length)
{
    AddAttributeCommon(
        name, 
        (float*)&(data[0].x), // Point to the first component of the first vector
        3, 
        length, 
        sizeof(vec3) - 3*sizeof(float), // Ideally 0
        GL_FLOAT);
}

/*
 * Add Attribute vec4
 */
void VertexArray::AddAttribute(const char* name, const vec4* data, int length)
{
    AddAttributeCommon(
        name, 
        (float*)&(data[0].x), // Point to the first component of the first vector
        4, 
        length, 
        sizeof(vec4) - 4*sizeof(float), // Ideally 0
        GL_FLOAT);
}

/*
 * Add Attribute int
 */
void VertexArray::AddAttribute(const char* name, const int* data, int numComponents, int length)
{
    AddAttributeCommon(
        name, 
        data,
        numComponents,
        length,
        0,
        GL_INT);
}

/*
 * Add Attribute short
 */
void VertexArray::AddAttribute(const char* name, const short* data, int numComponents, int length)
{
    AddAttributeCommon(
        name, 
        data,
        numComponents,
        length,
        0,
        GL_SHORT);
}

/*
 * Add Attribute char
 */
void VertexArray::AddAttribute(const char* name, const char* data, int numComponents, int length)
{
    AddAttributeCommon(
        name, 
        data,
        numComponents,
        length,
        0,
        GL_BYTE);
}

/*
 * Add Attribute float
 */
void VertexArray::AddAttribute(const char* name, const float* data, int numComponents, int length)
{
    AddAttributeCommon(
        name, 
        data,
        numComponents,
        length,
        0,
        GL_FLOAT);
}

/*
 * Add Attribute double
 */
void VertexArray::AddAttribute(const char* name, const double* data, int numComponents, int length)
{
    AddAttributeCommon(
        name, 
        data,
        numComponents,
        length,
        0,
        GL_DOUBLE);
}

/*
 * Add Attribute unsigned int
 */
void VertexArray::AddAttribute(const char* name, const unsigned int* data, int numComponents, int length)
{
    AddAttributeCommon(
        name, 
        data,
        numComponents,
        length,
        0,
        GL_UNSIGNED_INT);
}

/*
 * Add Attribute unsigned short
 */
void VertexArray::AddAttribute(const char* name, const unsigned short* data, int numComponents, int length)
{
    AddAttributeCommon(
        name, 
        data,
        numComponents,
        length,
        0,
        GL_UNSIGNED_SHORT);
}

/*
 * Add Attribute unsigned char
 */
void VertexArray::AddAttribute(const char* name, const unsigned char* data, int numComponents, int length)
{
    AddAttributeCommon(
        name, 
        data,
        numComponents,
        length,
        0,
        GL_UNSIGNED_BYTE);
}

/*
 * Add Attribute common
 */
template<class T>
void VertexArray::AddAttributeCommon(
    const char* name, 
    const T*    data, 
    int         numComponents, 
    int         length,
    GLsizei     stride,
    GLenum      type)
{
    // We cannot be currently bound for drawing while making changes to the
    // data in our VertexArray
    assert(!IsBound());

    // All attributes added must have the same number of vertices
    assert(numVertices == 0 || numVertices == length);

    // If this is our first attribute, save the number of vertices
    if (numVertices == 0)
    {
        numVertices = length;
    }

    std::string str(name);
    Attribute attribute;

    // Check if we already have the attribute
    if (attributes.count(str) != 0)
    {
        // We are overwriting an existing attribute with new data.
        // We can reuse the same buffer object though
        attribute = attributes[str];

        if (attribute.type != type ||
            attribute.numComponents != numComponents ||
            attribute.stride != stride)
        {
            // The attribute data is being replaced with data of a different
            // format, invalidating previously created VAOs
            MarkVAOsAsStale();
        }
    }
    else
    {
        // This is the first time we are adding this attribute,
        // we will need a new buffer
        glGenBuffers(1, &attribute.bufferId);

        // This invalidates previously created VAOs
        MarkVAOsAsStale();
    }

    // Add the data to the attribute's buffer.
    // This will destroy any existing data in the buffer
    glBindBuffer(GL_ARRAY_BUFFER, attribute.bufferId);
    glBufferData(GL_ARRAY_BUFFER,
        numComponents * length * sizeof(T),
        data,
        GL_STATIC_DRAW);
    
    // Save the properites of the data
    attribute.type = type;
    attribute.numComponents = numComponents;
    attribute.stride = stride;

    // Save the attribute to our map
    attributes[str] = attribute;
}

/*
 * Generate a vertex array object
 */
void VertexArray::GenerateVAO(const Shader& shader)
{
    GLuint vaoId;

    // Check if a VAO already exists or if we need to create a new one
    if (vertexArrayIds.count(shader.GetProgramId()) == 0)
    {
        // No existing VAO, create a new one
        glGenVertexArrays(1, &vaoId);
    }
    else
    {
        // Get the existing VAO
        vaoId = vertexArrayIds[shader.GetProgramId()].vaoId;
    }

    // Bind the VAO, so we can modify it
    glBindVertexArray(vaoId);

    // Link all of our attributes to the shader
    for (Shader::AttributeMap::const_iterator it = shader.GetAttributeIterator();
         it != shader.GetAttributeIteratorEnd();
         it++)
    {
        const std::string& name = it->first;

        // We have to skip attributes that we don't have data for.
        // These attributes will not be enabled
        if (attributes.count(name) == 0)
        {
            // Warn about the missing attribute -- might be due to a typo
            std::cerr << "Attribute " << name << " is not defined in the " <<
                "vertex array" << std::endl;
            continue;
        }

        // Get attribute info
        const Attribute& attribute = attributes.at(name);

        // Get where the attribute is in the shader
        GLint location = it->second.location;

        // Bind the attribute's buffer so we can reference it
        glBindBuffer(GL_ARRAY_BUFFER, attribute.bufferId);

        // Enable the attribute so it can be used to render
        glEnableVertexAttribArray((GLuint)location);

        // Set it to point to the start of the attribute buffer
        glVertexAttribPointer(
            (GLuint)location,
            attribute.numComponents,
            attribute.type,
            GL_FALSE,
            attribute.stride,
            0);
    }

    // Unbind the last attribute buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // If we have indices, attach those as well
    if (HasIndices())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
    }

    // Add or update the VertexArrayId entry
    vertexArrayIds[shader.GetProgramId()] = VertexArrayId(vaoId, false);
}

/*
 * Mark all VAOs as stale
 */
void VertexArray::MarkVAOsAsStale()
{
    for (VertexArrayIdMap::iterator it = vertexArrayIds.begin();
         it != vertexArrayIds.end();
         it++)
    {
        it->second.stale = true;
    }
}

/*
 * Add Indices unsigned int
 */
void VertexArray::AddIndices(const unsigned int* indices, int length)
{
    AddIndicesCommon(indices, length, GL_UNSIGNED_INT);
}

/*
 * Add Indices unsigned short
 */
void VertexArray::AddIndices(const unsigned short* indices, int length)
{
    AddIndicesCommon(indices, length, GL_UNSIGNED_SHORT);
}

/*
 * Add Indices unsigned char
 */
void VertexArray::AddIndices(const unsigned char* indices, int length)
{
    AddIndicesCommon(indices, length, GL_UNSIGNED_BYTE);
}

/*
 * Add Indices common
 */
template<class T>
void VertexArray::AddIndicesCommon(const T* indices, int length, GLenum type)
{
    // We cannot be currently bound for drawing while making changes to the
    // data in our VertexArray
    assert(!IsBound());

    // If this is our first addition to the array, save the length
    if (numVertices == 0)
    {
        numVertices = length;
    }

    // Create the indices buffer if it doesn't already exist
    if (indicesId == 0)
    {
        glGenBuffers(1, &indicesId);

        // This invalidates any previously created VAOs
        MarkVAOsAsStale();
    }

    // Bind the indices buffer so we can modify its contents
    glBindBuffer(GL_ARRAY_BUFFER, indicesId);

    // Save the indices to the buffer
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(T), indices, GL_STATIC_DRAW);

    // Unbind the indices buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Save the number and type of the indices for reference later
    numIndices  = length;
    indicesType = type;
}

/*
 * Draw
 */
void VertexArray::Draw(GLenum mode) const
{
    // We must be bound
    assert(IsBound());

    // Use indexed rendering if we can, otherwise use sequential rendering
    if (HasIndices())
    {
        glDrawElements(mode, NumIndices(), IndicesType(), NULL);
    }
    else
    {
        glDrawArrays(mode, 0, NumVertices());
    }
}

