
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
#include <glow/RenderBufferObject.h>
#include <glow/FrameBufferObject.h>
#include <glow/FrameBufferAttachment.h>
#include <glow/Texture.h>

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
#include <glowutils/StringTemplate.h>
#include <glowutils/ScreenAlignedQuad.h>

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
#include <glat/ExternalBoxState.h>
#include <glat/ExternalLabelState.h>
#include <glat/Styles.h>
#include <glat/Quad.h>

#include "building.h"

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
		glat::RendererFactory dfFactory;
		dfFactory.useNVpr(false);
		glow::debugmessageoutput::enable();

		glClearColor(1.0f, 1.0f, 1.0f, 0.f);

		m_fbo = new glow::FrameBufferObject();

		m_colorTex = new glow::Texture(GL_TEXTURE_2D);
		m_colorTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_colorTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_colorTex->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_colorTex->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		m_colorTex->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_normalTex = new glow::Texture(GL_TEXTURE_2D);
		m_normalTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_normalTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_normalTex->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_normalTex->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		m_normalTex->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_geometryTex = new glow::Texture(GL_TEXTURE_2D);
		m_geometryTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_geometryTex->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_geometryTex->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_geometryTex->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		m_geometryTex->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_depth = new glow::RenderBufferObject();

		//m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_colorTex);
		//m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT1, m_normalTex);
		//m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT2, m_geometryTex);
		//m_fbo->attachRenderBuffer(GL_DEPTH_ATTACHMENT, m_depth);

		//m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });
		//m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0});
		
		glowutils::StringTemplate* gbufferVertexShader = new glowutils::StringTemplate(new glowutils::File("data/gbuffer.vert"));
		glowutils::StringTemplate* gbufferFragmentShader = new glowutils::StringTemplate(new glowutils::File("data/gbuffer.frag"));
		glowutils::StringTemplate* phongVertexShader = new glowutils::StringTemplate(new glowutils::File("data/phong.vert"));
		glowutils::StringTemplate* phongFragmentShader = new glowutils::StringTemplate(new glowutils::File("data/phong.frag"));

		m_gbuffer = new glow::Program();
		m_gbuffer->attach(new glow::Shader(GL_VERTEX_SHADER, gbufferVertexShader), new glow::Shader(GL_FRAGMENT_SHADER, gbufferFragmentShader));

		m_phong = new glow::Program();
		m_phong->attach(new glow::Shader(GL_VERTEX_SHADER, phongVertexShader), new glow::Shader(GL_FRAGMENT_SHADER, phongFragmentShader));

		m_quad = new glowutils::ScreenAlignedQuad(m_phong);
		
		m_camera.setZNear(0.1f);
		m_camera.setZFar(1024.f);

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

		m_glatBox = new glat::FontAnnotation(new glat::ExternalBoxState(glm::vec3(-1.f, -4.f, 1.f), glm::vec3(2.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, -2.f), &m_camera, false), dfFactory);
		m_glatBox->setFontName("calibri.ttf");
		m_glatBox->setText("GLAT");
		m_glatBox->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		m_glatBox->getState()->setStyling(new glat::Styles::ExternalColor(glm::vec4(0.f, 0.f, 1.f, 0.25f)));

		glow::ref_ptr<glat::AbstractState> state = new glat::ViewportState(glm::vec2(-.4f, -.4f), glm::vec2(0.4f, 0.4f));
		m_glatBox->addState(state);

		m_glowText = new glat::FontAnnotation(new glat::ExternalLabelState(glm::vec3(1.f, 3.f, 3.f), glm::vec3(-3.f, 4.f, 12.f), 3.5f, 2.f, &m_camera, true), dfFactory);
		m_glowText->setText("GLOW");
		m_glowText->setFontName("calibri.ttf");
		m_glowText->setColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		m_glowText->getState()->setStyling(new glat::Styles::ExternalColor(glm::vec4(0.f, 0.f, 0.f, 1.f)));

		m_hpilogo = new glat::PNGAnnotation(new glat::InternalState(glm::vec3(-2.f, -4.f, 2.f), glm::vec3(-2.f, -4.f, 7.f), glm::vec3(-2.f, 1.0f, 7.f), &m_camera), "hpi.png", dfFactory);
		m_hpilogo->getState()->setStyling(new glat::Styles::Outline(2.f, glm::vec3(1.f, 1.f, 1.f)));
		m_hpilogo->addState(new glat::ViewportState(glm::vec2(-.25f, -.5f), glm::vec2(0.25f, 0.5f)));

		window.addTimer(0, 0, false);
	}
	virtual void finalize(Window &) override
	{
	}

	virtual void framebufferResizeEvent(ResizeEvent & event) override
	{
		int width = event.width();
		int height = event.height();

		glViewport(0, 0, width, height);
		CheckGLError();

		m_camera.setViewport(width, height);

		m_colorTex->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		m_normalTex->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		m_geometryTex->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

		//int result = glow::FrameBufferObject::defaultFBO()->getAttachmentParameter(GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE);
		//m_depth->storage(result == 16 ? GL_DEPTH_COMPONENT16 : GL_DEPTH_COMPONENT, width, height);
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
		//m_dfViewportPNGAnnotation->draw();
		m_glatBox->draw();
		m_glowText->draw();
		//m_fbo->unbind();

		glDisable(GL_DEPTH_TEST);
		CheckGLError();
		glDepthMask(GL_FALSE);
		CheckGLError();

		glEnable(GL_DEPTH_TEST);
		CheckGLError();
		glDepthMask(GL_TRUE);
		CheckGLError();

	}

	virtual void timerEvent(TimerEvent & event) override
	{
		float delta = static_cast<float>(m_timer.elapsed() / 1000.0 / 1000.0 / 1000.0);
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
			m_camera.setEye(vec3(-12.f, 5.f, -10.0f));
			m_camera.setUp(vec3(0, 1, 0));
			break;
		case GLFW_KEY_N:
			if (m_interpolation > 0.8)
				m_interpolation += 0.005;
			else
				m_interpolation += 0.01;
			m_interpolation = min(m_interpolation, 1.f);
			m_hpilogo->setInterpolatedState(0, 1, m_interpolation);
			break;
		case GLFW_KEY_M:
			m_interpolation -= 0.01;
			m_interpolation = max(m_interpolation, 0.f);
			m_hpilogo->setInterpolatedState(0, 1, m_interpolation);
			break;
		case GLFW_KEY_F11:
			event.window()->toggleMode();
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

protected:
	glow::ref_ptr<glat::PNGAnnotation> m_hpilogo;
	glow::ref_ptr<glat::FontAnnotation> m_glatBox;
	glow::ref_ptr<glat::FontAnnotation> m_glowText;
	glow::ref_ptr<glat::SVGAnnotation> m_nvprViewportSVGAnnotation;
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
	glow::ref_ptr<glat::Quad> m_quadBase;
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
};


/** This example shows ... .
*/
int main(int argc, char* argv[])
{
	ContextFormat format;
	format.setVersion(4, 0);
	format.setSamples(24);

	Window window;

	window.setEventHandler(new EventHandler());

	if (window.create(format, "DF Rendering Example")) {
		window.context()->setSwapInterval(Context::NoVerticalSyncronization);
		window.show();
		return MainLoop::run();
	}
	else {
		return 1;
	}
}
