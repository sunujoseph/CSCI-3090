in vec3 texCoord;
in vec3 dir;
uniform sampler3D tex;


void main(void) {
	int slices = 10;
	float step = 0.1;
	vec4 colour;
	int i;
	vec3 coord = texCoord;
	float value;
	
	float maxVal = 0.0;
	colour = vec4(0.1, 0.1, 0.1, 1.0);
	for(i=0; i<slices; i++) {
		value = texture(tex, coord).r;
			if(value > maxVal){
				maxVal = value;
				colour = vec4(0.0, 0.0, maxVal, 1.0);
			}
		coord += step*dir;
	}
	gl_FragColor = colour;

	

}
