#version 450
layout( triangles_adjacency ) in;
layout( triangle_strip, max_vertices = 18 ) out;

in vec3 vpos[];

layout (std140, binding = 0) uniform Camera {
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 screenProjection;
};

uniform vec3 lightPos;

float EPSILON = 0.01;

bool facesLight( vec3 a, vec3 b, vec3 c ) {
  vec3 n = cross( b - a, c - a );
  vec3 da = lightPos.xyz - a;
  vec3 db = lightPos.xyz - b;
  vec3 dc = lightPos.xyz - c;

  return dot(n, da) > 0 || dot(n, db) > 0 || dot(n, dc) > 0; 
}

void emitEdgeQuad( vec3 a, vec3 b ) {
	vec3 LightDir = normalize(a - lightPos.xyz); 
	vec3 deviation = LightDir * EPSILON;
	gl_Position = projection * vec4(a + deviation, 1);
	EmitVertex();
  
	gl_Position = projection * vec4(LightDir, 0);
	EmitVertex();

	LightDir = normalize(b - lightPos.xyz); 
	deviation = LightDir * EPSILON;
	gl_Position = projection * vec4(b + deviation, 1);
	EmitVertex();

	gl_Position = projection * vec4(LightDir, 0);
	EmitVertex();
	EndPrimitive();
}

void main() {
    if (facesLight(vpos[0], vpos[2], vpos[4])) {
        if (!facesLight(vpos[0],vpos[1],vpos[2])) 
			emitEdgeQuad(vpos[0],vpos[2]);
        if (!facesLight(vpos[2],vpos[3],vpos[4])) 
			emitEdgeQuad(vpos[2],vpos[4]);
        if (!facesLight(vpos[4],vpos[5],vpos[0])) 
			emitEdgeQuad(vpos[4],vpos[0]);

		//FRONT CAP
		vec3 LightDir = normalize(vpos[0] - lightPos.xyz); 
		vec3 deviation = LightDir * EPSILON;
		gl_Position = projection * vec4(vpos[0] + deviation, 1);
		EmitVertex();

		LightDir = normalize(vpos[2] - lightPos.xyz); 
		deviation = LightDir * EPSILON;
		gl_Position =  projection * vec4(vpos[2] + deviation, 1);
		EmitVertex();

		LightDir = normalize(vpos[4] - lightPos.xyz); 
		deviation = LightDir * EPSILON;
		gl_Position =  projection * vec4(vpos[4] + deviation, 1);
		EmitVertex();
	    EndPrimitive();
		
		//BACK CAP
		LightDir = normalize(vpos[0] - lightPos.xyz); 
		gl_Position = projection * vec4(LightDir, 0);
		EmitVertex();

		LightDir = normalize(vpos[4] - lightPos.xyz); 
		gl_Position =  projection * vec4(LightDir, 0);
		EmitVertex();

		LightDir = normalize(vpos[2] - lightPos.xyz); 
		gl_Position =  projection * vec4(LightDir, 0);
		EmitVertex();
	    EndPrimitive();
    }

}