#pragma once

#include <map>
#include <memory>
#include <glow/Referenced.h>
#include <glow/ref_ptr.h>
#include <glow/Texture.h>

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