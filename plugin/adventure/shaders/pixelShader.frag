#version 330
#extension GL_ARB_explicit_uniform_location: enable

// Interpolated values from the vertex shaders
in vec2 UV;

out vec4 outColor;

// Values that stay constant for the whole mesh.
layout(location = 5) uniform sampler2D textureSampler;
layout(location = 6) uniform vec4 uniformColor;

const float deadZone = 0.25;
const float width = 0.09765625;

void main()
{
	float d = distance(UV, vec2(0.5, 0.5));
	
	float alpha = uniformColor.a;
	if(d > deadZone)
	{
		float delta = (d - deadZone) / width;
		alpha = mix(alpha, 0.8, delta);
	}

	// Output color = color of the texture at the specified UV
	outColor = texture( textureSampler, UV ) * vec4(uniformColor.rgb, alpha);
	
	/*if(outColor.a <= 0.0)
		discard;*/
}
