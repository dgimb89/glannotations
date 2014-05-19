#pragma once

#include <glowbase/Referenced.h>
#include <glowbase/ref_ptr.h>
#include <glow/Texture.h>
#include <map>
#include <memory>

namespace glat {

	/// Singleton Texture Manager
	class TextureManager : public glow::Referenced {
	public:
		static glow::ref_ptr<TextureManager> getInstance();
		std::shared_ptr<glow::Texture> getTexture(std::string fileName);
	protected:
		TextureManager() {};
		std::map<std::string, std::weak_ptr<glow::Texture>> m_textures;
	private:
		static glow::ref_ptr<TextureManager> _instance;
	};
}