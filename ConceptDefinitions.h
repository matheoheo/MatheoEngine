#pragma once
#include <concepts>
#include <sstream>
#include <iostream>

//Definitions of concepts that might be reused in different files.


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
concept Streamable = requires(std::ostream& os, T t)
{
	{os << t} -> std::same_as<std::ostream&>;
};