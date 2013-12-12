#ifndef GLAT_ABSTRACT_ANNOTATION_H
#define GLAT_ABSTRACT_ANNOTATION_H

#include <glat/AbstractRenderer.h>
#include <glat/RendererFactory.h>
#include <glat/AbstractState.h>

#include <glow/Referenced.h>
#include <glow/ref_ptr.h>
#include <vector>
#include <memory>

namespace glat {
class AbstractAnnotation : public glow::Referenced {
public:
	AbstractAnnotation(const glat::RendererFactory& factory = RendererFactory());
	void draw();
	bool isDirty();
	void setDirty(bool dirty);
	void setState(float statePhase);
	void addState(glat::AbstractState* state);

protected:
	std::shared_ptr<glat::AbstractRenderer> m_renderer;
	std::vector<std::shared_ptr<glat::AbstractState>> m_states;
	bool m_dirty;
	float m_statePhase;
};
}

#endif