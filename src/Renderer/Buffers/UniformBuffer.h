#pragma once

#include <any>

#include "GLSLCPPBinder.h"
#include "Model/Data/ModelData.h"

#include "Debug/Debug.h"

class UniformBuffer : Buffer<BufferType::UniformBuffer> {
public:
	struct DrawHint {
		inline static constexpr GLenum Dynamic = GL_DYNAMIC_DRAW;	// modify repeatedly, use often
		inline static constexpr GLenum Static  = GL_STATIC_DRAW;	// modify once, use often
		inline static constexpr GLenum Stream  = GL_STREAM_DRAW;	// modify once, use rarely
	};

	UniformBuffer(Binder::UniformBufferInfo bufferInfo, GLenum hint = DrawHint::Dynamic);
	UniformBuffer(UniformBuffer&& other) noexcept;

	template<class T>
	static void insert(UniformBuffer& ubo, const std::vector<T>& data) {
		size_t dataSize = data.size() * sizeof(data[0]);
		LOG::SPGL::debug<static_cast<void(*)(UniformBuffer&, const std::vector<float>&)>(insert), UniformBuffer>(
			std::format("Inserting {} bytes into UBO {}", dataSize, ubo.buffer)
		);
		assert(dataSize <= ubo.size);

		glBindBufferBase(GL_UNIFORM_BUFFER, ubo.binding, ubo.buffer);
		glNamedBufferData(ubo.buffer, std::min(ubo.size, dataSize), data.data(), ubo.drawHint);
	}

	template<class T>
	static void insert(UniformBuffer& ubo, const T& data) {
		size_t dataSize = sizeof(data);
		LOG::SPGL::debug<static_cast<void(*)(UniformBuffer&, const float&)>(insert), UniformBuffer>(
			std::format("Inserting {} bytes into UBO {}", dataSize, ubo.buffer)
		);
		assert(dataSize <= ubo.size);

		glBindBufferBase(GL_UNIFORM_BUFFER, ubo.binding, ubo.buffer);
		glNamedBufferData(ubo.buffer, std::min(ubo.size, dataSize), &data, ubo.drawHint);
	}

private:
	const size_t binding;
	const size_t size;

	const GLenum drawHint;

public:
	inline static auto FR = DY::FunctionRegister<
        static_cast<void(*)(UniformBuffer&, const std::vector<float>&)>(insert),
        static_cast<void(*)(UniformBuffer&, const float&)>(insert)
	>("insert<vector>", "insert<>");

	DY::ObjectRegister<UniformBuffer,
		decltype(binding),
		decltype(size),
		decltype(drawHint)> OR;
	
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();
	inline static auto FB = DY::FunctionBinder(FR);

	using LOG = _LOG<DY::No_CB, decltype(OB), decltype(FB), DY::No_VB>;

};

