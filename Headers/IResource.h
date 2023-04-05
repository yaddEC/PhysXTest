#pragma once
#include <iostream>
#include <vector>
#include <filesystem>
#include "MyMaths.h"

using namespace Core::Maths;

namespace Resources
{
    class  IResource
    {
    public:
        IResource() 
        {
            resourceId = NULL;
        }
        virtual ~IResource() {}

        virtual void LoadResource() = 0;
        virtual void UnloadResource() = 0;
        virtual void BindResource() = 0;

        void SetResourceId(unsigned int id);
        unsigned int GetResourceId() const;

        void SetResourcePath(const std::string& path);
        std::string GetResourcePath() const;

        bool GetIsLoaded();
        bool GetIsBinded();

    protected:
        unsigned int resourceId;
        std::string resourcePath;
        bool isLoaded;
        bool isBinded;

    };


    class Model : public IResource
    {
    public:
        Model();
        ~Model();
        void LoadResource();
        void CustomLoadResource(std::vector<Vertex> _vertexBuffer);
        void UnloadResource();
        void BindResource();
        std::vector<Vertex> vertexBuffer;
        std::vector<uint32_t> indexBuffer;

        void SetBuffersAndVAO();
        unsigned int GetVAO();

        enum CustomModel
        {
            CUBE,
            SPHERE,
            CAPSULE,
        };
    private:
        unsigned int VBO, VAO, EBO;
    };

    class Texture : public IResource
    {
    public:
        Texture() { isLoaded = isBinded = false; }
        ~Texture();
        void LoadResource();
        void UnloadResource();
        void BindResource();
        unsigned int texKey;
    private:
        int width;
        int height;
        unsigned char* data;
    };

   class CubeMap : public IResource
    {
    public:
        CubeMap() { texKey = 0; };
        ~CubeMap() {};
        void LoadResource();
        void UnloadResource();
        void BindResource();
        void SetFaces(std::vector<std::string> cubeMapFaces);
        unsigned int texKey;
    private:
        std::vector<std::string> faces;
    };

    class Shader : public IResource
    {
    public:
        Shader() 
        {
            vertexShader = fragmentShader = shaderProgram = NULL;
            isLoaded = true;
            isBinded = false;;
        }
        ~Shader() {};
        void LoadResource();
        void UnloadResource();
        void BindResource();
        void SetShaders(std::string const& vertexPath, std::string const& fragPath);

        int GetProgram();

    private:
        enum class ShaderType
        {
            VERTEX,
            FRAG,
        };

        void LoadShader(std::string const& filename, Shader::ShaderType type);
        bool SetVertexShader();
        bool SetFragmentShader();
        bool Link();

        bool isReflection = false;
        int vertexShader;
        int fragmentShader;
        int shaderProgram;
        std::string vertexSource;
        std::string fragSource;
    };

 
}