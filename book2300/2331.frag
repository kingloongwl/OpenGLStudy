
varying vec3 N, L;
void main()
{
	float intensity = max( 0.0, dot(normalize(N), normalize(L)));
	
	gl_FragColor = gl_Color;
	gl_FragColor.rgb *= intensity;
}
