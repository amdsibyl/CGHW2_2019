#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 vp;
uniform mat4 view;

uniform vec3 light_pos;
uniform vec3 eye_pos;

uniform int per_vertex;
uniform int blinn_shading;

out vec3 v_position;
out vec2 v_uv;
out vec3 v_normal;

out vec3 v_diff;
out vec3 v_spec;

void main()
{
    v_position = (view * model * vec4(position, 1.0)).xyz;
    v_uv = texcoord;
    v_normal = (view * model * vec4(normal, 0.0)).xyz;
    gl_Position = vp * model * vec4(position, 1.0);

    v_diff = vec3(1.0);
    v_spec = vec3(1.0);

    if(per_vertex == 1){ // per-vertex
        vec3 l = normalize(light_pos - v_position);
        vec3 n = normalize(v_normal);
        float cosine = max(dot(l, n), 0); // diffuse
        
        vec3 e = normalize(eye_pos - v_position);
        float spec = 0.0;
        if (blinn_shading == 1) // Blinn-Phong
        {
            vec3 h = normalize(l + e);
            spec = cosine * pow(max(dot(h, n), 0), 32);
        }
        else { // Phong
            vec3 r = reflect(-l, n);
            spec = cosine * pow(max(dot(r, e), 0), 32);
        }
        v_diff = cosine * vec3(1.5);
        v_spec = spec * vec3(1.0);
        //v_color = vec4(cosine * vec3(1.5) + spec * vec3(1.0), 1.0);

    }

}