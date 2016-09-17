
varying vec3 V;
varying vec3 N;
varying vec3 L;

const vec3 myRed = vec3(0.6, 0.0, 0.0);
const vec3 myYellow = vec3(0.6, 0.5, 0.0);
const vec3 myBlue = vec3(0.0, 0.3, 0.6);

const vec3 myHalfSpace0 = vec3(0.31, 0.95, 0.0);
const vec3 myHalfSpace1 = vec3(-0.81, 0.59, 0.0);
const vec3 myHalfSpace2 = vec3(-0.81, -0.59, 0.0);
const vec3 myHalfSpace3 = vec3(0.31, -0.95, 0.0);
const vec3 myHalfSpace4 = vec3(1.0, 0.0, 0.0);

const float stripeThickness = 0.4;
const float starSize = 0.2;
const float smoothEdgeTol = 0.005;
const float ambientLighting = 0.2;
const float specularExp = 60.0;
const float specularIntensity = 0.5;

void main()
{
	vec4 distVector;
	float distScalar;

	vec3 NN = normalize( N );
	vec3 NL = normalize( L );
	vec3 NV = normalize( V );
	vec3 NH = normalize( NL + vec3( 0.0, 0.0, 1.0 ) );
	
	float myInOut = -3.0;
	
	distVector.x = dot( NV, myHalfSpace0 );
	distVector.y = dot( NV, myHalfSpace1 );
	distVector.z = dot( NV, myHalfSpace2 );
	distVector.w = dot( NV, myHalfSpace3 );
	distScalar = dot( NV, myHalfSpace4 );
	
	distVector += starSize;
	distScalar += starSize;
	
	distVector = smoothstep( 0.0, smoothEdgeTol, distVector );
	distScalar = smoothstep( 0.0, smoothEdgeTol, distScalar );
	myInOut += dot( distVector, vec4(1.0) );
	myInOut += distScalar;
	myInOut = clamp( myInOut, 0.0, 1.0 );
	
	vec3 surfColor = mix( myYellow, myRed, myInOut );
	
	myInOut = smoothstep( 0.0, smoothEdgeTol, abs(NV.z) - stripeThickness );
	
	surfColor = mix( myBlue, surfColor, myInOut );
	
	surfColor *= (ambientLighting + vec3(max(0.0, dot(NN, NL) )));
	
	surfColor += (specularIntensity * vec3(pow(max(0.0, dot(NN, NH)), specularExp)));
	
	gl_FragColor = vec4( surfColor, 1.0 );
}
