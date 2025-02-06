#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>
#include <vector>
using namespace sf;
using namespace std;
class Wind
{
public:
	Wind(float, float, int); // constructor
	~Wind(); // destructor

	void changeWindArrow();
	void windChangePower();
	void windChangeDirection();
	void setDifficulty(int);
	int getChangeSpeed();
	float getWindPower();
	void draw(RenderWindow& window);
	Vector2f AdjustCoordinates(Vector2f pos);

private:

	Sprite WindArrowObject;
	Texture TextureWindArrow;

	int difficulty{}; // mode: 0 - easy, 1 - normal, 2 - hard;
	int WindArrowNum = 0;
	float WindPower = 0;
	int ChangeSpeed = 1;
};

