#pragma once

#include <any>

#include "GLSLCPPBinder.h"
#include "Model/Data/ModelData.h"

#include "Debug/Debug.h"

class UniformBuffer : Buffer<BufferType::UniformBuffer> {
public:
	struct DrawHint {
		inline static constexpr GLenum Dynamic = GL_DYNAMIC_DRAW; // modify repeatedly, use often
		inline static constexpr GLenum Static  = GL_STATIC_DRAW; // modify once, use often
		inline static constexpr GLenum Stream  = GL_STREAM_DRAW; // modify once, use rarely
	};

	UniformBuffer(const Binder::UniformBufferInfo& bufferInfo, GLenum hint = DrawHint::Dynamic);
	UniformBuffer(UniformBuffer&& other) noexcept;

	template<class T>
	static void insert(UniformBuffer& ubo, const std::vector<T>& data) {
		size_t dataSize = data.size() * sizeof(data[0]);
		
		LOG::SPGL::debug<DY::OverloadSelector<void(UniformBuffer&, const std::vector<T>&)>::Get<&UniformBuffer::insert<T>>, UniformBuffer>(
			std::format("Inserting {} bytes into UBO {}", dataSize, ubo.buffer)
		);
		assert(dataSize <= ubo.size);

		glBindBufferBase(GL_UNIFORM_BUFFER, ubo.binding, ubo.buffer);
		glNamedBufferData(ubo.buffer, std::min(ubo.size, dataSize), data.data(), ubo.drawHint);
	
	}

	template<class T>
	static void insert(UniformBuffer& ubo, const T& data) {
		size_t dataSize = sizeof(data);

		// IF LNK1179 OCCUR EX.(https://zal.s-ul.eu/3lZYOnhU) it probably means debugging is done improper. A quick fix is to use static cast instead.
		LOG::SPGL::debug<DY::OverloadSelector<void(UniformBuffer&, const T&)>::Get<&UniformBuffer::insert<T>>, UniformBuffer>(
			std::format("Inserting {} bytes into UBO {}", dataSize, ubo.buffer)
		);
		
		assert(dataSize <= ubo.size);

		glBindBufferBase(GL_UNIFORM_BUFFER, ubo.binding, ubo.buffer);
		glNamedBufferData(ubo.buffer, std::min(ubo.size, dataSize), &data, ubo.drawHint);
		
		//LOG::SPGL::debug<static_cast<void(*)(UniformBuffer&, const T&)>(&UniformBuffer::insert<T>), UniformBuffer>(
		//	std::format("Inserting {} bytes into UBO {}", dataSize, ubo.buffer)
		//);
	}

	UniformBuffer(UniformBuffer& other) = delete;
	UniformBuffer(const UniformBuffer& other) = delete;
	UniformBuffer(const UniformBuffer&& other) = delete;

private:
	const size_t binding;
	const size_t size;

	const GLenum drawHint;

public:
	inline static auto FR = DY::FunctionRegister <
		DY::OverloadSelector<void(UniformBuffer&, const std::vector<ModelData::ModelUnitData>&)>::Get<&UniformBuffer::insert<ModelData::ModelUnitData>>,
		DY::OverloadSelector<void(UniformBuffer&, const std::vector<Material::MaterialData>&)>::Get<&UniformBuffer::insert<Material::MaterialData>>,
		DY::OverloadSelector<void(UniformBuffer&, const glm::mat4&)>::Get<&UniformBuffer::insert<glm::mat4>>
	>("insert<std::vector<ModelUnitData>>",  "insert<std::vector<MaterialData>>", "insert<glm::mat4>");

	DY::ObjectRegister<UniformBuffer,
		decltype(binding),
		decltype(size),
		decltype(drawHint)> OR;
	
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();
	inline static auto FB = DY::FunctionBinder(FR);

	using LOG = _LOG<DY::No_CB, decltype(OB), decltype(FB), DY::No_VB>;

};