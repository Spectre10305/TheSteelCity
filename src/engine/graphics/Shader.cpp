#include "Shader.h"
#include "../utils/Log.h"
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


//=============================================================


nothing::Shader::Shader(const char* vertFileName, const char* fragFileName)
{

    std::string   vertexCode,  fragmentCode;
    std::ifstream vShaderFile, fShaderFile;


    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


    try
    {

        vShaderFile.open(vertFileName);
        fShaderFile.open(fragFileName);


        std::stringstream vShaderStream, fShaderStream;


        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();


        vShaderFile.close();
        fShaderFile.close();


        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch (std::ifstream::failure e)
    {

        nothing::LogInfo("Shader file not succesfully read: " + std::string(e.what()));

    }


    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();


	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
    CheckErrs(vertexShader, "VERTEX");


	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
    CheckErrs(fragmentShader, "FRAGMENT");


	programID_ = glCreateProgram();
	glAttachShader(programID_, vertexShader);
	glAttachShader(programID_, fragmentShader);
	glLinkProgram(programID_);
    CheckErrs(programID_, "PROGRAM");


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}


//=============================================================


nothing::Shader::~Shader()
{

    glDeleteProgram(programID_);

}


//=============================================================


void nothing::Shader::Use()
{

    glUseProgram(programID_);

}


//=============================================================


uint32_t nothing::Shader::GetProgramID()
{

    return programID_;

}


//=============================================================


void nothing::Shader::SetUniform(const char* unifName, int newVal)
{

    glUniform1i(glGetUniformLocation(programID_, unifName), newVal);

}


//=============================================================


void nothing::Shader::SetUniform(const char* unifName, float newVal)
{

    glUniform1f(glGetUniformLocation(programID_, unifName), newVal);

}


//=============================================================


void nothing::Shader::SetUniform(const char* unifName, double newVal)
{

    glUniform1d(glGetUniformLocation(programID_, unifName), newVal);

}


//=============================================================


void nothing::Shader::SetUniform(const char* unifName, glm::mat4 newVal)
{

    glUniformMatrix4fv(glGetUniformLocation(programID_, unifName), 1, GL_FALSE, glm::value_ptr(newVal));

}


//=============================================================


void nothing::Shader::CheckErrs(uint32_t shader, const char* type)
{

    int success;
    char infoLog[1024];


    if (type != "PROGRAM")
    {

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);


        if (!success)
        {

            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Shader compilation ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            return;

        }
        else
        {

            nothing::LogInfo(std::string(type) + " Shader compiled succesfully");

        }

    }
    else
    {

        glGetProgramiv(shader, GL_LINK_STATUS, &success);


        if (!success)
        {

            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Shader program link ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            return;

        }
        else
        {

            nothing::LogInfo("Shader program linked succesfully");

        }

    }

}


//=============================================================