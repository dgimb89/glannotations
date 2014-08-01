#pragma once
#include <glm/glm.hpp>
#include <glowbase/Referenced.h>
#include <glowbase/ref_ptr.h>
#include <glow/Texture.h>
#include <map>
#include <memory>

namespace glat {

	// Wrapper to allow management via TextureManager
	class ManagedTexture : public glow::Texture {
	public:
		ManagedTexture(gl::GLenum target = gl::GL_TEXTURE_2D);
	};

	/// Singleton Texture Manager
	class TextureManager : public glow::Referenced {
	public:
		static TextureManager& getInstance();
		std::shared_ptr<glow::Texture> getTexture(std::string fileName);
		glm::ivec2 getTextureSize(std::string fileName);
	protected:
		TextureManager();
		std::map<std::string, std::weak_ptr<glat::ManagedTexture>> m_textures;

		static std::unique_ptr<TextureManager> _instance;
	};
}