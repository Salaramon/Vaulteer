#pragma once

#include <tuple>
#include <type_traits>

namespace stl {
	template<class Type, class... In>
	struct CheckForTypeInParameterPack {
		using type = std::disjunction<std::is_same<Type, In>...>::type;
		inline static constexpr bool value = std::disjunction_v<std::is_same<Type, In>...>;
	};

	template<class Type, class... In>
	using CheckForTypeInParameterPack_t = CheckForTypeInParameterPack<Type, In...>::type;

	template<class Type, class... In>
	constexpr bool CheckForTypeInParameterPack_v = CheckForTypeInParameterPack<Type, In...>::value;



	template<class Type, class Tuple>
	struct CheckForType {};

	template<class Type, class... In>
	struct CheckForType<Type, std::tuple<In...>> {
		using type = CheckForTypeInParameterPack_t<Type, In...>;
		inline static constexpr bool value = CheckForTypeInParameterPack_v<Type, In...>;
	};

	template<class Type, class Tuple>
	using CheckForType_t = CheckForType<Type, Tuple>::type;

	template<class Type, class Tuple>
	constexpr bool CheckForType_v = CheckForType<Type, Tuple>::value;



	template<class Superset, class Subset>
	struct SetIsSupersetOfSubset {};

	template<class Superset, class... Args>
	struct SetIsSupersetOfSubset<Superset, std::tuple<Args...>> {
		inline static constexpr bool value = std::conjunction_v<CheckForType<Args, Superset>...>;
	};

	template<class Superset, class Subset>
	constexpr bool SetIsSupersetOfSubset_v = SetIsSupersetOfSubset<Superset, Subset>::value;



	template<class Type, class Tuple>
	struct TypeExists {};

	template<class Type, class... Args>
	struct TypeExists<Type, std::tuple<Args...>> {
		inline static constexpr bool value = std::disjunction_v<std::is_same<Type, Args>...>;
		using type = std::enable_if_t<value, Type>;
	};

	template<class Type, class Tuple>
	using TypeExists_t = TypeExists<Type, Tuple>::type;

	template<class Type, class Tuple>
	constexpr bool TypeExists_v = TypeExists<Type, Tuple>::value;



	template<class Types, class Tuple>
	struct TypesExists {};

	template<class... Args, class Tuple>
	struct TypesExists<std::tuple<Args...>, Tuple> {
		inline static constexpr bool value = std::conjunction_v<TypeExists<Args, Tuple>...>;
		using type = std::enable_if_t<value, std::tuple<Args...>>;
	};

	template<class Types, class Tuple>
	using TypesExists_t = TypesExists<Types, Tuple>::type;

	template<class Types, class Tuple>
	constexpr bool TypesExists_v = TypesExists<Types, Tuple>::value;

	

	template<class Accumulator, class Type, class Tuple>
	struct _FilterType {};

	template<class Accumulator, class Type, class T, class... Args>
	struct _FilterType<Accumulator, Type, std::tuple<T, Args...>> {
		using Include = std::_Tuple_cat1<Accumulator, std::tuple<T>>::type;
		using Exclude = Accumulator;
		using type = std::conditional_t<std::is_same_v<Type, T>, typename _FilterType<Exclude, Type, std::tuple<Args...>>::type, typename _FilterType<Include, Type, std::tuple<Args...>>::type>;
	};

	template<class Accumulator, class Type, class T>
	struct _FilterType<Accumulator, Type, std::tuple<T>> {
		using Include = std::_Tuple_cat1<Accumulator, std::tuple<T>>::type;
		using Exclude = Accumulator;
		using type = std::conditional_t<std::is_same_v<Type, T>, Exclude, Include>;
	};

	template<class Type, class Tuple>
	struct FilterType {
		using type = _FilterType<std::tuple<>, Type, Tuple>::type;
	};

	template<class Type, class Tuple>
	using FilterType_t = FilterType<Type, Tuple>::type;



	template<class Accumulator, class Type, class Tuple>
	struct _InverseFilterType {};

	template<class Accumulator, class Type, class T, class... Args>
	struct _InverseFilterType<Accumulator, Type, std::tuple<T, Args...>> {
		using Include = std::_Tuple_cat1<Accumulator, std::tuple<T>>::type;
		using Exclude = Accumulator;
		using type = std::conditional_t<std::is_same_v<Type, T>, typename _InverseFilterType<Include, Type, std::tuple<Args...>>::type, typename _InverseFilterType<Exclude, Type, std::tuple<Args...>>::type>;
	};

	template<class Accumulator, class Type, class T>
	struct _InverseFilterType<Accumulator, Type, std::tuple<T>> {
		using Include = std::_Tuple_cat1<Accumulator, std::tuple<T>>::type;
		using Exclude = Accumulator;
		using type = std::conditional_t<std::is_same_v<Type, T>, Include, Exclude>;
	};

	template<class Type, class Tuple>
	struct InverseFilterType {
		using type = _InverseFilterType<std::tuple<>, Type, Tuple>::type;
	};

	template<class Type, class Tuple>
	using InverseFilterType_t = InverseFilterType<Type, Tuple>::type;



	template<class Filter, class Tuple>
	struct FilterTypes {};

	template<class T, class... Args, class Tuple>
	struct FilterTypes<std::tuple<T, Args...>, Tuple> {
		using type = FilterType_t<T, typename FilterTypes<std::tuple<Args...>, Tuple>::type>;
	};

	template<class Tuple>
	struct FilterTypes<std::tuple<>, Tuple> {
		using type = Tuple;
	};

	template<class Filter, class Tuple>
	using FilterTypes_t = FilterTypes<Filter, Tuple>::type;



	template<class Filter, class Tuple>
	struct InverseFilterTypes {};

	template<class T, class... Args, class Tuple>
	struct InverseFilterTypes<std::tuple<T, Args...>, Tuple> {
		using type = std::_Tuple_cat1<FilterType_t<T, typename InverseFilterTypes<std::tuple<Args...>, Tuple>::type>>::type;
	};

	template<class Tuple>
	struct InverseFilterTypes<std::tuple<>, Tuple> {
		using type = Tuple;
	};

	template<class Filter, class Tuple>
	using InverseFilterTypes_t = InverseFilterTypes<Filter, Tuple>::type;



	template<class TemplateType>
	struct Tuplify {};

	template<template<class...> class Template, class... Args>
	struct Tuplify<Template<Args...>> {
		using type = std::tuple<Args...>;
	};

	template<class TemplateType>
	using Tuplify_t = Tuplify<TemplateType>::type;



	template<template<class...> class... Args>
	struct TemplateTuple {

	};

	template<std::size_t I, class T>
	struct TemplateTupleElement;

	template<std::size_t I, template<class...> class Head, template<class...> class... Tail >
	struct TemplateTupleElement<I, TemplateTuple<Head, Tail...>>
		: TemplateTupleElement<I - 1, std::tuple<Tail...>> { };

	template<template<class...> class Head, template<class...> class... Tail >
	struct TemplateTupleElement<0, TemplateTuple<Head, Tail...>> {
		using type = TemplateTuple<Head>;
	};


	template<class T1, class T2>
	struct _ConcatenateTemplateTuples {};


	template<template<class...> class... Args1, template<class...> class... Args2>
	struct _ConcatenateTemplateTuples<TemplateTuple<Args1...>, TemplateTuple<Args2...>> {
		using type = TemplateTuple<Args1..., Args2...>;
	};

	template<class... Tuples>
	struct ConcatenateTemplateTuples {};

	template<class T1, class T2, class... Tuples>
	struct ConcatenateTemplateTuples<T1, T2, Tuples...> {
		using type = ConcatenateTemplateTuples<typename _ConcatenateTemplateTuples<T1, T2>::type, Tuples...>::type;
	};

	template<class T1, class T2>
	struct ConcatenateTemplateTuples<T1, T2> {
		using type = _ConcatenateTemplateTuples<T1, T2>::type;
	};
	
	template<class... Tuples>
	using ConcatenateTemplateTuples_t = ConcatenateTemplateTuples<Tuples...>::type;



	template<template<class> class Ty1, template<class> class Ty2>
	struct TemplateTypeIsSame : public std::false_type {};

	template<template<class> class T>
	struct TemplateTypeIsSame<T,T> : public std::true_type {};

	template<template<class> class Ty1, template<class> class Ty2>
	using TemplateTypeIsSame_t = TemplateTypeIsSame<Ty1, Ty2>::type;

	template<template<class> class Ty1, template<class> class Ty2>
	constexpr bool TemplateTypeIsSame_v = TemplateTypeIsSame<Ty1, Ty2>::value;



	template<class Ty1, class Ty2>
	struct IsNotSame : std::true_type {};

	template<class T>
	struct IsNotSame<T, T> : std::false_type {};

	template<class Ty1, class Ty2>
	using IsNotSame_t = IsNotSame<Ty1, Ty2>::type;

	template<class Ty1, class Ty2>
	constexpr bool IsNotSame_v = IsNotSame<Ty1, Ty2>::value;


	template<bool V>
	struct _Not {};

	template<>
	struct _Not<true> : std::false_type {};

	template<>
	struct _Not<false> : std::true_type {};

	template<class T>
	struct Not {
		inline static constexpr bool value = _Not<T::value>::value;
	};

	template<class T>
	constexpr bool Not_v = Not<T>::value;


	template<class Template>
	struct ExtractTemplateType {};

	template<template<class...> class TemplateType, class... Args>
	struct ExtractTemplateType<TemplateType<Args...>> {
	public:
		using type = TemplateTuple<TemplateType>;
	};

	template<class Template>
	using ExtractTemplateType_t = ExtractTemplateType<Template>::type;



	template<class Template>
	struct ExtractTemplateArguments {};

	template<template<class...> class TemplateType, class... Args>
	struct ExtractTemplateArguments<TemplateType<Args...>> {
	public:
		using type = std::tuple<Args...>;
	};

	template<class Template>
	using ExtractTemplateArguments_t = ExtractTemplateArguments<Template>::type;



	template<class Template>
	struct UnwrapTemplate {};

	template<template<class...> class TemplateType, class... Args>
	struct UnwrapTemplate<TemplateType<Args...>> {
		using template_tuple = stl::TemplateTuple<TemplateType>;
		using type_tuple = std::tuple<Args...>;
	};



	template<template<class...> class Template, class Tuple>
	struct ApplyTuple {};

	template<template<class...> class Template, class... Args>
	struct ApplyTuple<Template, std::tuple<Args...>> {
		using type = Template<Args...>;
	};

	template<template<class...> class Template, class Tuple>
	using ApplyTuple_t = ApplyTuple<Template, Tuple>::type;



	template<template<class...> class Template, class Tuple>
	struct ApplyEachTuple {};

	template<template<class...> class Template, class... Args>
	struct ApplyEachTuple<Template, std::tuple<Args...>> {
		using type = std::tuple<Template<Args>...>;
	};

	template<template<class...> class Template, class Tuple>
	using ApplyEachTuple_t = ApplyEachTuple<Template, Tuple>::type;



	template<template<class...> class TemplateType, template<class...> class... In>
	struct CheckForTemplateTypeInParameterPack {
		using type = std::disjunction<TemplateTypeIsSame_t<TemplateType, In>...>::type;
		static constexpr bool value = std::disjunction_v<TemplateTypeIsSame_t<TemplateType, In>...>::value;
	};

	template<template<class...> class TemplateType, template<class...> class... In>
	using CheckForTemplateTypeInParameterPack_t = CheckForTemplateTypeInParameterPack<TemplateType, In...>::type;

	template<template<class...> class TemplateType, template<class...> class... In>
	constexpr bool CheckForTemplateTypeInParameterPack_v = CheckForTemplateTypeInParameterPack<TemplateType, In...>::value;



	template<template<class...> class TemplateType, class TemplateTuple>
	struct CheckForTemplateType {};

	template<template<class...> class TemplateType, template<class...> class... In>
	struct CheckForTemplateType<TemplateType, TemplateTuple<In...>> {
		using type = CheckForTemplateTypeInParameterPack_t<TemplateType, In...>;
		inline static constexpr bool value = CheckForTemplateTypeInParameterPack_v<TemplateType, In...>;
	};

	template<template<class...> class TemplateType, class Tuple>
	using CheckForTemplateType_t = CheckForTemplateType<TemplateType, Tuple>::type;

	template<template<class...> class TemplateType, class Tuple>
	constexpr bool CheckForTemplateType_v = CheckForTemplateType<TemplateType, Tuple>::value;



	template<class Superset, class Subset>
	struct TemplateSetIsSupersetOfSubset {};

	template<class Superset, template<class...> class... Args>
	struct TemplateSetIsSupersetOfSubset<Superset, TemplateTuple<Args...>> {
		inline static constexpr bool value = std::conjunction_v<CheckForTemplateType_t<Args, Superset>...>;
	};

	template<class Superset, class Subset>
	constexpr bool TemplateSetIsSupersetOfSubset_v = TemplateSetIsSupersetOfSubset<Superset, Subset>::value;



	template<class Type, Type... Val>
	struct NonTypeTuple {};



	template<std::size_t I, class T>
	struct NonTypeTupleElement;

	template<std::size_t I, class Type, Type Head, Type... Tail >
	struct NonTypeTupleElement<I, NonTypeTuple<Type, Head, Tail...>>
		: NonTypeTupleElement<I - 1, NonTypeTuple<Type, Tail...>> { };

	template<class Type, Type Head, Type... Tail>
	struct NonTypeTupleElement<0, NonTypeTuple<Type, Head, Tail...>> {
		inline static constexpr Type value = Head;
	};

	template<bool... Val>
	using BoolTuple = NonTypeTuple<bool, Val...>;



	template<template<class, class> class Comparator, class Accumulator, class Type, class Tuple>
	struct _ConditionalFilter {};

	template<template<class, class> class Comparator, class Accumulator, class Type, class T, class... Args>
	struct _ConditionalFilter<Comparator, Accumulator, Type, std::tuple<T, Args...>> {
		using Include = std::_Tuple_cat1<Accumulator, std::tuple<T>>::type;
		using Exclude = Accumulator;
		using type = std::conditional_t<Comparator<Type, T>::value, typename _ConditionalFilter<Comparator, Exclude, Type, std::tuple<Args...>>::type, typename _ConditionalFilter<Comparator, Include, Type, std::tuple<Args...>>::type>;
	};

	template<template<class, class> class Comparator, class Accumulator, class Type, class T>
	struct _ConditionalFilter<Comparator, Accumulator, Type, std::tuple<T>> {
		using Include = std::_Tuple_cat1<Accumulator, std::tuple<T>>::type;
		using Exclude = Accumulator;
		using type = std::conditional_t<Comparator<Type, T>::value, Exclude, Include>;
	};

	template<template<class, class> class Comparator, class Type, class Tuple>
	struct ConditionalFilter {
		using type = _ConditionalFilter<Comparator, std::tuple<>, Type, Tuple>::type;
	};

	template<template<class, class> class Comparator, class Type, class Tuple>
	using ConditionalFilter_t = ConditionalFilter<Comparator, Type, Tuple>::type;



	template<template<class...> class MatchPattern, template<class, class> class Comparator, class Type, class Tuple>
	struct _0_ConditionalFilters {};

	template<template<class...> class MatchPattern, template<class, class> class Comparator, class Type, class... Args>
	struct _0_ConditionalFilters<MatchPattern, Comparator, Type, std::tuple<Args...>> {
		using type = std::conditional_t<MatchPattern<Comparator<Args, Type>...>::value, std::tuple<>, std::tuple<Type>>;
	};

	template<template<class...> class MatchPattern, template<class, class> class Comparator, class Filter, class Tuple>
	struct ConditionalFilters {};

	template<template<class...> class MatchPattern, template<class, class> class Comparator, class Filter, class... Args >
	struct ConditionalFilters<MatchPattern, Comparator, Filter, std::tuple<Args...>> {
		using type = std::_Tuple_cat1<typename _0_ConditionalFilters<MatchPattern, Comparator, Args, Filter>::type...>::type;
	};

	//NEED UPDATE
	/*
	template<template<class, class> class Comparator, class Tuple>
	struct _0_ConditionalFilters<Comparator, std::tuple<>, Tuple> {
	public:
		using type = Tuple;
	};
	*/

	template<template<class...> class MatchPattern, template<class, class> class Comparator, class Filter, class Tuple>
	using ConditionalFilters_t = ConditionalFilters<MatchPattern, Comparator, Filter, Tuple>::type;



	template<size_t I, template<class> class Comparator, class Tuple>
	struct _GetTupleIndex{};

	template<size_t I, template<class> class Comparator, class T, class... Args>
	struct _GetTupleIndex<I, Comparator, std::tuple<T, Args...>> {
		inline static constexpr size_t value = std::conditional_t<Comparator<T>::value, std::integral_constant<size_t, I>, _GetTupleIndex<I + 1, Comparator, std::tuple<Args...>>>::value;
	};

	template<size_t I, template<class> class Comparator, class T>
	struct _GetTupleIndex<I, Comparator, std::tuple<T>> {
		inline static constexpr size_t value = std::conditional_t<Comparator<T>::value, std::integral_constant<size_t, I>, std::integral_constant<size_t, -1>>::value;
	};

	template<template<class> class Comparator, class Tuple>
	struct GetTupleIndex : public _GetTupleIndex<0, Comparator, Tuple> {};
	
	template<template<class> class Comparator, class Tuple>
	constexpr size_t GetTupleIndex_v = GetTupleIndex<Comparator, Tuple>::value;
};