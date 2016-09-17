
varying vec3 V;
varying vec3 N;
varying vec3 L;

const vec3 onColor = vec3(1.0, 1.0, 1.0);
const vec3 offColor = vec3(0.0, 0.0, 0.0);

const float ambientLighting = 0.2;
const float specularExp = 60.0;
const float specularIntensity = 0.75;

const int numSquaresPerSide = 8;
void main()
{
	vec3 NN = normalize( N );
	vec3 NL = normalize( L );
	vec3 NV = normalize( V );
	vec3 NH = normalize( NL + vec3( 0.0, 0.0, 1.0 ) );
	
	vec3 onOrOff = ( (NV + 1.0) * float( numSquaresPerSide ) ) / 2.0;
	onOrOff = step( 1.0, mod(onOrOff, 2.0) );
	onOrOff.x = step( 0.5, mod(onOrOff.x + onOrOff.y + onOrOff.z, 2.0 ) );
	
	vec3 surfColor = mix( offColor, onColor, onOrOff.x );
	
	surfColor *= (ambientLighting + vec3(max(0.0, dot(NN, NL) )));
	
	surfColor += (specularIntensity * vec3(pow(max(0.0, dot(NN, NH)), specularExp)));
	
	gl_FragColor = vec4( surfColor, 1.0 );
}
