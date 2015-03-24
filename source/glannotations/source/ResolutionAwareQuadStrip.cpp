#include <glannotations/ResolutionAwareQuadStrip.h>

namespace glannotations {

	ResolutionAwareQuadStrip::ResolutionAwareQuadStrip(std::shared_ptr<globjects::Texture> texture, gl::GLuint matricesBindingIndex, bool isDistanceField)
		: QuadStrip(texture, matricesBindingIndex, isDistanceField) {
	}

	bool ResolutionAwareQuadStrip::setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) {
		cropExtends(ll, lr, ur);
		return QuadStrip::setPosition(ll, lr, ur);
	}

	bool ResolutionAwareQuadStrip::setViewportPosition(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur) {
		cropExtends(ll, lr, ur); 
		return QuadStrip::setViewportPosition(ll, lr, ur);
	}

} // END glannotation namespace

