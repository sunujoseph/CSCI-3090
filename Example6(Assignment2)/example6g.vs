/*
 *  Simple vertex shader for example six
 */

in vec4 vPosition;
in vec3 vNormal;
uniform mat4 modelView;
uniform mat4 projection;
uniform mat3 normalMat;
out vec3 normal;
out vec4 position;
out vec4 Idiff;
out vec4 Iamb;
out vec4 Ispec;



void main() {

	vec3 v = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	//vec3 N = vNormal;

	vec3 L = normalize(gl_LightSource[0].position.xyz - v);
	vec3 E = normalize(-v); 
	vec3 R = normalize(-reflect(L,N));
	
	//calculate Ambient Term:  
	vec4 Iamb = gl_FrontLightProduct[0].ambient;    

	//calculate Diffuse Term:  
	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.1);    

	// calculate Specular Term:
	vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);


	//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	
	gl_Position = projection * modelView * vPosition;
	position = vPosition;
	normal = vNormal;
} 