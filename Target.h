#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
using namespace sf;
using namespace std;

class Target
{
public:
	Target(); // constructor
	~Target(); // destructor

	void draw(RenderWindow& window);
	void restart();
	void animation();
	void complete();
	int detectRing(Vector2f);
	bool getAnimationStatus();
	static Vector2i targetCenter;

	// struktura pierścnia, zawiera zew. i wew. promieni i metodę, która sprawdza czy punkt jest w obszarze piersćnia
	struct targetRing {
		int smallerRadius;
		int biggerRadius;
		bool checkRingIntersection(Vector2f position) {
			position.x -= targetCenter.x;
			position.y -= targetCenter.y;
			if ((pow(position.x, 2) + pow(position.y, 2) > pow(smallerRadius, 2)) && (pow(position.x, 2) + pow(position.y, 2) < pow(biggerRadius, 2))) {
				return true;
			}
			return false;
		}
	};
	vector<targetRing> rings = {}; // tablica pierścniów
	int location_x, location_y; 
	
private:

	Sprite TargetObject;
	Texture TextureTarget;

	int arrY[4] = { 0, 341, 682, 1023 }; // tablica pozycji y-ów, te elementy sługują dla wyznaczenia obszeru odrycowania części zdjęcia
	int Frame = 0;
};

