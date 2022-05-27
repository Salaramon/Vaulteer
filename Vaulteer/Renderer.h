#pragma once

#include <type_traits>
#include <tuple>
#include <utility>

#include "RendererPrerequisites.h"
#include "Scene.h"
#include "TemplateUtility.h"

#include "StaticScene.h"
#include "DynamicScene.h"

#include <vector>
#include <map>
#include <iostream>

#include "Camera.h"
#include "Model.h"
#include "ModelData.h"
#include "ForwardRenderer.h"


template<class SceneInput, class SceneData>
struct SceneMatcher {
public:

	template<class T1, class T2>
	using CheckIfTemplateContainersAreNotSame = stl::Not<std::is_same<stl::ExtractTemplateType_t<T1>, stl::ExtractTemplateType_t<T2>>>;
	template<class T1, class T2>
	using CheckIfTemplateArgumentsAreNotSame = stl::Not<stl::SetIsSupersetOfSubset<stl::ExtractTemplateArguments_t<T1>, stl::ExtractTemplateArguments_t<T2>>>;
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
		
		//((std::cout << stl::GetTupleIndex_v<typename Condition<SceneData>::template template_type, InputScenes> << " "), ...);

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
	static void call(Scenes&... scenes) {}
};


template<class DedicatedRenderer>
class _Renderer {
public:
	template<class... Scenes>
	static void render(Scenes&... scenes) {
		using Match = SceneMatcher<std::tuple<Scenes...>, typename DedicatedRenderer::SceneData>;
		CallRenderer<Match::filteredResultIsValid, DedicatedRenderer, std::tuple<Scenes...>, typename DedicatedRenderer::SceneData>::call(scenes...);
	}
};

template<class... DedicatedRenderers>
class Renderer {
public:
	template<class... Scenes>
	static void render(Scenes&... scenes){
		(_Renderer<DedicatedRenderers>::render(scenes...), ...);
	}
};