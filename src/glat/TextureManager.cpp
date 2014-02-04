#include <glat/TextureManager.h>
#include <glat/PNGImage.h>

glat::TextureManager* glat::TextureManager::_instance = nullptr;

glat::TextureManager* glat::TextureManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new TextureManager;
	}

	return _instance;
}

std::shared_ptr<glow::Texture> glat::TextureManager::getTexture(std::string fileName) {
	if (m_textures.count(fileName)) {
		if (auto spt = m_textures[fileName].lock())
			return spt; // texture resource still valid
		else 
			m_textures.erase(fileName); // expired
	}

	// load texture
	auto texture = std::make_shared<glow::Texture>(GL_TEXTURE_2D);
	texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glat::PNGImage image(fileName);
	texture->image2D(0, image.getNumComponents() == 1 ? GL_RED : (image.getNumComponents() == 3 ? GL_RGB : GL_RGBA), image.getWidth(), image.getHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, image.getImage()->data);
	m_textures[fileName] = texture;
	return texture;
}
