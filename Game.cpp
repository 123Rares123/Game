using namespace std;
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

enum GameState {
    MENU,
    GAME
};


class Ball {
public:
    sf::CircleShape shape;
    float vx, vy;

    Ball(float radius, float x, float y, float vx, float vy)
        : shape(radius), vx(vx), vy(vy) {
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
    }

    void update(float dt, sf::RenderWindow& window) {
        
        shape.move(vx * dt, vy * dt);

        
        sf::Vector2f position = shape.getPosition();
        float radius = shape.getRadius();

        
        if (position.x < 0) {
            vx = std::abs(vx); 
            shape.setPosition(0, position.y); 
        }
        else if (position.x + radius * 2 > window.getSize().x) {
            vx = -std::abs(vx); 
            shape.setPosition(window.getSize().x - radius * 2, position.y); 
        }

        if (position.y < 0) {
            vy = std::abs(vy); 
            shape.setPosition(position.x, 0); 
        }
        else if (position.y + radius * 2 > window.getSize().y) {
            vy = -std::abs(vy); 
            shape.setPosition(position.x, window.getSize().y - radius * 2);
        }
    }

    
    void setSpeed(float newVx, float newVy) {
        vx = newVx;
        vy = newVy;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "FnAf", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    GameState currentState = MENU;

    
    sf::Texture eXit_textura;
    if (!eXit_textura.loadFromFile("D:\\CPP\\Game1\\Game1\\exit.png")) {
        return -1;
    }
    sf::Texture eXit_textura2;
    if (!eXit_textura2.loadFromFile("D:\\CPP\\Game1\\Game1\\exit2.png")) {
        return -1;
    }
    sf::Texture freddy;
    if (!freddy.loadFromFile("D:\\CPP\\Game1\\Game1\\freddy.png")) {
        return -1;
    }
    sf::Texture start_button;
    if (!start_button.loadFromFile("D:\\CPP\\Game1\\Game1\\start_button.png")) {
        return -1;
    }
    sf::Texture start_button2;
    if (!start_button2.loadFromFile("D:\\CPP\\Game1\\Game1\\start_button2.png")) {
        return -1;
    }

    
    sf::Sprite eXit_sprite;
    sf::Sprite freddy_sprite;
    sf::Sprite start_buttonS;
    start_buttonS.setTexture(start_button);
    freddy_sprite.setTexture(freddy);

    sf::Sprite freddy2_sprite;
    freddy2_sprite.setTexture(freddy);
    freddy2_sprite.setScale(0.50f, 0.50f);
    freddy2_sprite.setPosition(750, 1);

    freddy_sprite.setScale(1.0f, 1.0f);
    freddy_sprite.setPosition(750, 1);
    eXit_sprite.setTexture(eXit_textura);
    eXit_sprite.setPosition(60, 975);
    start_buttonS.setPosition(500, 500);

    float freddy_speed = 500.0f;
    sf::Clock clock;

    
    bool isVisible = true;
    bool canTurnInvisible = true;  
    sf::Clock invisibilityClock;

    
    std::vector<std::pair<sf::RectangleShape, bool>> glitchLines;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    float frameDelay = 0.01f;
    for (int i = 0; i < 10; i++) {
        sf::RectangleShape line(sf::Vector2f(1920, 2));
        line.setPosition(0, static_cast<float>(std::rand() % 1080));
        glitchLines.push_back(std::make_pair(line, true));
    }

    while (window.isOpen()) {
        if (currentState == MENU) {
            sf::Clock menuClock;

            while (window.isOpen() && currentState == MENU) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    if (event.type == sf::Event::MouseButtonPressed) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                        if (eXit_sprite.getGlobalBounds().contains(mousePosF)) {
                            window.close();
                        }
                        if (start_buttonS.getGlobalBounds().contains(mousePosF)) {
                            currentState = GAME;
                        }
                    }
                    if (event.type == sf::Event::MouseMoved) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                        if (eXit_sprite.getGlobalBounds().contains(mousePosF)) {
                            eXit_sprite.setTexture(eXit_textura2);
                        }
                        else {
                            eXit_sprite.setTexture(eXit_textura);
                        }
                        if (start_buttonS.getGlobalBounds().contains(mousePosF)) {
                            start_buttonS.setTexture(start_button2);
                        }
                        else {
                            start_buttonS.setTexture(start_button);
                        }
                    }
                }

                if (menuClock.getElapsedTime().asSeconds() > frameDelay) {
                    window.clear(sf::Color::Black);
                    for (auto& pair : glitchLines) {
                        auto& line = pair.first;
                        auto& isVisible = pair.second;

                        if (isVisible) {
                            window.draw(line);
                        }

                        if (std::rand() % 100 < 5) {
                            isVisible = !isVisible;
                        }

                        line.move(0, 1.0f);
                        if (line.getPosition().y > 1080) {
                            line.setPosition(0, -2);
                        }
                    }
                    window.draw(eXit_sprite);
                    window.draw(freddy_sprite);
                    window.draw(start_buttonS);

                    window.display();
                    menuClock.restart();
                }
            }
        }

        if (currentState == GAME) {
            
            std::vector<Ball> balls;
            balls.push_back(Ball(20.0f, 400.0f, 200.0f, 400.0f, 300.0f));
            balls.push_back(Ball(30.0f, 800.0f, 100.0f, -500.0f, 200.0f));
            balls.push_back(Ball(20.0f, 200.0f, 700.0f, 400.0f, 300.0f));
            balls.push_back(Ball(30.0f, 800.0f, 100.0f, -500.0f, 200.0f));
            balls.push_back(Ball(20.0f, 900.0f, 300.0f, 400.0f, 300.0f));
            balls.push_back(Ball(30.0f, 500.0f, 900.0f, -500.0f, 200.0f));

            while (window.isOpen() && currentState == GAME) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }

                sf::Time deltaTime = clock.restart();
                float dt = deltaTime.asSeconds();

                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    freddy2_sprite.move(0, -freddy_speed * dt);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    freddy2_sprite.move(-freddy_speed * dt, 0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    freddy2_sprite.move(0, freddy_speed * dt);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    freddy2_sprite.move(freddy_speed * dt, 0);
                }

                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canTurnInvisible) {
                    isVisible = false;
                    canTurnInvisible = false; 
                    invisibilityClock.restart();
                }

                if (invisibilityClock.getElapsedTime().asSeconds() >= 5.0f) {
                    isVisible = true;
                    canTurnInvisible = true; 
                }

               
                bool collisionDetected = false;
                for (auto& ball : balls) {
                    ball.update(dt, window);

                    
                    if (isVisible && freddy2_sprite.getGlobalBounds().intersects(ball.shape.getGlobalBounds())) {
                        collisionDetected = true;
                    }
                }

                
                if (collisionDetected) {
                    currentState = MENU;
                    break; 
                }

                window.clear();
                if (isVisible) {
                    window.draw(freddy2_sprite);
                }

                
                for (auto& ball : balls) {
                    window.draw(ball.shape);
                }

                window.display();
            }
        }
    }

    return 0;
}
