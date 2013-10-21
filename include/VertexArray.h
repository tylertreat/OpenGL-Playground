#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>
#include <map>
#include <string>
#include <Angel.h>
#include "Shader.h"

/**
 * \brief Class for managing OpenGL vertex array objects (VAOs)
 *
 * A vertex array is a collection of vertex attribute values that
 * describe all of the geometry of a particular model or other object.
 * This commonly includes vertex positions, normals and texture coordinates.
 * Binding the vertex array causes the data to be used when executing a
 * shader program.
 *
 * \author Jonathan Henze
 */
class VertexArray
{
public:

    /**
     * \brief Creates an empty vertex array
     */
    VertexArray();

    /**
     * \brief Vertex array destructor
     */
    ~VertexArray();

    /**
     * \brief Binds a vertex array, causing it to be active
     *
     * This binds the vertex array so that its contents will be used
     * in the next draw call.  Only one vertex array may be bound at a time.
     * Binding another vertex array will cause this array to be unbound.
     * 
     * All of the attributes in the vertex array will be matched to attributes
     * of the same name in the shader.
     *
     * \param[in] shader - Shader to bind to.  Must be currently bound
     */
    void Bind(const Shader& shader);

    /**
     * \brief Unbinds the vertex array, causing it to no longer be active
     */
    static void Unbind();

    /**
     * \brief Checks if the vertex array is currently bound
     */
    bool IsBound() const;

    /**
     * \brief Adds an attribute to the vertex array
     *
     * The vertex array must be bound before calling this.
     * If the given attribute has already been added to the vertex array,
     * this will overwrite the data for that attribute.  The data is copied
     * from the packed array, so the array can be deleted after calling
     * this function.  All attributes in the same vertex array must have
     * the same number of elements.
     *
     * \param[in] name   - Name of the attribute exactly as it appears
     *                     in the shader source
     * \param[in] data   - Data for the attribute
     * \param[in] length - Number of elements in the data array
     */
    void AddAttribute(const char* name, const vec2* data, int length);
    void AddAttribute(const char* name, const vec3* data, int length);
    void AddAttribute(const char* name, const vec4* data, int length);

    /**
     * \brief Adds an attribute to the vertex array
     *
     * The vertex array must be bound before calling this.
     * If the given attribute has already been added to the vertex array,
     * this will overwrite the data for that attribute.  The data is copied
     * from the packed array, so the array can be deleted after calling
     * this function.  All attributes in the same vertex array must have
     * the same number of elements.
     *
     * \param[in] name          - Name of the attribute exactly as it appears
     *                            in the shader source
     * \param[in] data          - Data for the attribute
     * \param[in] numComponents - Number of components per element.
     *                            Ex: for vec3's, use 3
     * \param[in] length        - Number of complete elements in the data array
     */
    void AddAttribute(const char* name, const int*    data, int numComponents, int length);
    void AddAttribute(const char* name, const short*  data, int numComponents, int length);
    void AddAttribute(const char* name, const char*   data, int numComponents, int length);
    void AddAttribute(const char* name, const float*  data, int numComponents, int length);
    void AddAttribute(const char* name, const double* data, int numComponents, int length);
    void AddAttribute(const char* name, const unsigned int*   data, int numComponents, int length);
    void AddAttribute(const char* name, const unsigned short* data, int numComponents, int length);
    void AddAttribute(const char* name, const unsigned char*  data, int numComponents, int length);

    /**
     * \brief Adds indices to the vertex array for indexed rendering
     *
     * The vertex array must be bound before calling this.
     * If the vertex array already has indices attached, this will overwrite them.
     * The indices are copied by this call, so it is okay to delete the indices
     * array after calling this function.
     */
    void AddIndices(const unsigned int*   indices, int length);
    void AddIndices(const unsigned short* indices, int length);
    void AddIndices(const unsigned char*  indices, int length);

    /**
     * \brief Draws the vertex data using the currently bound shader
     *
     * Draws using the currently bound shader.  The vertex array be currently
     * bound using the same shader.
     *
     * \param[in] mode - Type of primitive to use while drawing.
     *                   Must be one of: GL_POINTS,
     *                                   GL_LINE_STRIP,
     *                                   GL_LINE_LOOP,
     *                                   GL_LINES,
     *                                   GL_LINE_STRIP_ADJACENCY,
     *                                   GL_LINES_ADJACENCY,
     *                                   GL_TRIANGLE_STRIP,
     *                                   GL_TRIANGLE_FAN,
     *                                   GL_TRIANGLES,
     *                                   GL_TRIANGLE_STRIP_ADJACENCY,
     *                                   GL_TRIANGLES_ADJACENCY
     */
    void Draw(GLenum mode) const;

    /**
     * \brief Gets the number of vertices for the vertex array
     *
     * \return Number of vertices in the vertex array
     */
    inline int NumVertices() const { return numVertices; }

    /**
     * \brief Gets the number of indices for the vertex array
     *
     * \return Number of indices in the vertex array
     */
    inline int NumIndices() const { return numIndices; }

    /**
     * \brief Checks if the vertex array has indices for indexed rendering
     *
     * \return Whether the vertex array has indices for indexed rendering
     */
    inline bool HasIndices() const { return indicesId != 0; }

    /**
     * \brief Gets the type of the indices if the vertex array has any
     *
     * The return value is undefined if the vertex array does not have indices
     *
     * \return The type of the indices attached to the vertex array
     */
    inline GLenum IndicesType() const { return indicesType; }

private:

    /**
     * \brief Structure of a vertex attribute
     */
    struct Attribute
    {
    public:

        /**
         * \brief The buffer ID of the attribute in the vertex array
         */
        GLuint bufferId;

        /**
         * \brief The integral data type of the attribute's data
         *
         * Must be one of: GL_BYTE,
         *                 GL_UNSIGNED_BYTE,
         *                 GL_SHORT,
         *                 GL_UNSIGNED_SHORT,
         *                 GL_INT,
         *                 GL_UNSIGNED_INT,
         *                 GL_FLOAT,
         *                 GL_DOUBLE
         */
        GLenum type;

        /**
         * \brief Number of components of the attribute's data
         *
         * Must be 1, 2, 3 or 4
         */
        GLint numComponents;

        /**
         * \brief Byte offset between consecutive elements, usually 0 (tightly packed)
         */
        GLsizei stride;

        /**
         * \brief Default constructor
         */
        Attribute()
            : bufferId(0)
        {
        }
    };

    /**
     * \brief Structure to keep track of a vertex array ID
     */
    struct VertexArrayId
    {
    public:

        GLuint vaoId; //!< ID of the VAO object
        bool stale;   //!< Flag to state the VAO needs to be updated

        /**
         * \brief Default constructor
         */
        VertexArrayId()
            : vaoId(0), stale(true)
        {
        }

        /**
         * \brief Creates a VertexArrayId object
         *
         * \param[in] vaoId - ID of the VAO object
         * \param[in] stale - Whether the VAO needs recalculating
         */
        VertexArrayId(GLuint vaoId, bool stale)
            : vaoId(vaoId), stale(stale)
        {
        }
    };

    /**
     * \brief Mapping of shader program IDs to VAO IDs
     */
    typedef std::map<GLuint, VertexArrayId> VertexArrayIdMap;

    /**
     * \brief A VAO for each shader this has been attached to
     */
    VertexArrayIdMap vertexArrayIds;

    /**
     * \brief number of vertices in the array
     */
    int numVertices;

    /**
     * \brief Number of indices in the array, if any
     */
    int numIndices;

    /**
     * \brief OpenGL ID of the indices buffer (or 0 if none)
     */
    GLuint indicesId;

    /**
     * \brief Type of data in the indices buffer
     */
    GLenum indicesType;
    
    /**
     * \brief Type for maps of attributes
     */
    typedef std::map<std::string, Attribute> AttributeMap;

    /**
     * \brief Map of added attributes
     */
    AttributeMap attributes;

    /**
     * \brief ID of the vertex array
     */
    int id;

    /**
     * \brief Active vertex array, if any
     */
    static int activeVertexArrayId;

    /**
     * \brief Used to assign IDs to the vertex arrays
     */
    static int vertexArrayIdCounter;

    /**
     * \brief Creates a VAO object for pairing with a shader
     *
     * \param[in] shader - Shader to pair with
     */
    void GenerateVAO(const Shader& shader);

    /**
     * \brief Marks all previously created VAOs as stale
     */
    void MarkVAOsAsStale();

    /**
     * \brief Common method for adding attributes of an arbitrary type
     *
     * \tparam T - Primitive type for a component in the attribute
     *
     * \param[in] name - Name of the attribute
     * \param[in] data - Data for the attribute
     * \param[in] type - One of: GL_BYTE,
     *                           GL_UNSIGNED_BYTE,
     *                           GL_SHORT,
     *                           GL_UNSIGNED_SHORT,
     *                           GL_INT,
     *                           GL_UNSIGNED_INT,
     *                           GL_HALF_FLOAT,
     *                           GL_FLOAT,
     *                           GL_DOUBLE
     */
    template<class T>
    void AddAttributeCommon(
        const char* name, 
        const T* data, 
        int numComponents, 
        int length,
        GLsizei stride,
        GLenum type);

    /**
     * \brief Common method for adding indices
     *
     * \tparam T - Integral type of the indices
     *
     * \param[in] indices - Array of indices
     * \param[in] length  - Number of indices
     * \param[in] type    - One of: GL_UNSIGNED_BYTE,
     *                              GL_UNSIGNED_SHORT,
     *                              GL_UNSIGNED_INT
     */
    template<class T>
    void AddIndicesCommon(const T* indices, int length, GLenum type);

    VertexArray(const VertexArray&);            //!< No copy constructor
    VertexArray& operator=(const VertexArray&); //!< No assignment operator
};

#endif
