#version 430
/* compute shader inputs	
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID;
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID;
in uint gl_LocalInvocationIndex;
*/

layout(local_size_x = 1, local_size_y = 1) in;

//ssbo initialization
struct VolumeTileAABB{
    vec4 minPoint;
    vec4 maxPoint;
};
layout (std430, binding = 1) buffer clusterAABB{
    VolumeTileAABB cluster[ ];
};
layout (std430, binding = 2) buffer screenToView{
    mat4 inverseProjection;
    uvec4 tileSizes;
    uvec2 screenDimensions;
};

//Shared between all clusters
uniform float zNear;
uniform float zFar;

void main() {
    //Eye position is zero in view space
    const vec3 eyePos = vec3(0.0);
    
}