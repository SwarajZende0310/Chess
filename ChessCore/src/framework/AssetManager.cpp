#include"framework/AssetManager.h"

namespace chess
{
  unique<AssetManager> AssetManager::mAssetManager{nullptr};
  AssetManager::AssetManager()
    :mLoadedTextures{}

  {
    
  }

  AssetManager& AssetManager::Get()
  {
    if(!mAssetManager)
    {
      mAssetManager = std::move(unique<AssetManager>{new AssetManager});
    }
    return *mAssetManager;
  }

  shared<sf::Texture> AssetManager::LoadTexture(const std::string& texturePath)
  {
    return LoadAssets(texturePath, mLoadedTextures); 
  }

  shared<sf::Font> AssetManager::LoadFont(const std::string &path)
  {
    if(path.size() == 0) return shared<sf::Font>{nullptr};
    
    auto found  = mLoadedFontMap.find(path);

    if(found != mLoadedFontMap.end())
    {
      return mLoadedFontMap[path];
    }
    
    shared<sf::Font> newAsset{new sf::Font};
    if(newAsset->openFromFile(mRootDir + path))
    {
      return mLoadedFontMap[path] = newAsset; 
    }

    return nullptr;
  }
  void AssetManager::SetRootDirectory(const std::string &rootDir)
  {
    mRootDir = rootDir;
  }
}
