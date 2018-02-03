#version 410 core
out vec4 fragColor;
in vec4 vertexColor;	// used for input from vertex shader
uniform vec4 uColor;			// used for input from program (CPU)

void main()
{
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//fragColor = vertexColor;
}