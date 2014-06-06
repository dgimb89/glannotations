
#include <GL/glew.h>

#include <algorithm>
#include <random>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Error.h>
#include <glow/Uniform.h>
#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Buffer.h>
#include <glow/logging.h>
#include <glow/RenderBufferObject.h>
#include <glow/FrameBufferObject.h>
#include <glow/FrameBufferAttachment.h>
#include <glow/Texture.h>
#include <glow/VertexArrayObject.h>
#include <glow/debugmessageoutput.h>

#include <glowutils/Timer.h>
#include <glowutils/AxisAlignedBoundingBox.h>
#include <glowutils/Icosahedron.h>
#include <glowutils/Camera.h>
#include <glowutils/AdaptiveGrid.h>
#include <glowutils/AbstractCoordinateProvider.h>
#include <glowutils/WorldInHandNavigation.h>
#include <glowutils/FlightNavigation.h>
#include <glowutils/File.h>
#include <glowutils/File.h>
#include <glowutils/glowutils.h>
#include <glowutils/StringTemplate.h>
#include <glowutils/ScreenAlignedQuad.h>

#include <glowwindow/ContextFormat.h>
#include <glowwindow/Context.h>
#include <glowwindow/Window.h>
#include <glowwindow/WindowEventHandler.h>
#include <glowwindow/events.h>

#include <ExampleWindowEventHandler.h>

#include <glat/RendererFactory.h>
#include <glat/FontAnnotation.h>
#include <glat/SVGAnnotation.h>
#include <glat/PNGAnnotation.h>
#include <glat/ViewportState.h>
#include <glat/InternalState.h>
#include <glat/BoxReference.h>
#include <glat/Styles.h>

#include "building.h"

using namespace glowwindow;
using namespace glm;


class EventHandler : public ExampleWindowEventHandler, glowutils::AbstractCoordinateProvider
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
		glat::RendererFactory dfFactory;
		dfFactory.useNVpr(false);
		glow::debugmessageoutput::enable();

		glClearColor(1.0f, 1.0f, 1.0f, 0.f);

		glowutils::StringTemplate* gbufferVertexShader = new glowutils::StringTemplate(new glowutils::File("data/presentation-scene/gbuffer.vert"));
		glowutils::StringTemplate* gbufferFragmentShader = new glowutils::StringTemplate(new glowutils::File("data/presentation-scene/gbuffer.frag"));
		glowutils::StringTemplate* phongVertexShader = new glowutils::StringTemplate(new glowutils::File("data/presentation-scene/phong.vert"));
		glowutils::StringTemplate* phongFragmentShader = new glowutils::StringTemplate(new glowutils::File("data/presentation-scene/phong.frag"));

		m_gbuffer = new glow::Program();
		m_gbuffer->attach(new glow::Shader(GL_VERTEX_SHADER, gbufferVertexShader), new glow::Shader(GL_FRAGMENT_SHADER, gbufferFragmentShader));

		m_phong = new glow::Program();
		m_phong->attach(new glow::Shader(GL_VERTEX_SHADER, phongVertexShader), new glow::Shader(GL_FRAGMENT_SHADER, phongFragmentShader));

		m_quad = new glowutils::ScreenAlignedQuad(m_phong);

		m_camera.setZNear(0.1f);
		m_camera.setZFar(1024.f);
		m_camera.setCenter(vec3(0.f, 0.f, 5.f));
		m_camera.setEye(vec3(-17.f, 12.f, -15.0f));
		m_camera.setUp(vec3(0, 1, 0));

		m_building = new glat::Building();
		m_building1 = new glat::Building();
		m_building2 = new glat::Building();
		m_building3 = new glat::Building();
		m_building4 = new glat::Building();
		m_building5 = new glat::Building();
		m_building6 = new glat::Building();
		m_building7 = new glat::Building();
		m_building8 = new glat::Building();
		m_building9 = new glat::Building();
		m_building10 = new glat::Building();
		m_building11 = new glat::Building();
		m_building12 = new glat::Building();
		m_building13 = new glat::Building();
		m_building14 = new glat::Building();

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

		m_glatBox = new glat::FontAnnotation(new glat::InternalState(glm::vec3(-1.01f, -4.f, -1.f), glm::vec3(-1.01f, -4.f, 1.f), glm::vec3(-1.01f, -3.f, 1.f), &m_camera), dfFactory);
		m_glatBox->setFontName("calibri.ttf");
		m_glatBox->setText("GLAT");
		m_glatBox->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		m_glatBox->getState()->setStyling(new glat::Styles::Outline(1.f, glm::vec3(0.f, 0.f, 0.f)));
		m_glatBox->getState()->asInternalState().setExternalReference(new glat::BoxReference(glm::vec3(0.f, 0.f, 2.05f), glm::vec3(0.f, 1.05f, 0.f), glm::vec3(2.05f, 0.f, 0.f), &m_camera, true));
		/*m_glatBox = new glat::FontAnnotation(new glat::ExternalBoxState(glm::vec3(-1.f, -4.f, -1.f), glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(2.f, 0.f, 0.f), &m_camera, false), dfFactory);
		m_glatBox->setFontName("calibri.ttf");
		m_glatBox->setText("GLAT");
		m_glatBox->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		m_glatBox->getState()->setStyling(new glat::Styles::Outline(1.f, glm::vec3(0.f, 0.f, 0.f)));

		glat::AbstractState* state = new glat::ViewportState(glm::vec2(-.4f, -.4f), glm::vec2(0.4f, 0.4f));
		m_glatBox->addState(state); 

		m_treevisBox = new glat::FontAnnotation(new glat::ExternalBoxState(glm::vec3(-1.f, -4.f, 9.f), glm::vec3(0.f, 5.f, 0.f), glm::vec3(2.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 2.f), &m_camera, false), dfFactory);
		m_treevisBox->setFontName("calibri.ttf");
		m_treevisBox->setText("TreeVis");
		m_treevisBox->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		m_treevisBox->getState()->setStyling(new glat::Styles::Outline(1.f, glm::vec3(0.f, 0.f, 0.f)));

		m_glowText = new glat::FontAnnotation(new glat::ExternalLabelState(glm::vec3(.75f, 3.f, 3.f), glm::vec3(-3.f, 4.f, 12.f), 3.5f, 2.f, &m_camera, true), dfFactory);
		m_glowText->setText("Demo");
		m_glowText->setFontName("calibri.ttf");
		m_glowText->setColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		m_glowText->getState()->setStyling(new glat::Styles::Outline(1.f, glm::vec3(1.f, 1.f, 1.f)));
		m_glowText->getState()->setStyling(new glat::Styles::ExternalColor(glm::vec4(0.f, 0.f, 0.f, 1.f)));*/

		m_hpicgs = new glat::FontAnnotation(new glat::InternalState(glm::vec3(2.f, -1.f, -1.01f), glm::vec3(2.f, 3.f, -1.01f), glm::vec3(4.f, 3.f, -1.01f), &m_camera), dfFactory);
		m_hpicgs->setFontName("calibri.ttf");
		m_hpicgs->setText("GLOW");
		m_hpicgs->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		m_hpicgs->getState()->setStyling(new glat::Styles::Outline(1.f, glm::vec3(0.f, 0.f, 0.f)));
		/*glat::ExternalBoxState* extBox = new glat::ExternalBoxState(glm::vec3(), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f), &m_camera, true);
		extBox->setStyling(new glat::Styles::ExternalColor(glm::vec4(0.f, 0.f, 0.f, 1.f)));
		m_hpicgs->addState(extBox);*/

		m_hpilogo = new glat::PNGAnnotation(new glat::InternalState(glm::vec3(-2.f, -4.f, 2.f), glm::vec3(-2.f, -4.f, 7.f), glm::vec3(-2.f, 1.0f, 7.f), &m_camera), "hpi.png", dfFactory);
		// m_hpilogo->addState(new glat::ViewportState(glm::vec2(-.25f, -.5f), glm::vec2(0.25f, 0.5f)));

		window.addTimer(0, 0, false);

	}
	virtual void finalize(Window &) override
	{
		m_quad = nullptr;
		m_gbuffer = nullptr;
		m_phong = nullptr;

		m_hpilogo = nullptr;
		m_glatBox = nullptr;
		m_treevisBox = nullptr;
		m_glowText = nullptr;
		m_nvprViewportSVGAnnotation = nullptr;
		m_hpicgs = nullptr;
		m_building = nullptr;

		m_building1 = nullptr;
		m_building2 = nullptr;
		m_building3 = nullptr;
		m_building4 = nullptr;
		m_building5 = nullptr;
		m_building6 = nullptr;
		m_building7 = nullptr;
		m_building8 = nullptr;
		m_building9 = nullptr;
		m_building10 = nullptr;
		m_building11 = nullptr;
		m_building12 = nullptr;
		m_building13 = nullptr;
		m_building14 = nullptr;
	}

	virtual void framebufferResizeEvent(ResizeEvent & event) override
	{
		glViewport(0, 0, event.width(), event.height());
		CheckGLError();

		m_camera.setViewport(event.width(), event.height());
	}

	virtual void paintEvent(PaintEvent &) override
	{
		m_building->setModelViewProjection(m_camera.viewProjection());
		m_building1->setModelViewProjection(m_camera.viewProjection());
		m_building2->setModelViewProjection(m_camera.viewProjection());
		m_building3->setModelViewProjection(m_camera.viewProjection());
		m_building4->setModelViewProjection(m_camera.viewProjection());
		m_building5->setModelViewProjection(m_camera.viewProjection());
		m_building6->setModelViewProjection(m_camera.viewProjection());
		m_building7->setModelViewProjection(m_camera.viewProjection());
		m_building8->setModelViewProjection(m_camera.viewProjection());
		m_building9->setModelViewProjection(m_camera.viewProjection());
		m_building10->setModelViewProjection(m_camera.viewProjection());
		m_building11->setModelViewProjection(m_camera.viewProjection());
		m_building12->setModelViewProjection(m_camera.viewProjection());
		m_building13->setModelViewProjection(m_camera.viewProjection());
		m_building14->setModelViewProjection(m_camera.viewProjection());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		m_phong->setUniform("transformi", m_camera.viewProjectionInverted());

		//m_fbo->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		CheckGLError();

		//m_gbuffer->use();
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

		m_hpilogo->draw();
		m_glatBox->draw();
		/*m_treevisBox->draw();
		m_glowText->draw();*/
		m_hpicgs->draw();
	}

	virtual void timerEvent(TimerEvent & event) override
	{
		float delta = static_cast<float>(static_cast<float>(m_timer.elapsed().count()) / 1000.0 / 1000.0 / 1000.0);
		m_timer.reset();
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
			m_camera.setCenter(vec3(0.f, 0.f, 5.f));
			m_camera.setEye(vec3(-17.f, 12.f, -15.0f));
			m_camera.setUp(vec3(0, 1, 0));
			break;
		case GLFW_KEY_N:
			if (m_interpolation > 0.8)
				m_interpolation += 0.005;
			else
				m_interpolation += 0.01;
			m_interpolation = min(m_interpolation, 1.f);
			//m_hpilogo->setInterpolatedState(0, 1, m_interpolation);
			break;
		case GLFW_KEY_M:
			m_interpolation -= 0.01;
			m_interpolation = max(m_interpolation, 0.f);
			//m_hpilogo->setInterpolatedState(0, 1, m_interpolation);
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
			break;
		case glowutils::WorldInHandNavigation::NoInteraction:
			break;
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

	virtual void scrollEvent(ScrollEvent & event) override
	{
		if (m_flightEnabled)
			return;

		if (glowutils::WorldInHandNavigation::NoInteraction != m_nav.mode())
			return;

		m_nav.scaleAtMouse(event.pos(), -event.offset().y * 0.1f);
		event.accept();
	}

	virtual float depthAt(const ivec2 & windowCoordinates) const override
	{
		return AbstractCoordinateProvider::depthAt(m_camera, GL_DEPTH_COMPONENT, windowCoordinates);
	}

	virtual vec3 objAt(const ivec2 & windowCoordinates) const override
	{
		return unproject(m_camera, static_cast<GLenum>(GL_DEPTH_COMPONENT), windowCoordinates);
	}

	virtual vec3 objAt(const ivec2 & windowCoordinates, const float depth) const override
	{
		return unproject(m_camera, depth, windowCoordinates);
	}

	virtual glm::vec3 objAt(
		const ivec2 & windowCoordinates
		, const float depth
		, const mat4 & viewProjectionInverted) const override
	{
		return unproject(m_camera, viewProjectionInverted, depth, windowCoordinates);
	}

protected:
	glow::ref_ptr<glat::PNGAnnotation> m_hpilogo;
	glow::ref_ptr<glat::FontAnnotation> m_glatBox;
	glow::ref_ptr<glat::FontAnnotation> m_treevisBox;
	glow::ref_ptr<glat::FontAnnotation> m_glowText;
	glow::ref_ptr<glat::SVGAnnotation> m_nvprViewportSVGAnnotation;
	glow::ref_ptr<glat::FontAnnotation> m_hpicgs;
	glow::ref_ptr<glat::Building> m_building;
	bool m_drawViewport;

	glow::ref_ptr<glat::Building> m_building1;
	glow::ref_ptr<glat::Building> m_building2;
	glow::ref_ptr<glat::Building> m_building3;
	glow::ref_ptr<glat::Building> m_building4;
	glow::ref_ptr<glat::Building> m_building5;
	glow::ref_ptr<glat::Building> m_building6;
	glow::ref_ptr<glat::Building> m_building7;
	glow::ref_ptr<glat::Building> m_building8;
	glow::ref_ptr<glat::Building> m_building9;
	glow::ref_ptr<glat::Building> m_building10;
	glow::ref_ptr<glat::Building> m_building11;
	glow::ref_ptr<glat::Building> m_building12;
	glow::ref_ptr<glat::Building> m_building13;
	glow::ref_ptr<glat::Building> m_building14;


	glowutils::Camera m_camera;
	glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
	glow::ref_ptr<glow::FrameBufferObject> m_fbo;
	glow::ref_ptr<glow::Texture> m_colorTex;
	glow::ref_ptr<glow::Texture> m_normalTex;
	glow::ref_ptr<glow::Texture> m_geometryTex;
	glow::ref_ptr<glow::RenderBufferObject> m_depth;
	glow::ref_ptr<glow::Program> m_gbuffer;
	glow::ref_ptr<glow::Program> m_phong;

	glowutils::WorldInHandNavigation m_nav;
	glowutils::FlightNavigation m_flightNav;
	glm::ivec2 m_lastMousePos;
	bool m_flightEnabled;
	glowutils::Timer m_timer;
	float m_interpolation;

	glowutils::AxisAlignedBoundingBox m_aabb;
	float m_interpolation2;
};


/** This example shows ... .
*/
int main(int /*argc*/, char* /*argv*/[])
{
	glow::info() << "Usage:";
	glow::info() << "\t" << "ESC" << "\t\t" << "Close example";
	glow::info() << "\t" << "ALT + Enter" << "\t" << "Toggle fullscreen";
	glow::info() << "\t" << "F11" << "\t\t" << "Toggle fullscreen";
	glow::info() << "\t" << "F5" << "\t\t" << "Reload shaders";
	glow::info() << "\t" << "Space" << "\t\t" << "Reset camera";
	glow::info() << "\t" << "1" << "\t" << "Toggle flight mode / world in hand navigation";

	glow::info();
	glow::info() << "\t" << "During world in hand navigation";
	glow::info() << "\t" << "Left Mouse" << "\t" << "Pan scene";
	glow::info() << "\t" << "Right Mouse" << "\t" << "Rotate scene";
	glow::info() << "\t" << "Mouse Wheel" << "\t" << "Zoom scene";

	glow::info();
	glow::info() << "\t" << "During flight mode";
	glow::info() << "\t" << "Mouse Movement" << "\t" << "Look around";
	glow::info() << "\t" << "W" << "\t\t" << "Move forward";
	glow::info() << "\t" << "UP" << "\t\t" << "Move forward";
	glow::info() << "\t" << "A" << "\t\t" << "Move left";
	glow::info() << "\t" << "LEFT" << "\t\t" << "Move left";
	glow::info() << "\t" << "S" << "\t\t" << "Move backward";
	glow::info() << "\t" << "DOWN" << "\t\t" << "Move backward";
	glow::info() << "\t" << "D" << "\t\t" << "Move right";
	glow::info() << "\t" << "RIGHT" << "\t\t" << "Move right";

	ContextFormat format;
	format.setVersion(3, 0);
	format.setSamples(16);
	Window window;

	window.setEventHandler(new EventHandler());

	if (window.create(format, "Navigations Example"))
	{
		window.context()->setSwapInterval(Context::VerticalSyncronization);

		window.show();

		return MainLoop::run();
	}
	else
	{
		return 1;
	}
}
