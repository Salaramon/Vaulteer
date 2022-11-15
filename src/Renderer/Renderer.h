#pragma once

#include <type_traits>
#include <tuple>
#include <utility>

#include "Utils/TemplateUtility.h"

#include "Scene/StaticScene.h"
#include "Scene/DynamicScene.h"

#include <vector>
#include <map>
#include <iostream>

#include "API/Camera.h"
#include "Model/Model.h"
#include "Model/Data/ModelData.h"


template<class SceneInput, class SceneData>
struct SceneMatcher {
public:

	template<class T1, class T2>
	using CheckIfTemplateContainersAreNotSame = stl::Not<std::is_same<stl::ExtractTemplateType_t<T1>, stl::ExtractTemplateType_t<T2>>>;
	template<class T1, class T2>
	using CheckIfTemplateArgumentsAreNotSame = stl::Not<stl::SetIsSupersetOfSubset<stl::ExtractTemplateArguments_t<T2>, stl::ExtractTemplateArguments_t<T1>>>;
	template<class T1, class T2>
	using CombinedChecks = std::disjunction<CheckIfTemplateContainersAreNotSame<T1, T2>, CheckIfTemplateArgumentsAreNotSame<T1, T2>>;

	using FilteredResult = stl::ConditionalFilters_t<std::conjunction, CombinedChecks, SceneData, SceneInput>;

	inline static constexpr bool filteredResultIsValid = stl::SetIsSupersetOfSubset_v<FilteredResult, SceneInput>;

};


template<bool Valid, class InputRenderer, class InputScene, class SceneData>
struct CallRenderer {};

template<class InputRenderer, class InputScenes, class... SceneData>
struct CallRenderer<true, InputRenderer, InputScenes, std::tuple<SceneData...>> {
	template<class AllowedData>
	struct Condition {
	public:
		template<class T>
		using template_type = std::is_same<stl::ExtractTemplateType_t<AllowedData>, stl::ExtractTemplateType_t<T>>;
	};
public:
	template<class... Scenes>
	static void call(Scenes&... scenes) {
		
		//debug overload which only require class as template parameter but does not require a pointer to it is needed
		DIRLOG::SPCT::debug<CallRenderer<true, InputRenderer, InputScenes, std::tuple<SceneData...>>>(DY::std_format("Valid renderer was found for the scenes: {} and was reordered to {}",
			DY::types_to_string<Scenes...>(),
			DY::pack_to_string(stl::GetTupleIndex_v<typename Condition<SceneData>::template template_type, InputScenes>...)
		));

		InputRenderer::render(
			std::get<
				stl::GetTupleIndex_v<typename Condition<SceneData>::template template_type, InputScenes>
			>(std::tie<Scenes...>(scenes...))...
		);
	}

};

template<class InputRenderer, class InputScenes, class... SceneData>
struct CallRenderer<false, InputRenderer, InputScenes, std::tuple<SceneData...>> {
public:
	template<class... Scenes>
	static void call(Scenes&... scenes) {
		LOG::SPGL::debug<&CallRenderer::call<DynamicScene<>>, CallRenderer>(DY::std_format("No valid renderer was found for the scenes: {}", DY::types_to_string<Scenes...>()));
	}

	inline static auto FR = DY::FunctionRegister<&CallRenderer::call<DynamicScene<>>>("call");
	inline static auto FB = DY::FunctionBinder(FR);

	using LOG = _LOG<DY::No_CB, DY::No_OB, decltype(FB), DY::No_VB>;
};


template<class DedicatedRenderer>
class _Renderer {
public:
	template<class... Scenes>
	static void render(Scenes&... scenes) {
		using Match = SceneMatcher<std::tuple<Scenes...>, typename DedicatedRenderer::SceneData>;

		LOG::SPGL::debug<&_Renderer<DedicatedRenderer>::render<DynamicScene<>>, _Renderer<DedicatedRenderer>>(DY::std_format("Renderer found the following match: {}", DY::types_to_string<Match>()));

		CallRenderer<Match::filteredResultIsValid, DedicatedRenderer, std::tuple<Scenes...>, typename DedicatedRenderer::SceneData>::call(scenes...);
	}

	inline static auto FR = DY::FunctionRegister<&_Renderer<DedicatedRenderer>::render<DynamicScene<>>>("render");
	inline static auto FB = DY::FunctionBinder(FR);

	using LOG = _LOG<DY::No_CB, DY::No_OB, decltype(FB), DY::No_VB>;
};

template<class... DedicatedRenderers>
class Renderer {
public:
	template<class... Scenes>
	static void render(Scenes&... scenes){
		LOG::SPGL::debug<&Renderer<DedicatedRenderers...>::render<DynamicScene<>>, Renderer<DedicatedRenderers...>>(DY::std_format("Rendering scenes of type: {}", DY::types_to_string<Scenes...>()));
		(_Renderer<DedicatedRenderers>::render(scenes...), ...);
	}
	inline static auto FR = DY::FunctionRegister<&Renderer<DedicatedRenderers...>::render<DynamicScene<>>>("render");
	inline static auto FB = DY::FunctionBinder(FR);

	using LOG = _LOG<DY::No_CB, DY::No_OB, decltype(FB), DY::No_VB>;
	
};
