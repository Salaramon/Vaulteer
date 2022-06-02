#include "vpch.h"
#include "Model/Textures/PackedTexture2DArray.h"

PackedTexture2DArray::PackedTexture2DArray(std::vector<TextureResourceLocator> locators, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
    : Texture2DArray(locators, mipmapEnabled, repeatX, repeatY) {
    createPacked();

    //glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // hack OMG!!!!!
    //glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

PackedTexture2DArray::PackedTexture2DArray(TextureResourceLocator locator, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
    : Texture2DArray(locator, mipmapEnabled, repeatX, repeatY) {
}

PackedTexture2DArray::PackedTexture2DArray(GLsizei width, GLsizei height, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
    : Texture2DArray(width, height, mipmapEnabled, repeatX, repeatY) {
}

Texture2DArray::TextureUnit PackedTexture2DArray::getUnit(std::string texturePath) const {
    return Texture2DArray::getUnit(texturePath);
}

void PackedTexture2DArray::createPacked() {
    std::vector<Image2D> images;

    int maxComp = 0;
    int loadCount = 0;

    for (int i = 0; i < locators.size(); i++) {
        GLsizei w, h, comp;
        byte* data = stbi_load(locators[i].path.data(), &w, &h, &comp, 0);
        images.push_back(Image2D(TextureUnit(this, 0, 0, w, h, 0, locators[i].type), locators[i].path, data));

        if (images[i].loaded()) {
            loadCount++;
            maxComp = std::max(comp, maxComp);
        }
        else std::cout << "Failed to load texture: \n\t" << locators[i].path << "\n" << std::endl;
    }

    // attempt packing

    std::vector<rect_type> rectangles;
    bool success = true;

    for (Image2D img : images) {
        rectangles.push_back(img.unit);
    }

    attemptPacking(rectangles);
    width = ceilPowerOfTwo(width);
    height = ceilPowerOfTwo(height);

    // overwrite full sized units with packed data
    for (int i = 0; i < rectangles.size(); i++) {
        images[i].unit = TextureUnit(images[i].unit, rectangles[i]);
    }

    // finish texture data aggregation
    nrComponents = maxComp;

    if (loadCount == locators.size() && !packingErrorReported) {
        auto [inFormat, exFormat] = getFormatsFromComponents(nrComponents);
        createTextureArrayFromData(inFormat, exFormat, images);
    }
    else std::cout << "Texture data not stored: \t" << std::to_string(textureID) << "\n"
        << (packingErrorReported ? "Packing error reported" : "false") << std::endl;

    for (Image2D img : images) {
        if (img.loaded())
            stbi_image_free(img.data);

        units[img.path] = img.unit;
    }
}

bool PackedTexture2DArray::attemptPacking(std::vector<rect_type>& rectangles) {
    auto report_successful = [this](rect_type&) {
        return rectpack2D::callback_result::CONTINUE_PACKING;
    };

    auto report_unsuccessful = [this](rect_type&) {
        std::cout << "Packing unsuccessful for texture " << getTextureID() << std::endl;
        packingErrorReported = true;
        return rectpack2D::callback_result::ABORT_PACKING;
    };

    const auto runtime_flipping_mode = rectpack2D::flipping_option::ENABLED;
    const auto result_size = rectpack2D::find_best_packing<spaces_type>(
        rectangles,
        make_finder_input(
            pack_max_texture_side_size,
            pack_discard_step,
            report_successful,
            report_unsuccessful,
            runtime_flipping_mode
        ));

    width = result_size.w;
    height = result_size.h;

    return packingErrorReported;
}

uint PackedTexture2DArray::ceilPowerOfTwo(uint v) {
    v--;
    for (int i = 1; i <= 16; i *= 2)
        v |= v >> i;
    return ++v;
}
