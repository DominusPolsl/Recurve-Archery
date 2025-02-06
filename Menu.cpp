#include "Menu.h"
// Opis klasu Menu
Menu::Menu(float width, float height) // Parametry dla ustawienia rozmiaru Menu
{
    menuWidth = width; 
    menuHeight = height;
    vector<string> options = { "Play", "Difficulty", "Exit" }; // tablica nazw opcji menu
    font.loadFromFile("Font/sans.ttf"); 
    
    // Wgranie tła menu
    RectangleShape GamingMenu1(Vector2f(1280, 720));
    GamingMenu = GamingMenu1;
    TextureMenu.loadFromFile("Image/menu_background.png");
    GamingMenu.setTexture(&TextureMenu);
    GamingMenu.setPosition(Vector2f(0, 0));
    
    // Stworzenie objektów tekstu i wgrywanie do tablicy (dotyczy głównego menu)
    for (size_t i = 0; i < options.size(); i++) {
        Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(40);
        text.setFillColor(i == 0 ? selectedColor : normalColor);
        text.setPosition(Vector2f(width / 2 - 100, height / 3 + i * 60));
        menuOptions.push_back(text);
    }

    // Stworzenie objektów tekstu i wgrywanie do tablicy (dotyczy menu trudności)
    vector<string> difOptions = { "Easy", "Normal", "Hard" };
    for (size_t i = 0; i < difOptions.size(); i++) {
        Text text;
        text.setFont(font);
        text.setString(difOptions[i]);
        text.setCharacterSize(40);
        text.setFillColor(i == 0 ? selectedColor : normalColor);
        text.setPosition(Vector2f(menuWidth / 2 - 100, menuHeight / 3 + i * 60));
        difficultyOptions.push_back(text);
    }
    
    // Finalny tekst będzie wyświetlany po zakonczeniu gry
    Text final;
    final.setFont(font);
    final.setString("Result: ");
    final.setFillColor(Color::Red);
    final.setPosition(menuWidth / 2 - 100, menuHeight / 3);

    FinalText = final;
    selectedOption = 0;
    selectedDifOption = 0;
}
// Zmiana opcji, zmniejszenie jeśli indeks wybranej opcji większy od 0
void Menu::moveUpOptions() {
    if (selectedOption > 0) {
        menuOptions[selectedOption].setFillColor(normalColor);
        selectedOption--;
        menuOptions[selectedOption].setFillColor(selectedColor);
    }
}
// Zmiana opcji trudności
void Menu::moveUpDif() {
    if (selectedDifOption > 0) {
        difficultyOptions[selectedDifOption].setFillColor(normalColor);
        selectedDifOption--;
        difficultyOptions[selectedDifOption].setFillColor(selectedColor);
    }
}
// Ustawienie finalnego tekstu
void Menu::setFinalText(int score) {
    FinalText.setString("Result: " + to_string(score));
}
// Zmiana opcji, zwiększenie indeksu pocji jeśli indeks wybranej opcji jest mniejszy czym ilość opcji - 1;
void Menu::moveDownOptions() {
    if (selectedOption < menuOptions.size() - 1) {
        menuOptions[selectedOption].setFillColor(normalColor);
        selectedOption++;
        menuOptions[selectedOption].setFillColor(selectedColor);
    }
}
// Zmiana opcji trudności
void Menu::moveDownDif(){
    if (selectedDifOption < difficultyOptions.size() - 1) {
        difficultyOptions[selectedDifOption].setFillColor(normalColor);
        selectedDifOption++;
        difficultyOptions[selectedDifOption].setFillColor(selectedColor);
    }
}
// Otrzymania indeksu opcji menu
int Menu::getSelectedOption() {
    return selectedOption;
}
// Otrzymania indeksu opcji menu trudności
int Menu::getSelectedDifOption()
{
    return selectedDifOption;
}

// Rysowanie menu w załeżności od części menu
void Menu::draw(RenderWindow& window, int part) {
    window.draw(GamingMenu);
    if (part == 0) {
        for (auto& option : menuOptions) {
            window.draw(option);
        }
    }
    else if (part == 1) {
        for (auto& difoption : difficultyOptions) {
            window.draw(difoption);
        }
    }
    else if (part == 2) {
        window.draw(FinalText);
    }
    
}
