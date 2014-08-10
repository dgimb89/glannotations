#include <glowbase/ref_ptr.h>

#include <glat/wrapper/glat_wrapper_api.h>
#include <glat/AbstractAnnotation.h>
#include <osg/Drawable>

namespace glat {
namespace wrapper {
	class GLAT_WRAPPER_API DrawableAnnotation : public osg::Drawable {
	public:
		DrawableAnnotation();
		DrawableAnnotation(const DrawableAnnotation& teapot, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
		META_Object(GLAT_OSG_Wrapper, DrawableAnnotation)
		virtual void drawImplementation(osg::RenderInfo&) const override;
		virtual osg::BoundingBox computeBound() const override;

		void setAnnotation(glow::ref_ptr<glat::AbstractAnnotation> annotation);
		const glow::ref_ptr<glat::AbstractAnnotation>& getAnnotation() const;

	protected:
		glow::ref_ptr<glat::AbstractAnnotation> m_annotation;
		virtual ~DrawableAnnotation() {}
	};
}
}