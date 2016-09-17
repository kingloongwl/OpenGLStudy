
varying vec3 V;
varying vec3 N;
varying vec3 L;

const vec3 myRed = vec3(1.0, 0.0, 0.0);
const vec3 myYellow = vec3(1.0, 1.0, 0.0);
const vec3 myGreen = vec3(0.0, 1.0, 0.0);
const vec3 myBlue = vec3(0.0, 0.0, 1.0);
const vec3 myWhite = vec3(1.0, 1.0, 1.0);
const vec3 myBlack = vec3(0.0, 0.0, 0.0);
const vec3 northHalfSpace = vec3(0.0, 0.0, 1.0);
const vec3 northeastHalfSpace = vec3(0.707, 0.0, 0.707);
const vec3 northwestHalfSpace = vec3(-0.707, 0.0, 0.707);

const float capSize = 0.03;
const float smoothEdgeTol = 0.005;
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
	
	float mirror = (NV.x >= 0.0) ? 1.0 : -1.0;
	NV.xz *= mirror;
	
	vec4 distance;
	distance.x = dot( NV, northHalfSpace );
	distance.y = dot( NV, northeastHalfSpace );
	distance.z = dot( NV, northwestHalfSpace );
	
	distance.w = abs( NV.y ) - 1.0 + capSize;
	distance = smoothstep( vec4(0.0), vec4(smoothEdgeTol), distance );
	
	vec3 surfColor = mix( myBlack, myRed, distance.x );
	surfColor += mix( myBlack, myGreen, distance.y * (1.0-distance.z) );
	surfColor = mix( surfColor, myBlue, 1.0 - distance.y );
	surfColor = mix( surfColor, myWhite, distance.w );
	
	surfColor *= (ambientLighting + vec3(max(0.0, dot(NN, NL) )));
	
	surfColor += (specularIntensity * vec3(pow(max(0.0, dot(NN, NH)), specularExp)));
	
	gl_FragColor = vec4( surfColor, 1.0 );
}
