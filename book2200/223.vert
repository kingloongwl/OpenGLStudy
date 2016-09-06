uniform vec3 lightPos0;
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 N = normalize( gl_NormalMatrix * gl_Normal );
	vec3 V = gl_ModelViewMatrix * gl_Vertex;
	vec3 L = normalize( lightPos0 - V.xyz );
	vec3 H = normalize( L + vec3(0.0, 0.0, 1.0) );
	const float specularExp = 128.0;
	
	float NdotL = dot( N, L );
	vec4 diffuse = gl_Color * vec4(max(0.0, NdotL));
	
	float NdotH = dot( N, H );
	vec4 specular = vec4( pow( max(0.0, NdotH), specularExp ) );
	
	gl_FrontColor = diffuse + specular;
}
