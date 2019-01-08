#version 410

layout(location = 0) in vec3 position;

// Data from CPU
uniform mat4 P; // Projection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;
uniform float heightFactor;

// Texture-related data
uniform sampler2D rgbTexture;
uniform int widthTexture;
uniform int heightTexture;


// Output to Fragment Shader
out vec4 textureColor; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;


void main()
{
    // get texture value, compute height
    vec2 textureCoordinate = vec2(position.x/float(widthTexture), position.z/float(heightTexture));
    textureColor = texture(rgbTexture, textureCoordinate);
    //textureColor = vec4(widthTexture,widthTexture,widthTexture,1);
    float height = heightFactor*(0.2126*textureColor.x + 0.7152*textureColor.y + 0.0722*textureColor.z);
    // compute normal vector using also the heights of neighbor vertices

    // compute toLight vector vertex coordinate in VCS

   // set gl_Position variable correctly to give the transformed vertex position
   gl_Position = P * MV *  vec4(position.x, height, position.z, 1);

}
