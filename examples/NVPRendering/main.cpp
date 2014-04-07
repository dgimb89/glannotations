
#include <GL/glew.h>

#include <algorithm>
#include <random>
#include <cassert>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/logging.h>
#include <glow/VertexArrayObject.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/global.h>
#include <glowutils/AutoTimer.h>
#include <glowutils/AxisAlignedBoundingBox.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/Camera.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>
#include <glowutils/FlightNavigation.h>
#include <glowutils/File.h>
#include <glowutils/Timer.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <glat/FontAnnotation.h>
#include <glat/SVGAnnotation.h>
#include <glat/PNGAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/Styles.h>

using namespace glowwindow;
using namespace glm;


class EventHandler : public WindowEventHandler, glowutils::AbstractCoordinateProvider
{
public:
	EventHandler()
		: m_camera(vec3(0.f, 1.f, 4.0f))
		, m_flightEnabled(false)
	{
		m_aabb.extend(vec3(-8.f, -1.f, -8.f));
		m_aabb.extend(vec3(8.f, 1.f, 8.f));

		m_nav.setCamera(&m_camera);
		m_nav.setCoordinateProvider(this);
		m_nav.setBoundaryHint(m_aabb);

		m_flightNav.setCamera(&m_camera);

		m_timer.start();
		m_previousTime = 0.0;
		m_swapElapsedTime = 0.0;
		m_swapCount = 0;

		m_interpolation = 0.f;
	}

	virtual ~EventHandler()
	{
	}

	void createAndSetupTexture();
	void createAndSetupShaders();
	void createAndSetupGeometry();

	virtual void initialize(Window & window) override
	{
		glow::debugmessageoutput::enable();

		glClearColor(1.0f, 1.0f, 1.0f, 0.f);


		m_sphere = new glow::Program();
		m_sphere->attach(
			glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/adaptive-grid/sphere.vert")
			, glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/adaptive-grid/sphere.frag"));

		m_icosahedron = new glowutils::Icosahedron(2);
		m_agrid = new glowutils::AdaptiveGrid(16U);

		m_camera.setZNear(0.1f);
		m_camera.setZFar(10240.f);

		m_agrid->setCamera(&m_camera);

		m_nvprViewportFontAnnotation = new glat::FontAnnotation(new glat::ViewportState(glm::vec2(0.8f, -1.f), glm::vec2(1.f, 0.f)));
		m_nvprViewportFontAnnotation->getState()->setStyling(new glat::Styles::Outline(3.f, glm::vec3(.3f, .3f, .3f)));
		m_nvprViewportFontAnnotation->getState()->setStyling(new glat::Styles::BumpMap(1.0f));
		m_nvprViewportFontAnnotation->setColor(glm::vec4(0.75, 0.75, 0.75, 1.0));
		m_nvprViewportFontAnnotation->setText("0");
		m_nvprViewportFontAnnotation->setFontName("calibri.ttf");

		m_nvprInternalFontAnnotation = new glat::FontAnnotation(new glat::InternalState(glm::vec3(-3.f, -2.f, -5.f), glm::vec3(3.f, -2.f, -5.f), glm::vec3(3.f, 2.0f, -5.f), &m_camera));
		m_nvprInternalFontAnnotation->setFontName("calibri.ttf");
		m_nvprInternalFontAnnotation->setText("NVPR");
		m_nvprInternalFontAnnotation->setColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

		m_nvprViewportSVGAnnotation = new glat::SVGAnnotation(new glat::ViewportState(glm::vec2(-1.f, -1.f), glm::vec2(-0.3f, 0.f)));
		m_nvprViewportSVGAnnotation->getState()->setStyling(new glat::Styles::Outline(2.f, glm::vec3(.3f, .3f, .3f)));
		m_nvprViewportSVGAnnotation->setPathString("M100,180 L40,10 L190,120 L10,120 L160,10 z");
		m_nvprViewportSVGAnnotation->setHeight(190);
		m_nvprViewportSVGAnnotation->setWidth(200);

		window.addTimer(0, 0, false);
	}
	virtual void finalize(Window &) override
	{
		m_sphere = nullptr;
		m_icosahedron = nullptr;
		m_agrid = nullptr;
		m_nvprViewportFontAnnotation = nullptr;
	}

	virtual void framebufferResizeEvent(ResizeEvent & event) override
	{
		glViewport(0, 0, event.width(), event.height());
		m_camera.setViewport(event.width(), event.height());
	}

	virtual void paintEvent(PaintEvent & event) override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_agrid->update();
		m_sphere->setUniform("transform", m_camera.viewProjection());

		m_sphere->use();
		m_icosahedron->draw();
		m_sphere->release();

		m_agrid->draw();

		char clockBuffer[10];
		sprintf(clockBuffer, "%d", clock() / CLOCKS_PER_SEC);
		m_nvprViewportFontAnnotation->setText(clockBuffer);

		m_nvprViewportFontAnnotation->draw();
		m_nvprViewportSVGAnnotation->draw();
		m_nvprInternalFontAnnotation->draw();

		computeFps(event);
	}

	virtual void timerEvent(TimerEvent & event) override
	{
		float delta = static_cast<float>((m_timer.elapsed() - m_previousTime) / 1000.0 / 1000.0 / 1000.0);
		m_previousTime = m_timer.elapsed();
		m_flightNav.move(delta);

		event.window()->repaint();
	}

	virtual void keyPressEvent(KeyEvent & event) override
	{
		switch (event.key())
		{
		case GLFW_KEY_F5:
			glowutils::File::reloadAll();
			break;
		case GLFW_KEY_1:
			m_flightEnabled = !m_flightEnabled;
			if (!m_flightEnabled)
				m_flightNav.stopMovement(glowutils::FlightNavigation::All);
			event.window()->setInputMode(GLFW_CURSOR, m_flightEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
			break;
		case GLFW_KEY_SPACE:
			m_camera.setCenter(vec3());
			m_camera.setEye(vec3(0.f, 1.f, 4.0f));
			m_camera.setUp(vec3(0, 1, 0));
			break;
		case GLFW_KEY_F11:
			event.window()->toggleMode();
			break;
		case GLFW_KEY_N:
			m_interpolation += 0.01;
			m_interpolation = min(m_interpolation, 1.f);
			break;
		case GLFW_KEY_M:
			m_interpolation -= 0.01;
			m_interpolation = max(m_interpolation, 0.f);
			break;
		}

		if (!m_flightEnabled)
			return;
		switch (event.key())
		{
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			m_flightNav.startMovement(glowutils::FlightNavigation::Forward);
			break;
		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			m_flightNav.startMovement(glowutils::FlightNavigation::Left);
			break;
		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			m_flightNav.startMovement(glowutils::FlightNavigation::Backward);
			break;
		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			m_flightNav.startMovement(glowutils::FlightNavigation::Right);
			break;
		}
	}
	virtual void keyReleaseEvent(KeyEvent & event) override
	{
		if (!m_flightEnabled)
			return;
		switch (event.key())
		{
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			m_flightNav.stopMovement(glowutils::FlightNavigation::Forward);
			break;
		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			m_flightNav.stopMovement(glowutils::FlightNavigation::Left);
			break;
		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			m_flightNav.stopMovement(glowutils::FlightNavigation::Backward);
			break;
		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			m_flightNav.stopMovement(glowutils::FlightNavigation::Right);
			break;
		}
	}

	virtual void mousePressEvent(MouseEvent & event) override
	{
		if (m_flightEnabled)
			return;

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
		if (m_flightEnabled)
		{
			m_flightNav.mouseMove((event.pos() - m_lastMousePos));
			m_lastMousePos = event.pos();
			event.accept();
			return;
		}

		switch (m_nav.mode())
		{
		case glowutils::WorldInHandNavigation::PanInteraction:
			m_nav.panProcess(event.pos());
			event.accept();
			break;

		case glowutils::WorldInHandNavigation::RotateInteraction:
			m_nav.rotateProcess(event.pos());
			event.accept();
		}
	}
	virtual void mouseReleaseEvent(MouseEvent & event) override
	{
		if (m_flightEnabled)
			return;

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

	void scrollEvent(ScrollEvent & event) override
	{
		if (m_flightEnabled)
			return;

		if (glowutils::WorldInHandNavigation::NoInteraction != m_nav.mode())
			return;

		m_nav.scaleAtMouse(event.pos(), -event.offset().y * 0.1f);
		event.accept();
	}

	virtual float depthAt(const ivec2 & windowCoordinates) override
	{
		return AbstractCoordinateProvider::depthAt(m_camera, GL_DEPTH_COMPONENT, windowCoordinates);
	}

	virtual vec3 objAt(const ivec2 & windowCoordinates) override
	{
		return unproject(m_camera, static_cast<GLenum>(GL_DEPTH_COMPONENT), windowCoordinates);
	}

	virtual vec3 objAt(const ivec2 & windowCoordinates, const float depth) override
	{
		return unproject(m_camera, depth, windowCoordinates);
	}

	virtual glm::vec3 objAt(
		const ivec2 & windowCoordinates
		, const float depth
		, const mat4 & viewProjectionInverted) override
	{
		return unproject(m_camera, viewProjectionInverted, depth, windowCoordinates);
	}

	bool startsWith(const std::string & str, const std::string str2)
	{
		return str.compare(0, str2.length(), str2) == 0;
	}

	void computeFps(glowwindow::PaintEvent & event)
	{
		m_timer.update();

		++m_swapCount;

		if (m_timer.elapsed() - m_swapElapsedTime >= 1e+9)
		{
			const float fps = 1e+9f * static_cast<float>(static_cast<long double>(m_swapCount) / (m_timer.elapsed() - m_swapElapsedTime));

			std::string title = event.window()->title();
			if (!startsWith(title, m_baseTitle) || m_baseTitle.length() == 0)
			{
				m_baseTitle = title;
			}

			std::stringstream stream;
			stream << m_baseTitle << " (" << std::fixed << std::setprecision(2) << fps << " fps)";

			event.window()->setTitle(stream.str());

			m_swapElapsedTime = m_timer.elapsed();
			m_swapCount = 0;
		}
	}

protected:

	glow::ref_ptr<glow::Program> m_sphere;

	glow::ref_ptr<glowutils::Icosahedron> m_icosahedron;
	glow::ref_ptr<glowutils::AdaptiveGrid> m_agrid;
	glow::ref_ptr<glat::FontAnnotation> m_nvprViewportFontAnnotation;
	glow::ref_ptr<glat::SVGAnnotation> m_nvprViewportSVGAnnotation;
	glow::ref_ptr<glat::FontAnnotation> m_nvprInternalFontAnnotation;

	glowutils::Camera m_camera;
	glowutils::WorldInHandNavigation m_nav;
	glowutils::FlightNavigation m_flightNav;
	glm::ivec2 m_lastMousePos;
	bool m_flightEnabled;
	glowutils::Timer m_timer;
	long double m_previousTime;
	float m_interpolation;

	long double m_swapElapsedTime;
	unsigned int m_swapCount;
	std::string m_baseTitle;

	glowutils::AxisAlignedBoundingBox m_aabb;
};


/** This example shows ... .
*/
int main(int argc, char* argv[])
{
	ContextFormat format;
	format.setVersion(4, 0);
	format.setProfile(ContextFormat::CompatibilityProfile);
	format.setStencilBufferSize(4);
	format.setSamples(24);

	Window window;

	window.setEventHandler(new EventHandler());

	if (window.create(format, "NVP Rendering Example"))
	{
		window.context()->setSwapInterval(Context::NoVerticalSyncronization);

		window.show();

		return MainLoop::run();
	}
	else
	{
		return 1;
	}
}
