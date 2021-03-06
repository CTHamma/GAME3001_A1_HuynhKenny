#include "SpaceShip.h"


#include "Game.h"
#include "Util.h"

SpaceShip::SpaceShip()
{
	TextureManager::Instance()->load("../Assets/textures/spaceship.png", "spaceship");

	auto size = TextureManager::Instance()->getTextureSize("spaceship");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setEnabled(false);
	setType(SPACE_SHIP);
	setMaxSpeed(10.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.0f);
	setAccelerationRate(10.0f);
	setTurnRate(10.0f);
}

SpaceShip::~SpaceShip()
= default;

void SpaceShip::draw()
{
	TextureManager::Instance()->draw("spaceship", 
		getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	//Util::DrawLine(getTransform()->position, (getTransform()->position + getOrientation() * 60.0f) );
	Util::DrawLine(m_leftWhisker.Start(), m_leftWhisker.End());
	Util::DrawLine(m_centreWhisker.Start(), m_centreWhisker.End());
	Util::DrawLine(m_rightWhisker.Start(), m_rightWhisker.End());
}

void SpaceShip::update()
{
	m_leftWhisker.SetLine(getTransform()->position,
		(getTransform()->position + Util::getOrientation(m_rotationAngle - 30) * 100.0f));

	m_centreWhisker.SetLine(getTransform()->position,
		(getTransform()->position + getOrientation() * 150.0f));

	m_rightWhisker.SetLine(getTransform()->position,
		(getTransform()->position + Util::getOrientation(m_rotationAngle + 30) * 100.0f));

	m_Move();
}

void SpaceShip::clean()
{
}

void SpaceShip::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

glm::vec2 SpaceShip::getDestination() const
{
	return m_destination;
}

void SpaceShip::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

glm::vec2 SpaceShip::getOrientation() const
{
	return m_orientation;
}

float SpaceShip::getTurnRate() const
{
	return m_turnRate;
}

void SpaceShip::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

float SpaceShip::getAccelerationRate() const
{
	return m_accelerationRate;
}

void SpaceShip::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void SpaceShip::setWhisker(glm::vec2 start, glm::vec2 end)
{
	m_leftWhisker.SetLine(start, end);
	m_centreWhisker.SetLine(start, end);
	m_rightWhisker.SetLine(start, end);
}

void SpaceShip::setChase(bool behaviour)
{
	m_chase = behaviour;
}

bool SpaceShip::getChase()
{
	return false;
}

void SpaceShip::setIsNear(bool behaviour)
{
	m_isNear = behaviour;
}

bool SpaceShip::getIsNear()
{
	return false;
}

void SpaceShip::setOrientation(const glm::vec2 orientation)
{
	m_orientation = orientation;
}

void SpaceShip::setDirection(glm::vec2 direction)
{
	m_targetDirection = direction;
}

glm::vec2 SpaceShip::getDirection() const
{
	return m_targetDirection;
}

void SpaceShip::setRotation(const float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float SpaceShip::getRotation() const
{
	return m_rotationAngle;
}

void SpaceShip::m_Move()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	// direction with magnitude
	if (m_chase == false)
	{
		m_targetDirection = getTransform()->position - m_destination;
	}
	else { m_targetDirection = m_destination - getTransform()->position; }
	
	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

	auto turn_sensitivity = 5.0f;

	if(abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			setRotation(getRotation() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setRotation(getRotation() - getTurnRate());
		}
	}
	
	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;

	auto target_destination = sqrt((m_destination.x - getTransform()->position.x) * (m_destination.x - getTransform()->position.x) + (m_destination.y - getTransform()->position.y) * (m_destination.y - getTransform()->position.y));

	// Arrival method
	if (m_isNear == true)
	{
		if (target_destination < 300.0f)
		{
			m_maxSpeed = 5.0f;
		}

		if (target_destination < 100.0f)
		{
			m_maxSpeed = 2.0f;
		}

		if (target_destination < 20.0f)
		{
			m_maxSpeed = 0.0f;
		}
	}
}
