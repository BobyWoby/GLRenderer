#version 330 core

layout(location = 0) in vec3 position;
uniform vec3 u_Rot;

out vec4 pos;

void main() {
    mat3 rotX;
    rotX[0] = vec3(1, 0, 0);
    rotX[1] = vec3(0, cos(u_Rot[0]), -sin(u_Rot[0]));
    rotX[2] = vec3(0, sin(u_Rot[0]), cos(u_Rot[0]));

    mat3 rotY;
    rotY[0] = vec3(cos(u_Rot[1]), 0, sin(u_Rot[1]));
    rotY[1] = vec3(0, 1, 0);
    rotY[2] = vec3(-sin(u_Rot[1]), 0, cos(u_Rot[1]));

    mat3 rotZ;
    rotZ[0] = vec3(cos(u_Rot[2]), -sin(u_Rot[2]), 0);
    rotZ[1] = vec3(sin(u_Rot[2]), cos(u_Rot[2]), 0);
    rotZ[2] = vec3(0, 0, 1);

    vec3 tmp_pos;
    tmp_pos =  rotX * rotY *  rotZ * position;

    pos =  vec4(tmp_pos[0], tmp_pos[1], tmp_pos[2], 1.0);
    gl_Position = pos;
};
