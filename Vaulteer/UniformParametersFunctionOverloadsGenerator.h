#pragma once

#include "FunctionExpressionGeneration.h"
#include "SetUniformFunctor.h"


/*
class FunctionExpressionGeneration
{
public:
	template<class ReturnType, class... Params>
	ReturnType operator()(Params&... params) {
		return FunctionData(functionType, params...);
	};


};
*/

//Variadic template class generalization
template<const size_t n, class PreTypes, class Functor, class FunctionType, class... Args>
class _UniformParametersFunctionOverloadsGenerator {
public:

};

//Variadic template class entrypoint
template<const size_t n, template<class...> class PreTypes, class... Types, class Functor, class FunctionType, class Param>
class _UniformParametersFunctionOverloadsGenerator<n, PreTypes<Types...>, Functor, FunctionType, Param> : public _UniformParametersFunctionOverloadsGenerator<n - 1, PreTypes<Types...>, Functor, FunctionType, Param, Param> {
public:
	_UniformParametersFunctionOverloadsGenerator(const Functor* fp) : fp(fp), _UniformParametersFunctionOverloadsGenerator<n - 1, PreTypes<Types...>, Functor, FunctionType, Param, Param>(fp) {};
	using _UniformParametersFunctionOverloadsGenerator<n - 1, PreTypes<Types...>, Functor, FunctionType, Param, Param>::operator();

	FunctionType operator()(Types... types, Param param) {
		return (FunctionType *fp)(types..., param);
	}

private:
	const Functor* fp;
};

//Variadic template class recursion
template<const size_t n, template<class...> class PreTypes, class... Types, class Functor, class FunctionType, class Param, class... Rest>
class _UniformParametersFunctionOverloadsGenerator<n, PreTypes<Types...>, Functor, FunctionType, Param, Rest...> : public _UniformParametersFunctionOverloadsGenerator<n - 1, PreTypes<Types...>, Functor, FunctionType, Param, Param, Rest...> {
public:
	_UniformParametersFunctionOverloadsGenerator(const Functor* fp) : fp(fp), _UniformParametersFunctionOverloadsGenerator<n - 1, PreTypes<Types...>, Functor, FunctionType, Param, Param, Rest...>(fp) {};
	using _UniformParametersFunctionOverloadsGenerator<n - 1, PreTypes<Types...>, Functor, FunctionType, Param, Param, Rest...>::operator();

	FunctionType operator()(Types... types, Param param, Rest... rest) {
		return (FunctionType *fp)(types..., param, rest...);
	}
private:
	const Functor* fp;
};

//Variadic template class endpoint 
template<template<class...> class PreTypes, class... Types, class Functor, class FunctionType, class Param>
class _UniformParametersFunctionOverloadsGenerator<0, PreTypes<Types...>, Functor, FunctionType, Param> {
public:
	_UniformParametersFunctionOverloadsGenerator(const Functor* fp) : fp(fp) {};
	FunctionType operator()(Types... types, Param param) {
		return (FunctionType *fp)(types..., param);
	}
private:
	const Functor* fp;
};

//Variadic template class endpoint 
template<template<class...> class PreTypes, class... Types, class Functor, class FunctionType, class Param, class... Rest>
class _UniformParametersFunctionOverloadsGenerator<0, PreTypes<Types...>, Functor, FunctionType, Param, Rest...> {
public:
	_UniformParametersFunctionOverloadsGenerator(const Functor* fp) : fp(fp) {};
	FunctionType operator()(Types... types, Param param, Rest... rest) {
		return (FunctionType *fp)(types..., param, rest...);
	}
private:
	const Functor *fp;
};

template<const size_t n, class PreTypes, class Functor, class FunctionType, class... Params>
class UniformParametersFunctionOverloadsGenerator : public _UniformParametersFunctionOverloadsGenerator<n-1, PreTypes, Functor, FunctionType, Params>...
{
public:
	UniformParametersFunctionOverloadsGenerator(const Functor* fp) : _UniformParametersFunctionOverloadsGenerator<n-1, PreTypes, Functor, FunctionType, Params>(fp)... {};
	using _UniformParametersFunctionOverloadsGenerator<n-1, PreTypes, Functor, FunctionType, Params>::operator()...;
};

