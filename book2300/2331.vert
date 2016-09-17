uniform vec3 lightPos0;
varying vec3 N, L;
void main()
{
	N = gl_NormalMatrix * gl_Normal;
	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	L = lightPos0 - V.xyz;
	
	gl_FrontColor = gl_Color;
}
