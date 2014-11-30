#pragma once
#include <glbinding/gl/enum.h>
#include <glm/glm.hpp>
#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <map>
#include <memory>

namespace glannotations {

	// Wrapper to allow management via TextureManager
	class ManagedTexture : public globjects::Texture {
	public:
		ManagedTexture(gl::GLenum target = gl::GL_TEXTURE_2D);
	};

	/// Singleton Texture Manager
	class TextureManager : public globjects::Referenced {
	public:
		static TextureManager& getInstance();
		std::shared_ptr<globjects::Texture> getTexture(std::string fileName);
		glm::ivec2 getTextureSize(std::string fileName);
	protected:
		TextureManager();
		std::map<std::string, std::weak_ptr<glannotations::ManagedTexture>> m_textures;

		static std::unique_ptr<TextureManager> _instance;
	};
}