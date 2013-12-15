#ifndef GLAT_ABSTRACT_ANNOTATION_H
#define GLAT_ABSTRACT_ANNOTATION_H

#include <glat/Object.h>
#include <glat/AbstractRenderer.h>
#include <glat/RendererFactory.h>
#include <glat/AbstractState.h>

#include <glow/ref_ptr.h>
#include <vector>
#include <memory>

namespace glat {
class AbstractAnnotation : public glat::Object {
public:
	AbstractAnnotation(glat::AbstractState* initialState, const glat::RendererFactory& factory = RendererFactory());
	void draw();
	void setState(float statePhase);
	void addState(glat::AbstractState* state);
	glat::AbstractState* getState(unsigned statePosition = 0);

protected:
	glow::ref_ptr<glat::AbstractRenderer> m_renderer;
	std::vector<glow::ref_ptr<glat::AbstractState>> m_states;
	float m_statePhase;
};
}

#endif