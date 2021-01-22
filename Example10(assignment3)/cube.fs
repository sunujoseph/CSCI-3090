/*
 *  Simple fragment sharder for Lab 2
 */

#version 330 core

in vec3 normal;
in vec3 position;
uniform samplerCube tex; //irradiance
uniform vec3 Eye;


vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   



void main() {

	vec3 F0 = vec3(0.04); 
	float cosTheta = dot(normal, position);
	float roughness = 0.5;
	float ao = 0.5;
	
	vec3 N = normal;
	
	
	
	vec3 tc;
	//vec3 r;
	vec3 V = -normalize(Eye-position);
	//r = refract(V, normal,1.50);
	tc = reflect(V,normal);
	
	//vec3 albedo = pow(texture(tex, position).rgb, 2.2);
	
	vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness); 
	vec3 kD = 1.0 - kS;
	vec3 irradiance = texture(tex, N).rgb;
	vec3 diffuse    = irradiance;
	vec3 ambient    = (kD * diffuse) * ao; 
	
	
	
	//vec3 ambient = texture(tex, normal).rgb;
	gl_FragColor = texture(tex,normalize(position));
	//gl_FragColor = texture(tex,tc);

}