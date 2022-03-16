#pragma once

namespace Ceres
{
	struct GlyphSubtexture
	{
		GlyphSubtexture(float x, float y, float w, float h)
			:X(x), Y(y), W(w), H(h)
		{
		}
		GlyphSubtexture() 
		{
		}

		float X, Y, W, H;
		unsigned int PxW, PxH;
		float XOffset, YOffset, Advance;
		float Kerning[256];
	};
}