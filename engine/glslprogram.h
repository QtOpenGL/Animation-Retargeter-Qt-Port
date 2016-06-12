#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H


#include <map>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/*===============================
 * GLSLProgram
 * @description: This class handles the compilation of opengl shader code into
 * executables that are run on the graphics card.
 *===============================*/

class GLSLProgram
{
public:
    unsigned int m_programID;

    struct GLSLShader
    {
        unsigned int id;
        string filename;
        string source;
    };

    GLSLProgram();
    virtual ~GLSLProgram();

    void unload();

    bool initialize(string vertexShader, string fragmentShader);

    void linkProgram();

    GLuint getUniformLocation(const string& name);

    GLuint getAttribLocation(const string& name);

    void sendUniform4x4(const string& name, const float* matrix, bool transpose);

    void sendUniform(const string& name, const float scalar);

    void bindAttrib(unsigned int index, const string& attribName);

    void bindShader();

private:
    string readFile(const string& filename);

    bool compileShader(const GLSLShader& shader);

    void outputShaderLog(unsigned int shaderID);

    GLSLShader m_vertexShader;
    GLSLShader m_fragmentShader;

    map<string, GLuint> m_uniformMap;
    map<string, GLuint> m_attribMap;
};


#endif // GLSLPROGRAM_H
