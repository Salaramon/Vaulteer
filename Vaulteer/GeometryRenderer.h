#include <type_traits>

#include "DeferredGeometryTechnique.h"
#include "LineTechnique.h"
#include "Scene.h"

#include "Model.h"
#include "ModelData.h"
#include "LineData.h"
#include "Camera.h"

class GeometryRenderer : public DeferredGeometryTechnique
{
public:

	using Scene = Scene<Camera, Model<ModelData>, Model<LineData>>;

	void render(Scene& scene);
};