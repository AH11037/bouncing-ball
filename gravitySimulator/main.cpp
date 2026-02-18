#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

class Circle {
public:
    float circleRadius = rand() % 50 + 30;
    sf::CircleShape shape;
    sf::Vector2f gravityVelocity = {0.f,0.f};
    sf::Vector2f gravityAcceleration = {0.f, 9.8 / 100};
    bool isDragged = false;
    sf::Vector2i offset;

    Circle () {
        shape.setRadius(circleRadius);

        int red = rand() % 255, blue = rand() % 255, green = rand() % 255;
        float x = rand() % 1500;
        shape.setFillColor(sf::Color(red, blue, green));

        shape.setOrigin(sf::Vector2f(circleRadius, circleRadius));
        shape.setPosition(sf::Vector2f{x, 375.f });
    }
    void mouseCheck(sf::RenderWindow& window) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (!isDragged && shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
                offset = sf::Mouse::getPosition(window) - sf::Vector2i(shape.getPosition());
                isDragged = true;
            }
            if (isDragged) {
                gravityVelocity = { 0,0 };
                shape.setPosition(sf::Vector2f(sf::Mouse::getPosition(window) - offset));
                //std::cout << "pls";
            }
        }
        else {
            isDragged = false;
        }
    }
    void update(sf::RenderWindow& window) {
        window.draw(shape);
        gravity();
        xGravity();
    }
    void gravity() {
            gravityVelocity += gravityAcceleration;
            shape.move({0,gravityVelocity.y});

        if (shape.getPosition().y >= 750.f || shape.getPosition().y <= 0.f) {
            float y = shape.getPosition().y;
            y = std::max(0.f, std::min(750.f - circleRadius, y));
            shape.setPosition({shape.getPosition().x,y});
            gravityVelocity.y *= -1 * 0.9;
        }
    }
    void xGravity() {
        gravityVelocity.x += 0.5 * (-1.0 + 2.0 * (double)rand() / RAND_MAX);
        shape.move({ gravityVelocity.x, 0 });

        if (shape.getPosition().x >= 1500.f || shape.getPosition().x <= 0.f) {
            float x = shape.getPosition().x;
            x = std::max(0.f, std::min(1500.f - circleRadius, x));
            shape.setPosition({x, shape.getPosition().y});
            gravityVelocity.x *= -1 * 0.9;
        }
    }
};


    int main()
    {
        unsigned int windowLength = 1500;
        unsigned int windowHeight = 750;

        sf::RenderWindow window(sf::VideoMode({ windowLength, windowHeight }), "Gravity Simulator");
        window.setFramerateLimit(100);

        std::vector<Circle*> circleArray = {};
        for (int i = 0; i < 10; i++) { circleArray.push_back(new Circle); }

        srand(time(0));

        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
            }
            window.clear(sf::Color::Black);

            for (auto c : circleArray) {
                c->mouseCheck(window);
                c->update(window);
            }

            window.display();
        }
        return 0;
    }