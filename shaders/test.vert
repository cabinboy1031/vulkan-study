#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec3 fragColor;

void main(void)
{
    // Some drivers don't like position being written here
    // with the tessellation stages enabled also.
    // Comment next line when Tess.Eval shader is enabled.
    vec4 vertices[3] = {
        vec4(-0.5f, -0.5f, 0.0f,1),
        vec4(0.5f, -0.5f, 0.0f,1),
        vec4(0.0f,  0.5f, 0.0f,1)
    };  
    
    vec3 colors[3] = vec3[](
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
    );
    
    gl_Position = vertices[gl_VertexIndex];
    fragColor = colors[gl_VertexIndex];
    }