#pragma once

#include <glowbase/Referenced.h>
#include <glowbase/ref_ptr.h>
#include <glow/Texture.h>
#include <map>
#include <memory>

namespace glat {

	// Wrapper to allow management via TextureManager
	class ManagedTexture : public glow::Texture {
	public:
		ManagedTexture(GLenum target = GL_TEXTURE_2D);
		virtual ~ManagedTexture();
	};

	/// Singleton Texture Manager
	class TextureManager : public glow::Referenced {
	public:
		static glow::ref_ptr<TextureManager> getInstance();
		std::shared_ptr<glow::Texture> getTexture(std::string fileName);
	protected:
		TextureManager();
		std::map<std::string, std::weak_ptr<glat::ManagedTexture>> m_textures;
	private:
		static glow::ref_ptr<TextureManager> _instance;
	};
}