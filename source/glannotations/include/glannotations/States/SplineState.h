#pragma once

#include <glannotations/States/ReferenceableState.h>
#include <glannotations/Externals/AbstractExternalReference.h>
#include <glannotations/Utility/BSpline.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	enum GlyphOrientationOnSpline{ IN_SAME_PLANE, ORTHOGONAL_TO_PLANE, CUSTOM_SECOND_SPLINE };

	class GLANNOTATIONS_API SplineState : public glannotations::ReferenceableState {
		friend class AbstractPrimitiveRenderer; //todo:anne is this necessary?
	public:
		/*!
		*	\brief	Creates an Annotation State which can be positioned by a Quad in world space
		*	\param	position we the spline would begin, if first controlpoint is (0,0,0)
		*	\param	splineBase spline to put the glyphs onto
		*	\param	splineTop spline works as a roof for the glyphs
		*	\param	splineOrientation useful to calculate some standard splineTops
		*/
		SplineState(glm::vec3 position, BSpline splineBase, GlyphOrientationOnSpline splineOrientation = GlyphOrientationOnSpline::IN_SAME_PLANE);
		SplineState(glm::vec3 position, BSpline splineBase, BSpline splineTop);
		SplineState(glm::vec3 position, BSpline splineBase, BSpline splineTop, GlyphOrientationOnSpline splineOrientation);
		SplineState(glm::vec3 position
			, std::vector<glm::vec3> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
			, std::vector<glm::vec3> splineTopControlPoints, std::vector<float> splineTopKnotValues
			, GlyphOrientationOnSpline splineOrientation);
		
		//either change using orientation parameter or using a new splineTop
		void changeOrientation(std::shared_ptr<BSpline> splineTop);
		void changeOrientation(GlyphOrientationOnSpline splineOrientation);
		void updateSplineTop();

		/*!
		*	\brief	Calculates the connecting vector of two curvepoints specified by curve parameter t (0<t<1), using splineBase and splineTop
		*	\param	t Curve parameter for first curvepoint
		*/
		glm::vec3 retrieveConnectingVectorAt(float t);
		
		/*!
		*	\brief	Calculates secant vector between two curvepoints specified by curve parameters t and nextT
		*	\param	t Curve parameter for first curvepoint
		*	\param	nextT Curve parameter for second curvepoint
		*/
		glm::vec3 retrieveSecantVectorAt(float t, float nextT);

		const glm::vec3& getLL() const;
		virtual bool isValid() const override;
		virtual void setExternalReference(const globjects::ref_ptr<glannotations::AbstractExternalReference>& reference) override;
		virtual globjects::ref_ptr<glannotations::AbstractState> clone() const override;
		virtual glannotations::BoundingBox getBoundingBox() override;

	protected:
		virtual void draw(const AbstractRenderer& renderer) override;

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const InternalState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const SplineState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix);

		virtual void updateExtends(glm::vec2 sourceExtends) override;

	private:
		std::shared_ptr<glannotations::BSpline> m_splineBase;
		std::shared_ptr<glannotations::BSpline> m_splineTop;
		GlyphOrientationOnSpline m_splineOrientation;
		glm::vec3 m_ll;
	};
}