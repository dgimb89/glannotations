#include <glat/TextureManager.h>
#include <glat/PNGImage.h>

std::unique_ptr<glat::TextureManager> glat::TextureManager::_instance = nullptr;

glat::TextureManager& glat::TextureManager::getInstance() {
	if (!_instance) {
		_instance = std::move(std::unique_ptr<TextureManager>(new TextureManager));
	}

	return *_instance;
}

std::shared_ptr<glow::Texture> glat::TextureManager::getTexture(std::string fileName) {
	if (m_textures.count(fileName)) {
		if (auto spt = m_textures[fileName].lock())
			return spt; // texture resource still valid
		else 
			m_textures.erase(fileName); // expired
	}

	// load texture
	auto texture = std::make_shared<glat::ManagedTexture>(gl::GL_TEXTURE_2D);
	texture->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
	texture->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
	texture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
	texture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);
	texture->setParameter(gl::GL_TEXTURE_WRAP_R, gl::GL_CLAMP_TO_EDGE);
	glat::PNGImage image(fileName);
	gl::GLenum format = image.getNumComponents() == 1 ? gl::GL_RED : (image.getNumComponents() == 3 ? gl::GL_RGB : gl::GL_RGBA);
	texture->image2D(0, format, image.getWidth(), image.getHeight(), 0, format, gl::GL_UNSIGNED_BYTE, image.getImage()->data);
	m_textures[fileName] = texture;
	return texture;
}

glat::TextureManager::TextureManager() {
}

glat::ManagedTexture::~ManagedTexture() {
	glow::Texture::~Texture();
}

glat::ManagedTexture::ManagedTexture(gl::GLenum target /*= GL_TEXTURE_2D*/) : glow::Texture(target) {
}
