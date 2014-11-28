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
#include <glannotations/RendererFactory.h>
#include <glannotations/FontAnnotation.h>
#include <glannotations/SVGAnnotation.h>
#include <glannotations/PNGAnnotation.h>
#include <glannotations/ViewportState.h>
#include <glannotations/InternalState.h>
#include <glannotations/BoxReference.h>
#include <glannotations/LabelReference.h>
#include <glannotations/Styles.h>

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
		glat::cleanUp();
		delete m_camera;
    }

    virtual void initialize(Window & window) override
    {
		WindowEventHandler::initialize(window);

		glat::RendererFactory dfFactory;
		dfFactory.useNVpr(false);
		dfFactory.setMatricesBindingIndex(2);

        glClearColor(1.f, 1.f, 1.f, 0.f);
		gl::glEnable(gl::GL_CULL_FACE);
		gl::glEnable(gl::GL_DEPTH_TEST);
		gl::glEnable(gl::GL_BLEND);
		gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

        m_camera->setZNear(1.f);
		m_camera->setZFar(2048.f);
		m_camera->setCenter(vec3(0.f, 0.f, 5.f));
		m_camera->setEye(vec3(-17.f, 12.f, -15.0f));
		m_camera->setUp(vec3(0, 1, 0));

		m_building = new glat::Building(2);
		m_building1 = new glat::Building(2);
		m_building2 = new glat::Building(2);
		m_building3 = new glat::Building(2);
		m_building4 = new glat::Building(2);
		m_building5 = new glat::Building(2);
		m_building6 = new glat::Building(2);
		m_building7 = new glat::Building(2);
		m_building8 = new glat::Building(2);
		m_building9 = new glat::Building(2);
		m_building10 = new glat::Building(2);
		m_building11 = new glat::Building(2);
		m_building12 = new glat::Building(2);
		m_building13 = new glat::Building(2);
		m_building14 = new glat::Building(2);

		float color;
		srand(time(NULL));
		color = ((rand() * 0.4f) / RAND_MAX) + 0.6f;
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
		srand(time(NULL));
		glm::vec4 temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
		m_building->setColor(buildingColor + temp); temp = glm::vec4((rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, (rand()*0.2f) / RAND_MAX, 0.f);
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

		auto glatBox = new glat::FontAnnotation(new glat::InternalState(glm::vec3(10.01f, -4.f, 1.f), glm::vec3(10.01f, -4.f, -1.f), glm::vec3(10.01f, -2.75f, -1.f)), "Box", "calibri.ttf", dfFactory);
		m_annotations.addAnnotation(glatBox);
		glatBox->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		glatBox->getState()->setStyling(new glat::Styles::Outline(1.f, glm::vec3(0.f, 0.f, 0.f)));
		glatBox->getState()->asInternalState().setExternalReference(new glat::BoxReference(glm::vec2(0.f, 0.f), glm::vec2(0.f, 2.75f), glm::vec3(-5.f, 0.f, 0.f), false));

		auto cgsLabel = new glat::FontAnnotation(new glat::InternalState(glm::vec3(-4.75f, 3.f, 12.f), glm::vec3(-1.25f, 3.f, 12.f), glm::vec3(-1.25f, 5.f, 12.f)), "Label", "calibri.ttf", dfFactory);
		cgsLabel->getState()->asInternalState().setExternalReference(new glat::LabelReference(glm::vec3(0.f, -3.f, 10.f)));
		m_annotations.addAnnotation(cgsLabel);

		auto hpicgs = new glat::FontAnnotation(new glat::InternalState(glm::vec3(2.f, -1.f, -1.01f), glm::vec3(2.f, 3.f, -1.01f), glm::vec3(4.f, 3.f, -1.01f)), "Internal", "calibri.ttf", dfFactory);
		m_annotations.addAnnotation(hpicgs);
		hpicgs->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		hpicgs->getState()->setStyling(new glat::Styles::Outline(1.f, glm::vec3(0.f, 0.f, 0.f)));

		m_annotations.addAnnotation(new glat::PNGAnnotation(new glat::InternalState(glm::vec3(-2.f, -4.f, 2.f), glm::vec3(-2.f, -4.f, 7.f), glm::vec3(-2.f, 1.0f, 7.f)), "hpi.png", dfFactory));

		m_annotations.addAnnotation(new glat::FontAnnotation(new glat::ViewportState(glm::vec2(0.4f, -1.f), glm::vec2(0.9f, -0.7f)), "Viewport", "calibri.ttf", dfFactory));
		// m_hpilogo->addState(new glat::ViewportState(glm::vec2(-.25f, -.5f), glm::vec2(0.25f, 0.5f)));

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
		glat::setView(m_camera->view(), 2);
		glat::setProjection(m_camera->projection(), 2);
		glat::setViewport(m_camera->viewport(), 2);
		float fovx = m_camera->aspectRatio() * m_camera->fovy();
		glat::setViewFrustumVolume(glm::vec2(2 * m_camera->zNear() / std::sin(M_PI_2 - fovx) * std::sin(fovx),
			2 * m_camera->zNear() / std::sin(M_PI_2 - m_camera->fovy()) * std::sin(m_camera->fovy())), 
			2);
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

		m_annotations.draw();
    }

    virtual void keyPressEvent(KeyEvent & event) override
    {
        WindowEventHandler::keyPressEvent(event);

        switch (event.key())
        {
        case GLFW_KEY_1:
        case GLFW_KEY_2:
        case GLFW_KEY_3:
        case GLFW_KEY_4:
        case GLFW_KEY_5:
            //m_gBufferChoice->program()->setUniform<GLint>("choice", event.key() - 49);
            break;

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

	glat::AnnotationGroup m_annotations;

	globjects::ref_ptr<glat::Building> m_building;
	globjects::ref_ptr<glat::Building> m_building1;
	globjects::ref_ptr<glat::Building> m_building2;
	globjects::ref_ptr<glat::Building> m_building3;
	globjects::ref_ptr<glat::Building> m_building4;
	globjects::ref_ptr<glat::Building> m_building5;
	globjects::ref_ptr<glat::Building> m_building6;
	globjects::ref_ptr<glat::Building> m_building7;
	globjects::ref_ptr<glat::Building> m_building8;
	globjects::ref_ptr<glat::Building> m_building9;
	globjects::ref_ptr<glat::Building> m_building10;
	globjects::ref_ptr<glat::Building> m_building11;
	globjects::ref_ptr<glat::Building> m_building12;
	globjects::ref_ptr<glat::Building> m_building13;
	globjects::ref_ptr<glat::Building> m_building14;
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
	window.context()->setSwapInterval(Context::SwapInterval::NoVerticalSyncronization);
    window.show();

    return MainLoop::run();
}