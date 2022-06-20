#include "vpch.h"
#include "Renderer/Techniques/ForwardTechnique.h"

void ForwardTechnique::setModelView(const glm::dmat4& model, const glm::dmat4& view) {
    glm::fmat4 modelView = view * model;
    shader->setUniform(Binder::forward_vertex::uniforms::modelView, 1, GL_FALSE, modelView);
    setNormal(model);
}

void ForwardTechnique::setNormal(const glm::mat4& normal) {
    shader->setUniform(Binder::forward_vertex::uniforms::normal, 1, GL_FALSE, glm::transpose(glm::inverse(normal)));
}

void ForwardTechnique::setInverseViewMatrix(const glm::mat4& view) {
    shader->setUniform(Binder::forward_frag::uniforms::inverseViewMat, 1, GL_FALSE, glm::inverse(view));
}

void ForwardTechnique::setTextureUnit(const GLint& texture) {
    shader->setUniform(Binder::forward_frag::uniforms::textureLib, texture);
}


void ForwardTechnique::uploadProjection(const glm::mat4& projection) {
    auto& camera = getUBCamera();
    UniformBuffer::insert(camera, projection);
}

void ForwardTechnique::uploadModelUnitTables(const std::vector<ModelData*>& dataVector) {
    std::vector<ModelData::ModelUnitData> unitData;
    for (auto& data : dataVector) {
        unitData.push_back(data->getModelUnitTable().diffuseUnit);
        unitData.push_back(data->getModelUnitTable().specularUnit);
        unitData.push_back(data->getModelUnitTable().normalMapUnit);
    }

    auto& modelUnitTables = getUBModelUnitTables();
    UniformBuffer::insert(modelUnitTables, unitData);
}

void ForwardTechnique::uploadMaterialData(const std::vector<ModelData*>& modelVector) {
    std::vector<Material::MaterialData> materials;
    for (const auto& model : modelVector) {
        for (const auto& mesh : model->getMeshes()) {
            materials.push_back(mesh.material.data);
        }
    }
    auto& materialData = getUBMaterialData();
    UniformBuffer::insert(materialData, materials);
}
