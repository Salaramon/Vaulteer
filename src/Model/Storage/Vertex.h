#pragma once

#include <type_traits>

class Vertex {};

// used in compile time type validation of vertex usage
template<class T>
concept vertex_concept = std::is_base_of_v<Vertex, T>;
