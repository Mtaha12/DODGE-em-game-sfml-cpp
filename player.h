#include <SFML/Graphics.hpp>
#include<string.h>
#include <time.h>
#include<random>
#include <cstdlib> 
#include <ctime>   
using namespace sf;
using namespace std;
// class class (paarent class) 
class Car {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float Speed;
    int score;
    int lives;
    //default constructor
    Car()
    {
    }
    //Getter for Scores
    int getScore() const {
        return score;
    }
    //Getter for lives
    int getLives() const {
        return lives;
    }
    //Function to Increment Lives After eating Food
    void addLives(int value) {
        lives += value;
    }
    // Function to increase the score
    void IncScore(int value) {
        score += value;
    }
    void setSpeed(float incS) {
        Speed = incS;
    }

    void IncSpeed(float I) {
        Speed = Speed * I;
    }




    //parametrized constructor
    Car(float x, float y, const sf::Color& color)
        : shape(sf::Vector2f(29, 29)), velocity(0, 0) {
        shape.setPosition(x, y);
        shape.setFillColor(color);
    }

    void move() {
        shape.move(velocity);
    }
    void reset(float x, float y) {
        shape.setPosition(x, y);
        // Reset velocity as needed
        velocity = sf::Vector2f(0, 0);
    }


    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    virtual ~Car() {

    }


};

//opponent car
class OpponentCar : public Car {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity;

    OpponentCar(float x, float y)
        : shape(sf::Vector2f(29, 29)), velocity(0, 0) {
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Blue);
        random_device rd;
        // Seed the random number generator
        generator = mt19937(rd());
        velocity = sf::Vector2f(2, 0);
        {

        }
    }



    void update(const sf::Vector2f& playerPosition, const vector<vector<int>>& maze) {
        float distance = sqrt(pow(playerPosition.x - shape.getPosition().x, 2) +
            pow(playerPosition.y - shape.getPosition().y, 2));

        // 0 for left, 1 for right
        uniform_int_distribution<int> randomDirection(0, 1);

        // Make decisions based on the distance
        if (distance < 150) {
            int decision = randomDirection(generator);

            if (decision == 0) {
                // Move left
                velocity = sf::Vector2f(-1, 0);
                {

                }
            }
            else {
                // Move right
                velocity = sf::Vector2f(1, 0);
                {

                }
            }
        }
        else {
            velocity = sf::Vector2f(0, 0);
        }

        checkCollisionWithWalls(maze);
        move();


    }


    void checkCollisionWithWalls(const vector<vector<int>>& maze) {
        // Calculate the cell position of the opponent car
        int cellX = static_cast<int>((shape.getPosition().x + shape.getSize().x / 2) / 30);
        int cellY = static_cast<int>((shape.getPosition().y + shape.getSize().y / 2) / 30);

        // Check if the cell contains a wall (1 in the maze array)
        if (maze[cellY][cellX] == 1) {
            // If a wall is detected, stop the opponent car (set velocity to zero)
            velocity = sf::Vector2f(0, 0);
        }
    }


    void move() {
        shape.move(velocity);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape); {

        }
    }


private:
    mt19937 generator; // Mersenne Twister random number generator
};