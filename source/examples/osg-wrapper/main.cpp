#include <glat/wrapper/OSGAnnotationDrawable.h>
#include <glat/FontAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>

#include <osgViewer/Viewer>
#include <osg/GraphicsContext>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

osg::GraphicsContext * createContext() {
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
	const std::string version("3.1");
	traits->glContextVersion = version;
	return osg::GraphicsContext::createGraphicsContext(traits.get());
}

osg::Geode* createAnnotations() {
	osg::Geode* geode = new osg::Geode;

	glat::RendererFactory dfFactory;
	//dfFactory.useNVpr(false);

	// setup annotation
	glat::wrapper::DrawableAnnotation* annotationDrawable = new glat::wrapper::DrawableAnnotation;
	auto viewportAnnotation = new glat::FontAnnotation(new glat::ViewportState(glm::vec2(0.2f, -1.f), glm::vec2(1.f, -0.65f)), "osgWrapper", "calibri.ttf", dfFactory);
	viewportAnnotation->setColor(glm::vec4(0.75, 0.75, 0.75, 1.0));
	annotationDrawable->setAnnotation(viewportAnnotation);

	// add drawable to geode and return
	geode->addDrawable(annotationDrawable);
	return geode;
}

int main(int argc, char **argv) {
	// construct the viewer
	osgViewer::Viewer viewer;

	osg::setNotifyLevel(osg::DEBUG_INFO);
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
	osg::Group* root = new osg::Group;
	root->addChild(osgDB::readNodeFile("data/cessna.osg"));
	viewer.setSceneData(root);

	bool initialized = false;
	while (!viewer.done()) {
		viewer.frame();
		if (context->valid()) {
			context->makeCurrent();
			if (!initialized) {
				glat::initialize();
				//glat::setViewFrustumVolume()
				//createAnnotations();
				root->addChild(createAnnotations());
				initialized = true;
			}
			glat::setProjection(glm::mat4(1));
			glat::setView(glm::mat4());
			glat::setViewport(glm::ivec2(800, 600));
		}
	}
	glat::cleanUp();
	return 0;
}