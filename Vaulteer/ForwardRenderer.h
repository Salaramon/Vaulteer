#pragma once

#include <type_traits>

#include "ForwardTechnique.h"
#include "LineTechnique.h"
#include "Scene.h"

#include "Model.h"
#include "ModelData.h"
#include "LineData.h"
#include "Camera.h"

class ForwardRenderer : public ForwardTechnique, public LineTechnique
{
public:

	using Scene = Scene<Camera, Model<ModelData>, Model<LineData>>;

	void render(Scene& scene);
};