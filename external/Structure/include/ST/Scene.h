/*
    Scene.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Bridge Engine SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

#pragma once

#include <ST/Mesh.h>
#include <ST/MathTypes.h>
#include <ST/GL.h>

#include <memory>

namespace ST
{

//------------------------------------------------------------------------------

/** @brief Utility class to hold a pre-mapped scene. */
struct ST_API Scene
{
    /** @brief Constructor. Requires a valid and active OpenGLContext.
        @param context A valid and active OpenGLContext.
        @param freeTextureUnit Reserved. Can be ignored.
    */
    Scene(OpenGLContext context, GLenum freeTextureUnit = 0);
    ~Scene();

    /** @brief Lock-protect the data and return a pointer to the internal mesh in the scene. */
    MeshPtr lockAndGetSceneMesh();

    /** @brief Unlock the data, must be called after calling Scene::lockAndGetSceneMesh. */
    void unlockSceneMesh();

    /** @brief Set the initial underlying mesh data. @see Mesh
        @return True, if able to load and store the mesh. Otherwise, false.
    */
    bool setSceneMesh(const Mesh& mesh);

    /** @brief Renders the underlying mesh in the active OpenGLContext. This function uses its own rendering engine internally.
        @param cameraPose The current position of the camera.
        @param cameraProjection The OpenGL projection matrix.
        @param alpha The desired target opacity of the rendered mesh.
        @param highlightOutOfRangeDepth If true, depth information too close to or too far from the sensor will appear red.
        @param wireFrame If true, will render the mesh as a wireframe instead of a solid mesh.
        @return True, if we have data and were able to render successfully. Otherwise, false.
    */
    bool renderMeshFromViewpoint(const Matrix4& cameraPose, const Matrix4& cameraProjection,
                                 float alpha, bool highlightOutOfRangeDepth, bool wireFrame);

    /** @brief Clear the scene of all mesh data. */
    void clear();

    ST_DECLARE_OPAQUE_INTERNALS(Scene);
};

/** @brief Useful typedef for shared_ptr binding. */
typedef std::shared_ptr<Scene> ScenePtr;

//------------------------------------------------------------------------------

} // ST namespace
