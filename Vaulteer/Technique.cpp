#include "Technique.h"

Technique::Technique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode) 
	: shader(Shader(pathGLSLVertexCode, pathGLSLFragmentCode))
{
}


void Technique::use() 
{
	shader.use();
	init();
}