#pragma once

template <class T>
class VertexHash {
public:
	using VertexType = T;

	virtual bool operator()(const T& a, const T& b) const = 0;
	virtual size_t operator()(const T& k) const = 0;

	virtual ~VertexHash() = default;
};
