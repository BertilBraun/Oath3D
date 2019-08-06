#pragma once

#include "Common/Common.h"

namespace Oath {

	namespace Comp {

		struct Texture {

			Texture() : Texture(0) {}
			Texture(const Texture& o) : Texture(o.texID, o.offset, o.type) {}
			Texture(GLuint texID, GLuint offset = 0, GLuint type = GL_TEXTURE_2D) : 
				texID(texID), offset(offset), type(type) {}

			void Bind(int o = 0) const {
				GLCall(glActiveTexture(GL_TEXTURE0 + offset + o));
				GLCall(glBindTexture(type, texID));
			}

			void Delete() {
				if (texID)
					glDeleteTextures(1, &texID);
				texID = 0;
			}

			GLuint texID = 0, type = 0, offset = 0;
		};

		struct TextureAtlas : public Texture {

			TextureAtlas() : Texture(0) {}
			TextureAtlas(const TextureAtlas& o) : Texture(o), numOfRows(o.numOfRows) {}
			TextureAtlas(GLuint texID, GLuint numOfRows, GLuint offset = 0, GLuint type = GL_TEXTURE_2D) : 
				numOfRows(numOfRows), Texture(texID, offset, type) {}

			GLuint numOfRows = 0;
		};
	}
}