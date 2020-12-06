#version 330 core

in vec2 _texcoords;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main(){
	FragColor = texture(ourTexture,_texcoords);
	if(FragColor.a<0.5)discard;
}