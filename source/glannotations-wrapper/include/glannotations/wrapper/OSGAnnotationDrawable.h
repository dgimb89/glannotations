#include <globjects/base/ref_ptr.h>
#include <osg/Drawable>

#include <glannotations/wrapper/glat-wrapper_api.h>
#include <glannotations/AbstractAnnotation.h>

namespace glat {
namespace wrapper {
	class GLAT_WRAPPER_API DrawableAnnotation : public osg::Drawable {
	public:
		DrawableAnnotation();
		DrawableAnnotation(const DrawableAnnotation& teapot, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
		META_Object(GLAT_OSG_Wrapper, DrawableAnnotation)
		virtual void drawImplementation(osg::RenderInfo&) const override;
		virtual osg::BoundingBox computeBound() const override;

		void setAnnotation(globjects::ref_ptr<glat::AbstractAnnotation> annotation);
		const globjects::ref_ptr<glat::AbstractAnnotation>& getAnnotation() const;

	protected:
		globjects::ref_ptr<glat::AbstractAnnotation> m_annotation;
		virtual ~DrawableAnnotation() {}
	};
}
}