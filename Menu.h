#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;
using namespace std;
class Menu
{
private:
    Font font;
    Text FinalText;
    Texture TextureMenu;
    RectangleShape GamingMenu;
    vector<Text> menuOptions;
    vector<Text> difficultyOptions;
    vector<Text> afterGame;
    int selectedOption;
    int selectedDifOption;
    Color normalColor = Color::White;
    Color selectedColor = Color::Red;
    float menuWidth = 0;
    float menuHeight = 0;

public:
    Menu(float, float);
    ~Menu() {};

    void draw(RenderWindow& window, int part);
    void moveUpOptions();
    void moveUpDif();
    void moveDownOptions();
    void moveDownDif();
    void setFinalText(int);
    int getSelectedOption();
    int getSelectedDifOption();
};

