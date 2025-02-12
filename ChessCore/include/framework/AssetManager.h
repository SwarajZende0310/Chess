#pragma once

#include<string>
#include<SFML/Graphics.hpp>
#include"framework/Core.h"

namespace chess
{
  class AssetManager
  {
    public:
     static AssetManager& Get();
     shared<sf::Texture> LoadTexture(const std::string& texturePath);
     void SetRootDirectory(const std::string& rootDir);
    protected:
      AssetManager();
    private:
      template<typename T>
      shared<T> LoadAssets(const std::string& path, Dictionary<std::string, shared<T>> &container);
      static unique<AssetManager> mAssetManager;
      Dictionary<std::string,shared<sf::Texture>> mLoadedTextures;
      std::string mRootDir;
  };
  
  template<typename T>
  inline  shared<T> AssetManager::LoadAssets(const std::string& path, Dictionary<std::string, shared<T>> &container)
  {
    auto found  = container.find(path);

    if(found != container.end())
    {
      return container[path];
    }
    
    shared<T> newAsset{new T};
    if(newAsset->loadFromFile(mRootDir + path))
    {
      return container[path] = newAsset; 
    }

    return nullptr;
  }
}
