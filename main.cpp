#include "worklibgame.h"
#include <iostream>
#include <windows.h>

// Definicja stanów gry
enum GameState { MENU, GAME };
GameState state = MENU; // Początkowy stan gry to MENU

/**
 * Funkcja resetująca stan gry do wartości początkowych.
 * @param arrows - liczba strzał dostępnych w grze
 * @param score - wynik gracza
 * @param arrowText - tekst wyświetlający ilość strzał
 * @param scoreText - tekst wyświetlający aktualny wynik
 * @param lastArrowText - tekst wyświetlający wynik ostatniego strzału
 * @param wind - obiekt symulujący wiatr
 * @param WindText - tekst wyświetlający siłę wiatru
 * @param isStringPulled - flaga określająca, czy cięciwa jest naciągnięta
 * @param arrowSprite - obiekt reprezentujący strzałę
 * @param Frame - aktualna klatka animacji strzały
 * @param animStepFly - krok animacji lotu strzały
 * @param animateArrow - flaga określająca, czy animacja lotu strzały jest aktywna
 * @param clockGame, clockAnimFly, clockPull, clockTarget, clockWind - zegary gry
 */
void resetGame(int& arrows, int& score, Text& arrowText, Text& scoreText, Text& lastArrowText, Wind& wind, Text& WindText,
    bool& isStringPulled, Sprite& arrowSprite, int& Frame, int& animStepFly, bool& animateArrow, Clock& clockGame,
    Clock& clockAnimFly, Clock& clockPull, Clock& clockTarget, Clock& clockWind)
{
    arrows = 18;
    score = 0;
    arrowText.setString("18/18 arrows");
    scoreText.setString("0/180 points");
    lastArrowText.setString("Last Arrow: -");

    // Resetowanie wiatru
    wind.windChangeDirection();
    wind.windChangePower();
    int currentPower = int(wind.getWindPower());
    wind.changeWindArrow();
    WindText.setString(to_string(currentPower / 10) + "," + to_string(currentPower % 10) + " m/s");

    // Resetowanie animacji i logiki strzału
    isStringPulled = false;
    arrowSprite.setTextureRect(IntRect(0, 0, 60, 104));
    Frame = 0;
    animStepFly = 1;
    animateArrow = false;

    // Restart zegarów gry
    clockGame.restart();
    clockAnimFly.restart();
    clockPull.restart();
    clockTarget.restart();
    clockWind.restart();
}

int main()
{
    RenderWindow win(VideoMode(1280, 720), "Archery"); // Tworzenie głównego okna gry o rozdzielczości 1280x720 pikseli

    // Wczytywanie i ustawianie ikony gry
    Image icon; 
    if (!icon.loadFromFile("Image/recurve_archery_s.png")) { 
        return 3; // Jeśli nie uda się załadować pliku, zwróć błąd
    }

    win.setIcon(32, 32, icon.getPixelsPtr()); // Ustawienie ikony w lewym górnym rogu okna gry


    // Wczytywanie tekstur tła gry
    Texture textureField;
    textureField.loadFromFile("Image/field_v2.png");
    RectangleShape gamingBackground(Vector2f(1280, 720)); // Prostokąt o rozmiarze okna gry
    gamingBackground.setTexture(&textureField);
    gamingBackground.setPosition(Vector2f(0, 0)); // Pozycja tła


    // Wczytywanie tekstury chmur
    Texture textureSky;
    textureSky.loadFromFile("Image/sky_v2.png");
    RectangleShape gamingBackgroundSky(Vector2f(1280, 173));
    gamingBackgroundSky.setTexture(&textureSky);
    gamingBackgroundSky.setPosition(Vector2f(0, 0));

    // Druga warstwa chmur (dla efektu przewijania)
    RectangleShape gamingBackgroundSky2(Vector2f(1280, 173));
    gamingBackgroundSky2.setTexture(&textureSky);
    gamingBackgroundSky2.setPosition(Vector2f(1280, 0));

    // Wczytywanie tekstury nieba bez chmur
    Texture textureSkyNoClouds;
    textureSkyNoClouds.loadFromFile("Image/sky_no_clouds.png");
    RectangleShape gamingBackgroundSkyNoClouds(Vector2f(1280, 173));
    gamingBackgroundSkyNoClouds.setTexture(&textureSkyNoClouds);
    gamingBackgroundSkyNoClouds.setPosition(Vector2f(0, 0));


    // Tworzenie obiektu tarczy (mniejsza tarcza położona bliżej środka ekranu)
    Texture textureTarget1;
    textureTarget1.loadFromFile("Image/archery_target.png");
    RectangleShape gamingTarget1(Vector2f(150, 203));
    gamingTarget1.setTexture(&textureTarget1);
    gamingTarget1.setPosition(Vector2f(565, 235));

    // Tworzenie panelu wyników (półprzezroczysty prostokąt w lewym górnym rogu)
    RectangleShape scorePanel(Vector2f(200, 120)); 
    scorePanel.setFillColor(Color(0, 0, 0, 150)); // Polprzezroczyste tlo
    scorePanel.setPosition(20, 20); 
    
    // Wczytywanie czcionki
    Font font;
    font.loadFromFile("Font/sans.ttf");
  
    // Tekst wyświetlający wynik gracza
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(Color::White);
    scoreText.setString("0/180 points");
    scoreText.setPosition(40, 30); // Pozycja w panelu

    // Tekst wyświetlający liczbę strzał
    Text arrowText;
    arrowText.setFont(font);
    arrowText.setCharacterSize(18);
    arrowText.setFillColor(Color::White);
    arrowText.setString("18/18 arrows");
    arrowText.setPosition(40, 50);

    // Tekst wyświetlający wynik ostatniego strzału
    Text lastArrowText;
    lastArrowText.setFont(font);
    lastArrowText.setCharacterSize(18);
    lastArrowText.setFillColor(Color::White);
    lastArrowText.setString("Last Arrow: -");
    lastArrowText.setPosition(40, 70);
    
    // Tekst pozostałego czasu 
    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(30);
    timerText.setFillColor(Color::White);
    timerText.setString("72 s");
    timerText.setPosition(40, 90); // Pozycja na ekranie

    // Tekst wyświetlający siłę i kierunek wiatru
    Text WindText;
    WindText.setFont(font);
    WindText.setCharacterSize(25);
    WindText.setFillColor(Color::Red);
    WindText.setString("0 m/s");
    WindText.setPosition(50, 660); // Pozycja w panelu

    
    Clock clock; // Zegar dla ruchu chmur
    Clock clockAnimFly; // Zegar dla animacji lotu strzały
    Clock clockPull; // Zegar dla naciągania cięciwy
    Clock clockTarget; // Zegar dla ruchu tarczy dużej
    Clock clockGame; // Zegar dla całej gry
    Clock clockWind; // Zegar dla zmianny kierunku i siły wartości
    float time, timeSky; // zmienne dla ruchu chmur odnośnie czasu

    // Cełownik
    const float smoothingFactor = 0.0008; // jak wolno będzie celownik zbliżać do kursora myszy
    Vector2f aimPosition(640, 360); // Początkowa pozycja celownika

    // Ruch po kołu
    float radius = 8.0f; // radius koła po którym się porusza cełownik
    float angle = 0.0f;   // kąt w radianach
    const float angularSpeed = 0.002; // kątowa szybkość 
    
    // Wgrawanie tekstury celownika
    Texture textureAim;
    textureAim.loadFromFile("Image/bow_aim_1.png");
    RectangleShape aim(Vector2f(50, 50));
    aim.setTexture(&textureAim);
    // Ustawinie początku koordynat celownika w innym miejscu
    aim.setOrigin(textureAim.getSize().x / 2, textureAim.getSize().y / 2);
    //----------------------------------------------------------------

    // Wgrywanie tekstury strzały
    Texture arrowTexture;
    arrowTexture.loadFromFile("Image/arrow_sprite.png");
    Sprite arrowSprite;
    arrowSprite.setTexture(arrowTexture);
    arrowSprite.setTextureRect(IntRect(0, 0, 60, 104));
    int arrAnimFly[4]{ 0, 130, 250, 385 }; // tablica y-ów zdjęcia ze spritami strały, zmienna takiego y-ka powoduję zmianne kadru
    int Frame = 0, animStepFly = 1;
    bool animateArrow = false; // Czy animacja lotu strzały jest aktywna?

    // Łuk
    bool isStringPulled = false; // Czy cięciwa jest naciągnięta? 

    // Mała Tarcza
    // współrzędzne środka małej tarczcy;
    const int SMALL_TARGET_CENTER_X = 637; 
    const int SMALL_TARGET_CENTER_Y = 303;
    Target target;
    Vector2i smallTargetCenter(SMALL_TARGET_CENTER_X, SMALL_TARGET_CENTER_Y);

    // Duża tarcza
    bool animateTarget = true; // Czy animacja pojawienia dużej tarczy aktywna?

    // Menu
    Menu menu(1280, 720);

    // Wiatr
    Wind wind(0, 450, 0);
    int currentPower{};
        
    int menuPart = 0; // część menu
    int arrows = 18; // początkowa ilość strzał
    int score = 0; // początkowa ilość punktów
    float timerSec{};

    while (win.isOpen())
    {
        Event event;


        while (win.pollEvent(event))
        {
            if (event.type == Event::Closed)
                win.close();
            // Eventy jakie gra zczytuje kiedy użytkownik jest w menu
            if (state == MENU) {
                // Odczyt naciśniętych klawisz
                if (event.type == Event::KeyPressed) {
                    // Po nasciśnieniu Escape ekran menu się zmienie, wszystkie parametry gry zresetują
                    if (event.key.code == Keyboard::Escape) {
                        menuPart = 0;
                        resetGame(arrows, score, arrowText, scoreText, lastArrowText, wind, WindText,
                            isStringPulled, arrowSprite, Frame, animStepFly, animateArrow,
                            clockGame, clockAnimFly, clockPull, clockTarget, clockWind);
                    }
                    // Ruch po menu
                    if (event.key.code == Keyboard::Up) 
                        if (menuPart == 0) {
                            menu.moveUpOptions();
                        }
                        else if (menuPart == 1) {
                            menu.moveUpDif();
                        }
                        
                    if (event.key.code == Keyboard::Down)
                        if (menuPart == 0) {
                            menu.moveDownOptions();
                        }
                        else if (menuPart == 1) {
                            menu.moveDownDif();
                        }
                    // Enter: Play(option 0) - początek gry, Difficulty(option 1) - zmiana wyświetlanego kadru menu, Exit(option 2) - gra się zamyka
                    if (event.key.code == Keyboard::Enter) {
                        if (menuPart == 0) {
                            int option = menu.getSelectedOption();
                            if (option == 0) {
                                resetGame(arrows, score, arrowText, scoreText, lastArrowText, wind, WindText,
                                    isStringPulled, arrowSprite, Frame, animStepFly, animateArrow,
                                    clockGame, clockAnimFly, clockPull, clockTarget, clockWind);
                                state = GAME; // Begin game
                            }
                            else if (option == 1) {
                                menuPart = 1;
                            }
                            else if (option == 2) {
                                win.close(); // Exit From game
                            }
                        }
                        // Przy wyświetlanym kadrze menu "menuPart 1) można zmieniać trudność
                        else if (menuPart == 1) {
                            int difoption = menu.getSelectedDifOption();
                            if (difoption == 0) {
                                wind.setDifficulty(0); // ustawienie trudności wiatru 
                                menuPart = 0;
                            }
                            else if (difoption == 1) {
                                wind.setDifficulty(1);
                                menuPart = 0;
                            }
                            else if (difoption == 2) {
                                wind.setDifficulty(2);
                                menuPart = 0;
                            }
                        }
                    }
                }
            } 
            // Eventy które program odczytuje kiedy jest rozpoczęta gra
            else if (state == GAME) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Escape) {
                        state = MENU;
                        menuPart = 0;
                        resetGame(arrows, score, arrowText, scoreText, lastArrowText, wind, WindText,
                            isStringPulled, arrowSprite, Frame, animStepFly, animateArrow,
                            clockGame, clockAnimFly, clockPull, clockTarget, clockWind);
                    }
                }
                // Co się będzie działać kiedy LKM jest nacisnięta
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && animateArrow == false)
                {
                    isStringPulled = true;
                    animateTarget = true;
                    clockPull.restart();
                }
                // Co się będzie działać kiedy LKM jest zwolniona i również cięciwa jest naciągnięta
                if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left && isStringPulled) {
                    // Jeśli minęnło więcej czym jesdna sekunda po naciśnieniu LKM
                    if (clockPull.getElapsedTime() > seconds(1)) {
                        Vector2f hitPosition = wind.AdjustCoordinates(aimPosition);

                        // Otrzymanie informacji o pozycji trafienia strzały w konsol
                        OutputDebugStringA(to_string(hitPosition.x).c_str());
                        OutputDebugStringA("\n");
                        OutputDebugStringA(to_string(hitPosition.y).c_str());
                        OutputDebugStringA("\n");

                        int currentRing = target.detectRing(hitPosition);
                        int flyAdjust = 4 * (10 - currentRing); // ustawienia koficjenta, który się zmienia w załeżności od trafionych punktów i
                        // służy dla zmianny pozycji animacji wylotu strzały
                        score += currentRing; // powiększania ogólnej ilości punktów na ilość trafionych
                        arrows -= 1; // zmniejszenie ogólnej ilości strzał

                        // zmianna wyświetlanego tekstu
                        scoreText.setString(to_string(score) + "/180 points"); 
                        arrowText.setString(to_string(arrows) + "/18 arrows");
                        lastArrowText.setString("Last Arrow: " + to_string(currentRing));

                        animateArrow = true;

                        // W załeżności od trafionego pierścnia, ustawia się pozycja animacji wylotu strzały
                        // W tym celu zrobione rozbicie tarczy na cztery częsci (jak w kartezjańskim układzie współrzędnych)
                        // Czym mniejsze punktów trafił użytkownik tym dałej od środka będzie leciała strała, lecz po toru funkcji y = x(ćwiartki nieparzyste) 
                        // lub y = -x(ćwiartki parzyste)
                        if (currentRing == 10) {
                            arrowSprite.setPosition(smallTargetCenter.x - 34, smallTargetCenter.y + 10);
                        }
                        else if ((hitPosition.x - target.targetCenter.x > 0) && (hitPosition.y - target.targetCenter.y < 0)) {
                            arrowSprite.setPosition(smallTargetCenter.x - 34 + flyAdjust, smallTargetCenter.y + 10 - flyAdjust);
                        }
                        else if ((hitPosition.x - target.targetCenter.x < 0) && (hitPosition.y - target.targetCenter.y < 0)) {
                            arrowSprite.setPosition(smallTargetCenter.x - 34 - flyAdjust, smallTargetCenter.y + 10 - flyAdjust);
                        }
                        else if ((hitPosition.x - target.targetCenter.x < 0) && (hitPosition.y - target.targetCenter.y > 0)) {
                            arrowSprite.setPosition(smallTargetCenter.x - 34 - flyAdjust, smallTargetCenter.y + 10 + flyAdjust);
                        }
                        else if ((hitPosition.x - target.targetCenter.x > 0) && (hitPosition.y - target.targetCenter.y > 0)) {
                            arrowSprite.setPosition(smallTargetCenter.x - 34 + flyAdjust, smallTargetCenter.y + 10 + flyAdjust);
                        }

                    }
                    isStringPulled = false;
                    target.restart();
                }
            }
            
        }
        // Przypadek, kiedy stan gry jest Menu
        if (state == MENU) {
            win.setMouseCursorVisible(true); // widoczny kursor myszi
            win.clear();
            menu.draw(win, menuPart); 
            win.display();
        }
        // Przypadek, kiedy stan gry jest Game
        else if (state == GAME) {
            win.setMouseCursorVisible(false); // kursor myszi nie jest widoczny
            Vector2i mousePosition = Mouse::getPosition(win); // otrzymania pozycji myszi
            timerSec = 72 - clockGame.getElapsedTime().asSeconds(); // zmienna do odliczania od 72 sekund
            timerText.setString(to_string((int)timerSec) + "s"); // Ustawienie tekstu timera
            time = clock.getElapsedTime().asMicroseconds(); // zapisywania czasu clock w zmienną dla przemieszczenia chmur
            timeSky = time / 6000;
            clock.restart();
            
            // Warunek sprawdzenia czy gra jest skończona
            if (timerSec <= 0 || arrows == 0) {
                state = MENU;
                menuPart = 2;
                menu.setFinalText(score);
                resetGame(arrows, score, arrowText, scoreText, lastArrowText, wind, WindText,
                    isStringPulled, arrowSprite, Frame, animStepFly, animateArrow,
                    clockGame, clockAnimFly, clockPull, clockTarget, clockWind);
            }
            
            // funkcja dla przemiszczenia kursora
            aimMovement(aim, aimPosition, mousePosition, angle, radius, angularSpeed, smoothingFactor);

            // Warunek sprawdzenia czy cięciwa naciągnięta i czy minęła wkazana iłość czasu żeby spowolnić animacje pojawienia tarzy
            if (isStringPulled && clockTarget.getElapsedTime() > milliseconds(100)) {
                clockTarget.restart();
                target.animation();
            }
            // Jeśli minęła 1 sek, to zatrzymać animację tarczy
            if (clockPull.getElapsedTime() > seconds(1)) animateTarget = false;
            
            // Jeśli minęła ilość czasu uzałeżniona od poziomu trudności, to program wykonuje zmiannę wiatru
            if (clockWind.getElapsedTime() > seconds(wind.getChangeSpeed())) {

                wind.windChangeDirection();
                wind.windChangePower();
                currentPower = int(wind.getWindPower());
                wind.changeWindArrow();
                WindText.setString(to_string(currentPower / 10) + "," + to_string(currentPower % 10) + " m/s");
                clockWind.restart();
            }

            // Jeśli minęło 200 mlsek i strzałe mogę być animowana, to w załeności od kadru animacji będzie pojawiać zdjęcie w oknie gry
            if (clockAnimFly.getElapsedTime() > milliseconds(200) && animateArrow) {
                clockAnimFly.restart();
                Vector2f arrowPos = arrowSprite.getPosition();
                if (Frame == 1) arrowSprite.setPosition(arrowPos.x, arrowPos.y - 70);
                if (Frame == 2) arrowSprite.setPosition(arrowPos.x, arrowPos.y - 40);
                if (Frame == 3) arrowSprite.setPosition(arrowPos.x, arrowPos.y + 35);

                arrowSprite.setTextureRect(IntRect(0, arrAnimFly[Frame], 60, 104));
                Frame += animStepFly;

                if (Frame == 5) {
                    Frame = 0;
                    animateArrow = false;
                }
            }
            // Przemiszczenie chmur
            gamingBackgroundSky.move(-0.1f * timeSky, 0);
            gamingBackgroundSky2.move(-0.1f * timeSky, 0);

            // Ta część kodu zapewnia, że chmury będą ciągłe i cyklicznie przemieszczać w oknie gry
            if (gamingBackgroundSky.getPosition().x <= -1280) gamingBackgroundSky.setPosition(1280, 0);
            if (gamingBackgroundSky2.getPosition().x <= -1280) gamingBackgroundSky2.setPosition(1280, 0);

            // Oczyszczenie przy
            win.clear();
            win.draw(gamingBackgroundSkyNoClouds);
            win.draw(gamingBackgroundSky2);
            win.draw(gamingBackgroundSky);
            win.draw(gamingBackground);
            win.draw(gamingTarget1);
            if (isStringPulled && animateTarget) target.draw(win);
            if (target.getAnimationStatus()) {
                target.complete();
                target.draw(win);
            }
            if (animateArrow) win.draw(arrowSprite);
            if (isStringPulled)  win.draw(aim);;
            win.draw(scorePanel);
            win.draw(scoreText);
            win.draw(arrowText);
            win.draw(timerText);
            win.draw(lastArrowText);
            win.draw(WindText);
            wind.draw(win);

            win.display();
        }
        
    }

    return 0;
}
