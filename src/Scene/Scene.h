#pragma once

#include <memory>
#include <tuple>
#include <type_traits>

#include <vector>

template<template<class> class Container, class... Stores>
class Scene {
public:
	template<class StoreType>
	using Object = std::unique_ptr<StoreType>;
	template<class StoreType>
	using ObjectContainer = Container<Object<StoreType>>;

	using ObjectContainerTuple = std::tuple<ObjectContainer<Stores>...>;

	Scene() {}

	template<class StoreType>
	typename Container<StoreType>::iterator begin() {
		return std::get<ObjectContainer<StoreType>>(objectContainers.begin());
	}

	template<class StoreType>
	typename Container<StoreType>::iterator end() {
		return std::get<ObjectContainer<StoreType>>(objectContainers.end());
	}

	template<class Store>
	struct iterator {
		using iterator_type = typename ObjectContainer<Store>::iterator;
	public:
		using difference_type = typename ObjectContainer<Store>::iterator::difference_type;
		using value_type = typename ObjectContainer<Store>::iterator::value_type;
		using pointer = typename ObjectContainer<Store>::iterator::pointer;
		using reference = typename ObjectContainer<Store>::iterator::reference;

		iterator(iterator_type it) : it(it) {}
		iterator(const iterator<Store>& other) : it(other.it) {}

		iterator<Store> operator++(int) {
			it++;
			return *this;
		}

		iterator<Store>& operator++() {
			++it;
			return *this;
		}

		iterator<Store> operator--(int) {
			it--;
			return *this;
		}

		iterator<Store>& operator--() {
			--it;
			return *this;
		}

		pointer operator->() {
			return *it;
		}

		reference operator*() const {
			return *it;
		}

		bool operator==(const iterator<Store>& other) const {
			return it == other.it;
		}

		bool operator!=(const iterator<Store>& other) const {
			return it != other.it;
		}

	private:
		iterator_type it;
	};
	
	template<class StoreType>
	using StoreRangeIterator = std::pair<iterator<StoreType>, iterator<StoreType>>;


protected:
	//std::tupleContainer::iterator 
	ObjectContainerTuple objectContainers;
};