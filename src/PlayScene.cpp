#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "SoundManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();

	// Background Music
	SoundManager::Instance().load("../Assets/audio/The Inconceptual One.mp3", "BGM", SOUND_MUSIC);
	SoundManager::Instance().playMusic("BGM", -1, 0);
	SoundManager::Instance().setMusicVolume(32);
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 0);
}

void PlayScene::update()
{
	updateDisplayList();

	CollisionManager::AABBCheck(m_pSpaceShip, m_pObstacle);
	CollisionManager::AABBCheck(m_pSpaceShip, m_pTarget);

	if (CollisionManager::lineRectCheck(m_pSpaceShip->m_leftWhisker.Start(), m_pSpaceShip->m_leftWhisker.End(),
		(m_pObstacle->getTransform()->position - glm::vec2(25.0f, 12.5f)), 300.0f, 250.0f))
	{ 
		m_pSpaceShip->getRigidBody()->velocity.y += 0.2f;
		m_pSpaceShip->setRotation(135);
	}

	if (CollisionManager::lineRectCheck(m_pSpaceShip->m_centreWhisker.Start(), m_pSpaceShip->m_centreWhisker.End(),
		(m_pObstacle->getTransform()->position - glm::vec2(25.0f, 12.5f)), 300.0f, 250.0f))
	{
		m_pSpaceShip->setMaxSpeed(2.0f);
	}

	if (CollisionManager::lineRectCheck(m_pSpaceShip->m_rightWhisker.Start(), m_pSpaceShip->m_rightWhisker.End(),
		(m_pObstacle->getTransform()->position - glm::vec2(25.0f, 12.5f)), 300.0f, 250.0f))
	{ 
		m_pSpaceShip->getRigidBody()->velocity.y -= 0.2f;
		m_pSpaceShip->setRotation(45);
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// Escape
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();

		SoundManager::Instance().stopMusic(0);
	}

	// Back to Start
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_LEFT))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);

		SoundManager::Instance().stopMusic(0);
	}

	// Go to End
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_RIGHT))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);

		SoundManager::Instance().stopMusic(0);
	}

	// Clear Screen
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_0))
	{
		m_pTarget->setEnabled(false);
		m_pObstacle->setEnabled(false);
		m_pObstacle->getTransform()->position = glm::vec2(-1000.0f, -1000.0f);
		m_pSpaceShip->setEnabled(false);
		m_pSpaceShip->setIsNear(false);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f);
	}

	// Seek
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		m_pTarget->setEnabled(true);
		m_pTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);

		m_pObstacle->setEnabled(false);
		m_pObstacle->getTransform()->position = glm::vec2(-1000.0f, -1000.0f);

		m_pSpaceShip->setChase(true);
		m_pSpaceShip->setEnabled(true);
		m_pSpaceShip->setIsNear(false);
		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 300.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pSpaceShip->setMaxSpeed(10.0f);
	}

	// Flee
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		m_pTarget->setEnabled(true);
		m_pTarget->getTransform()->position = glm::vec2(400.0f, 350.0f);

		m_pObstacle->setEnabled(false);
		m_pObstacle->getTransform()->position = glm::vec2(-1000.0f, -1000.0f);

		m_pSpaceShip->setChase(false);
		m_pSpaceShip->setEnabled(true);
		m_pSpaceShip->setIsNear(false);
		m_pSpaceShip->getTransform()->position = glm::vec2(450.0f, 400.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pSpaceShip->setMaxSpeed(10.0f);
	}

	// Arrive
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3))
	{
		m_pTarget->setEnabled(true);
		m_pTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);

		m_pObstacle->setEnabled(false);
		m_pObstacle->getTransform()->position = glm::vec2(-1000.0f, -1000.0f);

		m_pSpaceShip->setChase(true);
		m_pSpaceShip->setEnabled(true);
		m_pSpaceShip->setIsNear(true);
		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 300.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pSpaceShip->setMaxSpeed(10.0f);
	}

	// Collsion Avoidance
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_4))
	{
		m_pTarget->setEnabled(true);
		m_pTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);

		m_pObstacle->setEnabled(true);
		m_pObstacle->getTransform()->position = glm::vec2(300.0f, 250.0f);

		m_pSpaceShip->setChase(true);
		m_pSpaceShip->setEnabled(true);
		m_pSpaceShip->setIsNear(false);
		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 300.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pSpaceShip->setMaxSpeed(10.0f);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pTarget = new Target();
	addChild(m_pTarget);

	m_pObstacle = new Obstacle();
	addChild(m_pObstacle);

	// instantiating spaceship
	m_pSpaceShip = new SpaceShip();
	addChild(m_pSpaceShip);

	// Instructions display on screen for user input
	const SDL_Color white = { 255, 255, 255, 255 };
	m_p0Label = new Label("Press 0 to Refresh and Clear All Behaviour", "Consolas", 25, white, glm::vec2(400.0f, 25.0f));
	m_p0Label->setParent(this);
	addChild(m_p0Label);

	m_p1Label = new Label("Press 1 to Test Seeking Behaviour", "Consolas", 25, white, glm::vec2(400.0f, 50.0f));
	m_p1Label->setParent(this);
	addChild(m_p1Label);

	m_p2Label = new Label("Press 2 to Test Fleeing Behaviour", "Consolas", 25, white, glm::vec2(400.0f, 75.0f));
	m_p2Label->setParent(this);
	addChild(m_p2Label);

	m_p3Label = new Label("Press 3 to Test Arrival Behaviour", "Consolas", 25, white, glm::vec2(400.0f, 100.0f));
	m_p3Label->setParent(this);
	addChild(m_p3Label);

	m_p4Label = new Label("Press 4 to Test Obstacle Avoidance", "Consolas", 25, white, glm::vec2(400.0f, 125.0f));
	m_p4Label->setParent(this);
	addChild(m_p4Label);

	m_pLeftArrow = new Label("<-- START", "Consolas", 20, white, glm::vec2(60.0f, 580.0f));
	m_pLeftArrow->setParent(this);
	addChild(m_pLeftArrow);

	m_pRightArrow = new Label("END -->", "Consolas", 20, white, glm::vec2(750.0f, 580.0f));
	m_pRightArrow->setParent(this);
	addChild(m_pRightArrow);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Lab 2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float speed = 10.0f;
	if(ImGui::SliderFloat("MaxSpeed", &speed, 0.0f, 100.0f))
	{
		m_pSpaceShip->setMaxSpeed(speed);
	}

	static float acceleration_rate = 2.0f;
	if(ImGui::SliderFloat("Acceleration Rate", &acceleration_rate, 0.0f, 50.0f))
	{
		m_pSpaceShip->setAccelerationRate(acceleration_rate);
	}

	static float angleInRadians = m_pSpaceShip->getRotation();
	if(ImGui::SliderAngle("Orientation Angle", &angleInRadians))
	{
		m_pSpaceShip->setRotation(angleInRadians * Util::Rad2Deg);
	}

	static float turn_rate = 5.0f;
	if(ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pSpaceShip->setTurnRate(turn_rate);
	}
	
	if(ImGui::Button("Start"))
	{
		m_pSpaceShip->setEnabled(true);
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pSpaceShip->setEnabled(false);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set Angle to 0 degrees
		turn_rate = 5.0f;
		acceleration_rate = 2.0f;
		speed = 10.0f;
		angleInRadians = m_pSpaceShip->getRotation();
	}

	ImGui::Separator();

	static float targetPosition[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("Target", targetPosition, 0.0f, 800.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(targetPosition[0], targetPosition[1]);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
