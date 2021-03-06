#pragma once
#include "TetrisShape.h"
#include "Board.h"
#include "IGame.h"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/Music.hpp>

class Game : public IGame
{
public:
	using Position = sf::Vector2i;

public:
	Game();
	void Run(bool&, uint16_t&);

private:
	void Proceed(Direction);
	void Update(const sf::Time&);
	void Rotate();
	void ScaleUp();
	void ScaleDown();
	void AddSpecialShape();
	void CreateShape();
	bool IsValidMovement(std::array<Position, BLOCK_ARRAY_COLUMNS>);
	bool IsOccupied(const uint16_t, const uint16_t);
	void ProcessEvents(bool&, uint16_t& levelSound);
	void Render();
	void FileWriter(const std::string& file);

private:
};

