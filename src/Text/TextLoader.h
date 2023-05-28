#pragma once

#include <array>
#include <nlohmann/json.hpp>

#include "Utils/FileUtils.h"
#include "Utils/TypeDefUtils.h"
#include "Model/Storage/QuadVertex.h"

struct Quad {
	Quad(float w, float h, float x, float y) {
		vertices[0] = QuadVertex({x, y, 0}, glm::vec2(0.0, 1.0));
		vertices[1] = QuadVertex({x + w, y, 0}, glm::vec2(1.0));
		vertices[2] = QuadVertex({x + w, y + h, 0}, glm::vec2(1.0, 0.0));
		vertices[3] = QuadVertex({x, y + h, 0}, glm::vec2(0.0));

		indices = {0, 1, 2, 2, 3, 0};
	}

	std::array<QuadVertex, 4> vertices;
	std::array<uint, 6> indices;
};

class TextLoader {
public:
	using json = nlohmann::json;

	struct Glyph {
		char code;
		float advance;

		struct PlaneBounds {
			float left, bottom, right, top;
		} planeBounds;

		struct AtlasBounds {
			int left, bottom, right, top;
		} atlasBounds;
	};

	struct FontAtlas {
		/*
		 * type
		 * distanceRange
		 * size (pts)
		 * width
		 * height
		 * yOrigin
		 */
		
		/*
		 * emSize (1)
		 * lineHeight
		 * ascender
		 * descender
		 * underlineY
		 * underlineThickness
		 */

		uint width, height;

		std::array<Glyph, 96> ascii; 
	} fontAtlas;



	GLint loadTexture() {
		const char* path = "resources/fonts/hack/msdf.json";
		json mask = json::parse(openFile(path));

		populateGlyphs(mask["glyphs"]);

		fontAtlas.width = mask["atlas"]["width"].get<uint>();
		fontAtlas.height = mask["atlas"]["height"].get<uint>();

		Image2D image({"resources/fonts/hack/msdf.png", aiTextureType_DIFFUSE});
		Texture2DArray texture(fontAtlas.width, fontAtlas.height);
		std::vector imgs = {image};
		texture.initialize(imgs);

		return 0;
	}


	void populateGlyphs(json glyphArr) {
		for (auto& maskGlyph : glyphArr) {
			int code = maskGlyph["unicode"].get<int>();
			json plane = maskGlyph["planeBounds"];
			json atlas = maskGlyph["atlasBounds"];

			Glyph::PlaneBounds planeBounds;
			if (!plane.is_null()) planeBounds = {plane["left"].get<float>(), plane["bottom"].get<float>(), plane["right"].get<float>(), plane["top"].get<float>()};
			else planeBounds = {0,0,0,0};

			Glyph::AtlasBounds atlasBounds;
			if (!atlas.is_null()) atlasBounds = {atlas["left"].get<int>(), atlas["bottom"].get<int>(), atlas["right"].get<int>(), atlas["top"].get<int>()};
			else atlasBounds = {0,0,0,0};

			fontAtlas.ascii[code - 32] = Glyph(
				code,
				maskGlyph["advance"].get<float>(),
				planeBounds,
				atlasBounds
			);
		}
	}
};


