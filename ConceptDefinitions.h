#pragma once
#include <concepts>
#include <sstream>
#include <iostream>

//Definitions of concepts that might be reused in different files.
namespace ksn
{
	/*
	* Streamable Concept
	* ------------------
	* Requires, that type T can be streamed into standard output stream using the '<<' operator.
	*
	* A type is considered Streamable if this expression is valid:
	*	std::ostream << T
	*
	* Example Usage:
	*	struct MyData {}
	*	std::ostream& operator<<(std::ostream& os, const MyData& data) {return os;}
	*	MyData now is streamable.
	*/
	template <typename T>
	concept Streamable = requires(std::ostream os, T t)
	{
		{os << t} -> std::same_as<std::ostream&>;
	};


	/*
	* is_sfml_asset trait
	* -------------------
	* This trait determines wheter a type T is considered a valid SFML asset.
	* By default, all types are treated as unsupported (std::false_type), but users can specialize this trait
	*	for custom types if needed.
	* 
	* Supported by default:
	*	- sf::Texture
	*   - sf::Font
	* 
	* User can easily extend this without modifying core code.
	* Example:
	*	template<>
	*   struct is_sfml_asset<sf::SoundBuffer> : std::true_type {}
	*   Now sf::SoundBuffer can be used also as SFML Asset.
	*/
	template <typename T>
	struct is_sfml_asset : std::false_type {};

	template<>
	struct is_sfml_asset<sf::Texture> : std::true_type {};

	template<>
	struct is_sfml_asset<sf::Font> : std::true_type {};


	/*
	* SFMLAsset Concept
	* -----------------
	* A concept that restricts a type to only those marked as valid SFML Assets
	*  via is_sfml_asset<T> trait.
	* 
	* This concept is used to constrain template like AssetManager to ensure type safety and extensibility.
	* 
	* Example:
	*	AssetManager<std::string, sf::Texture> textures; //valid
	*   AssetManager<MyEnum, sf::Font> fonts; //valid
	*   AssetManager<std::string, sf::SoundBuffer> sounds; //invalid unless trait is specialized
	*/
	template <typename T>
	concept SFMLAsset = is_sfml_asset<T>::value;

	/*
	* UnorderedKey Concept
	* --------------------
	* Unordered containers need to have defined 2 things for key-type, which is:
	*   -a hash function
	*   -a way to compare hashes (in this case, overloaded '==' operator)
	* 
	* UnorderedKey Concept is for specifying constraints on classes that might use template arguments for
	*	unorderd containers keys (the engine uses it for AssetManager).
	* 
	*/
	template <typename T>
	concept UnorderedKey = requires(T t)
	{
		{std::hash<T>{}(t)} -> std::convertible_to<std::size_t>;
		{t == t} -> std::convertible_to<bool>;
	};
}