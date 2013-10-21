#include <cassert>
#include <iostream>
#include <string>
#include "Shader.h"

/*
 * The currently active shader
 */
GLuint Shader::activeProgramId = 0;

/*
 * Shader constructor
 */
Shader::Shader(const char* vertexShaderPath,
               const char* fragShaderPath,
               const char* geoShaderPath)
               : uniforms(),
               attributes()
{
    assert(vertexShaderPath && fragShaderPath);

    // Compile the individual shaders, or load them if they have been previously
    // compiled for other shader programs
    vertexId  = CompileShader(GL_VERTEX_SHADER, vertexShaderPath);
    fragId    = CompileShader(GL_FRAGMENT_SHADER, fragShaderPath);
    geoId     = geoShaderPath ?
                CompileShader(GL_GEOMETRY_SHADER, geoShaderPath) :
                0;
    
    // Link the shader program
    programId = LinkProgram(vertexId, fragId, geoId);

    // If any of the above steps failed, programId will be 0.
    // Otherwise, look up all the uniforms in the shader
    if (programId != 0)
    {
        GetShaderInfo();
    }
}

/*
 * Shader destructor
 */
Shader::~Shader()
{
    if (IsBound() && programId != 0)
    {
        Unbind();
    }

    // Delete the shader program.
    // This will automatically detach any linked shaders,
    // but will not delete them.
    if (programId != 0)
    {
        glDeleteProgram(programId);
    }

    // Delete the individual shader programs
    glDeleteShader(vertexId);
    glDeleteShader(fragId);
    if (geoId != 0)
    {
        glDeleteShader(geoId);
    }

    // Delete all info objects
    uniforms.clear();
    attributes.clear();
}

/*
 * Bind
 */
void Shader::Bind()
{
    if (!IsBound())
    {
        glUseProgram(programId);
        Shader::activeProgramId = programId;
    }
}

/*
 * Unbind the shader
 */
void Shader::Unbind()
{
    glUseProgram(0);
    Shader::activeProgramId = 0;
}

/*
 * Is bound
 */
bool Shader::IsBound() const
{
    return Shader::activeProgramId == programId;
}

/*
 * Get attribute location c-string
 */
GLint Shader::GetAttributeLocation(const char* name) const
{
    return GetAttributeLocation(std::string(name));
}

/*
 * Get attribute location string
 */
GLint Shader::GetAttributeLocation(const std::string& name) const
{
    if (attributes.count(name) != 0)
    {
        return attributes.at(name).location;
    }
    else
    {
        // Attribute was not in our map, it probably doesn't exist
        GLint location = glGetAttribLocation(programId, name.c_str());
        if (location == -1)
        {
            // Attribute does not exist in the shader
            std::cerr << "Attribute " << name << 
                " is not referenced in the shaders" << std::endl;
        }

        // Add the attribute to our map to prevent duplicate warnings.
        attributes[name] = AttributeInfo(0, 0, location);
        return location;
    }
}

/*
 * Get uniform location c-string
 */
GLint Shader::GetUniformLocation(const char* name) const
{
    return GetUniformLocation(std::string(name));
}

/*
 * Get uniform location string
 */
GLint Shader::GetUniformLocation(const std::string& name) const
{
    if (uniforms.count(name) != 0)
    {
        return uniforms.at(name).location;
    }
    else
    {
        // Uniform is not in our map.
        // It either doesn't exist, or it could be a structure member
        // or an array element access
        GLint location = glGetUniformLocation(programId, name.c_str());
        if (location == -1)
        {
            // Uniform does not exist
            std::cerr << "Uniform " << name << 
                " is not referenced in the shaders" << std::endl;
        }

        // Add the uniform to our map to prevent duplicate warnings
        uniforms[name] = UniformInfo(0, 0, location);
        return location;
    }
}

/*
 * Set uniform 1f
 */
void Shader::SetUniform(const char* name, float v0)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform1f(GetUniformLocation(name), v0);
    }
}

/*
 * Set uniform 2f
 */
void Shader::SetUniform(const char* name, float v0, float v1)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform2f(GetUniformLocation(name), v0, v1);
    }
}

/*
 * Set uniform 3f
 */
void Shader::SetUniform(const char* name, float v0, float v1, float v2)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform3f(GetUniformLocation(name), v0, v1, v2);
    }
}

/*
 * Set uniform 4f
 */
void Shader::SetUniform(const char* name, float v0, float v1, float v2, float v3)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
    }
}

/*
 * Set uniform 1i
 */
void Shader::SetUniform(const char* name, int v0)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform1i(GetUniformLocation(name), v0);
    }
}

/*
 * Set uniform 2i
 */
void Shader::SetUniform(const char* name, int v0, int v1)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform2i(GetUniformLocation(name), v0, v1);
    }
}

/*
 * Set uniform 3i
 */
void Shader::SetUniform(const char* name, int v0, int v1, int v2)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform3i(GetUniformLocation(name), v0, v1, v2);
    }
}

/*
 * Set uniform 4i
 */
void Shader::SetUniform(const char* name, int v0, int v1, int v2, int v3)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform4i(GetUniformLocation(name), v0, v1, v2, v3);
    }
}

/*
 * Set uniform vec2
 */
void Shader::SetUniform(const char* name, const vec2& vec2)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform2f(GetUniformLocation(name), vec2[0], vec2[1]);
    }
}

/*
 * Set uniform vec3
 */
void Shader::SetUniform(const char* name, const vec3& vec3)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform3f(GetUniformLocation(name), vec3[0], vec3[1], vec3[2]);
    }
}

/*
 * Set uniform vec4
 */
void Shader::SetUniform(const char* name, const vec4& vec4)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniform4f(GetUniformLocation(name), vec4[0], vec4[1], vec4[2], vec4[3]);
    }
}

/*
 * Set uniform mat2
 */
void Shader::SetUniform(const char* name, const mat2& matrix)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniformMatrix2fv(GetUniformLocation(name), 1, GL_TRUE, matrix);
    }
}

/*
 * Set uniform mat3
 */
void Shader::SetUniform(const char* name, const mat3& matrix)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_TRUE, matrix);
    }
}

/*
 * Set uniform mat4
 */
void Shader::SetUniform(const char* name, const mat4& matrix)
{
    assert(IsBound());
    if (programId != 0)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_TRUE, matrix);
    }
}

/*
 * Get parameter info for the shader
 */
void Shader::GetShaderInfo()
{
    GLint numUniforms;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &numUniforms);

    GLint maxUniformNameLength;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

    // Read all the active uniforms
    for (int i = 0; i < numUniforms; i++)
    {
        char* name = new char[maxUniformNameLength];
        GLenum type;
        GLint size;
        glGetActiveUniform(
            programId,
            i,
            maxUniformNameLength,
            NULL,
            &size,
            &type,
            name);

        // Look up the location of the uniform
        GLuint location = glGetUniformLocation(programId, name);

        // Copy the name to a string object
        std::string str(name);
        str.shrink_to_fit();
        delete[] name;
        
        // Add to the uniforms array
        uniforms[str] = UniformInfo(size, type, location);
    }

    GLint numAttributes;
    glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numAttributes);

    GLint maxAttributeNameLength;
    glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);

    // Read all the active attributes
    for (int i = 0; i < numAttributes; i++)
    {
        char* name = new char[maxAttributeNameLength];
        GLenum type;
        GLint size;
        glGetActiveAttrib(
            programId,
            i,
            maxAttributeNameLength,
            NULL,
            &size,
            &type,
            name);

        // Look up the location of the attribute
        GLuint location = glGetAttribLocation(programId, name);

        // Copy the name to a string object
        std::string str(name);
        str.shrink_to_fit();
        delete[] name;

        attributes[str] = AttributeInfo(size, type, location);
    }
}

/*
 * Compile shader
 */
GLuint Shader::CompileShader(GLenum shaderType, const char* srcPath) 
{
    // Load the shader source file to memory
    FILE* file = fopen(srcPath, "rb");
    if (file == NULL)
    {
        std::cerr << "Couldn't open shader file " << srcPath << std::endl;
        return 0;
    }

    fseek(file, 0, SEEK_END);
    GLint size = (GLint)ftell(file);
    fseek(file, 0, SEEK_SET);
    char* src = new char[size+1];

    if (fread(src, size, 1, file) != 1)
    {
        std::cerr << "Error reading shader file " << srcPath << std::endl;
        fclose(file);
        delete[] src;
        return 0;
    }
    fclose(file);

    src[size] = '\0';

    // Create the shader
    GLuint shaderId = glCreateShader(shaderType);

    // Attach the source code to the shader
    glShaderSource(shaderId, 1, (const GLchar**)&src, &size);

    // The source code is copied to the shader, we no longer need the buffer
    delete[] src;

    // Compile the shader
    glCompileShader(shaderId);

    // Check for errors
    GLint compileStatus;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
        // Read back the error log
        GLint logLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetShaderInfoLog(shaderId, logLength, NULL, log);
        std::cerr<< "Shader compilation of " << srcPath << " failed:" << std::endl;
        std::cerr << log << std::endl;

        // Free used memory
        delete[] log;
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

/*
 * Link shader program
 */
GLuint Shader::LinkProgram(GLuint vertexId, GLuint fragId, GLuint geoId)
{
    // Don't try to link if the vertex or fragment shader failed to compile
    if (vertexId == 0 || fragId == 0)
    {
        // 0 represents an empty program from a failed link
        return 0;
    }

    // Create the shader program
    GLuint programId = glCreateProgram();

    // Attach the shaders
    glAttachShader(programId, vertexId);
    glAttachShader(programId, fragId);
    if (geoId)
    {
        glAttachShader(programId, geoId);
    }

    // Link the shader program
    glLinkProgram(programId);

    // Check for errors
    GLint linkStatus;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        // Read back the error log
        GLint logLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetShaderInfoLog(programId, logLength, NULL, log);
        std::cerr<< "Link of shader program failed:" << std::endl;
        std::cerr << log << std::endl;

        // Free used memory
        delete[] log;
        glDeleteProgram(programId);
        return 0;
    }
    return programId;
}
