#version 120
attribute vec3 position;
attribute vec3 normal;

varying vec3 FragPos;
varying vec3 Normal;

uniform mat4 modelview;
uniform mat4 projection;
uniform mat3 timodel;
void main()
{
    Normal = timodel * normalize(normal);
    vec4 pos = vec4(position, 1.0);
    FragPos = vec3(modelview * pos);
    gl_Position = projection * modelview * pos;
}
