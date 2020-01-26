uniform vec2 planeInnerPosition;
uniform float dotMod;
uniform vec3 backgroundColor;
uniform vec3 dotColor;

void main()
{
	if((mod(round(gl_FragCoord.x) + (planeInnerPosition.x * 2.0), dotMod) == 1.0 && mod(round(gl_FragCoord.y) + (planeInnerPosition.y * 2.0), dotMod * 2.0) == 1.0) ||
		(mod(round(gl_FragCoord.x) + (dotMod / 2.0) + (planeInnerPosition.x * 2.0), dotMod) == 1.0 && mod(round(gl_FragCoord.y) + dotMod + (planeInnerPosition.y * 2.0), dotMod * 2.0) == 1.0))
	{
		gl_FragColor = vec4(dotColor.x, dotColor.y, dotColor.z, 1.0);
	}
	else
	{
		gl_FragColor = vec4(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0);
	}
}
