#pragma once

#include <array>
#include <nlohmann/json.hpp>

#include "Model/Resources/FontResourceLocator.h"
#include "Model/Storage/Quad.h"
#include "Utils/FileUtils.h"

struct Glyph {
	char code;
	float advance;

	struct PlaneBounds {
		float left, bottom, right, top;
	} planeBounds;

	struct AtlasBounds {
		float left, bottom, right, top;
	} atlasBounds;

	float width() {
		return planeBounds.right - planeBounds.left;
	}

	float height() {
		// assuming yOrigin: bottom
		return planeBounds.top - planeBounds.bottom;
	}
};

struct FontAtlas {
	/*
	 * atlas.type
	 * atlas.distanceRange
	 * atlas.size (pts)
	 * atlas.yOrigin
	 *
	 * metrics.emSize (1)
	 * metrics.lineHeight
	 * metrics.ascender
	 * metrics.descender
	 * metrics.underlineY
	 * metrics.underlineThickness
	 */

	uint size; // pixels per em
	uint width, height;
	float lineHeight;

	std::array<Glyph, 96> ascii;
};

struct Font {
	FontAtlas atlas;
	std::unique_ptr<Texture2D> mapTexture;

	Font(FontAtlas atlas, const std::string& texturePath) : atlas(atlas) {
		Image2D image{texturePath, aiTextureType_DIFFUSE};
		mapTexture = std::make_unique<Texture2D>(image);
	}
};

class FontLibrary {
public:
	using json = nlohmann::json;

	inline static std::unordered_map<std::string, std::unique_ptr<Font>> fontLibrary; 

	static std::tuple<glm::vec2, std::vector<Quad>> stringToQuads(const Font* font, std::string& chars, glm::vec2 pos, float size = 1.0) {
		std::vector<Quad> quads;
		quads.reserve(chars.size());

		auto& atlas = font->atlas;

		float biggestW = 0.f, biggestH = 0.f;
		float lastGlyphW = 0.f;

		float sumAdvanceW = 0.f, sumAdvanceH = 0.f;
		float scaleFactor = atlas.size * size;

		for (auto it = chars.begin(); it != chars.end(); it++) {
			char c = *it;

			float advance = 0.f;
			if (c >= 32) {
				Glyph g = atlas.ascii[c - 32];
				advance = g.advance;

				if (g.height() != 0.f) {
					glm::vec2 cursorPos = pos + glm::vec2(sumAdvanceW, sumAdvanceH) * scaleFactor;
					lastGlyphW = g.width();
 					quads.push_back(quadFromGlyph(atlas, g, cursorPos, scaleFactor));
				}
			}

			if (it + 1 != chars.end()) {
				if (c == '\n') {
					sumAdvanceW = 0.f;
					sumAdvanceH += atlas.lineHeight;
					biggestH = std::max(biggestH, sumAdvanceH);
				}
				else {
					sumAdvanceW += advance;
					biggestW = std::max(biggestW, sumAdvanceW);
				}
			}
		}

		return {{(biggestW + lastGlyphW) * scaleFactor, biggestH * scaleFactor}, quads};
	}

	static Quad quadFromGlyph(const FontAtlas& atlas, Glyph& g, glm::vec2 pos, float scale) {
		uint aw = atlas.width, ah = atlas.height;
		glm::vec2 texFrom = {g.atlasBounds.left / aw, 1.0 - g.atlasBounds.bottom / ah};
		glm::vec2 texTo = {g.atlasBounds.right / aw, 1.0 - g.atlasBounds.top / ah};

		glm::vec2 truePos = {pos.x + g.planeBounds.left * scale, pos.y - g.planeBounds.bottom * scale};
		return Quad(truePos.x, truePos.y, g.width() * scale, g.height() * scale, texFrom, texTo);
	}


	static Font* loadFont(const FontResourceLocator locator) {
		json mask = json::parse(openFile(locator.atlasJsonPath));

		FontAtlas atlas;

		for (auto& maskGlyph : mask["glyphs"]) {
			json planeB = maskGlyph["planeBounds"];
			json atlasB = maskGlyph["atlasBounds"];

			Glyph::PlaneBounds planeBounds;
			if (!planeB.is_null()) planeBounds = {planeB["left"].get<float>(), planeB["bottom"].get<float>(), planeB["right"].get<float>(), planeB["top"].get<float>()};
			else planeBounds = {0, 0, 0, 0};

			Glyph::AtlasBounds atlasBounds;
			if (!atlasB.is_null()) atlasBounds = {atlasB["left"].get<float>(), atlasB["bottom"].get<float>(), atlasB["right"].get<float>(), atlasB["top"].get<float>()};
			else atlasBounds = {0, 0, 0, 0};
			
			int code = maskGlyph["unicode"].get<int>();
			atlas.ascii[code - 32] = Glyph(
				code,
				maskGlyph["advance"].get<float>(),
				planeBounds,
				atlasBounds
			);
		}

		atlas.width = mask["atlas"]["width"].get<uint>();
		atlas.height = mask["atlas"]["height"].get<uint>();
		atlas.size = mask["atlas"]["size"].get<uint>();

		atlas.lineHeight = mask["metrics"]["lineHeight"].get<float>();

		auto font = std::make_unique<Font>(atlas, locator.atlasMapPath);
		Font* fontPtr = font.get();
		fontLibrary[locator.name] = std::move(font);
		return fontPtr;
	}

};
