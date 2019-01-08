#version 410

layout(location = 0) in vec3 position;

// Data from CPU
uniform mat4 P; // Projection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec3 cameraPosition;
uniform float heightFactor;

// Texture-related data
uniform sampler2D rgbTexture;
uniform int widthTexture;
uniform int heightTexture;


// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;

vec3 findNormal(in vec3 currentPosition, in vec2 neighbor1, in vec2 neighbor2){
    vec4 neighbor1Color = texture(rgbTexture, vec2(-neighbor1.x/float(widthTexture), -neighbor1.y/float(heightTexture)));
    float neighbor1Height = heightFactor*(0.2126*neighbor1Color.x + 0.7152*neighbor1Color.y + 0.0722*neighbor1Color.z);
    vec3 neighbor1Position = vec3(neighbor1.x, neighbor1Height, neighbor1.y);
    vec4 neighbor2Color = texture(rgbTexture, vec2(-neighbor2.x/float(widthTexture), -neighbor2.y/float(heightTexture)));
    float neighbor2Height = heightFactor*(0.2126*neighbor2Color.x + 0.7152*neighbor2Color.y + 0.0722*neighbor2Color.z);
    vec3 neighbor2Position = vec3(neighbor2.x, neighbor2Height, neighbor2.y);
    vec3 v1 = neighbor1Position - currentPosition;
    vec3 v2 = neighbor2Position - currentPosition;
    return (normalize(cross(v2, v1)));
}

void main()
{
    // get texture value, compute height
    textureCoordinate = vec2(-position.x/float(widthTexture), -position.z/float(heightTexture));
    vec4 textureColor = texture(rgbTexture, textureCoordinate);
    float height = heightFactor*(0.2126*textureColor.x + 0.7152*textureColor.y + 0.0722*textureColor.z);
    vec3 currentPosition = vec3(position.x, height, position.z);

    // compute normal vector using also the heights of neighbor vertices
    vec3 totalNormal = vec3(0.0,0.0,0.0);
    float count = 0;
    if(position.x!=0 && position.z!=0){
        totalNormal += findNormal(currentPosition, vec2(currentPosition.x-1, currentPosition.z), vec2(currentPosition.x-1, currentPosition.z-1));
        totalNormal += findNormal(currentPosition, vec2(currentPosition.x-1, currentPosition.z-1), vec2(currentPosition.x, currentPosition.z-1));
        count += 2;
    }
    if(position.x!=widthTexture && position.z!=heightTexture){
        totalNormal += findNormal(currentPosition, vec2(currentPosition.x+1, currentPosition.z), vec2(currentPosition.x+1, currentPosition.z+1));
        totalNormal += findNormal(currentPosition, vec2(currentPosition.x+1, currentPosition.z+1),vec2(currentPosition.x, currentPosition.z+1));
        count += 2;
    }
    if(position.x!=0 && position.z!=heightTexture){
        totalNormal += findNormal(currentPosition,vec2(currentPosition.x, currentPosition.z+1), vec2(currentPosition.x-1, currentPosition.z));
        count += 1;
    }
    if(position.x!=widthTexture && position.z!=0){
        totalNormal += findNormal(currentPosition,vec2(currentPosition.x+1, currentPosition.z), vec2(currentPosition.x, currentPosition.z-1));
        count += 1;
    }
    vertexNormal = normalize(totalNormal/count);

    // compute toLight vector vertex coordinate in VCS
    vec3 lightPosition = vec3(float(widthTexture)/float(2), widthTexture+heightTexture, float(heightTexture)/float(2));
    ToLightVector = normalize(lightPosition - currentPosition);
    ToCameraVector = normalize(cameraPosition - currentPosition); 

   // set gl_Position variable correctly to give the transformed vertex position
   gl_Position = P * MV * vec4(currentPosition,1.0);

}
