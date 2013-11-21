#ifndef ABSTRACT_ANNOTATION_H
#define ABSTRACT_ANNOTATION_H

#include <glat/AbstractRenderer.h>
#include <glat/RendererFactory.h>
#include <glat/AbstractState.h>

#include <memory>
#include <vector>

namespace glat {
class AbstractAnnotation {
public:
	AbstractAnnotation(glat::RendererFactory* factory);
	void render();
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