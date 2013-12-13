#ifndef GLAT_ABSTRACT_STATE_H
#define GLAT_ABSTRACT_STATE_H

#include <glow/Referenced.h>
#include <glow/ref_ptr.h>
#include <glat/Styling.h>
#include <vector>

namespace glat {
	typedef std::vector<glow::ref_ptr<glat::Styling>> StylingList;
	class AbstractState : public glow::Referenced {
	public:
		void addStyling(glat::Styling* style);
		const StylingList& getStylings();

	protected:
		AbstractState();
		StylingList m_stylings;

	};
}

#endif