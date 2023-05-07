#pragma once

#include "Model/Material.h"
#include "Model/Textures/TextureViewData.h"

class TextureLibrary {
	inline static size_t numTextures = 0;
	inline static size_t numTextureViews = 0;

	inline static std::unordered_map<std::string, size_t> viewIndexByTexturePath;

	inline static std::vector<TextureView> views;
	inline static std::vector<TextureData> textureData;
	
	
	inline static bool packingErrorReported = false;

public:
	static void storeTextures(const std::vector<Material>& materials) {
		std::vector<Image2D> images;
		for (const Material& mat : materials) {
			int dataTextureViewId = numTextureViews;
			auto& data = textureData.emplace_back(dataTextureViewId);

			for (auto& locator : mat.textureTypeLocators | std::views::values) {
				auto& image = images.emplace_back(locator);
				viewIndexByTexturePath[locator.path] = numTextureViews++;
				data.putView(image.view);
			}
		}
	}

	static std::vector<TextureView>& getAllViews() {
		return views;
	}

private:
	/*
	static void pack() {
 		std::vector<Image2D> images;

		int maxComp = 0;
		size_t loadCount = 0;

		for (int i = 0; i < locators.size(); i++) {
			GLsizei w, h, comp;
			byte* data = stbi_load(locators[i].path.data(), &w, &h, &comp, 0);
			images.push_back(Image2D(TextureView({0, 0, w, h}, 0, locators[i].type), locators[i].path, data));

			if (images[i].loaded()) {
				loadCount++;
				maxComp = std::max(comp, maxComp);
			}
			else std::cout << "Failed to load texture: \n\t" << locators[i].path << "\n" << std::endl;
		}

		// attempt packing

		std::vector<rect_type> rectangles;

		for (const Image2D& img : images) {
			rectangles.push_back(img.view);
		}

		packingErrorReported = attemptPacking(rectangles);
		width = ceilPowerOfTwo(width);
		height = ceilPowerOfTwo(height);

		assert(!errorReported); // "Packing returned error." // TODO find repeat strategy

		// overwrite full sized units with packed data
		for (size_t i = 0; i < rectangles.size(); i++) {
			images[i].view = TextureView(images[i].view, rectangles[i]);
		}

		// finish texture data aggregation
		nrComponents = maxComp;

		if (loadCount == locators.size() && !packingErrorReported) {
			auto [inFormat, exFormat] = getFormatsFromComponents(nrComponents);
			createTextureArrayFromData(inFormat, exFormat, images);
		}
		else
			std::cout << "Texture data not stored: \t" << std::to_string(textureID) << "\n -"
				<< (packingErrorReported ? "packing error reported" : "no packing error") << std::endl;

		for (Image2D& img : images) {
			if (img.loaded())
				stbi_image_free(img.data);

			views[img.path] = img.view;
		}
	}
	 
	bool attemptPacking(std::vector<rect_type>& rectangles) {
		packingErrorReported = false;

		auto reportSuccessful = [this](rect_type&) {
			return rectpack2D::callback_result::CONTINUE_PACKING;
		};

		auto reportUnsuccessful = [this](rect_type&) {
			std::cout << "Packing unsuccessful for texture " << getTextureID() << std::endl;
			packingErrorReported = true;
			return rectpack2D::callback_result::ABORT_PACKING;
		};

		constexpr auto runtimeFlippingMode = rectpack2D::flipping_option::ENABLED;
		const auto resultSize = rectpack2D::find_best_packing<spaces_type>(
			rectangles,
			make_finder_input(
				pack_max_texture_side_size,
				pack_discard_step,
				reportSuccessful,
				reportUnsuccessful,
				runtimeFlippingMode
			));

		width = resultSize.w;
		height = resultSize.h;

		return packingErrorReported;
	}

	int ceilPowerOfTwo(uint v) {
		v--;
		for (int i = 1; i <= 16; i *= 2)
			v |= v >> i;
		return ++v;
	}
	 */
};
