// header file for CustomTexture

// Author - Payas Awadhutkar

// Based on code provided by Dr. Radkowski


#pragma once

#include <GL/glew.h>

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// header files from Dr. Radkowski's code
#include "GLAppearanceBase.h"
#include "Texture.h"

class CustomTexture : public GLTextureBase {

 private:
    // Allow the class GLApperance access to protected variables.
    friend class GLAppearance;

    // These are the variable names which are used in our glsl shader programs.
    // Make sure that you use the correct names in your programs.
    const string      _glsl_names[3] = { "texture1", "texture2", "texture3"};

 public:
    CustomTexture();
    ~CustomTexture(); // destructor

    /**
     * Load two bitmap images as textures from files.
     * @param path_and_file_texture_1 - path and file of the first image.
     * @param path_and_file_texture_1 - path and file of the second image.
     * @return int - the texture id when the texture was sucessfully loaded.
     */
    int loadAndCreateTextures(
        string path_and_file_texture_1,
        string path_and_file_texture_2,
        string path_and_file_texture_3);

    // Adds the variables of this object to the shader program
    virtual bool addVariablesToProgram(GLuint program, int variable_index);

    virtual bool dirty(GLuint program) {
        return true;
    }

private:
    // The texture for this program.
    GLuint      _texture_1;
    GLuint      _texture_2;
    GLuint      _texture_3;

    // location of the texture in the glsl program
    int         _textureIdx1;
    int         _textureIdx2;
    int         _textureIdx3;
};
