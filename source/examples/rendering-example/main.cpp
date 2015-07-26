#define _USE_MATH_DEFINES
#include <glbinding/gl/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/Uniform.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/Framebuffer.h>
#include <globjects/VertexArray.h>
#include <globjects/Texture.h>

#include <common/AxisAlignedBoundingBox.h>
#include <common/Camera.h>
#include <common/AbstractCoordinateProvider.h>
#include <common/WorldInHandNavigation.h>

#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <glannotations/AnnotationGroup.h>
#include <glannotations/Renderer/RendererFactory.h>
#include <glannotations/FontAnnotation.h>
#include <glannotations/SVGAnnotation.h>
#include <glannotations/PNGAnnotation.h>
#include <glannotations/States/ViewportState.h>
#include <glannotations/States/QuadState.h>
#include <glannotations/States/SplineState.h>
#include <glannotations/Externals/BoxReference.h>
#include <glannotations/Externals/LabelReference.h>
#include <glannotations/Externals/FlagReference.h>
#include <glannotations/Externals/RectReference.h>
#include <glannotations/Styles/Styles.h>

#include "building.h"


using namespace gl;
using namespace glm;
using namespace globjects;

class EventHandler : public WindowEventHandler, AbstractCoordinateProvider
{
public:
    EventHandler()
    : m_camera(new Camera(vec3(0.f, 1.f, 4.f)))
    {
        m_aabb.extend(vec3(-8.f, -1.f, -8.f));
        m_aabb.extend(vec3( 8.f,  1.f,  8.f));

        m_nav.setCamera(m_camera);
        m_nav.setCoordinateProvider(this);
		m_nav.setBoundaryHint(m_aabb);
    }

    virtual ~EventHandler()
	{
		glannotations::cleanUp();
		delete m_camera;
    }

    virtual void initialize(Window & window) override
    {
		WindowEventHandler::initialize(window);

		glannotations::RendererFactory dfFactory;
		dfFactory.useNVpr(false);
		dfFactory.setMatricesBindingIndex(2);

        glClearColor(1.f, 1.f, 1.f, 0.f);
		gl::glEnable(gl::GL_CULL_FACE);
		gl::glEnable(gl::GL_DEPTH_TEST);
		gl::glEnable(gl::GL_BLEND);
		gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

        m_camera->setZNear(0.1f);
		m_camera->setZFar(1024.f);
		m_camera->setCenter(vec3(0.f, 0.f, 5.f));
		m_camera->setEye(vec3(-17.f, 12.f, -15.0f));
		m_camera->setUp(vec3(0, 1, 0));

		m_building = new Building(2);
		m_building1 = new Building(2);
		m_building2 = new Building(2);
		m_building3 = new Building(2);
		m_building4 = new Building(2);
		m_building5 = new Building(2);
		m_building6 = new Building(2);
		m_building7 = new Building(2);
		m_building8 = new Building(2);
		m_building9 = new Building(2);
		m_building10 = new Building(2);
		m_building11 = new Building(2);
		m_building12 = new Building(2);
		m_building13 = new Building(2);
		m_building14 = new Building(2);

		m_building->setPosition(glm::vec3(-1.f, -4.f, 1.f), glm::vec3(1.f, -3.f, -1.f));
		m_building1->setPosition(glm::vec3(-1.f, -4.f, 5.f), glm::vec3(1.f, 4.f, 2.f));
		m_building2->setPosition(glm::vec3(-1.f, -4.f, 8.f), glm::vec3(1.f, 0.f, 6.f));
		m_building3->setPosition(glm::vec3(-1.f, -4.f, 11.f), glm::vec3(1.f, 1.f, 9.f));

		m_building4->setPosition(glm::vec3(2.f, -4.f, 1.f), glm::vec3(4.f, 6.f, -1.f));
		m_building5->setPosition(glm::vec3(2.f, -4.f, 5.f), glm::vec3(4.f, 2.f, 2.f));
		m_building6->setPosition(glm::vec3(2.f, -4.f, 8.f), glm::vec3(4.f, -1.f, 6.f));
		m_building7->setPosition(glm::vec3(2.f, -4.f, 11.f), glm::vec3(4.f, -2.f, 9.f));

		m_building8->setPosition(glm::vec3(5.f, -4.f, 1.f), glm::vec3(7.f, -1.f, -1.f));
		m_building9->setPosition(glm::vec3(5.f, -4.f, 5.f), glm::vec3(7.f, 1.f, 2.f));
		m_building10->setPosition(glm::vec3(5.f, -4.f, 11.f), glm::vec3(7.f, 3.f, 6.f));

		m_building11->setPosition(glm::vec3(8.f, -4.f, 1.f), glm::vec3(10.f, 0.f, -1.f));
		m_building12->setPosition(glm::vec3(8.f, -4.f, 5.f), glm::vec3(10.f, 1.f, 2.f));
		m_building13->setPosition(glm::vec3(8.f, -4.f, 8.f), glm::vec3(10.f, 3.f, 6.f));
		m_building14->setPosition(glm::vec3(8.f, -4.f, 11.f), glm::vec3(10.f, 0.f, 9.f));


		glm::vec4 buildingColor = glm::vec4(0.25f, 0.2f, 0.25f, 1.f);
		srand(static_cast<unsigned int>(time(NULL)));
		glm::vec4 temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		auto flagColor = buildingColor + temp;
		flagColor.a = 1.f;
		m_building1->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building2->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building3->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building4->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building5->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building6->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building7->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building8->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building9->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building10->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building11->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building12->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building13->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building14->setColor(buildingColor + temp);
		
		auto boxAnnotation = new glannotations::FontAnnotation(new glannotations::QuadState(glm::vec3(10.01f, -4.f, 1.f), glm::vec3(10.01f, -4.f, -1.f), glm::vec3(10.01f, -2.5f, -1.f)), "Box", "segoeuil.ttf", dfFactory);
		m_annotations.addAnnotation(boxAnnotation);
		boxAnnotation->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxAnnotation->getState()->setMaximumHeight(1.5f);
		boxAnnotation->getState()->setStyling(new glannotations::Styles::Outline(0.05f, glm::vec3(0.f, 0.f, 0.f)));
		boxAnnotation->getState()->asQuadState().setExternalReference(new glannotations::BoxReference(glm::vec2(0.f, 0.f), glm::vec2(0.f, 2.75f), glm::vec3(-5.f, 0.f, 0.f), false));
		

		auto labelAnnotation = new glannotations::FontAnnotation(new glannotations::QuadState(glm::vec3(-4.75f, 3.f, 12.f), glm::vec3(-1.25f, 3.f, 12.f), glm::vec3(-1.25f, 5.f, 12.f)), "Label", "segoeuil.ttf", dfFactory);
		auto labelRef = new glannotations::LabelReference(glm::vec3(0.f, -3.f, 10.f));
		labelRef->setColor(glm::vec4(0.f, 0.f, 0.f, 1.f));
		labelAnnotation->getState()->asQuadState().setExternalReference(labelRef);
		labelAnnotation->getState()->setMaximumHeight(1.5f);
		labelAnnotation->getState()->setKeepSourceAspectRatio(true);
		labelAnnotation->getState()->setVerticalAnchor(glannotations::Anchor::BOTTOM);
		labelAnnotation->getState()->setHorizontalAnchor(glannotations::Anchor::RIGHT);
		m_annotations.addAnnotation(labelAnnotation);


		//spline 3D
		std::vector<glm::vec3> ctrlPoints;
		ctrlPoints.push_back(glm::vec3(0, 0, 0));
		ctrlPoints.push_back(glm::vec3(-1, 1, 0));
		ctrlPoints.push_back(glm::vec3(-2, 0, 0));
		ctrlPoints.push_back(glm::vec3(-3, 1, 0));

		std::vector<glm::vec3> ctrlPoints2;
		ctrlPoints2.push_back(glm::vec3(0, 2, -2));
		ctrlPoints2.push_back(glm::vec3(2, 4, 0));
		ctrlPoints2.push_back(glm::vec3(4, 2, 2));
		ctrlPoints2.push_back(glm::vec3(6, 4, 0));
		

		//spline 2D
		std::vector<glm::vec2> ctrlPoints2D;
		ctrlPoints2D.push_back(glm::vec2(0, 15));
		ctrlPoints2D.push_back(glm::vec2(0, 8));
		ctrlPoints2D.push_back(glm::vec2(0, 0));
		ctrlPoints2D.push_back(glm::vec2(7, 0));
		ctrlPoints2D.push_back(glm::vec2(15, 0));
		ctrlPoints2D.push_back(glm::vec2(15, 8));
		ctrlPoints2D.push_back(glm::vec2(15, 15));

		std::vector<glm::vec2> ctrlPoints2D2;
		ctrlPoints2D2.push_back(glm::vec2(1, 15));
		ctrlPoints2D2.push_back(glm::vec2(1, 8));
		ctrlPoints2D2.push_back(glm::vec2(1, 1));
		ctrlPoints2D2.push_back(glm::vec2(7, 1));
		ctrlPoints2D2.push_back(glm::vec2(14, 1));
		ctrlPoints2D2.push_back(glm::vec2(14, 8));
		ctrlPoints2D2.push_back(glm::vec2(14, 15));

		std::vector<float> knotValues = { 0.f, 0.f, 0.f, 1.f, 2.f, 3.f, 3.f, 3.f };
		/*
		auto splineAnnotation = new glannotations::FontAnnotation(
			new glannotations::SplineState(
				glm::vec3(-8, 4, 4), ctrlPoints, knotValues, ctrlPoints2, knotValues
			), "Spline State", "segoeuil.ttf", dfFactory
		);
		//*/

		/*
		auto splineAnnotation = new glannotations::FontAnnotation(
			new glannotations::SplineState( glm::vec3(-3, -4, 4), ctrlPoints, 3, glm::vec3(0, 1.25f, 0.5f))
			, "Spline State", "segoeuil.ttf", dfFactory
		);
		//*/
		
		auto splineAnnotation = new glannotations::FontAnnotation(
			new glannotations::SplineState(glm::vec3(-5.f, -4.f, -5.f), glm::vec3(0, 0, 20), glm::vec3(20, 0, 0), ctrlPoints2D, 3, ctrlPoints2D2, 3)
			, "   Final Presentation: Spline State with external Reference (Rect)   ", "segoeuil.ttf", dfFactory
			);
		//*/

		//splineAnnotation->getState()->asSplineState().setExternalReference(new glannotations::LabelReference(glm::vec3(0.f, -3.f, 10.f)));
		splineAnnotation->getState()->asSplineState().setExternalReference(new glannotations::RectReference());
		//splineAnnotation->getState()->setKeepSourceAspectRatio(true);
		splineAnnotation->getState()->setVerticalAnchor(glannotations::Anchor::BOTTOM);
		splineAnnotation->getState()->setHorizontalAnchor(glannotations::Anchor::LEFT);
		m_annotations.addAnnotation(splineAnnotation);

		auto internalAnnotation = new glannotations::FontAnnotation(new glannotations::QuadState(glm::vec3(2.f, -3.f, -1.01f), glm::vec3(2.f, 5.f, -1.01f), glm::vec3(4.f, 5.f, -1.01f)), "Quad State", "segoeuil.ttf", dfFactory);
		internalAnnotation->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		internalAnnotation->getState()->setKeepSourceAspectRatio(true);
		internalAnnotation->getState()->setMaximumHeight(1.5f);
		internalAnnotation->getState()->setStyling(new glannotations::Styles::Outline(0.05f, glm::vec3(0.f, 0.f, 0.f)));
		m_annotations.addAnnotation(internalAnnotation);

		auto topAnnotation = new glannotations::FontAnnotation(new glannotations::QuadState(glm::vec3(-1.f, -2.98f, -1.f), glm::vec3(-1.f, -2.98f, 1.f), glm::vec3(1.f, -2.98f, 1.f)), "Rect", "segoeuil.ttf", dfFactory);
		topAnnotation->getState()->setMaximumHeight(1.f);
		topAnnotation->getState()->setKeepSourceAspectRatio(true);
		topAnnotation->getState()->setVerticalAnchor(glannotations::Anchor::BOTTOM);
		topAnnotation->getState()->asQuadState().setExternalReference(new glannotations::RectReference);
		topAnnotation->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		m_annotations.addAnnotation(topAnnotation);

		m_annotations.addAnnotation(new glannotations::PNGAnnotation(new glannotations::QuadState(glm::vec3(-2.f, -4.f, 2.f), glm::vec3(-2.f, -4.f, 7.f), glm::vec3(-2.f, 1.0f, 7.f)), "hpi.png", dfFactory));
		auto viewportAnnotation = new glannotations::FontAnnotation(new glannotations::ViewportState(glm::vec2(0.4f, -1.f), glm::vec2(0.95f, -0.8f), glm::vec2(0.95f, 0.f)), "Viewport State", "segoeuil.ttf", dfFactory);
		viewportAnnotation->getState()->setKeepSourceAspectRatio(true);
		viewportAnnotation->getState()->setHorizontalAnchor(glannotations::Anchor::RIGHT);
		viewportAnnotation->getState()->setVerticalAnchor(glannotations::Anchor::BOTTOM);
		m_annotations.addAnnotation(viewportAnnotation);
		// m_hpilogo->addState(new glannotations::ViewportState(glm::vec2(-.25f, -.5f), glm::vec2(0.25f, 0.5f)));

		auto flagAnnotation = new glannotations::FontAnnotation(new glannotations::QuadState(glm::vec3(-1.f, 2.5f, 5.f), glm::vec3(1.f, 2.5f, 5.f), glm::vec3(1.f, 4.f, 5.f)), "Flag", "segoeuil.ttf", dfFactory);
		auto flagRef = new glannotations::FlagReference(2.5f, glm::vec3(0.f, 0.f, -3.f), false);
		flagRef->setColor(flagColor);
		flagAnnotation->getState()->asQuadState().setExternalReference(flagRef);
		flagAnnotation->getState()->setMaximumHeight(1.3f);
		flagAnnotation->getState()->setKeepSourceAspectRatio(true);
		flagAnnotation->getState()->setVerticalAnchor(glannotations::Anchor::TOP);
		flagAnnotation->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
		m_annotations.addAnnotation(flagAnnotation);

        window.addTimer(0, 0, false);

        cameraChanged();
    }

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
        m_camera->setViewport(event.width(), event.height());

        cameraChanged();
    }

    void cameraChanged()
    {
		glannotations::setView(m_camera->view(), 2);
		glannotations::setProjection(m_camera->projection(), 2);
		glannotations::setAspectRatio(m_camera->aspectRatio(), 2);
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// draw calls
		m_building->draw();
		m_building1->draw();
		m_building2->draw();
		m_building3->draw();
		m_building4->draw();
		m_building5->draw();
		m_building6->draw();
		m_building7->draw();
		m_building8->draw();
		m_building9->draw();
		m_building10->draw();
		m_building11->draw();
		m_building12->draw();
		m_building13->draw();
		m_building14->draw();

		m_annotations.draw(500);
    }

    virtual void keyPressEvent(KeyEvent & event) override
    {
        WindowEventHandler::keyPressEvent(event);

        switch (event.key())
        {
        case GLFW_KEY_SPACE:
			m_camera->setCenter(vec3(0.f, 0.f, 5.f));
			m_camera->setEye(vec3(-17.f, 12.f, -15.0f));
			m_camera->setUp(vec3(0, 1, 0));
            cameraChanged();
            break;
        }
    }

    virtual void mousePressEvent(MouseEvent & event) override
    {
        switch (event.button())
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            m_nav.panBegin(event.pos());
            event.accept();
            break;

        case GLFW_MOUSE_BUTTON_RIGHT:
            m_nav.rotateBegin(event.pos());
            event.accept();
            break;
        }
    }

    virtual void mouseMoveEvent(MouseEvent & event) override
    {
        switch (m_nav.mode())
        {
        case WorldInHandNavigation::PanInteraction:
            m_nav.panProcess(event.pos());
            event.accept();
            cameraChanged();
            break;

        case WorldInHandNavigation::RotateInteraction:
            m_nav.rotateProcess(event.pos());
            event.accept();
            cameraChanged();
            break;
        case WorldInHandNavigation::NoInteraction:
            break;
        }
    }

    virtual void mouseReleaseEvent(MouseEvent & event) override
    {
        switch (event.button())
        {
		case GLFW_MOUSE_BUTTON_LEFT:
            m_nav.panEnd();
            event.accept();
            break;

        case GLFW_MOUSE_BUTTON_RIGHT:
            m_nav.rotateEnd();
            event.accept();
            break;
        }
    }

    virtual void scrollEvent(ScrollEvent & event) override
    {
        if (WorldInHandNavigation::NoInteraction != m_nav.mode())
            return;

        m_nav.scaleAtMouse(event.pos(), -event.offset().y * 0.1f);
        event.accept();
        cameraChanged();
    }

    virtual float depthAt(const ivec2 & windowCoordinates) const override
    {
        float depth = AbstractCoordinateProvider::depthAt(*m_camera, GL_DEPTH_COMPONENT, windowCoordinates);

        return depth;
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates) const override
    {
        return unproject(*m_camera, static_cast<GLenum>(GL_DEPTH_COMPONENT), windowCoordinates);
    }

    virtual vec3 objAt(const ivec2 & windowCoordinates, const float depth) const override
    {
        return unproject(*m_camera, depth, windowCoordinates);
    }

    virtual vec3 objAt(
        const ivec2 & windowCoordinates
    ,   const float depth
    ,   const mat4 & viewProjectionInverted) const override
    {
        return unproject(*m_camera, viewProjectionInverted, depth, windowCoordinates);
    }

protected:

    Camera* m_camera;
    WorldInHandNavigation m_nav;
    ivec2 m_lastMousePos;

	AxisAlignedBoundingBox m_aabb;

	glannotations::AnnotationGroup m_annotations;

	globjects::ref_ptr<Building> m_building;
	globjects::ref_ptr<Building> m_building1;
	globjects::ref_ptr<Building> m_building2;
	globjects::ref_ptr<Building> m_building3;
	globjects::ref_ptr<Building> m_building4;
	globjects::ref_ptr<Building> m_building5;
	globjects::ref_ptr<Building> m_building6;
	globjects::ref_ptr<Building> m_building7;
	globjects::ref_ptr<Building> m_building8;
	globjects::ref_ptr<Building> m_building9;
	globjects::ref_ptr<Building> m_building10;
	globjects::ref_ptr<Building> m_building11;
	globjects::ref_ptr<Building> m_building12;
	globjects::ref_ptr<Building> m_building13;
	globjects::ref_ptr<Building> m_building14;
};


int main(int /*argc*/, char * /*argv*/[])
{
    info() << "Usage:";
    info() << "\t" << "ESC" << "\t\t"        << "Close example";
    info() << "\t" << "ALT + Enter"          << "\t" << "Toggle fullscreen";
    info() << "\t" << "F11" << "\t\t"        << "Toggle fullscreen";
    info() << "\t" << "F10" << "\t\t"        << "Toggle vertical sync";
    info() << "\t" << "F5" << "\t\t"         << "Reload shaders";
    info() << "\t" << "Space" << "\t\t"      << "Reset camera";
    info() << "\t" << "Left Mouse" << "\t"   << "Pan scene";
    info() << "\t" << "Right Mouse" << "\t"  << "Rotate scene";
    info() << "\t" << "Mouse Wheel" << "\t"  << "Zoom scene";

    ContextFormat format;
    format.setVersion(3, 2);
	format.setSamples(24);
    format.setProfile(ContextFormat::Profile::Core);

    Window::init();

    Window window;
    window.setEventHandler(new EventHandler());

    if (!window.create(format, "glannotations - Rendering Example"))
        return 1;
	window.context()->setSwapInterval(Context::SwapInterval::AdaptiveVerticalSyncronization);
    window.show();

    return MainLoop::run();
}
