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
	Container<StoreType>::iterator begin() {
		return std::get<ObjectContainer<StoreType>>(objectContainers.begin());
	}

	template<class StoreType>
	Container<StoreType>::iterator end() {
		return std::get<ObjectContainer<StoreType>>(objectContainers.end());
	}

	template<class Store>
	struct iterator {
		using IteratorType = ObjectContainer<Store>::iterator;
	public:
		using difference_type = ObjectContainer<Store>::iterator::difference_type;
		using value_type = ObjectContainer<Store>::iterator::value_type;
		using pointer = ObjectContainer<Store>::iterator::pointer;
		using reference = ObjectContainer<Store>::iterator::reference;

		iterator(IteratorType it) : it(it) {}
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

		const bool operator==(const iterator<Store>& other) const {
			return it == other.it;
		}

		const bool operator!=(const iterator<Store>& other) const {
			return it != other.it;
		}

	private:
		IteratorType it;
	};
	
	template<class StoreType>
	using StoreRangeIterator = std::pair<iterator<StoreType>, iterator<StoreType>>;


protected:
	//std::tupleContainer::iterator 
	ObjectContainerTuple objectContainers;
};