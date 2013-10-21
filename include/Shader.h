#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <map>
#include <string>
#include <Angel.h>

// Forward declaration to prevent circular include loop
class VertexArray;

/**
 * \brief Class representing a shader program
 *
 * A shader program is a graphics executable consisting of a
 * vertex shader, fragment shader and potentially a geometry shader.
 * The individual shaders are written in GLSL and compiled at runtime
 * using the compiler provided by the graphics driver.
 *
 * Shaders have two types of variables: attribute and uniform.
 *
 * Attribute variables are typically arrays of values that are passed
 * to the vertex shader one at a time.  Common attribute variables are
 * vertices, normals and texture coordinates.  They are per-vertex variables.
 * These are typically bound through vertex array objects, which are independant
 * of the shader program.
 *
 * Uniform variables remain constant per draw call.  They may be updated freely
 * inbetween draws.  Common uniform variables are view and projection matrices
 * and light position, facing and color.  Uniforms are contained within the shader
 * program.  The values set for one shader program have no effect on another
 * shader program.  Uniforms retain their values even when a shader program
 * is no longer active
 *
 * \author Jonathan Henze
 */
class Shader
{
public:

    /**
     * \brief Structure of the information associated with shader variables
     */
    struct ParamInfo
    {
    public:

        /**
         * \brief Creates an empty parameter info object
         */
        ParamInfo()
            : size(0), type(0), location(0)
        {
        }

        /**
         * \brief Creates a new parameter info object
         *
         * \param[in] size - Array length of the parameter, if it is an array
         * \param[in] type - Type of the parameter
         * \param[in] location - Location of the parameter
         */
        ParamInfo(GLint size, GLenum type, GLint location)
            : size(size), type(type), location(location)
        {
        }

        GLint   size;     //!< Array length of the parameter if it is an array
        GLenum  type;     //!< Type of the parameter
        GLint   location; //!< Location for this parameter
    };

    typedef ParamInfo UniformInfo;   //!< Structure of uniform variable info
    typedef ParamInfo AttributeInfo; //!< Structure of attribute variable info
    typedef std::map<
        std::string, 
        UniformInfo> 
        UniformMap;                  //!< Map of uniforms
    typedef std::map<
        std::string, 
        AttributeInfo> 
        AttributeMap;                //!< Map of attributes

    /**
     * \brief Creates a shader program from the given source files
     *
     * The source code is loaded from the given file paths and then compiled
     * and linked into a complete shader program.  If the same source file
     * is used in multiple shader programs, it will only be compiled the
     * first time, after which it is saved in static memory.
     *
     * If there are compilation errors, a detailed log will be printed to stderr.
     * A shader that fails to compile will have a programID of 0, all others
     * will have a non-zero ID.
     *
     * \param[in] vertexShaderPath - Location of the vertex shader source file
     * \param[in] fragShaderPath   - Location of the fragment shader source file
     * \param[in] geoShaderPath    - Optionally, location of the geometry shader source file.
     *                               Ignored if set to NULL
     */
    Shader(const char* vertexShaderPath,
           const char* fragShaderPath,
           const char* geoShaderPath = NULL);

    /**
     * \brief Shader destructor
     */
    ~Shader();

    /**
     * \brief Binds a shader, causing it to be active
     *
     * When a shader program is active, it will be used on the graphics card
     * for rendering.  Only one shader program may be active at a time.
     * The shader will remain bound until another shader is bound.
     *
     * Binding a shader that failed compilation will result in the default
     * OpenGL fixed function shaders (pre-OpenGL2.0) being loaded.
     */
    void Bind();

    /**
     * \brief Unbinds any active shader, causing it to no longer be active
     */
    static void Unbind();

    /**
     * \brief Checks whether the shader program is currently active
     */
    bool IsBound() const;

    /**
     * \brief Gets the OpenGL location of an attribute variable
     *
     * \param[in] name - Name of the variable, exactly as written in the shader file
     *
     * \return Location of the uniform, which can be used with OpenGL functions
     * \return 0 if the shader doesn't have an attribute variable by that name
     */
    GLint GetAttributeLocation(const char* name) const;
    GLint GetAttributeLocation(const std::string& name) const;

    /**
     * \brief Gets the OpenGL location of a uniform variable
     *
     * \param[in] name - Name of the variable, exactly as written in the shader file
     *
     * \return Location of the uniform, which can be used with OpenGL functions
     * \return 0 if the shader doesn't have a uniform variable by that name
     */
    GLint GetUniformLocation(const char* name) const;
    GLint GetUniformLocation(const std::string& name) const;

    /**
     * \brief Sets the value of a uniform variable
     *
     * The shader must be currently bound, or this will fail
     *
     * \param[in] name - Name of the uniform, exactly as written in the shader file
     * \param[in] v#   - Components of a 1-4D float uniform
     */
    void SetUniform(const char* name, float v0);
    void SetUniform(const char* name, float v0, float v1);
    void SetUniform(const char* name, float v0, float v1, float v2);
    void SetUniform(const char* name, float v0, float v1, float v2, float v3);

    /**
     * \brief Sets the value of a uniform variable
     *
     * The shader must be currently bound, or this will fail
     *
     * \param[in] name - Name of the uniform, exactly as written in the shader file
     * \param[in] v#   - Components of a 1-4D int uniform
     */
    void SetUniform(const char* name, int v0);
    void SetUniform(const char* name, int v0, int v1);
    void SetUniform(const char* name, int v0, int v1, int v2);
    void SetUniform(const char* name, int v0, int v1, int v2, int v3);

    /**
     * \brief Sets the value of a uniform variable
     *
     * The shader must be currently bound, or this will fail
     *
     * \param[in] name - Name of the uniform, exactly as written in the shader file
     * \param[in] vec# - Vector containing the value of the uniform
     */
    void SetUniform(const char* name, const vec2& vec2);
    void SetUniform(const char* name, const vec3& vec3);
    void SetUniform(const char* name, const vec4& vec4);

    /**
     * \brief Sets the value of a uniform variable
     *
     * The shader must be currently bound, or this will fail
     *
     * \param[in] name   - Name of the uniform, exactly as written in the shader file
     * \param[in] matrix - Matrix value for the uniform
     */
    void SetUniform(const char* name, const mat2& matrix);
    void SetUniform(const char* name, const mat3& matrix);
    void SetUniform(const char* name, const mat4& matrix);

    /**
     * \brief Gets the OpenGL ID of the shader program
     *
     * \return The OpenGL ID of the shader program
     */
    inline GLuint GetProgramId() const { return programId; }

    /**
     * \brief Gets the OpenGL ID of the vertex shader
     *
     * \return The OpenGL ID of the vertex shader
     */
    inline GLuint GetVertexId() const { return vertexId; }

    /**
     * \brief Gets the OpenGL ID of the fragment shader
     *
     * \return The OpenGL ID of the fragment shader
     */
    inline GLuint GetFragId() const { return fragId; }

    /**
     * \brief Gets the OpenGL ID of the geometry shader
     *
     * \return The OpenGL ID of the geometry shader
     * \return 0 if there is no geometry shader in this shader program
     */
    inline GLuint GetGeoId() const { return geoId; }

    /**
     * \brief Gets an iterator over all the attributes in the shader
     *
     * Gets an iterator over all the attributes in the shader.  The iterator has
     * as its first (key) parameter a string containing the name of the attribute as
     * written in the shader source.  The second (value) parameter of contains
     * additional information about the parameter such as the location and type
     * of the attribute.  The values given by the iterator cannot be modified.
     *
     * \return Attribute iterator
     */
    inline AttributeMap::const_iterator GetAttributeIterator() const
    {
        return attributes.cbegin();
    }

    /**
     * \brief Gets an iterator at the end of the attributes
     *
     * \return Attribute iterator end
     */
    inline AttributeMap::const_iterator GetAttributeIteratorEnd() const
    {
        return attributes.cend();
    }

    /**
     * \brief Gets an iterator over all the uniforms in the shader
     *
     * Gets an iterator over all the uniforms in the shader.  The iterator has
     * as its first (key) parameter a string containing the name of the uniform as
     * written in the shader source.  The second (value) parameter of contains
     * additional information about the parameter such as the location and type
     * of the uniform.  The values given by the iterator cannot be modified.
     *
     * \return Uniform iterator
     */
    inline UniformMap::const_iterator GetUniformIterator() const
    {
        return uniforms.cbegin();
    }

    /**
     * \brief Gets an iterator at the end of the uniforms
     *
     * \return Uniform iterator end
     */
    inline UniformMap::const_iterator GetUniformIteratorEnd() const
    {
        return uniforms.cend();
    }

private:

    /**
     * \brief Shader IDs
     */
    GLuint vertexId, fragId, geoId;

    /**
     * \brief Shader program ID
     */
    GLuint programId;

    /**
     * \brief Information about the uniforms mapped by the variable name
     */
    mutable UniformMap uniforms;

    /**
     * \brief Information about the attributes mapped by the variable name
     */
    mutable AttributeMap attributes;

    /**
     * \brief The currently bound shader
     */
    static GLuint activeProgramId;

    /**
     * \brief Retrives data about the variables in the shader
     */
    void GetShaderInfo();

    /**
     * \brief Helper function to compile a shader source file
     *
     * A detailed log will be printed to stderr if the shader fails compilation
     *
     * \param[in] shaderType - One of GL_VERTEX_SHADER, GL_FRAGMENT_SHADER,
     *                         or GL_GEOMETRY_SHADER
     * \param[in] srcPath    - Path to shader source file
     *
     * \return The OpenGL ID of the shader
     * \return 0 if the shader failed compilation
     */
    static GLuint CompileShader(GLenum shaderType, const char* srcPath);

    /**
     * \brief Links a previously compiled shaders into a shader program
     *
     * A detailed log will be printed to stderr if the program fails linking
     *
     * \param[in] vertexId - OpenGL ID of the vertex shader
     * \param[in] fragId   - OpenGL ID of the fragment shader
     * \param[in] geoId    - Optionally, OpenGL ID of the geometry shader
     *
     * \return The OpenGL ID of the shader program
     * \return 0 if the program failed linking
     */
    static GLuint LinkProgram(GLuint vertexId, GLuint fragId, GLuint geoId = 0);

    Shader(const Shader&);            //!< No copy constructor
    Shader& operator=(const Shader&); //!< No assignment operator
};

#endif
