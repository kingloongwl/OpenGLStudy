
uniform sampler2D sampler0;
uniform vec2 tc_offset[9];
void main()
{
	vec4 sample[9];
	
	for ( int i = 0; i < 9; i++ )
	{
		sample[i] = texture2D( sampler0, gl_TexCoord[0].st + tc_offset[i] );
	}
	
	gl_FragColor = (sample[0] + (2.0*sample[1]) + sample[2] +
					(2.0*sample[3]) + sample[4] + (2.0*sample[5]) +
					sample[6] + (2.0*sample[7]) + sample[8])/13.0;
}
