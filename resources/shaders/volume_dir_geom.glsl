#version 450
layout( triangles_adjacency ) in;
layout( triangle_strip, max_vertices = 18 ) out;

in vec3 vpos[];

layout (std140, binding = 0) uniform Camera {
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 screenProjection;
};

uniform vec3 lightDir_viewSpace;

vec3 lightDir = normalize(vec3(lightDir_viewSpace.x, lightDir_viewSpace.y * 1.0001, lightDir_viewSpace.z));
float EPSILON = 0.01;

bool facesLight( vec3 a, vec3 b, vec3 c ) {
  vec3 n = cross( b - a, c - a );
  vec3 da = -lightDir;

  return dot(n, da) > 0; 
}

void emitEdgeQuad( vec3 a, vec3 b ) {
	vec3 deviation = lightDir * EPSILON;
	gl_Position = projection * vec4(a + deviation, 1);
	EmitVertex();
  
	gl_Position = projection * vec4(lightDir, 0);
	EmitVertex();

	gl_Position = projection * vec4(b + deviation, 1);
	EmitVertex();

	gl_Position = projection * vec4(lightDir, 0);
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
		vec3 deviation = -lightDir * EPSILON;
		gl_Position = projection * vec4(vpos[0] + deviation, 1);
		EmitVertex();

		gl_Position =  projection * vec4(vpos[2] + deviation, 1);
		EmitVertex();

		gl_Position =  projection * vec4(vpos[4] + deviation, 1);
		EmitVertex();
	    EndPrimitive();
		
		//BACK CAP
		gl_Position = projection * vec4(-lightDir, 0);
		EmitVertex();

		gl_Position =  projection * vec4(-lightDir, 0);
		EmitVertex();

		gl_Position =  projection * vec4(-lightDir, 0);
		EmitVertex();
	    EndPrimitive();
    }

}