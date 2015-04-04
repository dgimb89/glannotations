#include <glannotations/TextureManager.h>
#include <glannotations/PNGImage.h>

#include <glbinding/gl/enum.h>

std::unique_ptr<glannotations::TextureManager> glannotations::TextureManager::_instance = nullptr;

glannotations::TextureManager& glannotations::TextureManager::getInstance() {
	if (!_instance) {
		_instance = std::move(std::unique_ptr<TextureManager>(new TextureManager));
	}

	return *_instance;
}

std::shared_ptr<globjects::Texture> glannotations::TextureManager::getTexture(std::string fileName) {
	m_mutex.lock();
	// we make use of weak_ptr to release textures to reduce memory consumption but still shared allocated textures whenever possible
	if (m_textures.count(fileName)) {
		if (auto spt = m_textures[fileName].lock()) {
			m_mutex.unlock();
			return spt; // texture resource still valid
		} else {
			m_textures.erase(fileName); // expired
		}
	}

	// load texture
	auto texture = std::make_shared<glannotations::ManagedTexture>(gl::GL_TEXTURE_2D);
	texture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
	texture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
	texture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
	texture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);
	texture->setParameter(gl::GL_TEXTURE_WRAP_R, gl::GL_CLAMP_TO_EDGE);
	glannotations::PNGImage image(fileName);
	gl::GLenum format = image.getNumComponents() == 1 ? gl::GL_RED : (image.getNumComponents() == 3 ? gl::GL_RGB : gl::GL_RGBA);
	texture->image2D(0, format, image.getWidth(), image.getHeight(), 0, format, gl::GL_UNSIGNED_BYTE, image.getImage()->data);
	m_textures[fileName] = texture;
	m_mutex.unlock();
	return texture;
}

glannotations::TextureManager::TextureManager() {
}

glm::ivec2 glannotations::TextureManager::getTextureSize(std::string fileName) {
	glannotations::PNGImage image(fileName);
	return glm::ivec2(image.getWidth(), image.getHeight());
}

glannotations::ManagedTexture::ManagedTexture(gl::GLenum target /*= GL_TEXTURE_2D*/) : globjects::Texture(target) {
}
