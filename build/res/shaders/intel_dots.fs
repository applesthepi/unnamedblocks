uniform vec2 windowSize;
uniform vec2 planeInnerPosition;
uniform float dotMod;
uniform vec3 backgroundColor;
uniform vec3 dotColor;

void main()
{
	vec2 uv = gl_FragCoord.xy / windowSize;
	
	if((mod(round(uv.x * (windowSize.x / 2.0)) + planeInnerPosition.x, dotMod) == 1.0 && mod(round(uv.y * (windowSize.y / 2.0)) + planeInnerPosition.y, dotMod * 2.0) == 1.0) ||
		(mod(round(uv.x * (windowSize.x / 2.0)) + (dotMod / 2.0) + planeInnerPosition.x, dotMod) == 1.0 && mod(round(uv.y * (windowSize.y / 2.0)) + dotMod + planeInnerPosition.y, dotMod * 2.0) == 1.0))
	{
		gl_FragColor = vec4(dotColor.x, dotColor.y, dotColor.z, 1.0);
	}
	else
	{
		gl_FragColor = vec4(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0);
	}
}
