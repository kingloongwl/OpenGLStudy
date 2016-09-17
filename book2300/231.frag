
uniform sampler1D sampler0;
void main()
{
	float gray = dot(gl_Color.rgb, vec3(0.3, 0.59, 0.11) );
	//gl_FragColor = vec4( gray, gray, gray, 1.0 );	//灰度
	
	//gl_FragColor = vec4( gray * vec3(1.2, 1.0, 0.8), 1.0 ); //调棕色
	
	//gl_FragColor.rgb = 1.0 - gl_Color.rgb;	//反色
	//gl_FragColor.a = 1.0;
	
	gl_FragColor = texture1D( sampler0, gray );
}
