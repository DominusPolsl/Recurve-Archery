#include "Wind.h"
// Opis klasu Wind

Wind::Wind(float posX, float posY, int mode) // Parametry ustawienia rozmiaru i prametr mode od kt?rego za?e?y poziom trudno?ci gry
{	
	srand(time(NULL)); // ustawinia seed dla randomowych liczb w za?e?nosci od czasu

	difficulty = mode;

	// Wgrywania zdj?cia strza?ek, kt?re r??ni? obrotem na k?t 30 stopni
	TextureWindArrow.loadFromFile("Image/wind_arrow_sprite.png");
	WindArrowObject.setTexture(TextureWindArrow);
	WindArrowObject.setTextureRect(IntRect(0, 0, 200, 200));
	WindArrowObject.setPosition(posX, posY);
}

Wind::~Wind()
{
}

// zmiana odrysowanej cz??ci zdj?cia ze strza?ami
void Wind::changeWindArrow()
{
	WindArrowObject.setTextureRect(IntRect(0, WindArrowNum * 200, 200, 200));
}

// zmiana si?y wiatru
void Wind::windChangePower()
{
	switch (difficulty) {
		case 0:
			WindPower = (rand() % 10) + 1;
			break;
		case 1:
			WindPower = (rand() % 20) + 1;
			break;
		case 2:
			WindPower = (rand() % 30) + 100;
			break;
	}
}

// Randomowa zmiana zwrotu strza?y, czym wy?ej poziom trudno?ci tym wi?cej opciji dla zwrotu strza?y
void Wind::windChangeDirection()
{
	int randomIndex{};
	vector<int> WindArrows;
	switch (difficulty) {
	case 0:
		break;
	case 1:
		WindArrows = { 0,3,6,9 };
		randomIndex = rand() % 4;
		WindArrowNum = WindArrows[randomIndex];
		break;
	case 2:
		WindArrows = { 0,1,2,3,4,5,6,7,8,9,10,11 };
		randomIndex = rand() % 12;
		WindArrowNum = WindArrows[randomIndex];
		break;
	}
	
}

void Wind::setDifficulty(int x)
{
	difficulty = x;
	switch (difficulty) {
	case 0: 
		ChangeSpeed = 8;
		break;
	case 1:
		ChangeSpeed = 4;
	case 2:
		ChangeSpeed = 2;
	}
}

int Wind::getChangeSpeed()
{
	return ChangeSpeed;
}

float Wind::getWindPower()
{
	return WindPower;
}

Vector2f Wind::AdjustCoordinates(Vector2f pos)
{
	float x = 0, y = 0;
	float radians = (WindArrowNum * 30) * 3.14159 / 180;
	x = round((pos.x + cos(radians) * WindPower));
	y = round((pos.y - sin(radians) * WindPower));

	return Vector2f(x, y);
}

void Wind::draw(RenderWindow& window)
{	
	window.draw(WindArrowObject);
}



