#version 330 core

layout (location = 0) out vec4  color;

uniform vec4 colour;
uniform vec4 light_color;
uniform int text;

in DATA
{
    vec4 position;
    vec2 uv;
    float tid;
    vec4 color;
} fs_in;


uniform sampler2D textures[16];
uniform sampler2D tex;

void main()
{
    vec4 texColor = fs_in.color;
    float textBody = 0.9;

    if (fs_in.tid > 0.0f)
    {
        if (fs_in.tid == 1.0f)
        texColor = fs_in.color * texture(textures[0], fs_in.uv);
        if (fs_in.tid == 2.0f)
        texColor = fs_in.color * texture(textures[1], fs_in.uv);
        if (fs_in.tid == 3.0f)
        texColor = fs_in.color * texture(textures[2], fs_in.uv);
        if (fs_in.tid == 4.0f)
        texColor = fs_in.color * texture(textures[3], fs_in.uv);
        if (fs_in.tid == 5.0f)
        texColor = fs_in.color * texture(textures[4], fs_in.uv);
        if (fs_in.tid == 6.0f)
        texColor = fs_in.color * texture(textures[5], fs_in.uv);
        if (fs_in.tid == 7.0f)
        texColor = fs_in.color * texture(textures[6], fs_in.uv);
        if (fs_in.tid == 8.0f)
        texColor = fs_in.color *  texture(textures[7], fs_in.uv);
        if (fs_in.tid == 9.0f)
        texColor = fs_in.color *  texture(textures[8], fs_in.uv);
        if (fs_in.tid == 10.0f)
        texColor = fs_in.color *  texture(textures[9], fs_in.uv);
        if (fs_in.tid == 11.0f)
        texColor = fs_in.color *  texture(textures[10], fs_in.uv);
        if (fs_in.tid == 12.0f)
        texColor = fs_in.color *  texture(textures[11], fs_in.uv);
        if (fs_in.tid == 13.0f)
        texColor = fs_in.color *  texture(textures[12], fs_in.uv);
        if (fs_in.tid == 14.0f)
        texColor = fs_in.color *  texture(textures[13], fs_in.uv);
        if (fs_in.tid == 15.0f)
        texColor = fs_in.color *  texture(textures[14], fs_in.uv);
        if (fs_in.tid == 16.0f)
        texColor = fs_in.color *  texture(textures[15], fs_in.uv);
    }

    if (fs_in.color.w < textBody) {
        discard;
    }

    color = texColor;
}

