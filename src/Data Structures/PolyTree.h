#pragma once

#include <iterator>
#include <array>
#include <utility>
#include <cmath>
#include <type_traits>
#include <unordered_set>
#include <vector>
#include <memory>
#include <limits>

#include <iostream>

#include "Utils/MathUtils.h"


template <typename _Ty, size_t _Size>
std::array<_Ty, _Size> make_array(const _Ty& v) {
	std::array<_Ty, _Size> ret;
	ret.fill(v);
	return ret;
}

template<class T>
bool is_overlap(
	typename T rect1Pos,
	typename T rect1Len,
	typename T rect2Pos,
	typename T rect2Len)
{
	bool overlap = true;
	for (size_t i = 0; i < rect1Pos.size(); i++) {
		overlap &= (rect1Pos[i] < (rect2Pos[i] + rect2Len[i]) && (rect1Pos[i] + rect1Len[i]) >= rect2Pos[i]);
	}

	return overlap;
}

template<class Content, class Value, size_t dimensions>
class TreeNode {
public:
	typedef std::array<Value, dimensions> Position;
	typedef std::array<std::unique_ptr<TreeNode<Content, Value, dimensions>>, mut::ctexp2(dimensions)> Orthants;

	TreeNode(Value size = 0, Position position = make_array<Value, dimensions>(0), TreeNode<Content, Value, dimensions>* parent = nullptr, size_t nodeIndex = 0) :
		size(size), position(position), parent(parent), nodeIndex(nodeIndex) {}

	Orthants nodes;
	std::pair<Position, Content>* insertion = nullptr;
	Position position;
	Value size;
	TreeNode<Content, Value, dimensions>* parent;
	size_t nodeIndex;

	size_t getNodeID(const Position& position);
	Position getOrthantPosition(size_t orthantID);
	Value getSubNodeSize();
};

template<class Content, class Value, size_t dimensions>
size_t TreeNode<Content, Value, dimensions>::getNodeID(const Position& position)
{
	Content value;
	if (insertion != nullptr) {
		value = insertion->second;
		value = value;
	}
	size_t size = getSubNodeSize();
	size_t orthantID = 0;
	for (size_t i = 0; i < TreeNode::position.size(); i++) {
		
		orthantID |= ((TreeNode::position[i] + size) <= position[i]) << i;
	}
	return orthantID;
}

template<class Content, class Value, size_t dimensions>
std::array<Value, dimensions> TreeNode<Content, Value, dimensions>::getOrthantPosition(const size_t orthantID)
{
	const size_t forEnd = mut::ctexp2(position.size());
	Value orthantSize = getSubNodeSize();
	Position orthantPosition;
	size_t index = 0;
	for (size_t i = 1; i < forEnd; i = (i << 1)) {
		if (i & orthantID) {
			orthantPosition[index] = orthantSize + position[index];
		}
		else {
			orthantPosition[index] = position[index];
		}

		index++;
	}

	return orthantPosition;
}

template<class Content, class Value, size_t dimensions>
Value TreeNode<Content, Value, dimensions>::getSubNodeSize()
{
	Value newSize;
	if (size == 0) {
		newSize = (std::numeric_limits<Value>::max() / 2) + (((Value)1 / (Value)2) ? ((Value)1 / (Value)2) : (Value)1);
	}
	else {
		newSize = size / 2;
	}
	
	return newSize;
}



template<class Content, class Value, size_t dimensions>
class PolyTree
{
public:
	typedef std::array<Value, dimensions> Position;

	class iterator {
	public:
		iterator() {}

		std::pair<Position, Content>& operator*() {
			return (*valuePointer);
		}
		std::pair<Position, Content>* operator->() {
			return valuePointer;
		}

		bool operator!=(iterator& other) {
			
			return (valuePointer != other.valuePointer);
		}

		void surface() {
			while (index == nodePointer->nodes.size()) {
				index = nodePointer->nodeIndex + 1;
				nodePointer = nodePointer->parent;
			}
		}

		void delve() {
			nodePointer = nodePointer->nodes[index].get();
			index = 0;
		}

		void incrementIndex() {
			index++;
			surface();
		}

		void getNextValue() {
			valuePointer = nullptr;
			while (!valuePointer && (nodePointer->parent != nullptr || index == 0)) {
				if (nodePointer->nodes[index] == nullptr) {
					incrementIndex();
				}
				else {
					bool overlap = is_overlap(nodePointer->nodes[index]->position, make_array<Value, dimensions>(nodePointer->nodes[index]->size), rangePosition, rangeLenght);
					if (overlap) {
						Value bottomValue = std::numeric_limits<Value>::min();
						if (bottomValue == 0) {
							bottomValue = 1;
						}
						if (nodePointer->nodes[index]->size == bottomValue) {
							valuePointer = nodePointer->nodes[index]->insertion;
							incrementIndex();
						}
						else {
							delve();
						}
					}
					else {
						incrementIndex();
					}
				}
			}
		}

		iterator& operator++() {
			getNextValue();

			return *this;
		}

		iterator operator++(int) {
			getNextValue();

			return *this;
		}

		size_t index = 0;
		std::pair<Position,Content>* valuePointer;
		TreeNode<Content, Value, dimensions>* nodePointer;
		Position rangePosition;
		Position rangeLenght;
	};

	PolyTree() {}
	~PolyTree();

	void insert(Content&& object, const Position& position);

	std::pair<typename PolyTree<Content, Value, dimensions>::iterator, typename PolyTree<Content, Value, dimensions>::iterator> 
	equal_range(Position position, Position lenght);

	void remove(Value x, Value y, Value width, Value height);

	std::vector<std::unique_ptr<std::pair<Position, Content>>> insertions;
	TreeNode<Content, Value, dimensions> rootNode;

};

template<class Content, class Value, size_t dimensions>
PolyTree<Content, Value, dimensions>::~PolyTree() {

	std::vector<size_t> indexStackTrace;
	indexStackTrace.push_back(0);
	std::vector<TreeNode<Content, Value, dimensions>*> pointerStackTrace;
	pointerStackTrace.push_back((&rootNode));
	size_t currentDepth = 0;
	while (!pointerStackTrace.empty()) {
		bool surfaced = false;
		bool delved = false;

		//Surfacing
		if (indexStackTrace[currentDepth] >= rootNode.nodes.size() - 1) {
			surfaced = true;
			currentDepth--;
			pointerStackTrace.pop_back();
			indexStackTrace.pop_back();

			if ((!pointerStackTrace.empty())) {
				pointerStackTrace[currentDepth]->nodes[indexStackTrace[currentDepth]].reset();
				indexStackTrace[currentDepth]++;
			}
		}


		//Delving
		if (!pointerStackTrace.empty() && indexStackTrace[currentDepth] < rootNode.nodes.size() && pointerStackTrace[currentDepth]->nodes[indexStackTrace[currentDepth]] != nullptr) {
			delved = true;
			pointerStackTrace.push_back(pointerStackTrace[currentDepth]->nodes[indexStackTrace[currentDepth]].get());
			indexStackTrace.push_back(0);
			currentDepth++;
		}

		if (!surfaced & !delved) {
			indexStackTrace[currentDepth]++;
		}
		
	}

}

template<class Content, class Value, size_t dimensions>
void PolyTree<Content, Value, dimensions>::insert(Content&& object, const Position& position) {

	TreeNode<Content, Value, dimensions>* currentPointer;
	currentPointer = &rootNode;
	insertions.emplace_back(std::make_unique<std::pair<Position, Content>>(std::make_pair(position, object)));

	Value bottomValue = std::abs(std::numeric_limits<Value>::min());
	if (bottomValue == 0 || bottomValue < -1) {
		bottomValue = 1;
	}

	while (currentPointer->size != bottomValue) {

		size_t nodeID = currentPointer->getNodeID(position);

		//Create new node
		if (currentPointer->nodes[nodeID] == nullptr) {
			Position orthantPosition = currentPointer->getOrthantPosition(nodeID);

			currentPointer->nodes[nodeID] = std::make_unique<TreeNode<Content, Value, dimensions>>(currentPointer->getSubNodeSize(), orthantPosition, currentPointer, nodeID);
		}
		
		currentPointer = currentPointer->nodes[nodeID].get();

	}

	//Cood 1,1,1 leads to 0,0,0 ?
	currentPointer->insertion = insertions.back().get();

}

template<class Content, class Value, size_t dimensions>
std::pair<typename PolyTree<Content, Value, dimensions>::iterator, typename PolyTree<Content, Value, dimensions>::iterator>
PolyTree<Content, Value, dimensions>::equal_range(Position position, Position length)
{
	iterator beginIterator;
	iterator endIterator;

	beginIterator.nodePointer = &rootNode;
	beginIterator.rangePosition = position;
	beginIterator.rangeLenght = length;
	beginIterator++;

	endIterator.nodePointer = nullptr;
	endIterator.rangePosition = position;
	endIterator.rangeLenght = length;

	return std::make_pair(beginIterator, endIterator);

}

template<class Content, class Value, size_t dimensions>
inline void PolyTree<Content, Value, dimensions>::remove(Value x, Value y, Value width, Value height)
{
	assert(false); // missing implementation
}
