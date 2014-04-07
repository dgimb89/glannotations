#pragma once

#include <glat/AbstractState.h>
#include <glat/InternalState.h>
#include <glat/AbstractDrawingPrimitive.h>

#include <glowutils/Camera.h>

namespace glat {

	class AbstractExternalState : public glat::AbstractState {

	public:
		AbstractExternalState(glowutils::Camera* camera);

		// TODO: make return state const
		glow::ref_ptr<glat::InternalState> getInternalAnnotation();

		void setCamera(glowutils::Camera* camera);
		glowutils::Camera* getCamera() const;
		const glm::mat4& getViewProjection() const;

		virtual const glm::vec3& getLL() const = 0;
		virtual const glm::vec3& getUR() const = 0;
		virtual const glm::vec3& getLR() const = 0;

		virtual bool isDirty() const;

		void setDrawExternal(bool drawExternal);
		bool getDrawExternal() const;
		void setupExternalPrimitives() const;
		void drawExternalPrimitives() const;

	protected:
		void setupExternalColor(const glat::Styling* externalColor) const;
		glow::ref_ptr<glat::InternalState> m_internalState;
		glowutils::Camera* m_camera;
		mutable glm::mat4 m_camProjection;
		mutable glow::ref_ptr<glat::AbstractDrawingPrimitive> m_externalPrimitive;
		bool m_drawExternal;
		float m_interpolation;
	};
}