/**
 * @file AssetManager.h
 * @brief Asset management system for loading and caching game resources
 * 
 * This file contains the AssetManager class which provides a singleton-based
 * resource management system for loading and caching textures, fonts, and other
 * game assets to prevent redundant file loading.
 */

#pragma once

#include<string>
#include<SFML/Graphics.hpp>
#include"framework/Core.h"

namespace chess
{
  /**
   * @brief Manages loading and caching of game assets
   * 
   * The AssetManager provides a singleton interface for loading and caching
   * game assets such as textures and fonts. It ensures that each asset is
   * loaded only once and reused when needed.
   */
  class AssetManager
  {
    public:
      /**
       * @brief Get the singleton instance of AssetManager
       * 
       * @return AssetManager& Reference to the singleton instance
       */
      static AssetManager& Get();
      
      /**
       * @brief Load a texture from file
       * 
       * @param texturePath Path to the texture file relative to the root directory
       * @return shared<sf::Texture> Shared pointer to the loaded texture
       * 
       * @note If the texture is already loaded, returns the cached version
       */
      shared<sf::Texture> LoadTexture(const std::string& texturePath);
      
      /**
       * @brief Load a font from file
       * 
       * @param path Path to the font file relative to the root directory
       * @return shared<sf::Font> Shared pointer to the loaded font
       * 
       * @note If the font is already loaded, returns the cached version
       */
      shared<sf::Font> LoadFont(const std::string &path);
      
      /**
       * @brief Set the root directory for asset loading
       * 
       * @param rootDir The root directory path
       */
      void SetRootDirectory(const std::string& rootDir);
      
    protected:
      /**
       * @brief Construct a new Asset Manager (protected for singleton pattern)
       */
      AssetManager();
      
    private:
      /**
       * @brief Template method to load and cache assets
       * 
       * @tparam T Type of asset to load (sf::Texture, sf::Font, etc.)
       * @param path Path to the asset file
       * @param container Reference to the container storing loaded assets
       * @return shared<T> Shared pointer to the loaded asset
       */
      template<typename T>
      shared<T> LoadAssets(const std::string& path, Dictionary<std::string, shared<T>> &container);
      
      static unique<AssetManager> mAssetManager;                       ///< Singleton instance
      Dictionary<std::string, shared<sf::Texture>> mLoadedTextures;    ///< Cache of loaded textures
      Dictionary<std::string, shared<sf::Font>> mLoadedFontMap;        ///< Cache of loaded fonts
      std::string mRootDir;                                            ///< Root directory for asset loading
  };
  
  /**
   * @brief Template method to load and cache assets
   * 
   * @tparam T Type of asset to load (must have loadFromFile method)
   * @param path Path to the asset file relative to root directory
   * @param container Reference to the container storing loaded assets
   * @return shared<T> Shared pointer to the loaded asset, or nullptr if loading failed
   * 
   * @note This is an internal method used by LoadTexture and LoadFont
   */
  template<typename T>
  inline  shared<T> AssetManager::LoadAssets(const std::string& path, Dictionary<std::string, shared<T>> &container)
  {
    if(path.size() == 0) return shared<T>{nullptr};
    
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
