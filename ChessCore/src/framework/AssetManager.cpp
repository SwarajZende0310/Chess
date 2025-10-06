/**
 * @file AssetManager.cpp
 * @brief Implementation of the asset loading and caching system.
 */

#include"framework/AssetManager.h"

namespace chess
{
  unique<AssetManager> AssetManager::mAssetManager{nullptr};

  /**
   * @brief Construct the AssetManager (singleton instance).
   *
   * Initializes internal containers used for caching loaded assets.
   */
  AssetManager::AssetManager()
    :mLoadedTextures{}

  {
    
  }

  /**
   * @brief Get the singleton instance of the AssetManager.
   *
   * Creates the instance on the first call and returns a reference to it.
   * @return AssetManager& Reference to the global AssetManager instance.
   */
  AssetManager& AssetManager::Get()
  {
    if(!mAssetManager)
    {
      mAssetManager = std::move(unique<AssetManager>{new AssetManager});
    }
    return *mAssetManager;
  }

  /**
   * @brief Load a texture from disk or return a cached one.
   *
   * @param texturePath Relative path from the configured root directory.
   * @return shared<sf::Texture> Shared pointer to the loaded texture, or nullptr on failure.
   */
  shared<sf::Texture> AssetManager::LoadTexture(const std::string& texturePath)
  {
    return LoadAssets(texturePath, mLoadedTextures); 
  }

  /**
   * @brief Load a font from disk or return a cached one.
   *
   * Uses openFromFile for fonts. Paths are resolved relative to the set root directory.
   * @param path Relative path to the font file.
   * @return shared<sf::Font> Shared pointer to the loaded font, or nullptr on failure.
   */
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
  /**
   * @brief Set the root directory used to resolve asset paths.
   *
   * All subsequent load calls will prepend this directory to the given relative paths.
   * @param rootDir The root assets directory.
   */
  void AssetManager::SetRootDirectory(const std::string &rootDir)
  {
    mRootDir = rootDir;
  }
}
