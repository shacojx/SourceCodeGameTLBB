uniform sampler2D inRTT;

varying vec2 uv;

void main(void)
{
	
    vec4 accum = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 LUMINENCE_FACTOR  = vec4(0.27, 0.67, 0.06, 0.0);
	// Approximate ratio from viewport to texture
	vec2 texelSize = vec2(0.005, 0.005);


    // Get colour from source
    accum += texture2D(inRTT, uv + texelSize * vec2(-0.5, -0.5));
    accum += texture2D(inRTT, uv + texelSize * vec2(-0.5, 0.5));
    accum += texture2D(inRTT, uv + texelSize * vec2(0.5, 0.5));
    accum += texture2D(inRTT, uv + texelSize * vec2(0.5, -0.5));
    
	// Adjust the accumulated amount by lum factor
	float lum = dot(accum, LUMINENCE_FACTOR);
	// take average of 4 samples
	lum *= 0.25;
	gl_FragColor = vec4(lum, lum, lum, 1.0);

}
