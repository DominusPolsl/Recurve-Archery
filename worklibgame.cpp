#include "worklibgame.h"

// Przemieszczenie celownika 
void aimMovement(RectangleShape& aim, Vector2f& aimPosition, Vector2i& mousePosition, float& angle, float radius, float angularSpeed, float smoothingFactor) {
	Vector2f targetPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)); // Otrzymywanie pozycji kursora
	Vector2f pos;

	float offsetX = radius * cos(angle); // odchylenia po wsp??rz?dnej x od ?rodka ko?a z zadanem radiusem
	float offsetY = radius * sin(angle); // odchyhlenia po wsp??rz?dnej y od ?rodka ko?a z zadanem radiusem
	angle += angularSpeed;

	// Zbli?enie celownika do kursora myszi poprzez wymno?enia od?eg?o?ci na smoothingFactor;
	// Skutkuje to efektem zatrzymywania ruchu kursora
	aimPosition.x += (targetPosition.x - aimPosition.x) * smoothingFactor;
	aimPosition.y += (targetPosition.y - aimPosition.y) * smoothingFactor;
	
	// Aktualizowanie pozycji celownika
	aim.setPosition(aimPosition.x + offsetX, aimPosition.y + offsetY);
	pos = aim.getPosition();

	// Warunki, ?eby ce?ownik pozostawa? w oknie i nie wychodzi? za granicy
	if (pos.x > 1255) aim.setPosition(1255, pos.y);
	if (pos.x < 25) aim.setPosition(25, pos.y);
	if (pos.y > 695) aim.setPosition(pos.x, 695);
	if (pos.y < 25) aim.setPosition(pos.x, 25);

	if ((pos.x > 1255) && (pos.y < 25)) aim.setPosition(1255, 25);
	if ((pos.x < 25) && (pos.y < 25)) aim.setPosition(25, 25);
	if ((pos.x < 25) && (pos.y > 695)) aim.setPosition(25, 695);
	if ((pos.x > 1255) && (pos.y > 695)) aim.setPosition(1255, 695);
}


