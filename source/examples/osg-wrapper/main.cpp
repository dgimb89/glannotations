#include <glat/wrapper/OSGAnnotationDrawable.h>
#include <glat/FontAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>

#include <osgViewer/Viewer>
#include <osg/GraphicsContext>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

osg::GraphicsContext * createContext()
{
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 100;
	traits->y = 100;
	traits->width = 800;
	traits->height = 600;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->samples = 4;
	traits->sampleBuffers = 1;
	traits->stencil = 8; // necessary for NVPR
	return osg::GraphicsContext::createGraphicsContext(traits.get());
}

osg::Geode* createAnnotations() {
	osg::Geode* geode = new osg::Geode;

	// setup annotation
	glat::wrapper::DrawableAnnotation* annotationDrawable = new glat::wrapper::DrawableAnnotation;
	glow::ref_ptr<glat::AbstractAnnotation> viewportAnnotation = new glat::FontAnnotation(new glat::ViewportState(glm::vec2(-1.f, -1.f), glm::vec2(0.f, -0.3f)), "osgWrapper", "Segoe UI");
	annotationDrawable->setAnnotation(viewportAnnotation);

	// add drawable to geode and return
	geode->addDrawable(annotationDrawable);
	return geode;
}

int main(int argc, char **argv) {
	// construct the viewer
	osgViewer::Viewer viewer;
	viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
	osg::ref_ptr<osg::GraphicsContext> context = createContext();
	viewer.getCamera()->setGraphicsContext(context);
	viewer.getCamera()->setViewport(0, 0, 800, 600);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	if (!viewer.isRealized()) {
		viewer.realize();
	}
	viewer.init();
	context->makeCurrent();
	glat::initialize();
	osg::Group* root = new osg::Group;
	root->addChild(osgDB::readNodeFile("data/cessna.osg"));
	root->addChild(createAnnotations());
	viewer.setSceneData(root);

	bool initialized = false;
	while (!viewer.done()) {
		context->makeCurrent();
		viewer.frame();
		/*viewer.advance();
		viewer.eventTraversal();
		viewer.updateTraversal();
		viewer.renderingTraversals();
		if (context->valid())
		{
			if (!initialized) {
				initialized = true;
			}
		}*/
	}
	return 0;
}