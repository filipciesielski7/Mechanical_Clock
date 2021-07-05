#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

layout (location=0) in vec4 vertex;
layout (location=1) in vec4 normal;
layout (location=2) in vec2 texCoord;

out vec2 iTexCoord;
out vec4 l1;
out vec4 l2;
out vec4 l3;
out vec4 n;
out vec4 v;

void main(void) {
    gl_Position=P*V*M*vertex;
    vec4 lp[3];
    lp[0] = vec4(0, 5, -1, 1);
    lp[1] = vec4(2, -1, -2, 1);
    lp[2] = vec4(0.3, 4.3, -2, 1);

    l1 = normalize(V * lp[0] - V*M*vertex);
    l2 = normalize(V * lp[1] - V*M*vertex);
    l3 = normalize(V * lp[2] - V*M*vertex);
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex);
    n = normalize(V * M * normal);

    iTexCoord=texCoord;
}
