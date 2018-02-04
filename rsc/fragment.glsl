#version 410 core
out vec4 fragColor;
in vec4 vertexColor;	// used for input from vertex shader
uniform vec4 uColor;			// used for input from program (CPU)
uniform float drawPoints = 1.0;		// used for debugging. Will change color of just points

void main()
{
	fragColor = vec4(1.0, drawPoints, 0.0, 1.0);	// if not point - red, if point - yellow
	//fragColor = vertexColor;
}