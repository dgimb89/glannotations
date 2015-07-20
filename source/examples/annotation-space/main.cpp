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

#include <glannotations/Positioning/ObjectAlignedBoundingBox.h>
#include <glannotations/Positioning/AnnotationSpace.h>


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
		auto spaceObject = new glannotations::ObjectAlignedBoundingBox(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		spaceObject->setAnnotationClasses({"test", "rect"});
		m_annotationSpace.addSpaceObjects({ spaceObject });

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
    }

    virtual void paintEvent(PaintEvent & event) override
    {
        WindowEventHandler::paintEvent(event);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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
	glannotations::AnnotationSpace m_annotationSpace;
    Camera* m_camera;
    WorldInHandNavigation m_nav;
    ivec2 m_lastMousePos;

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
