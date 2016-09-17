uniform vec3 lightPos;
varying vec3 N, L, V;
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	V = gl_Vertex.xyz;
	
	N = gl_NormalMatrix * gl_Normal;
	
	vec4  Veye = gl_ModelViewMatrix * gl_Vertex;
	L = lightPos - Veye.xyz;
}
