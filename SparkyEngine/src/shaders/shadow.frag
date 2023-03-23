#version 330 core

layout (location = 0) out vec4 color;

uniform vec2 light_pos;
uniform vec4 light_color;

in DATA
{
    vec4 position;
    vec2 uv;
    float tid;
    vec4 color;
    vec4 ShadowCoords;
} fs_in;

uniform sampler2D textures[32];
uniform sampler2D tex;

void main()
{
    float intensityOn = 3.5 / length(fs_in.position.xy - light_pos);
    float intensityOff = 1;
    vec4 texColor = fs_in.color;
    vec4 shadowColor;
    float textBody = 0.2;
    vec4 resultColor = fs_in.color * light_color;

    if (fs_in.tid > 0.0f)
    {
        if (fs_in.tid == 1.0f)
            texColor = fs_in.color * texture(textures[0], fs_in.uv);
        if (fs_in.tid == 2.0f)
            texColor = fs_in.color * texture(textures[1], fs_in.uv);
    }

    //just shadow
    if (intensityOn <= 1.0)
        shadowColor = vec4(resultColor.x, resultColor.y, resultColor.z, 0.4f * intensityOn);
    else
        shadowColor = vec4(resultColor.x, resultColor.y, resultColor.z, 0.42f);

    if (shadowColor.w < 0.1)
        discard;

    //shadowColor = vec4(fs_in.color.x, fs_in.color.y, fs_in.color.z, fs_in.color.w * intensityOn * 2);

    //color = texColor;
    color = shadowColor;
}