#pragma once

#include <globjects/utils/Camera.h>

#include <glannotations/AbstractState.h>
#include <glannotations/InternalState.h>
#include <glannotations/AbstractDrawingPrimitive.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {

	class GLANNOTATIONS_API AbstractExternalState : public glannotations::AbstractState {

	public:
		AbstractExternalState(gloutils::Camera* camera);

		// TODO: make return state const
		globjects::ref_ptr<glannotations::InternalState> getInternalAnnotation();

		void setCamera(gloutils::Camera* camera);
		gloutils::Camera* getCamera() const;
		const glm::mat4& getViewProjection() const;

		const glm::vec3& getLL() const;
		const glm::vec3& getUR() const;
		const glm::vec3& getLR() const;

		virtual bool isDirty() const;

		void setDrawExternal(bool drawExternal);
		bool getDrawExternal() const;
		void setupExternalPrimitives() const;
		void drawExternalPrimitives() const;
		virtual void updateInternalPosition() const = 0;

	protected:
		void setupExternalColor(const glannotations::Styling* externalColor) const;
		mutable globjects::ref_ptr<glannotations::InternalState> m_internalState;
		gloutils::Camera* m_camera;
		mutable glm::mat4 m_camProjection;
		mutable globjects::ref_ptr<glannotations::AbstractDrawingPrimitive> m_externalPrimitive;
		bool m_drawExternal;
		float m_interpolation;
	};
}