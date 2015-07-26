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

#include <gloperate/primitives/ScreenAlignedQuad.h>

#include <common/AxisAlignedBoundingBox.h>
#include <common/Camera.h>
#include <common/AbstractCoordinateProvider.h>
#include <common/WorldInHandNavigation.h>
#include <common/ContextFormat.h>
#include <common/Context.h>
#include <common/Window.h>
#include <common/WindowEventHandler.h>
#include <common/events.h>

#include <glannotations/Positioning/ObjectAlignedBoundingBox.h>
#include <glannotations/AnnotationSpace.h>
#include <glannotations/AnnotationPositioner.h>

#include <iostream>

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
		delete m_camera;
    }

    virtual void initialize(Window & window) override
    {
		WindowEventHandler::initialize(window);
		glannotations::initializeMatricesUBO();

		m_colorTexture = Texture::createDefault(GL_TEXTURE_2D);
		m_depthTexture = Texture::createDefault(GL_TEXTURE_2D);

		glClearColor(1.f, 1.f, 1.f, 0.f);
		gl::glEnable(gl::GL_CULL_FACE);
		gl::glEnable(gl::GL_DEPTH_TEST);
		gl::glEnable(gl::GL_BLEND);
		gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

		m_fbo = new globjects::Framebuffer;
		m_fbo->attachTexture(GL_COLOR_ATTACHMENT0, m_colorTexture);
		m_fbo->attachTexture(GL_DEPTH_ATTACHMENT, m_depthTexture);
		m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });

		m_quad = new gloperate::ScreenAlignedQuad(m_colorTexture);

		m_camera->setZNear(0.1f);
		m_camera->setZFar(1024.f);
		m_camera->setCenter(vec3(0.f, 0.f, 0.f));
		m_camera->setEye(vec3(0.f, 0.f, -5.0f));
		m_camera->setUp(vec3(0, 1, 0));
		cameraChanged();

		m_annotationSpace = new glannotations::AnnotationSpace;
		auto data = new glannotations::AnnotationData;
		data->set("revision", "42");
		auto spaceObject = new glannotations::ObjectAlignedBoundingBox(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		spaceObject->setAnnotationClasses({"revision_information"});
		spaceObject->setData(data);
		m_annotationSpace->addSpaceObjects({ spaceObject });

		auto annotationPositioner = std::make_shared<glannotations::AnnotationPositioner>(m_annotationSpace);
		m_annotationGroups = annotationPositioner->generateAnnotationGroups("scenarioConfig.json");

        window.addTimer(0, 0, false);
    }

    virtual void framebufferResizeEvent(ResizeEvent & event) override
    {
        glViewport(0, 0, event.width(), event.height());
        m_camera->setViewport(event.width(), event.height());
		m_colorTexture->image2D(0, GL_RGBA8, event.width(), event.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		m_depthTexture->image2D(0, GL_DEPTH_COMPONENT, event.width(), event.height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        cameraChanged();
    }

    void cameraChanged()
	{
		glannotations::setView(m_camera->view());
		glannotations::setProjection(m_camera->projection());
		glannotations::setAspectRatio(m_camera->aspectRatio());
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);
		m_fbo->bind();
		for (auto group : m_annotationGroups) {
			group->update(500);
		}

		// clear depth buffer after update to use it for positioning update
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (auto group : m_annotationGroups) {
			group->draw();
		}
		m_fbo->unbind();

		// screen aligned quad rendering the color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_colorTexture->bindActive(GL_TEXTURE0);
		m_depthTexture->bindActive(GL_TEXTURE1);

		m_quad->draw();

		m_depthTexture->unbindActive(GL_TEXTURE1);
		m_colorTexture->unbindActive(GL_TEXTURE0);

    }

    virtual void keyPressEvent(KeyEvent & event) override
    {
        WindowEventHandler::keyPressEvent(event);

        switch (event.key())
        {
		case GLFW_KEY_SPACE:
			m_camera->setCenter(vec3(0.f, 0.f, 0.f));
			m_camera->setEye(vec3(0.f, 0.f, -5.0f));
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
		m_fbo->bind();
        float depth = AbstractCoordinateProvider::depthAt(*m_camera, GL_DEPTH_COMPONENT, windowCoordinates);
		m_fbo->unbind();
		std::cout << depth << std::endl;
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
	std::vector<globjects::ref_ptr<glannotations::AnnotationGroup> > m_annotationGroups;
	globjects::ref_ptr<glannotations::AnnotationSpace> m_annotationSpace;
    Camera* m_camera;
    WorldInHandNavigation m_nav;
	ivec2 m_lastMousePos;

	globjects::ref_ptr<globjects::Framebuffer> m_fbo;
	globjects::ref_ptr<globjects::Texture> m_depthTexture;
	globjects::ref_ptr<globjects::Texture> m_colorTexture;

	globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;

	AxisAlignedBoundingBox m_aabb;
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
