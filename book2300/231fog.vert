uniform vec3 lightPos0;
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 N = normalize( gl_NormalMatrix * gl_Normal );
	vec3 V = gl_ModelViewMatrix * gl_Vertex;
	vec3 L = normalize( lightPos0 - V.xyz );
	
	float NdotL = dot( N, L );
	gl_FrontColor = gl_Color * vec4( max(0.0, NdotL) );
}
