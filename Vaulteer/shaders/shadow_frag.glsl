#version 330 core

void main()
{
	// depth is set (from gl_FragCoord.z) automatically like so:
	//gl_FragDepth = gl_FragCoord.z;
	gl_FragDepth = gl_FragCoord.z;
}

