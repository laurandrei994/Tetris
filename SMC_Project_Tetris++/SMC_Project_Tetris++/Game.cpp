#include "Game.h"
#include "Utils.h"
#include "Menu.h"
#include <SFML/Window/Event.hpp>
#include <iostream>

Game::Game()
	: m_RenderWindow{ sf::VideoMode{20 * 18 + 100, 18 * 18}, "TETRIS++", sf::Style::Default }, m_Texture{}, m_SeparationLine{}, m_TetrisShape{ nullptr }, m_Preview{ nullptr }, m_Board{}, m_Score{}, m_ElapsedTime{ sf::Time::Zero }, m_ID{ Utils::GetRandomNumber(7) }
{
	m_SeparationLine.setSize(sf::Vector2f{ 1.f, 18.f * 18.f });
	m_SeparationLine.setPosition(sf::Vector2f{ 20.f * 18.f, 0 });
	m_SeparationLine.setFillColor(sf::Color::Red);
	if (!m_Texture.loadFromFile("Blocks.png"))
		std::cout << "Could not load texture from file !! \n";
	m_Board = std::make_unique<Board>(Position{ 20,18 }, *this);
	CreateShape();
}

void Game::Run()
{
	sf::Clock clock; // starting the timer-ul
	sf::Time deltaTime(sf::Time::Zero);

	m_Board->Clean();

	while (m_RenderWindow.isOpen())
	{
		
		sf::Time trigger(sf::seconds(85.f / (85.f + (m_Score.GetLevel() * (m_Score.GetLevel() * 5.f))))); // la inceput este = 1;
		std::cout << "Trigger =" << trigger.asMilliseconds() << std::endl;
		deltaTime = clock.restart(); // restarting the timer and returning the time passed until this point
		m_ElapsedTime += deltaTime;
		std::cout << "m_ElapsedTime = " << m_ElapsedTime.asMilliseconds() << std::endl << std::endl;

		ProcessEvents();
		Update(deltaTime);

		if (m_ElapsedTime > trigger)
		{
			m_ElapsedTime = sf::Time::Zero;
			Proceed(Direction::Down);
			std::cout << "Should go Down" << std::endl;
		}
		Render();
	}
}

void Game::Proceed(Direction direction)
{
	if (!m_TetrisShape)
		return;

	if (IsValidMovement(m_TetrisShape->GetFutureBlockPosition(direction)))
	{
		m_TetrisShape->Move(direction);
		if (direction == Direction::UserPressedDown)
			m_Score.AddPressedScore(1);
	}
	else
	{
		if (direction == Direction::Down || direction == Direction::UserPressedDown)
		{
			int id = m_TetrisShape->GetID();
			m_Board->AddBlock(id, m_TetrisShape->GetBlockPosition());
			m_TetrisShape.reset(nullptr);
			m_Score.SumPressedScore();
		}
	}
}

void Game::Update(const sf::Time& dt)
{
	m_Board->Update(dt);
	m_Score.Update(dt);
	if (!m_TetrisShape)
	{
		if (m_Board->IsToRemoveBlocks())
			return;
		CreateShape();
	}
}


void Game::Rotate()
{
	if (!m_TetrisShape)
		return;

	m_TetrisShape->Rotate();
	if (!IsValidMovement(m_TetrisShape->GetBlockPosition()))
		m_TetrisShape->RevertState();
}

void Game::ScaleUp()
{
	if (!m_TetrisShape)
		return;

	m_TetrisShape->ScaleUp();
	if (!IsValidMovement(m_TetrisShape->GetBlockPosition()))
	{
		m_TetrisShape->RevertState();
	}
}

void Game::ScaleDown()
{
	if (!m_TetrisShape)
		return;

	m_TetrisShape->ScaleDown();
	if (!IsValidMovement(m_TetrisShape->GetBlockPosition()))
	{
		m_TetrisShape->RevertState();
	}
}

void Game::CreateShape()
{
	m_TetrisShape.reset(new TetrisShape(m_Texture, m_ID));
	//create new game if necessary
	if (m_Board->IsOccupied(m_TetrisShape->GetBlockPosition()))
	{
		m_Board->Clean();
	}
	m_ID = Utils::GetRandomNumber(7);
	m_Preview.reset(new TetrisShape(m_Texture, m_ID));
	m_Preview->SetPosition(sf::Vector2i{ 20,12 });
}

bool Game::IsValidMovement(std::array<Position, 16> block)
{
	for (int i = 0; i < 16; i++)
	{
		if (block[i].x < 0 || block[i].x > 19 || block[i].y > 17)
		{
			std::cout << "INVALID" << std::endl;
			return false;
		}
		if (IsOccupied(block[i].x, block[i].y))
		{
			std::cout << "INVALID" << std::endl;
			return false;
		}
	}
	std::cout << "VALID" << std::endl;
	return true;
}

bool Game::IsOccupied(int x, int y)
{
	return m_Board->GetField(x, y)->m_Occupied;
}

void Game::ProcessEvents()
{
	sf::Event e;
	while (m_RenderWindow.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			m_RenderWindow.close();
		else if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::Down)
				Proceed(Direction::Down);
			else if (e.key.code == sf::Keyboard::Left)
				Proceed(Direction::Left);
			else if (e.key.code == sf::Keyboard::Right)
				Proceed(Direction::Right);
			else if (e.key.code == sf::Keyboard::Space)
				Rotate();
			else if (e.key.code == sf::Keyboard::A)
				ScaleUp();
			else if (e.key.code == sf::Keyboard::Z)
				ScaleDown();
		}
	}
}

void Game::Render()
{
	m_RenderWindow.clear(sf::Color::Black);
	m_Score.Draw(m_RenderWindow);
	m_Board->Draw(m_RenderWindow);
	if (m_TetrisShape)
		m_RenderWindow.draw(*m_TetrisShape);
	m_RenderWindow.draw(*m_Preview);
	m_RenderWindow.draw(m_SeparationLine);
	m_RenderWindow.display();
}