#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include<fstream>
using namespace std;
#include "player.h"
const char title[] = "OOP-Project, Spring-2023";
using namespace sf;
int scores = 0;
void writeScoreToFile() {
    std::ofstream file("scores.txt", std::ios::app);

    if (file.is_open()) {
        file << "Final Score: " << scores << std::endl;
        file.close();
        std::cout << "Final Score has been saved to 'scores.txt'.\n";
    }
    else {
        std::cerr << "Unable to open the file to write the score." << std::endl;
    }
}
int getHighestScoreFromFile() {
    std::ifstream file("scores.txt");

    if (file.is_open()) {
        std::vector<int> scores;
        int score;

        // Read all scores from the file
        while (file >> score) {
            scores.push_back(score);
        }

        file.close();

        // Find the highest score
        if (!scores.empty()) {
            int highestScore = *std::max_element(scores.begin(), scores.end());
            return highestScore;
        }
        else {
            std::cout << "No scores found in the file.\n";
            return 0;  // Return 0 if no scores are found
        }
    }
    else {
        std::cerr << "Unable to open the file to read scores." << std::endl;
        return 0;  // Return 0 if the file cannot be opened
    }
}
// food class (base class) 
//(polymorphism abstract class)
class Food {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void applyEffect() = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual ~Food() {}
};

// Child class of Food class
class RedFood : public Food {
private:
    sf::RectangleShape shape;
    Car& pc1;
public:
    RedFood(float x, float y, Car& p)
        : shape(sf::Vector2f(12, 12)), pc1(p) { // Decreased size to 12x12
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Red);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);

    }

    void applyEffect() override {
        // Award 10 points
        cout << "Awarded 10 points!\n";
        pc1.IncScore(10);
    }

    sf::Vector2f getPosition() const override {
        return shape.getPosition();
    }
};

// Child class of Food class
class GreenFood : public Food {
private:
    sf::CircleShape shape;
    Car& pc2;  // Reference to the Car class

public:
    GreenFood(float x, float y, Car& p)
        : shape(10), pc2(p) {
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }

    void applyEffect() override {
        // Give an extra life and award 20 points
        cout << "Extra life and awarded 20 points!\n";
        pc2.addLives(1);
        pc2.IncScore(20);
    }

    sf::Vector2f getPosition() const override {
        return shape.getPosition();
    }
};


// Child class of Food class
class OrangeFood : public Food {
private:
    sf::ConvexShape shape;
    Car& pc3;
public:
    OrangeFood(float x, float y, Car& p)
        : shape(6), pc3(p) { // Decreased size to 6 points
        // Set the points to form a hexagon
        shape.setPoint(0, sf::Vector2f(7.5, 0));
        shape.setPoint(1, sf::Vector2f(15, 5));
        shape.setPoint(2, sf::Vector2f(15, 15));
        shape.setPoint(3, sf::Vector2f(7.5, 20));
        shape.setPoint(4, sf::Vector2f(0, 15));
        shape.setPoint(5, sf::Vector2f(0, 5));

        shape.setPosition(x, y);
        shape.setFillColor(sf::Color(255, 165, 0)); // Orange color
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }

    void applyEffect() override {
        // Increase speed by 1.5x for 10 seconds and award 10 points
        pc3.IncScore(10);
        pc3.IncSpeed(1.5);
        cout << "Increased speed by 1.5x for 10 seconds and awarded 10 points!\n";
    }

    sf::Vector2f getPosition() const override {
        return shape.getPosition();
    }
};

// Child class of Food class
class WhiteFood : public Food {
private:
    sf::CircleShape shape;

public:
    WhiteFood(float x, float y)
        : shape(12) { // Decreased radius to 15
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }

    void applyEffect() override {
        // Make enemies disappear for 10 seconds
        cout << "Made enemies disappear for 10 seconds!\n";
    }

    sf::Vector2f getPosition() const override {
        return shape.getPosition();
    }
};
//game class to handle the game 
class Game {
    //  int scores;

public:
    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackgroundSprite;

    Game() : playerCar(375, 530, sf::Color::Green) {
        isPaused = false;
        // Load menu background image
        if (!menuBackgroundTexture.loadFromFile("img/CAR5.jpg")) {
            // Handle loading error
            std::cerr << "Failed to load menu background image. Using default background color." << std::endl;

            // Set a default background color (e.g., white)
            sf::Color defaultColor(255, 255, 255);  // White color
            sf::Image defaultImage;
            defaultImage.create(800, 600, defaultColor);  // Adjust the size as needed

            // Assign the default image to the texture
            menuBackgroundTexture.loadFromImage(defaultImage);
        }

        // Set up menu background sprite
        menuBackgroundSprite.setTexture(menuBackgroundTexture);



        setupOpponentCars(3);

        // Initialize the maze grid (1 represents walls, 0 represents empty space)
        maze = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0, 0, 1, 1, 1, 1, 1, 1, 1, 1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0},
            {0, 0, 1, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
            {0, 0, 1, 0, 1, 1, 1, 1, 1, 1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,0},
            {0, 0, 1, 0, 1, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0},
            {0, 0, 1, 0, 1, 0, 1, 1, 1, 1,1,0,0,0,0,1,1,1,1,1,1,0,1,0,1,0,0},
            {0, 0, 1, 0, 1, 0, 1, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
            {0, 0, 1, 0, 1, 0, 1, 0, 0, 0,0,1,1,1,1,0,0,0,0,0,1,0,1,0,1,0,0},
            {0, 0, 1, 0, 1, 0, 1, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0},
            {0, 0, 1, 0, 1, 0, 1, 1, 1, 1,1,0,0,0,0,1,1,1,1,1,1,0,1,0,1,0,0},
            {0, 0, 1, 0, 1, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0},
            {0, 0, 1, 0, 1, 1, 1, 1, 1, 1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,0},
            {0, 0, 1, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
            {0, 0, 1, 1, 1, 1, 1, 1, 1, 1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},




        };

        createRandomFoods();
        isPaused = false;
        setupMenu();


    }


    //destructor to delete food to avoid memory leakage
    ~Game() {
        for (auto food : foods) {
            delete food;
        }
    }
    // main loop to run game
    void runGameLoop(sf::RenderWindow& window) {
        while (window.isOpen()) {
            handleInput(window);
            update();
            updateOpponentCars(); // Assuming this is the function to update opponent cars
            draw(window);
        }
    }
    //function for handling imput given by user
    void handleInput(sf::RenderWindow& window) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (isPaused) {
                    handlePauseMenuKeyPress(event.key.code);
                }
                else {
                    handleGameKeyPress(event.key.code);
                }
            }
        }

        // Allow continuous movement (not just on key press)
        if (!isPaused) {
            handleContinuousMovement();
        }
    }

    // ther maze for the game is drawn by this function
    void drawMaze(sf::RenderWindow& window) {
        sf::RectangleShape wall(sf::Vector2f(29, 29));
        wall.setFillColor(sf::Color(139, 69, 19)); // Dark gray color

        for (size_t i = 0; i < maze.size(); ++i) {
            for (size_t j = 0; j < maze[i].size(); ++j) {
                if (maze[i][j] == 1) {
                    wall.setPosition(j * 30, i * 30);
                    window.draw(wall);
                }
            }
        }

        // Display score and lives on the first row
        sf::Font font;
        if (!font.loadFromFile("font/arial.ttf")) {
            cerr << "Error loading font file!\n";
            // Handle the error appropriately
            return;
        }

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setPosition(10, 5); // Adjust the position as needed
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("Score: " + to_string(playerCar.getScore()));

        sf::Text livesText;
        livesText.setFont(font);
        livesText.setCharacterSize(24);
        livesText.setPosition(700, 5); // Adjust the position as needed
        livesText.setFillColor(sf::Color::White);
        livesText.setString("Lives: " + to_string(playerCar.getLives()));

        window.draw(scoreText);
        window.draw(livesText);
    }
    //function to check whether the player car is inside canvas screen and to keep the car in canvas 
    bool isInsideCanvas(const sf::Vector2f& position) {
        // Check if the new position is inside the canvas
        return position.x >= 0 && position.y >= 0 &&
            position.x + playerCar.shape.getSize().x <= 800 &&
            position.y + playerCar.shape.getSize().y <= 600;
        {

        }
    }

    //for the moment of the user ie player car 
    void handleGameKeyPress(sf::Keyboard::Key key) {
        // Store the current position
        sf::Vector2f currentPosition = playerCar.shape.getPosition();

        // Set the new position based on the key press
        switch (key) {
        case sf::Keyboard::Left:
            currentPosition.x -= 1;
            break;
        case sf::Keyboard::Right:
            currentPosition.x += 1;
            break;
        case sf::Keyboard::Up:
            currentPosition.y -= 1;
            break;
        case sf::Keyboard::Down:
            currentPosition.y += 1;
            break;
        case sf::Keyboard::P:
            // Pause the game
            isPaused = true;
            return;  // Add this line to return after setting isPaused to true
        default:
            break;
        }

        // Check for collision with maze walls
        checkCollisionWithWalls(currentPosition);
    }
    //for continous movement of player car
    void handleContinuousMovement() {
        // Store the current position
        sf::Vector2f currentPosition = playerCar.shape.getPosition();

        // Allow continuous movement (not just on key press)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            currentPosition.x -= 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            currentPosition.x += 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            currentPosition.y -= 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            currentPosition.y += 1;
        }

        // Check for collision with maze walls
        checkCollisionWithWalls(currentPosition);
    }
    //to keep the player car on track and collide it with the walls of the maze
    void checkCollisionWithWalls(sf::Vector2f& position) {
        // Check if the new position is inside the canvas
        if (isInsideCanvas(position)) {
            // Check for collision with maze walls
            int cellX = static_cast<int>((position.x + playerCar.shape.getSize().x / 2) / 30);
            int cellY = static_cast<int>((position.y + playerCar.shape.getSize().y / 2) / 30);

            // Check if the cell contains a wall (1 in the maze array)
            if (maze[cellY][cellX] == 0) {
                // If not a wall, update the position
                playerCar.shape.setPosition(position);
            }
        }
        {

        }
    }


    // function to update game
    void update() {
        if (!isPaused) {
            playerCar.move();
            updateOpponentCars();
            sf::Vector2f currentPosition = playerCar.shape.getPosition();

            handleContinuousMovement();

            checkCollisionWithWalls(currentPosition);

            // Check and handle the transition when the score is 100
            handleScore100Transition();
        }
    }


    void handleScore100Transition() {
        if (playerCar.getScore() >= 200) {
            // Clear the screen
            sf::RenderWindow clearScreen(sf::VideoMode(800, 600), "Clear Screen", sf::Style::None);

            // Display a message or perform any other visual effects
            sf::Font font;
            if (!font.loadFromFile("font/arial.ttf")) {
                cerr << "Error loading font file!\n";
                // Handle the error appropriately
                return;
            }

            sf::Text levelText;
            levelText.setFont(font);
            levelText.setCharacterSize(48);
            levelText.setFillColor(sf::Color::White);
            levelText.setString("GameEnd");

            // Center the text on the screen
            sf::FloatRect textBounds = levelText.getLocalBounds();
            levelText.setPosition((800 - textBounds.width) / 2, (600 - textBounds.height) / 2);

            // Display the message on the clear screen
            while (clearScreen.isOpen()) {
                sf::Event event;
                while (clearScreen.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        clearScreen.close();
                    }
                    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                        clearScreen.close();
                    }
                }

                clearScreen.clear();
                clearScreen.draw(levelText);
                clearScreen.display();
            }

            // Clear the screen again after Level 2 message
            clearScreen.create(sf::VideoMode(800, 600), "Clear Screen", sf::Style::None);

            // Setup Level 2 only after the user presses Enter

        }
    }


    sf::Texture backgroundImage;
    sf::Sprite backgroundSprite;



    // function to draw on the game screen
    void draw(sf::RenderWindow& window) {
        window.clear();

        drawMaze(window); // Draw the maze first


        for (auto food : foods) {
            food->draw(window);
        }
        playerCar.draw(window);

        for (auto& car : opponentCars) {
            car.draw(window);
        }
        // Draw the background image
        window.draw(backgroundSprite);

        // Draw the menu background
        if (isPaused) {
            window.draw(menuBackgroundSprite);
            drawPauseMenu(window);
        }

        window.display();
    }

    void drawOpponentCars(sf::RenderWindow& window) {
        // Draw opponent cars
        for (auto& car : opponentCars) {
            car.draw(window);
        }
    }
    //the function so that the player can eat the food
    void handleCollisionWithPlayer(sf::Vector2f playerPosition) {
        for (auto it = foods.begin(); it != foods.end(); /* no increment here */) {
            if (isCollision(playerPosition, (*it)->getPosition())) {
                (*it)->applyEffect();
                delete (*it);
                it = foods.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void setupOpponentCars(int count) {
        for (int i = 0; i < count; ++i) {
            // Randomly spawn opponent cars within the screen
            float x = rand() % 800;
            float y = rand() % 600;
            opponentCars.emplace_back(x, y);
        }
    }

    void updateOpponentCars() {
        // Update positions of opponent cars
        for (auto& car : opponentCars) {
            if (!isPaused) {
                car.update(playerCar.shape.getPosition(), maze);
            }
        }
    }


    void handleFoodCollection() {
        sf::Vector2f playerPosition = playerCar.shape.getPosition();

        // Iterate through the food items
        for (auto it = foods.begin(); it != foods.end(); /* no increment here */) {
            if (isCollision(playerPosition, (*it)->getPosition())) {
                // Handle the effect of collecting the food
                (*it)->applyEffect();

                // Delete the collected food and remove it from the vector
                delete (*it);
                it = foods.erase(it);
            }
            else {
                ++it;
            }
        }
    }
    // function to check collision
    bool isCollision(sf::Vector2f pos1, sf::Vector2f pos2) {
        // Simple bounding box collision check
        return pos1.x < pos2.x + 30 &&
            pos1.x + 30 > pos2.x &&
            pos1.y < pos2.y + 30 &&
            pos1.y + 30 > pos2.y;
        {

        }
    }
    // to check collision between the player and the opponent car
    bool checkCollisionBetweenCars() {
        sf::Vector2f playerPosition = playerCar.shape.getPosition();

        for (const auto& opponentCar : opponentCars) {
            sf::Vector2f opponentPosition = opponentCar.shape.getPosition();

            if (isCollision(playerPosition, opponentPosition)) {
                // Collision detected between playerCar and opponentCar
                handleCollisionBetweenCars();
                return true;
            }
        }

        return false; // No collision
    }

    void handleCollisionBetweenCars() {
        // Handle the collision as needed
        cout << "Collision between player car and opponent car!\n";
        // For example, you might decrease player's health or take some other action
    }

    void GameLoop(sf::RenderWindow& window) {


        while (window.isOpen()) {
            handleInput(window);
            update();
            handleFoodCollection();
            updateOpponentCars(); // Add this line to update opponent cars
            if (!isPaused) {
                checkCollisionBetweenCars();
            }
            draw(window);
        }
    }
private:
    vector<vector<int>> maze;  // 2D array representing the maze
    Car playerCar;
    vector<OpponentCar> opponentCars;
    vector<Food*> foods;

    bool isPaused;
    // Function to create random foods in empty places (excluding first two rows)
    void createRandomFoods() {
        //Food for 1st line
        createRandomFood(100, 120);
        createRandomFood(200, 120);
        createRandomFood(280, 120);
        createRandomFood(500, 120);
        createRandomFood(600, 120);
        createRandomFood(700, 120);

        //for 1st and 2nd column
        createRandomFood(100, 170);
        createRandomFood(100, 220);
        createRandomFood(160, 220);

        //Food for 2nd line 
        createRandomFood(200, 180);
        createRandomFood(280, 180);
        createRandomFood(500, 180);
        createRandomFood(600, 180);

        //Food for 3rd line
        createRandomFood(200, 420);
        createRandomFood(280, 420);
        createRandomFood(500, 420);
        createRandomFood(600, 420);

        //Food for 4th line
        createRandomFood(100, 480);
        createRandomFood(200, 480);
        createRandomFood(280, 480);
        createRandomFood(500, 480);
        createRandomFood(600, 480);
        createRandomFood(700, 480);

    }

    void createRandomFood(int x, int y) {
        int type = rand() % 4;

        switch (type) {
        case 0:
            foods.push_back(new RedFood(x, y, playerCar));
            break;
        case 1:
            foods.push_back(new GreenFood(x, y, playerCar));
            break;
        case 2:
            foods.push_back(new OrangeFood(x, y, playerCar));
            break;
        case 3:
            foods.push_back(new WhiteFood(x, y));
            break;
        }
    }



    void drawGrid(sf::RenderWindow& window) {
        sf::RectangleShape wall(sf::Vector2f(20, 20));
        wall.setFillColor(sf::Color(105, 105, 105));  // Dark gray color

        for (size_t i = 0; i < maze.size(); ++i) {
            for (size_t j = 0; j < maze[i].size(); ++j) {
                if (maze[i][j] == 1) {
                    wall.setPosition(j * 21, i * 21);
                    window.draw(wall);
                }
            }
        }
        {

        }
    }

    sf::Font font;
    sf::Text pauseMenuText;
    int selectedMenuItem;

    void setupMenu() {
        if (!font.loadFromFile("font/arial.ttf")) {
            cerr << "Error loading font file!\n";
            // Handle the error appropriately
        }

        pauseMenuText.setFont(font);
        pauseMenuText.setCharacterSize(24);
        pauseMenuText.setPosition(250, 200);
        pauseMenuText.setFillColor(sf::Color::White);

        selectedMenuItem = 1; // Default selection is "Start a new game"
        updatePauseMenuText();
    }

    // to handle pause menu
    void handlePauseMenuKeyPress(sf::Keyboard::Key key) {
        switch (key) {
        case sf::Keyboard::Up:
            selectedMenuItem = (selectedMenuItem - 1 + 5) % 5; // Wrap around the menu
            updatePauseMenuText();
            break;
        case sf::Keyboard::Down:
            selectedMenuItem = (selectedMenuItem + 1) % 5; // Wrap around the menu
            updatePauseMenuText();
            break;
        case sf::Keyboard::Enter:
            handlePauseMenuSelection();
            break;
        default:
            break;
        }
    }
    // update to show the function is pause menu
    void updatePauseMenuText() {
        switch (selectedMenuItem) {
        case 1:
            pauseMenuText.setString("1) Start a new game");
            break;
        case 2:
            pauseMenuText.setString("2) See high-scores");
            break;
        case 3:
            pauseMenuText.setString("3) Help");
            break;
        case 4:
            pauseMenuText.setString("4) Exit");
            break;
        case 0:
            pauseMenuText.setString("5) Continue");
            break;
        default:
            break;
        }
    }
    // todraw pause menu
    void drawPauseMenu(sf::RenderWindow& window) {
        sf::RectangleShape background(sf::Vector2f(300, 200));
        background.setPosition(200, 150);
        background.setFillColor(sf::Color(0, 0, 0, 200)); // Semi-transparent black

        window.draw(background);
        window.draw(pauseMenuText);
        {

        }
    }
    // reset game so that thew player car come to the starting position when new gsame is pressed 
    void resetGame() {
        // Reset relevant game state variables
        playerCar.reset(375, 530); // Set the initial position as needed
        opponentCars.clear();
        setupOpponentCars(3); // Set the initial number of opponent cars
        createRandomFoods(); // Reset food items
    }

    void handlePauseMenuSelection() {

        switch (selectedMenuItem) {
        case 1:
            // Start a new game
            resetGame();
            isPaused = false;

            break;
        case 2:
            // See high-scores
            int highestScore;
            cout << "Displaying high-scores...\n";
            highestScore = getHighestScoreFromFile();
            cout << highestScore << endl;
            break;
        case 3:
            // Help
            displayHelp();
            break;
        case 4:
            // Exit
            cout << "Exiting the game. Goodbye!\n";
            cout << scores << endl;
            writeScoreToFile();
            exit(0);
            break;
        case 0:
            // Continue
            isPaused = false;
            break;
        default:
            break;
        }
    }

    void displayHelp() {
        // Display help information
        cout << "Help:\n";
        cout << "Use arrow keys to drive the player car.\n";
        cout << "Press 'P' to pause the game and see the menu.\n";
        cout << "Press '5' to continue during the paused state.\n";
        cout << "Press any key to return to the main menu.\n";
        cin.ignore(); // Wait for user input before returning to the menu
    }


};