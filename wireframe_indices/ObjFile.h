#ifndef OBJFILE_H
#define OBJFILE_H

#include <fstream>
#include <Angel.h>

/**
 * \brief Class to read a model from an obj file
 *
 * \author Jonathan Henze
 */
class ObjFile
{
public:

    /**
     * \brief Imports a model from an obj file
     *
     * \param[in] filename - File name and path to read from
     */
    ObjFile(const char* filename);

    /**
     * \brief Creates a deep copy of an ObjFile
     *
     * \param[in] other - ObjFile to copy
     */
    ObjFile(const ObjFile& other);

    /**
     * \brief Assigns this ObjFile to be a deep copy of an ObjFile
     *
     * \param[in] other - ObjFile to copy
     */
    ObjFile& operator=(const ObjFile& other);

    /**
     * \brief ObjFile destructor
     */
    ~ObjFile();

    /**
     * \brief Gets the array of vertices in the model
     *
     * \return The array of vertices in the model
     * \return Null if the obj file was not read successfully
     */
    inline const vec3* GetVertices() const 
    { 
        return vertices; 
    }

    /**
     * \brief Gets the array of normals in the model
     *
     * \return The array of normals in the model
     * \return Null if the obj file was not read successfully
     */
    inline const vec3* GetNormals() const 
    { 
        return normals; 
    }

    /**
     * \brief Gets the array of texture coordinates in the model
     *
     * \return The array of texture coordinates in the model
     * \return Null if the obj file did not have texture coordinates
     */
    inline const vec2* GetTexCoords() const 
    { 
        return texCoords; 
    }

    /**
     * \brief Gets the array of tangent vectors in the model
     *
     * Gets the array of tangent vectors for the model, which is generated
     * based on the positions and texture coordinates.
     *
     * \return The array of tangent vectors in the model
     * \return Null if the obj file did not have texture coordinates
     */
    inline const vec3* GetTangents() const
    {
        return tangents;
    }

    /**
     * \brief Gets the array of indices in the model for indexed rendering
     *
     * \return The array of indices in the model
     * \return Null if the obj file was not read successfully
     */
    inline const unsigned int* GetIndices() const 
    { 
        return indices; 
    }

    /**
     * \brief Gets the number of vertices in the model
     *
     * Gets the number of vertices in the model.  The vertices, normals and
     * texture coordinate arrays all contain this many elements.
     *
     * \return The number of vertices in the model
     */
    inline int GetNumVertices() const 
    { 
        return numVertices; 
    }

    /**
     * \brief Gets the number of indices in the indices array
     *
     * \return The number of indices in the indices array
     */
    inline int GetNumIndices() const 
    { 
        return numIndices; 
    }

    /**
     * \brief Gets the lower left corner of a bounding box containing the model
     *
     * \return The minimum x, y and z values of any vertex in the model
     */
    inline const vec3& getMinXYZ() const 
    { 
        return minXYZ; 
    }

    /**
     * \brief Gets the upper right corner of a bounding box containing the model
     *
     * \return The maximum x, y and z values of any vertex in the model
     */
    inline const vec3& getMaxXYZ() const 
    { 
        return maxXYZ; 
    }

    /**
     * \brief Gets the point in the center of the model's bounding box
     *
     * \return A rough approximation of the center of the model
     */
    vec3 GetCenter() const 
    { 
        return (minXYZ + maxXYZ) / 2; 
    }

    /**
     * \brief Gets a scaling factor that give the model a radius of ~1
     *
     * \return A scale factor to size a model to have a radius of ~1
     */
    float GetScaleFactor() const;

private:

    /**
     * \brief Helper method to copy another ObjFile to this one
     *
     * \param[in] other - ObjFile to copy
     */
    void Copy(const ObjFile& other);

    /**
     * \brief Helper method to free up memory used by this ObjFile
     */
    void FreeMemory();

    /**
     * \brief Reads an obj file into the data arrays
     *
     * \param[in] file - Open file stream to read from
     */
    void ReadObjFile(std::ifstream& file);

    /**
     * \brief Calculates the vertex normals by averaging face normals
     */
    void CalculateNormals();

    /**
     * \brief Calculate the tangent vectors based on the texture coordinates
     */
    void CalculateTangents();

    int numVertices;  //!< Number of vertices the model has
    int numIndices;   //!< Number of indices the model has (3 for each triangle)

    vec3*         vertices;  //!< Array of vertices or NULL
    vec3*         normals;   //!< Array of normals or NULL
    vec2*         texCoords; //!< Array of texture coordinates or NULL
    vec3*         tangents;  //!< Array of tangents or NULL
    unsigned int* indices;   //!< Array of indices or NULL

    vec3  minXYZ; //!< Minimum x, y and z values for a bounding box
    vec3  maxXYZ; //!< Maximum x, y and z values for a bounding box
};

#endif
