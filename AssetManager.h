#pragma once
#include <unordered_map>
#include <mutex>
#include <memory>
#include <optional>
#include "Logger.h"

namespace ksn
{
	/*
	* AssetManager.hpp
	* ----------------
	* Part of the ksn Engine Assets Manager system.
	* Provides a thread safe mechanism for loading, storing, returning and erasing assets.
	* 
	* Template parametrs:
	*	1. Identifier - A type used to uniquely identify assets
	*   2. Asset - A type satisfying SFMLAsset Concept
	* 
	* Requirments:
	*	1. Identifier must be usable as a key for unordered_map
	*   2. Asset must be a valid SFMLAsset type, as defined in SFMLAsset Concept
	*   See ConceptDefinitions.h for more info.
	* 
	* Example usage:
	*	AssetManager<std::string, sf::Texture> textures;
	*   textures.load("player", "assets/player.png")
	*   const auto& texture = textures.get("player")
	*/
	template<UnorderedKey Identifier, SFMLAsset Asset>
	class AssetManager
	{
	public:
		AssetManager() = default;
		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;

		bool load(const Identifier& id, const std::string_view pathToAsset);
		void erase(const Identifier& id);
		
		//Retrieves the asset associated with the given identifier.
		//Returns a const reference wraped in std::optional if found, std::nullopt otherwise.
		std::optional<std::reference_wrapper<const Asset>> get(const Identifier& id) const;
	private:
		std::unordered_map<Identifier, std::unique_ptr<Asset>> mAssets;
		mutable std::mutex mAssetMutex; //could potentially be a shared_mutex?
	};
	template<UnorderedKey Identifier, SFMLAsset Asset>
	inline bool AssetManager<Identifier, Asset>::load(const Identifier& id, const std::string_view pathToAsset)
	{
		if (pathToAsset.empty())
		{
			gLogger("Path to asset is empty.\n");
			return false;
		}

		try
		{
			auto assetPtr = std::make_unique<Asset>(pathToAsset);
			std::scoped_lock lock(mAssetMutex);
			auto tryPair = mAssets.try_emplace(id, std::move(assetPtr));
			if (!tryPair.second)
			{
				gLogger("Asset with this id already exists.\n", LOG_WARNING);
				return false;
			}
		}
		catch (const sf::Exception& e)
		{
			gLogger("Exception thrown while loading an asset: " + std::string(e.what()) + "\n", LOG_ERROR);
			return false;
		}
		return true;
	}
	template<UnorderedKey Identifier, SFMLAsset Asset>
	inline void AssetManager<Identifier, Asset>::erase(const Identifier& id)
	{
		gLogger("Erasing an asset.\n");
		std::scoped_lock lock(mAssetMutex);
		mAssets.erase(id);
	}
	template<UnorderedKey Identifier, SFMLAsset Asset>
	inline std::optional<std::reference_wrapper<const Asset>> AssetManager<Identifier, Asset>::get(const Identifier& id) const
	{
		gLogger("Returning an asset.\n");
		std::scoped_lock lock(mAssetMutex);
		auto it = mAssets.find(id);
		if (it == std::end(mAssets))
		{
			gLogger("An asset does't exist.\n");
			return std::nullopt;
		}

		return std::cref(*it->second);
	}
}