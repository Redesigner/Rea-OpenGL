#pragma once

extern "C"
{
    #include <SDL2/SDL.h>
	#include <gl/glew.h>
}

#include "GlyphSubtexture.h"

#include <array>
#include <string>

namespace Ceres
{
	class FontAtlas
	{
		public:
			FontAtlas(std::string name, unsigned int resolution, unsigned int fontSize);
			FontAtlas(FontAtlas&) = delete;
			FontAtlas(FontAtlas&& fontAtlas);
			~FontAtlas();

			const GlyphSubtexture& GetCharUV(char glyph) const;

			GLuint GetTextureID() const;

		private:
			GLuint _openGLTextureID = 0;
			// Assuming we use UTF-8, cover all possible values
			std::array<GlyphSubtexture, 256> _glyphSubs;

			bool _initialized = true;
	};
}