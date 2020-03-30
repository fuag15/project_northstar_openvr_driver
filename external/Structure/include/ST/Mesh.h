/*
    Mesh.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Bridge Engine SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

#pragma once

#include <ST/MathTypes.h>
#include <ST/GL.h>
#include <ST/Macros.h>

#include <memory>

namespace ST
{

//------------------------------------------------------------------------------

/** @brief Container class for meshes.
    Since OpenGL ES only supports 16 bits unsigned short for face indices, meshes larges than 65535 faces have to be split into smaller submeshes.
    STMesh is therefore a reference to a collection of partial meshes, each of them having less than 65k faces.
*/
struct ST_API Mesh
{
    /** @brief Constructor. */
    Mesh();
    ~Mesh();

    /** @brief Initializes the mesh with input data. Will deep copy and assume no ownership of the input data.
        @param numVertices The number of XYZ vertices.
        @param inputVertices An array of XYZ vertices of numVertices length.
        @param inputPerVertexNormals An array of XYZ normals for each XYZ vertex of numVertices length.
        @param inputPerVertexColors An array of RGB colors for each XYZ vertex of numVertices length.
        @param inputPerVertexUVTextureCoords An array of UV coords for each XYZ vertex of numVertices length.
        @param numFaces The number of triangles in the mesh. (number of indices would be numFaces * 3)
        @param faceIndices The index of the vertices for each triangle of numFaces * 3 length.
    */
    void copyFromRawData(int numVertices, const Vector3f* inputVertices,
                         const Vector3f* inputPerVertexNormals,
                         const Vector3f* inputPerVertexColors,
                         const Vector2f* inputPerVertexUVTextureCoords,
                         int numFaces, const unsigned int* faceIndices);

    /** @brief Special initialization case for lines, we assume that lines are defined as pairs of adjacent vertices.
        @param numLines The number of lines.
        @param inputLineEndPoints An array of XYZ vertices of numLines length. Vertices should be adjacent (pt0=>pt1, pt2=>pt3).
    */
    void copyLinesFromRawData(int numLines, const Vector3f* inputLineEndPoints);

    /** @brief The number of mesh chunks contained. */
    int numberOfMeshes() const;

    /** @brief The number of triangles present in the requested mesh chunk.
        @param meshIndex The id of the mesh chunk.
    */
    int numberOfMeshFaces(int meshIndex) const;

    /** @brief The number of XYZ vertices present in the requested mesh chunk.
        @param meshIndex The id of the mesh chunk.
    */
    int numberOfMeshVertices(int meshIndex) const;

    /** @brief The number of lines present in the requested mesh chunk.
        @param meshIndex The id of the mesh chunk.
    */
    int numberOfMeshLines(int meshIndex) const;

    /** @brief Returns true if this mesh has per-vertex normal information. */
    bool hasPerVertexNormals() const;

    /** @brief Returns true if this mesh has per-vertex color information. */
    bool hasPerVertexColors() const;

    /** @brief Returns true if this mesh has per-vertex UV-texture information. */
    bool hasPerVertexUVTextureCoords() const;

    /** @brief Retrieves the XYZ vertex data for the requested mesh chunk.
        @param meshIndex The id of the mesh chunk.
    */
    const Vector3f* meshVertices(int meshIndex) const;

    /** @brief Retrieves the XYZ normal per-vertex data for the requested mesh chunk.
        @param meshIndex The id of the mesh chunk.
    */
    const Vector3f* meshPerVertexNormals(int meshIndex) const;

    /** @brief Retrieves the RGB color per-vertex data for the requested mesh chunk.
        @param meshIndex The id of the mesh chunk.
    */
    const Vector3f* meshPerVertexColors(int meshIndex) const;

    /** @brief Retrieves the UV-texture coords per-vertex data for the requested mesh chunk.
        @param meshIndex The id of the mesh chunk.
    */
    const Vector2f* meshPerVertexUVTextureCoords(int meshIndex) const;

    /** @brief Retrieves a list of the index of each vertex for all faces in the mesh, assumed to be triangles.
        @param meshIndex The id of the mesh chunk.
        @return Pointer to a contiguous chunk of `(3 * numberOfMeshFaces:meshIndex)` 16 bits `unsigned short` values representing vertex indices.
                Each face is represented by three vertex indices.
    */
    const unsigned int* meshFaces(int meshIndex) const;

    /** @brief Retrieves a list of the index of each vertex for all faces in the mesh as lines.
        @param meshIndex The id of the mesh chunk.
        @return Pointer to a contiguous chunk of `(2 * numberOfMeshLines:meshIndex)` 16 bits `unsigned short` values representing vertex indices.
    */
    const unsigned int* meshLines(int meshIndex) const;


    /** @brief Load a mesh from a file. Currently only supports PLY format.
        @param filePath The fullpath to a PLY file.
        @return True, if the file was found and loaded successfully. Otherwise, false.
    */
    bool readFromFile(const char* filePath);

    /** @brief Export a mesh to a file. Currently only supports PLY format.
        @param filePath The fullpath to a non-existing PLY file.
        @return True, if the file was exported and written successfully. Otherwise, false.
    */
    bool writeToFile(const char* filePath);

    ST_DECLARE_CLONABLE_OPAQUE_INTERNALS(Mesh);
};

/** @brief Useful typedef for shared_ptr binding. */
typedef std::shared_ptr<Mesh> MeshPtr;

//------------------------------------------------------------------------------

/** @brief Utility wrapper for binding an ST::Mesh as a renderable OpenGL object. */
struct ST_API OpenGLMesh
{
    /** @brief Constructor. */
    OpenGLMesh();
    ~OpenGLMesh();

    /** @brief Initialize the OpenGL mesh for use.
        @param glContext A valid and active OpenGLContext.
    */
    void initialize(OpenGLContext glContext);

    /** @brief Provide data to store and be processed internally for OpenGL use.
        @param mesh The mesh data to use.
    */
    void upload(const Mesh& mesh);

    ST_DECLARE_OPAQUE_INTERNALS(OpenGLMesh);
};

//------------------------------------------------------------------------------

} // ST namespace


