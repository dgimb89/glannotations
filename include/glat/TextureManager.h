#ifndef GLAT_TEXTURE_MANAGER_H
#define GLAT_TEXTURE_MANAGER_H

#include <map>
#include <memory>
#include <glat/Object.h>
#include <glow/Texture.h>

namespace glat {

	/// Singleton Texture Manager
	class TextureManager : public glat::Object {
	public:
		static TextureManager* getInstance();
		std::shared_ptr<glow::Texture> getTexture(std::string fileName);
	protected:
		TextureManager() {};
		std::map<std::string, std::weak_ptr<glow::Texture>> m_textures;
	private:
		static TextureManager* _instance;
	};
}

#endif // GLAT_TEXTURE_MANAGER_H