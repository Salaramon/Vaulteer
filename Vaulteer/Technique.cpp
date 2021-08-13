#include "Technique.h"


void Technique::use() 
{
	Shader::use();
	init();
}