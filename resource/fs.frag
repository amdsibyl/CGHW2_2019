#version 330
layout(location=0) out vec4 color;

uniform sampler2D text;
uniform vec3 object_color;
uniform vec3 light_pos;
uniform vec3 eye_pos;

uniform int per_vertex;
uniform int blinn_shading;

in vec3 g_position;
in vec2 g_uv;
in vec3 g_normal;

in vec3 g_diff;
in vec3 g_spec;

void main()
{
    if(per_vertex == 0){ // per-pixel
        vec3 l = normalize(light_pos - g_position);
        vec3 n = normalize(g_normal);
        float cosine = max(dot(l, n), 0); // diffuse
        
        vec3 e = normalize(eye_pos - g_position);
        float spec = 0.0;
        if (blinn_shading == 1) // Blinn-Phong
        {
            vec3 h = normalize(l + e);
            spec = cosine * pow(max(dot(h, n), 0), 30);
        }
        else { // Phong
            vec3 r = reflect(-l, n);
            spec = cosine * pow(max(dot(r, e), 0), 30);
        }
        color = vec4(object_color * texture(text, g_uv).rgb * cosine * vec3(1.5) + spec * vec3(1.0), 1.0);

    }
    else{ // per-vertex
        color = vec4(object_color * texture(text, g_uv).rgb * g_diff + g_spec, 1.0);
    }
    // color = vec4(g_normal, 1.0);
}