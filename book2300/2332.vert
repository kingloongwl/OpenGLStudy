uniform vec3 lightPos0;
uniform vec3 lightPos1;
uniform vec3 lightPos2;
varying vec3 N, L[3];
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	N = gl_NormalMatrix * gl_Normal;
	L[0] = lightPos0 - V.xyz;
	L[1] = lightPos1 - V.xyz;
	L[2] = lightPos2 - V.xyz;
	
	gl_FrontColor = gl_Color;
}
