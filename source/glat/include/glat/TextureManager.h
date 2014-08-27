#pragma once
#include <glm/glm.hpp>
#include <globjects-base/Referenced.h>
#include <globjects-base/ref_ptr.h>
#include <globjects/Texture.h>
#include <map>
#include <memory>

namespace glat {

	// Wrapper to allow management via TextureManager
	class ManagedTexture : public glo::Texture {
	public:
		ManagedTexture(gl::GLenum target = gl::GL_TEXTURE_2D);
	};

	/// Singleton Texture Manager
	class TextureManager : public glo::Referenced {
	public:
		static TextureManager& getInstance();
		std::shared_ptr<glo::Texture> getTexture(std::string fileName);
		glm::ivec2 getTextureSize(std::string fileName);
	protected:
		TextureManager();
		std::map<std::string, std::weak_ptr<glat::ManagedTexture>> m_textures;

		static std::unique_ptr<TextureManager> _instance;
	};
}