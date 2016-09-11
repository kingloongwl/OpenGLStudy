
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 V = gl_ModelViewMatrix * gl_Vertex;
	
	gl_FrontColor = gl_Color;
	
	float ptSize = length(V);
	ptSize *= ptSize;
	gl_Position = 100000.0 / ptSize;
	
}
