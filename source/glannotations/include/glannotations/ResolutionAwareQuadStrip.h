#pragma once

#include <glannotations/QuadStrip.h>
#include <iostream>

namespace glannotations {
	class ResolutionAwareQuadStrip : public QuadStrip {
	public:
		ResolutionAwareQuadStrip(std::shared_ptr<globjects::Texture> texture, gl::GLuint matricesBindingIndex, bool isDistanceField);

		virtual bool setPosition(glm::vec3 ll, glm::vec3 lr, glm::vec3 ur) override;
		virtual bool setViewportPosition(glm::vec2 ll, glm::vec2 lr, glm::vec2 ur) override;
		
	protected:
		template<typename T>
		void cropExtends(T& ll, T& lr, T& ur, float aspectRatio = 1.f) {
			glm::vec2 advance = getQuadStripTextureAdvance();
			float expHeight = (advance.y / advance.x) * glm::distance(ll, lr) * aspectRatio;
			float height = glm::distance(lr, ur);
			// check expected quadstrip height according to width/height ratio
			if (expHeight > height) {
				// shrink width
				T center = (ll + lr) / 2.f;
				T heightAdvance = (ur - lr);
				float expWidth = 0.5f * height * (advance.x / advance.y) / aspectRatio;
				T widthAdvance = glm::normalize(lr - ll) * expWidth;
				ll = center - widthAdvance;
				lr = center + widthAdvance;
				ur = lr + heightAdvance;
			}
			else {
				// shrink height
				T center = (lr + ur) / 2.f;
				T widthAdvance = (lr - ll);
				expHeight *= 0.5f;
				T heightAdvance = glm::normalize(ur - lr) * expHeight;
				lr = center - heightAdvance;
				ll = lr - widthAdvance;
				ur = center + heightAdvance;
			}
		}

		gl::GLuint m_matricesBindingIndex;
	};
}