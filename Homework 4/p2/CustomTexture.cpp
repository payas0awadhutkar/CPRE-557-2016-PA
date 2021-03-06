// Code to create a texture

// Author - Payas Awadhutkar

#include "CustomTexture.h"

CustomTexture::CustomTexture()
    : _texture_1( 0 )
    , _texture_2( 0 )
    , _texture_3( 0 )
    , _textureIdx1( -1 )
    , _textureIdx2( -1 )
    , _textureIdx3( -1 )
{
}

CustomTexture::~CustomTexture(){
	// destructor
}

int CustomTexture::loadAndCreateTextures(
    string path_and_file_texture_1,
    string path_and_file_texture_2,
    string path_and_file_texture_3)
{
    unsigned int channels1;
    unsigned int width1;
    unsigned int height1;

    unsigned int channels2;
    unsigned int width2;
    unsigned int height2;

    unsigned int channels3;
    unsigned int width3;
    unsigned int height3;

    unsigned char* data1 = loadBitmapFile(path_and_file_texture_1, channels1, width1, height1);
    unsigned char* data2 = loadBitmapFile(path_and_file_texture_2, channels2, width2, height2);
    unsigned char* data3 = loadBitmapFile(path_and_file_texture_3, channels3, width3, height3);

    if (data1 == NULL || data2 == NULL || data3 == NULL) {
        return -1;
    }

    // Texture generation - background

    // Activate a texture unit
    glActiveTexture(GL_TEXTURE0);

    // Generate a texture, this function allocates the memory and
    // associates the texture with a variable.
    glGenTextures(1, &_texture_1);

    // Set a texture as active texture.
    glBindTexture(GL_TEXTURE_2D, _texture_1);

    // Change the parameters of your texture units.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);

    // Create a texture and load it to your graphics hardware. 
	 // This texture is automatically associated with texture 0 and the active texture.
    if (channels1 == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_BGR, GL_UNSIGNED_BYTE, data1);
    } else if (channels1 == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_BGRA, GL_UNSIGNED_BYTE, data1);
    }

    // free allocated memory (loaded data)
    free(data1);

    // Second texture
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &_texture_2);
    glBindTexture(GL_TEXTURE_2D, _texture_2);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);

    if (channels2 == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_BGR, GL_UNSIGNED_BYTE, data2);
    } else if (channels2 == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_BGRA, GL_UNSIGNED_BYTE, data2);
    }

    
    free(data2);

    // Third texture
    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, &_texture_3);
    glBindTexture(GL_TEXTURE_2D, _texture_3);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);

    if (channels3 == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_BGR, GL_UNSIGNED_BYTE, data3);
    } else if (channels3 == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width3, height3, 0, GL_BGRA, GL_UNSIGNED_BYTE, data3);
    }

    // free allocated memory (loaded data)
    free(data3);

    // Return the texture.
    return _texture_1;
}

bool CustomTexture::addVariablesToProgram(GLuint program, int variable_index) {
    if (program == -1) {
        return false;
    }

    GLint params;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
	 
	 // checking if program is linked or not
    if (params == GL_FALSE) {
        cerr << "GLTexture Program " << program << " is not linked. Can't add textures" << endl;
        return false;
    }

    // enable the program
    glUseProgram(program);

    // location of the texture in the glsl program
    _textureIdx1 = glGetUniformLocation(program, _glsl_names[0].c_str() );
    checkUniform(_textureIdx1, _glsl_names[0]);

    _textureIdx2 = glGetUniformLocation(program, _glsl_names[1].c_str() );
    checkUniform(_textureIdx2, _glsl_names[1]);

    _textureIdx3 = glGetUniformLocation(program, _glsl_names[2].c_str() );
    checkUniform(_textureIdx3, _glsl_names[2]);

    // linking with texture unit 0
    glActiveTexture(GL_TEXTURE0);

    // bind our texture into the active texture unit.
    glBindTexture(GL_TEXTURE_2D, _texture_1);

    // Set the tex uniform of the shaders to the index of the texture unit
    glUniform1i(_textureIdx1, 0);

    // Repeating for remaining 2 textures
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texture_2);
    glUniform1i(_textureIdx2, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _texture_3);
    glUniform1i(_textureIdx3, 2);

    return true;
}
