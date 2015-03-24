#pragma once

#include <glannotations/QuadStrip.h>

namespace glannotations {
	class ResolutionAwareQuadStrip : public QuadStrip {
	public:
		ResolutionAwareQuadStrip(std::shared_ptr<globjects::Texture> texture, gl::GLuint matricesBindingIndex, bool isDistanceField);

		virtual bool setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) override;
		virtual bool setViewportPosition(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur) override;
		
	protected:
		template<typename T>
		void cropExtends(T& ll, T& lr, T& ur) {
			glm::vec2 advance = getQuadStripTextureAdvance();
			float expHeight = (advance.y / advance.x) * glm::distance(ll, lr);
			float height = glm::distance(lr, ur);
			// check expected quadstrip height according to width/height ratio
			if (expHeight > height) {
				// shrink width
				float expWidth = 0.5f * height * (advance.x / advance.y);
				T widthAdvance = glm::normalize(lr - ll) * expWidth;
				T heightAdvance = ur - lr;
				T center = (ll + lr) / 2.f;
				ll = center - widthAdvance;
				lr = center + widthAdvance;
				ur = lr + heightAdvance;
			}
			else {
				// shrink height
				expHeight *= 0.5f;
				T center = (lr + ur) / 2.f;
				T widthAdvance = lr - ll;
				T heightAdvance = glm::normalize(ur - lr) * expHeight;
				lr = center - heightAdvance;
				ll = lr - widthAdvance;
				ur = center + heightAdvance;
			}
		}
	};
}