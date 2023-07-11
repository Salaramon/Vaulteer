#pragma once

#include <array>
#include <nlohmann/json.hpp>

#include "Model/Resources/FontResourceLocator.h"
#include "Model/Storage/Quad.h"
#include "Utils/FileSystem.h"

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

enum class Justify {
	Left,
	Center,
	Right
};

class FontLibrary {
public:
	using json = nlohmann::json;

	inline static std::unordered_map<std::string, std::unique_ptr<Font>> fontLibrary;

	// produces a set of quads and the bounds for string of text with the given font
	static std::tuple<glm::vec2, std::vector<Quad>> stringToQuads(
		const Font* font, std::string& chars, glm::vec2 pos, float size = 1.0, Justify justify = Justify::Left) {
		std::vector<Quad> quads;
		quads.reserve(chars.size());

		auto& atlas = font->atlas;

		float biggestW = 0.f;
		float firstGlyphW = 0.f, lastGlyphW = 0.f;

		uint numLines = 1;
		std::vector<float> lineWidths;
		std::vector<uint> lineBreakPos;

		float sumAdvanceW = 0.f, sumAdvanceH = 0.f;
		float emToPixels = atlas.size * size;

		for (auto it = chars.begin(); it != chars.end(); it++) {
			char c = *it;

			float advance = 0.f;
			if (c >= 32 && c < 128) {
				Glyph g = atlas.ascii[c - 32];
				advance = g.advance;

				if (g.height() != 0.f) {
					if (firstGlyphW == 0.f)
						firstGlyphW = g.planeBounds.left;

					glm::vec2 cursorPos = pos + glm::vec2(sumAdvanceW, sumAdvanceH) * emToPixels;
					lastGlyphW = g.planeBounds.right;
					quads.push_back(quadFromGlyph(atlas, g, cursorPos, emToPixels));
				}
				else {
					lastGlyphW = 0.f;
				}
			}

			if (it + 1 != chars.end()) {
				if (c == '\n') {
					lineWidths.push_back(sumAdvanceW + lastGlyphW - firstGlyphW);
					lineBreakPos.push_back(quads.size());
					numLines++;
					firstGlyphW = 0.f;

					sumAdvanceW = 0.f;
					sumAdvanceH += atlas.lineHeight;
				}
				else {
					sumAdvanceW += advance;
					biggestW = std::max(biggestW, sumAdvanceW);
				}
			}
			else {
				sumAdvanceW += advance;
				lineWidths.push_back(sumAdvanceW + lastGlyphW - firstGlyphW);
				lineBreakPos.push_back(quads.size());

				biggestW = std::max(biggestW, sumAdvanceW);
			}
		}

		if (justify == Justify::Center) {
			float justifyMove = -lineWidths[0] / 2;
			uint lineIndex = 0;
			for (uint i = 0; i < quads.size();) {
				if (i == lineBreakPos[lineIndex]) {
					justifyMove = -lineWidths[++lineIndex] / 2;
				}
				else {
					quads[i].x += justifyMove * emToPixels;
					i++;
				}
			}
		}
		else if (justify == Justify::Right) {
			float justifyMove = -lineWidths[0];
			uint lineIndex = 0;
			for (uint i = 0; i < quads.size();) {
				if (i == lineBreakPos[lineIndex]) {
					justifyMove = -lineWidths[++lineIndex];
				}
				else {
					quads[i].x += justifyMove * emToPixels;
					i++;
				}
			}
		}

		glm::vec2 bounds = {(biggestW + lastGlyphW - firstGlyphW) * emToPixels, atlas.lineHeight * numLines * emToPixels};
		return {bounds, quads};
	}

	static Quad quadFromGlyph(const FontAtlas& atlas, Glyph& g, glm::vec2 pos, float emToPixels) {
		uint aw = atlas.width, ah = atlas.height;
		glm::vec2 texFrom = {g.atlasBounds.left / aw, 1.0 - g.atlasBounds.bottom / ah};
		glm::vec2 texTo = {g.atlasBounds.right / aw, 1.0 - g.atlasBounds.top / ah};

		glm::vec2 truePos = {pos.x + g.planeBounds.left * emToPixels, pos.y - g.planeBounds.bottom * emToPixels};
		return Quad(truePos.x, truePos.y, g.width() * emToPixels, g.height() * emToPixels, texFrom, texTo);
	}


	static Font* loadFont(const FontResourceLocator locator) {
		json mask = json::parse(FileSystem::openFile(locator.atlasJsonPath));

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
		atlas.ascii[95] = Glyph('\x7F', atlas.ascii[0].advance, {0, 0, 0, 0}, {0, 0, 0, 0});

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
