#pragma once

#include <glannotations/States/ReferenceableState.h>
#include <glannotations/Externals/AbstractExternalReference.h>
#include <glannotations/Utility/BSpline.h>
#include <glannotations/Utility/BSpline2D.h>
#include <glannotations/Utility/BSpline3D.h>
#include <glannotations/glannotations_api.h>

namespace glannotations {
	class GLANNOTATIONS_API SplineState : public glannotations::ReferenceableState {
		friend class AbstractPrimitiveRenderer;
	public:
		/*!
		*	\brief	Creates an Annotation State which can be positioned by a Quad in world space
		*	\param	position we the spline would begin, if first controlpoint is (0,0,0)
		*	\param	splineBase spline to put the glyphs onto
		*	\param	splineTop spline works as a roof for the glyphs
		*	\param	splineOrientation useful to calculate some standard splineTops
		*/
		SplineState(glm::vec3 position
			, std::vector<glm::vec3> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
			, glm::vec3 upToTopSpline
		);
		
		SplineState(glm::vec3 position
			, std::vector<glm::vec3> splineBaseControlPoints, unsigned int baseDegree
			, glm::vec3 upToTopSpline
		);

		SplineState(glm::vec3 position
			, std::vector<glm::vec3> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
			, std::vector<glm::vec3> splineTopControlPoints, std::vector<float> splineTopKnotValues
		);

		SplineState(glm::vec3 position
			, std::vector<glm::vec3> splineBaseControlPoints, unsigned int baseDegree
			, std::vector<glm::vec3> splineTopControlPoints, unsigned int topDegree
		);

		//same same but different: for 2D Splines on a given plane (planeAxisDirection, planeAxisUp)
		//planeNormal would not be sufficient, we would not know the exact rotation of the plane in world space
		SplineState(glm::vec3 position
			, glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp
			, std::vector<glm::vec2> splineBaseControlPoints
			, std::vector<float> splineBaseKnotValues
			, glm::vec2 upToTopSpline
		);

		SplineState(glm::vec3 position
			, glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp
			, std::vector<glm::vec2> splineBaseControlPoints
			, unsigned int baseDegree
			, glm::vec2 upToTopSpline
		);

		SplineState(glm::vec3 position
			, glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp
			, std::vector<glm::vec2> splineBaseControlPoints, std::vector<float> splineBaseKnotValues
			, std::vector<glm::vec2> splineTopControlPoints, std::vector<float> splineTopKnotValues
		);

		SplineState(glm::vec3 position
			, glm::vec3 planeAxisDirection, glm::vec3 planeAxisUp
			, std::vector<glm::vec2> splineBaseControlPoints, unsigned int baseDegree
			, std::vector<glm::vec2> splineTopControlPoints, unsigned int topDegree
		);

		//either change using orientation parameter or using a new splineTop
		void changeOrientation(std::shared_ptr<BSpline2D> splineTop);
		void changeOrientation(glm::vec2 newUp);
		void changeOrientation(std::shared_ptr<BSpline3D> splineTop);
		void changeOrientation(glm::vec3 newUp);

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

		virtual void prepare() override;

	protected:
		virtual void draw(const globjects::ref_ptr<glannotations::AbstractAnnotation>& annotation, const AbstractRenderer& renderer) override;

		virtual globjects::ref_ptr<AbstractState> interpolateWith(const QuadState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const SplineState& mixState, float mix);
		virtual globjects::ref_ptr<AbstractState> interpolateWith(const ViewportState& mixState, float mix);

		virtual void updateExtends(glm::vec2 sourceExtends) override;

	private:

		void initialize(glm::vec3 position, bool is3DSpline);
		void calculateSplineTop(glm::vec3 upVecInWorldSpace);
		void calculateSplineTop(glm::vec2 upVecInPlaneSpace);

		std::shared_ptr<glannotations::BSpline> m_splineBase;
		std::shared_ptr<glannotations::BSpline> m_splineTop;
		glm::vec3 m_ll;

		bool m_valid;
		bool m_acceptsExternalReference;
	};
}