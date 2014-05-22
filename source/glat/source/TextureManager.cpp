#include <glat/TextureManager.h>
#include <glat/PNGImage.h>

glow::ref_ptr<glat::TextureManager> glat::TextureManager::_instance = nullptr;

glow::ref_ptr<glat::TextureManager> glat::TextureManager::getInstance() {
	if (!_instance) {
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
	auto texture = std::make_shared<glat::ManagedTexture>(GL_TEXTURE_2D);
	texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glat::PNGImage image(fileName);
	GLenum format = image.getNumComponents() == 1 ? GL_RED : (image.getNumComponents() == 3 ? GL_RGB : GL_RGBA);
	texture->image2D(0, format, image.getWidth(), image.getHeight(), 0, format, GL_UNSIGNED_BYTE, image.getImage()->data);
	m_textures[fileName] = texture;
	return texture;
}

glat::ManagedTexture::~ManagedTexture() {
	glow::Texture::~Texture();
}

glat::ManagedTexture::ManagedTexture(GLenum target /*= GL_TEXTURE_2D*/) : glow::Texture(target) {
}
