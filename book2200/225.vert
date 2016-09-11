uniform vec3 lightPos0;
uniform float density;
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
	
	const float e = 2.71828;
	float fogFactor = (density * length(V));
	fogFactor *= fogFactor;
	fogFactor = clamp(pow(e, -fogFactor), 0.0, 1.0 );
	const vec4 fogColor = vec4(0.5, 0.8, 0.5, 1.0);
	
	gl_FrontColor = mix( fogColor, clamp(diffuse + specular, 0.0, 1.0), fogFactor );
}
