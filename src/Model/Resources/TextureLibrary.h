#pragma once

#include "Model/Material.h"
#include "Model/Textures/TextureViewData.h"

constexpr size_t max_texture_view_count = 128;

constexpr int pack_max_texture_side_size = 8192;
constexpr int pack_discard_step = 1;

class TextureLibrary {
	inline static int numTextures = 0;
	inline static int numTextureViews = 0;

	inline static std::unordered_map<std::string, size_t> viewIndexByTexturePath;

	inline static std::vector<TextureView> views;
	inline static std::vector<TextureData> textureData;
	
	inline static bool packingErrorReported = false;

	inline static std::vector<std::unique_ptr<Texture2DArray>> textureLibrary;

	static Texture2DArray* initializeTextureArray(int w, int h, int layers) {
		return textureLibrary.emplace_back(std::make_unique<Texture2DArray>(w, h, layers)).get();
	}

public:
	static Texture2DArray* storeTextures(const std::vector<Material>& materials) {
		std::vector<Image2D> images;
		int maxW = 0, maxH = 0;

		for (const Material& mat : materials) {
			int dataTextureViewId = numTextureViews;
			auto& data = textureData.emplace_back(dataTextureViewId);

			for (auto& locator : mat.textureTypeLocators | std::views::values) {
				auto& image = images.emplace_back(locator);

				image.view.textureViewId = numTextureViews;
				viewIndexByTexturePath[locator.path] = numTextureViews++;

				data.putView(image.view);
				views.push_back(image.view);

				maxW = std::max(maxW, image.width);
				maxH = std::max(maxH, image.height);
			}
		}

		auto [width, height] = pack2D(images);

		// todo how does calling this function multiple times work? everything stored here will have global IDs, but a new texture will be made each call

		auto ptr = initializeTextureArray(width, height, 1);
		ptr->initialize(images);
		return ptr;
	}

	static TextureView& getView(unsigned int index) {
		return views[index];
	}

	static TextureView& getView(const std::string& name) {
		return getView(viewIndexByTexturePath.at(name));
	}

	static std::array<TextureView, max_texture_view_count> getTextureViewData() {
		std::array<TextureView, max_texture_view_count> data;
		for (auto& view : views) {
			data[view.textureViewId] = view;
		}
		return data;
	}

	static std::array<TextureData, max_texture_view_count> getTextureData() {
		std::array<TextureData, max_texture_view_count> data;
		int i = 0;
		for (auto& texture : textureData) {
			data[i++] = texture;
		}
		return data;
	}

private:
	// attempt packing with rectpack2D
	static std::pair<int,int> pack2D(std::vector<Image2D>& images) {
		std::vector<rect_type> rectangles;

		for (const Image2D& img : images) {
			rectangles.push_back(img.view.rect);
		}

		auto result = attemptPacking(rectangles);
		int width = ceilPowerOfTwo(result.width);
		int height = ceilPowerOfTwo(result.height);

		// TODO find repeat strategy
		assert(!result.errorReported); // "Packing returned error."

		// overwrite views with packed data
		for (size_t i = 0; i < rectangles.size(); i++) {
			images[i].view = TextureView(images[i].view, rectangles[i]);
		}

		return { width, height };
	}

	struct PackResult {
		bool errorReported;
		int width, height;
	};
	 
	static PackResult attemptPacking(std::vector<rect_type>& rectangles) {
		packingErrorReported = false;

		auto reportSuccessful = [](rect_type&) {
			return rectpack2D::callback_result::CONTINUE_PACKING;
		};

		auto reportUnsuccessful = [](rect_type&) {
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

		return PackResult(packingErrorReported, resultSize.w, resultSize.h);
	}

	static int ceilPowerOfTwo(uint v) {
		v--;
		for (int i = 1; i <= 16; i *= 2)
			v |= v >> i;
		return ++v;
	}
};
