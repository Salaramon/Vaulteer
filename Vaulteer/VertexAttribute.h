#pragma once

#include <string>
#include <vector>

#include "VertexArray.h"


class VertexAttribute
{
public:
	template<class StorageType, class shrType>
	VertexAttribute(VertexBuffer& vertexArray, VertexBuffer& vertexBuffer, shr::LocIn<shrType>& location)

private:
	/*
	template<class Container>
	struct TemplateArgument : public TemplateArgument<Container> {
		using Type = TemplateArgument<Container>::Type;
	};

	template <template<class> class ContainerType, class Container>
	struct TemplateArgument<ContainerType<Container>>{
		using Type = ContainerType;
	};

	static std::vector<GLuint> takenShaderLayoutLocationIDs;
	*/
};

template<class Type,class shrType>
inline VertexAttribute::VertexAttribute(VertexBuffer& vertexArray, VertexBuffer& vertexBuffer, shr::LocIn<shrType>& location)
{
	glBindVertexArray(vertexArray.getVAO());

	glEnableVertexAttribArray(location.id());
	glVertexAttribPointer(location.id(), location.size(), GL_FLOAT, GL_FALSE, vertexBuffer.byteSize(), 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));
	
	glBindVertexArray(0);

}
