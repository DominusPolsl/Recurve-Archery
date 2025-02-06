#include "Target.h"
#include <vector>

Vector2i Target::targetCenter(640, 300); // Ustawienie środka tarczi

Target::Target()
{	
	// Ustawienie rozmieszczenia tarczy
	location_x = 470;
	location_y = 130;
	// Wgrywanie tarczy
	TextureTarget.loadFromFile("Image/target_face_sprite.png");
	TargetObject.setTexture(TextureTarget);
	TargetObject.setTextureRect(IntRect(0, 0, 340, 340));
	TargetObject.setPosition(Vector2f(location_x, location_y));

	// tabela wewnętrznych i zewnętrznych promieni pierśćniów
	rings = { {0,13}, {13,33}, {33,50}, {50,67}, {67,85}, {85,102}, {102,120}, {120,136}, {136,153}, {153,170} };
}

Target::~Target()
{
}

// zmiana dziedziny zdjęcia, która będzie odrysowana
void Target::animation() 
{
	if (Frame < 4) {
		TargetObject.setTextureRect(IntRect(0, arrY[Frame], 340, 340));
		Frame++;
	}
}

// Ustawnienie statycznej dzidziny odrysowania zdjęcia
void Target::complete()
{
	TargetObject.setTextureRect(IntRect(0, 1023, 340, 340));
}

// Funkcja, która wyznacza czy punkt się nakłada z jakimś z pierścniów
int Target::detectRing(Vector2f hitPosition)
{
	for (int i = 0; i < 10; i++) {
		if (rings[i].checkRingIntersection(hitPosition)) return 10 - i;
	}
	return 0;
}

// Funkcja getter, z pomocą której się wyznacza czy animacja jest skończona czy aktywna
bool Target::getAnimationStatus()
{
	if (Frame == 4) return true;
	return false;
}


// Odrysowanie tarczy
void Target::draw(RenderWindow& window)
{
	window.draw(TargetObject);
}

// Resetowanie animacji
void Target::restart()
{
	TargetObject.setTextureRect(IntRect(0, 0, 340, 340));
	Frame = 0;
}
