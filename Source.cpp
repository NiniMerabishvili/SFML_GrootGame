#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

enum class GameState { Menu, Description, SpriteSelection, Gameplay, GameOver };

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void showDescription();
    void showSpriteSelection();
    void startGame();

    bool checkCollision(const Sprite& sprite1, const Sprite& sprite2);

    RenderWindow window;
    Texture menuBackgroundTexture;
    Sprite menuBackground;
    Font font;
    Text startGameText;
    Text descriptionButton;
    Text descriptionText;
    Text spriteSelectionText;
    Texture spriteSelectionBackgroundTexture;
    Sprite spriteSelectionBackground;
    Texture sprite1Texture;
    Sprite sprite1;
    Texture sprite2Texture;
    Sprite sprite2;
    Texture sprite3Texture;
    Sprite sprite3;
    Texture gameBackgroundTexture;
    Sprite gameBackground;
    Texture greenAppleTexture;
    Sprite greenApple;
    Texture redAppleTexture;
    Sprite redApple;
    Texture blackAppleTexture;
    Sprite blackApple;
    Texture grootTexture;
    Sprite grootSprite;
    int selectedSprite;
    int score;
    int lives;
    Text scoreText;
    Text gameOverText;
    int previousScore;
    bool gameOver;
    GameState gameState;
    const float movementSpeed = 200.0f;
    float appleSpeed = 200.0f;
    float appleScale = 0.1f;
    Clock clock;
};

Game::Game()
{
    window.create(VideoMode::getDesktopMode(), "Groot Game", Style::Fullscreen);

    // Load menu background
    if (!menuBackgroundTexture.loadFromFile("bg.jpg")) {
        cerr << "Failed to load menu background texture" << endl;
    }
    menuBackground.setTexture(menuBackgroundTexture);

    // Load font
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Failed to load font" << endl;
    }

    // Set up start game text
    startGameText.setFont(font);
    startGameText.setString("Start Game");
    startGameText.setCharacterSize(48);
    startGameText.setPosition(window.getSize().x / 2 - 148, window.getSize().y / 2 - 70);

    // Set up description text
    descriptionText.setFont(font);
    descriptionText.setString("I am Groot :)\n\nGeneral Rules:\nClick on Right arrow (>) to move right\nClick on left arrow (<) to go left.\nTry to collect green and red apples, but not apple with warm.\nIf you eat red apple, 1 point is yours\nGreen apple - 2 points\nApple with warm - I die :(\n\nPress esc to go back to the menu");
    descriptionText.setCharacterSize(24);
    descriptionText.setPosition(window.getSize().x / 2 - 270, window.getSize().y / 2 - 170);

    descriptionButton.setFont(font);
    descriptionButton.setCharacterSize(48);
    descriptionButton.setPosition(window.getSize().x / 2 - 148, window.getSize().y / 2 - 20);
    descriptionButton.setString("Description");

    // Set up sprite selection text
    spriteSelectionText.setFont(font);
    spriteSelectionText.setString("Choose a Sprite");
    spriteSelectionText.setCharacterSize(40);
    spriteSelectionText.setPosition(window.getSize().x / 2 - 148, window.getSize().y / 2 + 30);

    // Load sprite selection background
    if (!spriteSelectionBackgroundTexture.loadFromFile("bg.jpg")) {
        cerr << "Failed to load sprite selection background texture" << endl;
    }
    spriteSelectionBackground.setTexture(spriteSelectionBackgroundTexture);

    // Load sprite textures
    if (!sprite1Texture.loadFromFile("sprite1.png")) {
        cerr << "Failed to load sprite1 texture" << endl;
    }
    sprite1.setTexture(sprite1Texture);

    if (!sprite2Texture.loadFromFile("sprite2.png")) {
        cerr << "Failed to load sprite2 texture" << endl;
    }
    sprite2.setTexture(sprite2Texture);

    if (!sprite3Texture.loadFromFile("sprite3.png")) {
        cerr << "Failed to load sprite3 texture" << endl;
    }
    sprite3.setTexture(sprite3Texture);

    // Load game background
    if (!gameBackgroundTexture.loadFromFile("bg.jpg")) {
        cerr << "Failed to load game background texture" << endl;
    }
    gameBackground.setTexture(gameBackgroundTexture);

    // Load apple textures
    if (!greenAppleTexture.loadFromFile("green_apple.png")) {
        cerr << "Failed to load green apple texture" << endl;
    }
    greenApple.setTexture(greenAppleTexture);
    greenApple.setScale(appleScale, appleScale);

    if (!redAppleTexture.loadFromFile("red_apple.png")) {
        cerr << "Failed to load red apple texture" << endl;
    }
    redApple.setTexture(redAppleTexture);
    redApple.setScale(appleScale, appleScale);

    if (!blackAppleTexture.loadFromFile("black_apple.png")) {
        cerr << "Failed to load black apple texture" << endl;
    }
    blackApple.setTexture(blackAppleTexture);
    blackApple.setScale(appleScale, appleScale);

    // Load Groot texture
    if (!grootTexture.loadFromFile("groot_handup.png")) {
        cerr << "Failed to load Groot texture" << endl;
    }
    grootSprite.setTexture(grootTexture);
    grootSprite.setPosition(400, 470); // Initial position of Groot
    grootSprite.setScale(0.5f, 0.5f);

    // Set up score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setPosition(10, 10);

    selectedSprite = 0;
    score = 0;
    lives = 3;
    gameOver = false;
    gameState = GameState::Menu;
    previousScore = score;

    // Set up game over text
    gameOverText.setFont(font);
    gameOverText.setString("Game Over\nYour Score is: " + to_string(score));
    gameOverText.setCharacterSize(48);
    gameOverText.setPosition(window.getSize().x / 2 - 130, window.getSize().y / 2 - 70);
}

void Game::run()
{
    while (window.isOpen()) {
        processEvents();
        update(clock.restart().asSeconds());
        render();
    }
}

void Game::processEvents()
{
    Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
        case Event::Closed:
            window.close();
            break;
        case Event::KeyPressed:
            if (event.key.code == Keyboard::Escape) {
                if (gameState == GameState::Description || gameState == GameState::SpriteSelection || gameState == GameState::GameOver) {
                    gameState = GameState::Menu;
                }
                else if (gameState == GameState::Menu) {
                    window.close();
                }
            }
            break;
        case Event::MouseButtonPressed:
            if (event.mouseButton.button == Mouse::Left) {
                if (gameState == GameState::Menu) {
                    Vector2i mousePosition = Mouse::getPosition(window);
                    FloatRect startGameBounds = startGameText.getGlobalBounds();
                    FloatRect descriptionBounds = descriptionButton.getGlobalBounds();
                    if (startGameBounds.contains(mousePosition.x, mousePosition.y)) {
                        gameState = GameState::Gameplay;
                    }
                    else if (descriptionBounds.contains(mousePosition.x, mousePosition.y)) {
                        gameState = GameState::Description;
                    }
                }
                else if (gameState == GameState::Description) {
                    gameState = GameState::Menu;
                }
                else if (gameState == GameState::SpriteSelection) {
                    Vector2i mousePosition = Mouse::getPosition(window);
                    FloatRect sprite1Bounds = sprite1.getGlobalBounds();
                    FloatRect sprite2Bounds = sprite2.getGlobalBounds();
                    FloatRect sprite3Bounds = sprite3.getGlobalBounds();
                    if (sprite1Bounds.contains(mousePosition.x, mousePosition.y)) {
                        selectedSprite = 1;
                        gameState = GameState::Gameplay;
                    }
                    else if (sprite2Bounds.contains(mousePosition.x, mousePosition.y)) {
                        selectedSprite = 2;
                        gameState = GameState::Gameplay;
                    }
                    else if (sprite3Bounds.contains(mousePosition.x, mousePosition.y)) {
                        selectedSprite = 3;
                        gameState = GameState::Gameplay;
                    }
                }
            }
            break;
        }
    }
}

void Game::update(float deltaTime)
{
    if (gameState == GameState::Gameplay) {
        Vector2f movement(0.0f, 0.0f);
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            movement.x += movementSpeed * deltaTime;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            movement.x -= movementSpeed * deltaTime;
        }
        // Move the apples down
        greenApple.move(0.0f, appleSpeed * deltaTime);
        redApple.move(0.0f, appleSpeed * deltaTime);
        blackApple.move(0.0f, appleSpeed * deltaTime);
        scoreText.setString("Score: " + to_string(score) + "  Lives: " + to_string(lives));
        grootSprite.move(movement);

        // Check for collisions between apples and avoid overlapping
        if (checkCollision(greenApple, redApple) || checkCollision(greenApple, blackApple)) {
            greenApple.setPosition(Vector2f(rand() % window.getSize().x, 0));
        }
        if (checkCollision(redApple, greenApple) || checkCollision(redApple, blackApple)) {
            redApple.setPosition(Vector2f(rand() % window.getSize().x, 0));
        }
        if (checkCollision(blackApple, greenApple) || checkCollision(blackApple, redApple)) {
            blackApple.setPosition(Vector2f(rand() % window.getSize().x, 0));
        }

        if (greenApple.getPosition().y >= window.getSize().y) {
            greenApple.setPosition(Vector2f(rand() % window.getSize().x, 0));
        }
        if (redApple.getPosition().y >= window.getSize().y) {
            redApple.setPosition(Vector2f(rand() % window.getSize().x, 0));
        }
        if (blackApple.getPosition().y >= window.getSize().y) {
            blackApple.setPosition(Vector2f(rand() % window.getSize().x, 0));
        }
        // Check collision with apples
        if (checkCollision(grootSprite, greenApple)) {
            score += 2;
            greenApple.setPosition(Vector2f(rand() % window.getSize().x, 0));
        }
        else if (checkCollision(grootSprite, redApple)) {
            score += 1;
            redApple.setPosition(Vector2f(rand() % window.getSize().x, 0));
        }
        else if (checkCollision(grootSprite, blackApple)) {
            lives--;
            if (lives <= 0) {
                gameOver = true;
                gameState = GameState::GameOver;
            }
            blackApple.setPosition(Vector2f(rand() % window.getSize().x, 0));
        }
        // Check if the player has lost the game
        if (lives <= 0) {
            gameOver = true;
            gameState = GameState::GameOver;
            gameOverText.setString("Game Over\nYour Score is: " + to_string(score)); // Update game over text with the final score
        }
        scoreText.setString("Score: " + to_string(score) + "  Lives: " + to_string(lives));
    }
}

void Game::render()
{
    window.clear();

    if (gameState == GameState::Menu) {
        window.draw(menuBackground);
        window.draw(startGameText);
        window.draw(descriptionButton);
    }
    else if (gameState == GameState::Description) {
        window.draw(menuBackground);
        window.draw(descriptionText);
    }
    else if (gameState == GameState::SpriteSelection) {
        window.draw(spriteSelectionBackground);
        window.draw(spriteSelectionText);
        window.draw(sprite1);
        window.draw(sprite2);
        window.draw(sprite3);
    }
    else if (gameState == GameState::Gameplay) {
        window.draw(gameBackground);
        window.draw(grootSprite);
        window.draw(greenApple);
        window.draw(redApple);
        window.draw(blackApple);
        window.draw(scoreText);
    }
    else if (gameState == GameState::GameOver) {
        window.draw(menuBackground);
        window.draw(gameOverText);
        window.draw(scoreText);
    }

    window.display();
}

void Game::showDescription()
{
    gameState = GameState::Description;
}

void Game::showSpriteSelection()
{
    gameState = GameState::SpriteSelection;
}

void Game::startGame()
{
    gameState = GameState::Gameplay;
}

bool Game::checkCollision(const Sprite& sprite1, const Sprite& sprite2)
{
    return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
}

int main()
{
    Game game;
    game.run();

    return 0;
}
