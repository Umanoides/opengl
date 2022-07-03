#version 450

in vec3 out_color;
in vec3 view_dir;
in vec3 frag_light_dir;
in vec3 frag_normal;
in vec2 interpolated_uv;
in vec3 frag_tangent;

uniform sampler2D normal_map;

layout(location = 0) out vec4 output_color;

vec3 CalcBumpedNormal()
{
    vec3 normal = normalize(frag_normal);
    vec3 tangent = normalize(frag_tangent);

    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);

    vec3 bump_map_normal = texture(normal_map, interpolated_uv).xyz;

    bump_map_normal = bump_map_normal / 2.0 + vec3(0.5, 0.5, 0.5);

    vec3 result_normal;
    mat3 TBN = mat3(tangent, bitangent, normal);
    result_normal = TBN * bump_map_normal;
    //result_normal = normalize(result_normal);

    return result_normal;
}

void main()
{
    vec3 light = vec3(1.0, 1.0, 1.0); // Could be pass as uniform

    // Bump mapping
    vec3 normal = CalcBumpedNormal();

    // Specular
    float spec_strength = 0.3;
    vec3 reflect_dir = reflect(-frag_light_dir, normalize(normal));
    float spec = pow(abs(dot(view_dir, reflect_dir)), 25);
    vec3 specular = spec_strength * spec * light;

    // Diffuse
    vec3 diffuse = clamp(dot(normal, frag_light_dir), 0.0, 1.0) * light;

    vec3 final = (specular + diffuse) * out_color;

    output_color = vec4(final, 1.0);
}