#include "vpch.h"
#include "Renderer/Techniques/Technique.h"


void Technique::use() 
{
	Shader::use();
	init();
}