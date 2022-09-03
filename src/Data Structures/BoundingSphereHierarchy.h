#pragma once

#include <array>
#include <memory>
#include <vector>
#include <unordered_set>
#include <optional>
#include <limits>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Debug/Debug.h>


struct HierarchyPair {
public:
	HierarchyPair() :
		OR(this, DY::V(
			&first,
			&second,
			&center,
			&radius), DY::N(
				"first",
				"second",
				"center",
				"radius")),

		first(0), second(0), center(glm::vec3(0,0,0)), radius(0) {
		OB.add(OR);
	}

	size_t first;
	size_t second;
	glm::vec3 center;
	float radius;

	DY::ObjectRegister<HierarchyPair,
		decltype(first),
		decltype(second),
		decltype(center),
		decltype(radius)> OR;
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();
};

template<class Store>
struct BoundedItem {
public:
	BoundedItem(Store& value, glm::vec4 sphere) :
		value(value), center(sphere), radius(sphere.w)
	{}

	BoundedItem(Store&& value, glm::vec4 sphere) :
		value(std::forward<Store>(value)), center(sphere), radius(sphere.w)
	{}

	BoundedItem(Store& value, glm::vec3 center, double radius) :
		value(std::forward<Store>(value)), center(center), radius(radius)
	{}

	BoundedItem(Store&& value, glm::vec3 center, double radius) :
		value(std::forward<Store>(value)), center(center), radius(radius)
	{}

	Store value;
	glm::vec3 center;
	float radius;
};

template<class Store>
class BoundingSphereHierarchyNode {
public:

	BoundingSphereHierarchyNode() :
		node({ nullptr, nullptr }),
		sphere(glm::vec4(0, 0, 0, 0))
	{}

	BoundingSphereHierarchyNode(BoundingSphereHierarchyNode&& other) :
		node({ std::move(other.node[0]), std::move(other.node[1]) }),
		sphere(std::move(other.sphere)),
		value(other.value)
	{
		other.node[0].reset();
		other.node[1].reset();
		other.value = nullptr;
	}

	BoundingSphereHierarchyNode(glm::vec4 sphere, BoundingSphereHierarchyNode&& first, BoundingSphereHierarchyNode&& second) :
		sphere(sphere),
		node({ std::make_unique<BoundingSphereHierarchyNode>(std::move(first)), std::make_unique<BoundingSphereHierarchyNode>(std::move(second)) })
	{}
	BoundingSphereHierarchyNode(glm::vec4 sphere) :
		sphere(sphere)
	{}

	BoundingSphereHierarchyNode(glm::vec4 sphere, Store* value) :
		sphere(sphere), value(value)
	{}

	BoundingSphereHierarchyNode& operator=(BoundingSphereHierarchyNode&& other) noexcept {
		this->node[0] = std::move(other.node[0]);
		this->node[1] = std::move(other.node[1]);
		this->sphere = std::move(other.sphere);

		other.node[0].reset();
		other.node[1].reset();

		return *this;
	}

	bool isEndNode() {
		return node[0] == nullptr && node[1] == nullptr;
	}

	std::array<std::unique_ptr<BoundingSphereHierarchyNode>, 2> node;
	glm::vec4 sphere;
	Store* value;
};

template<class Store>
class BoundingSphereHierarchy
{
public:

	class iterator {
	public:
		using difference_type = size_t;
		using value_type = Store;
		using pointer = Store*;
		using reference = Store&;

		inline static const intmax_t FORWARD_INCREMENT = 1;
		inline static const intmax_t BACKWARD_INCREMENT = -1;
		inline static const intmax_t FORWARD_INITIALIZATION = 0;
		inline static const intmax_t BACKWARD_INITIALIZATION = 1;
		inline static const intmax_t FORWARD_LIMIT = 2;
		inline static const intmax_t BACKWARD_LIMIT = -1;

		BoundingSphereHierarchyNode<Store>& getValue() {
			path.back().first->value;
		}

		size_t getIndex() {
			return path.back().second;
		}

		void descendNext(intmax_t direction) {
			intmax_t nodeSelect = path.back().second;
			path.push_back(std::make_pair(path.back().first->node[nodeSelect].get(), direction)); //0, 1
		}

		void ascendNext(intmax_t direction) {
			path.pop_back();
			if (!path.empty()){
				path.back().second += direction; //1, -1
			}
		}

		bool nextReached() {
			bool end = false;
			if (!path.empty()) {
				end = path.back().first->isEndNode();
			}
			return end || path.empty();
		}

		bool nextDescendPath(intmax_t limit, intmax_t increment) {
			for (0; path.back().second < limit; path.back().second += increment) {
				if (comparator(path.back().first->node[path.back().second]->sphere)) {
					return true;
				}
			}
			return false;
			/*
			if (path.back().second != direction) {
				intmax_t nodeSelect = path.back().second;
				return comparator(path.back().first->node[nodeSelect]->sphere); //2, -1
			}
			return false;
			*/
		}

		iterator& operator++(int) {
			if (path.back().first->isEndNode()) {
				ascendNext(FORWARD_INCREMENT);
			}
			while (!nextReached()) {
				if (nextDescendPath(FORWARD_LIMIT, FORWARD_INCREMENT)) {
					descendNext(FORWARD_INITIALIZATION);
				}
				else {
					ascendNext(FORWARD_INCREMENT);
				}
			}

			return *this;
		}

		iterator& operator--(int) {
			if (path.back().first->isEndNode()) {
				ascendNext(BACKWARD_INCREMENT);
			}
			while (!nextReached()) {
				if (nextDescendPath(BACKWARD_LIMIT, BACKWARD_INCREMENT)) {
					descendNext(BACKWARD_INITIALIZATION);
				}
				else {
					ascendNext(BACKWARD_INCREMENT);
				}
			}

			return *this;
		}

		pointer operator->() {
			return *(path.back().first->value);
		}

		reference operator*() const {
			return *(path.back().first->value);
		}

		const bool operator==(const iterator& other) const {

			if (path.empty() != other.path.empty()) {
				return false;
			}
			else if (path.empty() && other.path.empty()) {
				return true;
			}
			else {
				return path.back().first == other.path.back().first;;
			}
			/*
			bool equalValue = path.empty() && other.path.empty();
			if (!other.path.empty() && !path.empty()) {
				equalValue = path.back().first- == other.path.back().first;
			}
			return this->container == other.container && equalValue;
			*/
		}

		const bool operator!=(const iterator& other) const {
			if (path.empty() != other.path.empty()) {
				return true;
			}
			else if (path.empty() && other.path.empty()) {
				return false;
			}
			else {
				return path.back().first != other.path.back().first;;
			}

			/*
			bool equalValue = path.empty() != other.path.empty();
			if (!path.empty() && !equalValue) {
				equalValue = path.back().first != other.path.back().first;
			}
			return this->container != other.container || equalValue;
			*/
		}

		std::vector<std::pair<BoundingSphereHierarchyNode<Store>*, intmax_t>> path;

		BoundingSphereHierarchy<Store>* container;

		std::function<bool(glm::vec4)> comparator = [&](glm::vec4 sphere) -> bool {return true; };
	};

	iterator end(std::function<bool(glm::vec4)> comp) {
		iterator it;
		it.container = this;
		it.path.clear();
		//it.path.push_back(std::make_pair<BoundingSphereHierarchyNode<Store>*, size_t>(&originNode, 1));
		it.comparator = comp;
		//it--;
		return it;
	}

	iterator begin(std::function<bool(glm::vec4)> comp) {
		iterator it;
		it.container = this;
		it.path.clear();
		it.path.push_back(std::make_pair<BoundingSphereHierarchyNode<Store>*, size_t>(&originNode, 0));
		it.comparator = comp;
		it++;

		return it;
	}

	const Store& insert(Store& elm, glm::vec4 sphere) {
		container.push_back(std::make_unique<BoundedItem<Store>>(BoundedItem<Store>(std::forward<Store>(elm), sphere)));
		return container.back()->value;
	}

	const Store& insert(Store&& elm, glm::vec4 sphere) {
		container.push_back(std::make_unique<BoundedItem<Store>>(BoundedItem<Store>(std::forward<Store>(elm), sphere)));
		return container.back()->value;
	}

	std::pair<BoundingSphereHierarchy<Store>::iterator, BoundingSphereHierarchy<Store>::iterator> equal_range(std::function<bool(glm::vec4)> comp) {
		std::pair<BoundingSphereHierarchy<Store>::iterator, BoundingSphereHierarchy<Store>::iterator> iteratorPair;
		iteratorPair.first = begin(comp);
		iteratorPair.second = end(comp);
		
		return iteratorPair;
	}

	std::vector<std::unique_ptr<BoundedItem<Store>>> container;
	BoundingSphereHierarchyNode<Store> originNode;



	void establishHierarchy() {
		if (!container.empty()) {
			std::pair<std::vector<BoundingSphereHierarchyNode<Store>>, std::optional<BoundingSphereHierarchyNode<Store>>> currentBounds;
			for (std::unique_ptr<BoundedItem<Store>>& sphere : container) {
				currentBounds.first.emplace_back(glm::vec4(sphere->center, sphere->radius), &sphere->value);
			}

			while (currentBounds.first.size() != 1 || currentBounds.second.has_value()) {
				if (currentBounds.second.has_value()) {
					currentBounds.first.push_back(std::move(currentBounds.second.value()));
					currentBounds.second.reset();
				}
				currentBounds = getHierarchyPairs(currentBounds.first);
			}

			originNode = std::move(currentBounds.first.back());
		}
	}


	template<class Store>
	std::pair<std::vector<BoundingSphereHierarchyNode<Store>>, std::optional<BoundingSphereHierarchyNode<Store>>> getHierarchyPairs(std::vector<BoundingSphereHierarchyNode<Store>>& nodes) {
		std::vector<BoundingSphereHierarchyNode<Store>> resultPairs;
		std::unordered_set<size_t> skipLocations;
		std::optional<BoundingSphereHierarchyNode<Store>> oddNode;
		size_t best = 0;
		for (size_t i = 0; i < nodes.size(); i++) {
			if (skipLocations.find(i) == skipLocations.end()) {
				HierarchyPair shortestPair{};
				shortestPair.radius = std::numeric_limits<float>::max();
				for (size_t j = 0; j < nodes.size(); j++) {
					if (skipLocations.find(j) == skipLocations.end() && i != j) {
						glm::vec3 ijCenterVector = glm::vec3(nodes[j].sphere) - glm::vec3(nodes[i].sphere);
						float ijCenterLength = glm::length(ijCenterVector);

						float radius = (ijCenterLength + nodes[i].sphere.w + nodes[j].sphere.w) / 2;

						if (radius < shortestPair.radius) {
							glm::vec3 center = glm::vec3(nodes[i].sphere);
							if (ijCenterLength != 0) {
								glm::vec3 radiusVector = (radius / ijCenterLength) * ijCenterVector;

								float ijOuterEdge = ijCenterLength + nodes[j].sphere.w;
								glm::vec3 ijOuterEdgeVector = (ijOuterEdge / ijCenterLength) * ijCenterVector;

								glm::vec3 iOffsetVector = ijOuterEdgeVector - radiusVector;
								center = iOffsetVector + glm::vec3(nodes[i].sphere);
							}
							shortestPair.radius = radius;
							shortestPair.center = center;
							shortestPair.first = i;
							shortestPair.second = j;
						}
					}
				}

				if (nodes.size() % 2 == 1 && skipLocations.size() == nodes.size() - 1) {
					oddNode = std::move(nodes.at(i));
				}
				else {
					resultPairs.emplace_back(glm::vec4(shortestPair.center, shortestPair.radius), std::move(nodes[shortestPair.first]), std::move(nodes[shortestPair.second]));
					skipLocations.insert(shortestPair.first);
					skipLocations.insert(shortestPair.second);
				}
			}
		}


		return std::make_pair(std::move(resultPairs), std::move(oddNode));
	}


	HierarchyPair calculatePairData(glm::vec4 node1, glm::vec4 node2) {

	}
	
};