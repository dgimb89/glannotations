#include <globjects/base/ref_ptr.h>
#include <osg/Drawable>

#include <glannotations/wrapper/glannotations-wrapper_api.h>
#include <glannotations/AbstractAnnotation.h>

namespace glannotations {
namespace wrapper {
	class GLANNOTATIONS_WRAPPER_API DrawableAnnotation : public osg::Drawable {
	public:
		DrawableAnnotation();
		DrawableAnnotation(const DrawableAnnotation& teapot, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
		META_Object(GLAT_OSG_Wrapper, DrawableAnnotation)
		virtual void drawImplementation(osg::RenderInfo&) const override;
		virtual osg::BoundingBox computeBound() const override;

		void setAnnotation(globjects::ref_ptr<glannotations::AbstractAnnotation> annotation);
		const globjects::ref_ptr<glannotations::AbstractAnnotation>& getAnnotation() const;

	protected:
		globjects::ref_ptr<glannotations::AbstractAnnotation> m_annotation;
		virtual ~DrawableAnnotation() {}
	};
}
}