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

  void AssetManager::SetRootDirectory(const std::string& rootDir)
  {
    mRootDir = rootDir;
  }
}
