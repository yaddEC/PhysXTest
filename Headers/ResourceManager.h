#pragma once
#include <iostream>
#include <unordered_map>
#include <thread>
#include "IResource.h"
#include "Debug.h"
#include "ThreadPool.h"

using namespace Threading;

namespace Resources
{
    class ResourceManager
    {
        //Signgleton structure
    public:
        ResourceManager(const ResourceManager&) = delete;

        static ResourceManager& GetInstance()
        {
            static ResourceManager instance;
            return instance;
        }
    private:
        ResourceManager() {}

        //Resource Manager

    public:
        bool mulithreadLoading = true;
        bool everythingLoaded = false;
        bool timerPrinted = false;
        std::chrono::high_resolution_clock::time_point beginTime;
        int loadingTime;
        int averageTime;

        static void DeleteMap() { GetInstance().IDeleteMap(); }

        template <typename T>
        static T* Create(const std::string& name, const std::string& path) { return GetInstance().ICreate<T>(name, path); }

        static Model* CustomCreate(const std::string& name, Model::CustomModel modelType) { return GetInstance().ICustomCreate(name, modelType); }

        template <typename T>
        static T* Get(const std::string& name) { return GetInstance().IGet<T>(name); }

        static void TextureBinding() { return GetInstance().ITextureBinding(); }

        static bool CheckLoading() { return GetInstance().ICheckLoading(); }

    private:
        void IDeleteMap()
        {
            resources.clear();
        }

        template <class T>
        T* ICreate(const std::string& name, const std::string& path)
        {
            bool IsInherit = std::is_base_of<IResource, T>::value;
            Assert(!IsInherit);

            IResource* resource_base = (IResource*)(new T);

            if (resources.find(name) == resources.end()) //Create new resource
            {
                resource_base->SetResourceId(resources.size());
                resource_base->SetResourcePath(path);
                resources.emplace(name, resource_base);
            }
            else //Replace resource
            {
                resource_base->SetResourcePath(path);
                resources.at(name) = resource_base;
            }

            if (typeid(T) != typeid(Shader))
            {
                if (mulithreadLoading)
                    ThreadPool::AddTask([resource_base]() mutable
                        {
                            resource_base->LoadResource();
                        });
                else
                    resource_base->LoadResource();
                    
            }
            return (T*)resource_base;

        }

        Model* ICustomCreate(const std::string& name, Model::CustomModel modelType)
        {
            Model* resource_base = new Model;

            if (resources.find(name) == resources.end()) //Create new resource
            {
                resource_base->SetResourceId(resources.size());
                resources.emplace(name, resource_base);
            }
            else //Replace resource
            {
                resources.at(name) = resource_base;
            }

            switch (modelType)
            {
            case Model::CustomModel::SPHERE:
                if (mulithreadLoading)
                {
                    ThreadPool::AddTask([resource_base]() mutable
                        {
                            resource_base->CustomLoadResource(Core::Maths::CreateSphere(1));
                        });
                }
                else
                {
                    resource_base->CustomLoadResource(Core::Maths::CreateSphere(1));
                }
                break;

            case Model::CustomModel::CAPSULE:
                if (mulithreadLoading)
                {
                    ThreadPool::AddTask([resource_base]() mutable
                        {
                            resource_base->CustomLoadResource(Core::Maths::CreateCapsule(1, 2, 5, 10));
                        });
                }
                else
                {
                    resource_base->CustomLoadResource(Core::Maths::CreateCapsule(1, 2, 5, 10));
                }
                break;

            default:
                if (mulithreadLoading)
                {
                    ThreadPool::AddTask([resource_base]() mutable
                        {
                            resource_base->CustomLoadResource(Core::Maths::CreateCube(1));
                        });
                }
                else
                {
                    resource_base->CustomLoadResource(Core::Maths::CreateCube(1));
                }
                break;
            }
            
            return resource_base;
        }

        template <typename T>
        T* IGet(const std::string& name)
        {
            return (T*)(resources.find(name)->second);
        }

        void ITextureBinding()
        {
            for (std::pair<std::string, IResource*> resource : resources)
            {
                if (resource.second->GetIsLoaded() && !resource.second->GetIsBinded())
                {
                    resource.second->BindResource();
                }
            }
        }

        bool ICheckLoading()
        {
            if (everythingLoaded)
                return true;

            for (std::pair<std::string, IResource*> resource : resources)
            {
                if (!resource.second->GetIsLoaded())
                {
                    return false;
                }
            }
            everythingLoaded = true;
            return true; 
        }
        std::unordered_map<std::string, IResource*> resources;
    };
}