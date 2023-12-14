#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include "game.h"

class Menu {
public:
    Sprite background; //Game background sprite
    Texture bg_texture;

    Menu() {
        // Constructor body
        if (!font.loadFromFile("font/arial.ttf")) {
            cerr << "Error loading font file!\n";
        }


        // Set up buttons
        setupButtons();
    }

    void display_menu() {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Game Menu");
        Game g;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed) {
                    handle_key_press(event.key.code, g);
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
                    handle_mouse_click(event.mouseButton.x, event.mouseButton.y, g);
                }

                // Update button states
                updateButtons(event, window);
            }

            window.clear(sf::Color::Red); // Set background color
            window.clear(); // Remove the red background color

            // Draw the background image from the Game class
            window.draw(g.menuBackgroundSprite);

            // Draw the buttons
            drawButtons(window);

            
             // Draw the buttons
            drawButtons(window);

            window.display();
        }
        {

        }

    }

private:
    class Button {
    public:
        Button() : buttonText("", font, 16) {
            // Default constructor with an empty string for text
        }

        Button(const string& text, float x, float y, const sf::Font& font)
            : buttonText(text, font, 16) {
            buttonRect.setSize(sf::Vector2f(200, 30));
            buttonRect.setPosition(x, y);

            buttonText.setPosition(x + 50, y + 5); // Adjust the position to center the text
        }

        void draw(sf::RenderWindow& window) {
            buttonRect.setFillColor(sf::Color(200, 200, 200)); // Light gray color
            buttonText.setFillColor(sf::Color::Black); // Black text color

            window.draw(buttonRect);
            window.draw(buttonText);
            {

            }
        }

        void update(sf::Event& event, sf::RenderWindow& window) {
            sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
            if (buttonRect.getGlobalBounds().contains(mousePos)) {
                // Change appearance when hovered
                buttonRect.setOutlineThickness(2);
                buttonRect.setOutlineColor(sf::Color::Black);
            }
            else {
                // Reset appearance when not hovered
                buttonRect.setOutlineThickness(0);
            }
        }

        bool isClicked(float mouseX, float mouseY) const {
            return buttonRect.getGlobalBounds().contains(mouseX, mouseY);
        }

    private:
        sf::RectangleShape buttonRect;
        sf::Text buttonText;
        sf::Font font;
    };

    sf::Font font; // Add a font member
    Button startButton, highScoresButton, helpButton, exitButton;

    void setupButtons() {
        // Set up buttons with positions and font
        startButton = Button("Start a new game", 50, 100, font);
        highScoresButton = Button("See high-scores", 50, 150, font);
        helpButton = Button("Help", 50, 200, font);
        exitButton = Button("Exit", 50, 250, font);
        {

        }
    }

    void drawButtons(sf::RenderWindow& window) {
        // Draw the buttons
        startButton.draw(window);
        highScoresButton.draw(window);
        helpButton.draw(window);
        exitButton.draw(window);
    }

    void updateButtons(sf::Event& event, sf::RenderWindow& window) {
        // Update button states
        startButton.update(event, window);
        highScoresButton.update(event, window);
        helpButton.update(event, window);
        exitButton.update(event, window);
    }

    void handle_key_press(sf::Keyboard::Key key, Game& g) {
        switch (key) {
        case sf::Keyboard::Num1:

            break;

        case sf::Keyboard::Num2:
            // Implement high-scores functionality
            cout << "Displaying high-scores...\n";
            break;

        case sf::Keyboard::Num3:
            display_help();
            break;

        case sf::Keyboard::Num4:
            cout << "Exiting the game. Goodbye!\n";
            exit(0);
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    void handle_mouse_click(float mouseX, float mouseY, Game& g) {
        if (startButton.isClicked(mouseX, mouseY)) {
            // Start the game loop
            sf::RenderWindow window(sf::VideoMode(800, 600), "Dodge 'Em");

            Game game;

            game.GameLoop(window);

        }
        else if (highScoresButton.isClicked(mouseX, mouseY)) {
            cout << "Displaying high-scores...\n";
        }
        else if (helpButton.isClicked(mouseX, mouseY)) {
            display_help();
        }
        else if (exitButton.isClicked(mouseX, mouseY)) {
            cout << "Exiting the game. Goodbye!\n";
            exit(0);
        }
    }


    void display_help() {
        // Display help information
        cout << "Help:\n";
        cout << "Use arrow keys to drive the player car.\n";
        cout << "Press 'P' to pause the game and see the menu.\n";
        cout << "Press '5' to continue during the paused state.\n";
        cout << "Press any key to return to the main menu.\n";
        cin.ignore(); // Wait for user input before returning to the menu
    }
};