#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
constexpr auto _PI_ =  3.14159265358979323846;

class Car {
public:

    Car() {
         angle = 90;
         x = 100;
         y = 300;
         oldX = 100;
         oldY = 300;
         oldAngle = 90;
    }

    float getCarXCord() const {
        return x;
    }

    float getCarYCord() const {
        return y;
    }

    float getCarOldXCord() const {
        return oldX;
    }

    float getCarOldYCord() const {
        return oldY;
    }

    void setCarXCord(float newX) {
        x = newX;
    }

    void setCarYCord(float newY) {
        y = newY;
    }

    void setCarOldXCord(float newX) {
        oldX = newX;
    }

    void setCarOldYCord(float newY) {
        oldY = newY;
    }
    float getCarAngle() {
        return angle;
    }
    void carRotate(float newAngle) {
        oldAngle = angle;
        angle += newAngle;
    }
    float getCarOldAngle() {
        return oldAngle;
    }
    void moveForward(float speed) {
        float dx = speed * std::sin(angle * (_PI_ / 180));
        float dy = -speed * std::cos(angle * (_PI_ / 180));
        oldX = x;
        oldY = y;
        x += dx;
        y += dy;
        oldAngle = angle;
    }
    
private:
    float oldAngle;
    float angle;
    float x;
    float y;
    float oldX;
    float oldY;
};

class BaseCar :public Car {
public:
    BaseCar() {
        circle.setRadius(50.f); 
        circle.setFillColor(sf::Color::Blue); 
        circle.setOrigin(circle.getRadius(), circle.getRadius()); 

     
        arrow.setPointCount(3);
        arrow.setPoint(0, sf::Vector2f(0.f, -50.f)); 
        arrow.setPoint(1, sf::Vector2f(-25.f, 0.f));
        arrow.setPoint(2, sf::Vector2f(25.f, 0.f));
        arrow.setFillColor(sf::Color::Red); 
        arrow.setOrigin(0.f, -25.f); 
    }

    void updateGraphics() { 
        circle.setPosition(getCarXCord(), getCarYCord());
        circle.setRotation(getCarAngle());

        arrow.setPosition(getCarXCord(), getCarYCord());
        arrow.setRotation(getCarAngle());
    }

    void drawCar(sf::RenderWindow& window) {
        updateGraphics();
        window.draw(circle);
        window.draw(arrow);
    }

private:
    sf::CircleShape circle; 
    sf::ConvexShape arrow; 
};

class Road {
public:
    Road() {
        width = 800.0f;
        length = 350.0f;

        roadRect.setSize(sf::Vector2f(width, length));
        roadRect.setFillColor(sf::Color(128, 128, 128));
        roadRect.setPosition(x, y);
    }

    void drawRoad(sf::RenderWindow& window) {
        window.draw(roadRect);
    }

private:
    float x = 0; 
    float y = 50; 
    float width;
    float length;
    sf::RectangleShape roadRect;
};

class RoadMarking  {
public:
    RoadMarking() {
        markLeft.setSize(sf::Vector2f(160.0f, 25.0f));
        markLeft.setFillColor(sf::Color::White);
        markLeft.setPosition(0, 200);

        markMidle.setSize(sf::Vector2f(160.0f, 25.0f));
        markMidle.setFillColor(sf::Color::White);
        markMidle.setPosition(320, 200);

        markRight.setSize(sf::Vector2f(160.0f, 25.0f));
        markRight.setFillColor(sf::Color::White);
        markRight.setPosition(640, 200);
    }

   void  drawMark(sf::RenderWindow& window) {
        window.draw(markLeft);
        window.draw(markMidle);
        window.draw(markRight);
    }
private:
    sf::RectangleShape markLeft;
    sf::RectangleShape markMidle;
    sf::RectangleShape markRight;

};

class MessageDisplay {
public:
    MessageDisplay() {
        font.loadFromFile("ArialRegular.ttf");
        message.setFont(font);
        message.setCharacterSize(20);
        message.setFillColor(sf::Color::Red);
    }

    void displayMessage(sf::RenderWindow& window, const sf::String& msg, int Y) {
        message.setPosition(25, Y); 
        message.setString(msg);
        window.draw(message);
    }

private:
    sf::Font font;
    sf::Text message;
};

class RuleTracker {
public:
    RuleTracker() : messageDisplay() {}

    struct Point {
        float x;
        float y;
    };

    std::vector<Point> getCirclePoints(float centerX, float centerY, float radius, int pointsCount) {
        std::vector<Point> points;
        points.reserve(pointsCount);

        float angleStep = (2 * _PI_) / pointsCount;

        for (int i = 0; i < pointsCount; ++i) {
            float angle = i * angleStep;
            Point point;
            point.x = centerX + radius * std::cos(angle);
            point.y = centerY + radius * std::sin(angle);
            points.push_back(point);
        }

        return points;
    }

    bool isPointInside(const Point& point, float minX, float maxX, float minY, float maxY) {
        return point.x >= minX && point.x <= maxX && point.y >= minY && point.y <= maxY;
    }

    bool areAnyPointsInside(const std::vector<Point>& points, float minX, float maxX, float minY, float maxY) {
        for (const Point& point : points) {
            if (isPointInside(point, minX, maxX, minY, maxY)) {
                return true; 
            }
        }
        return false; 
    }

    void wrongMovement(sf::RenderWindow& window,BaseCar car) {
        if (((car.getCarOldXCord() < car.getCarXCord() && car.getCarYCord() < 225) ||
            (car.getCarOldXCord() > car.getCarXCord() && car.getCarYCord() > 225)) &&
            (!hasTurned(car)) ) {
            messageDisplay.displayMessage(window, L"Езда по встречной полосе", 450);
        }
    }
    bool hasTurned(BaseCar car) {
 
        if (car.getCarAngle() != car.getCarOldAngle()) {
            return true;
        }
        return false;
       
    }
    void turn(sf::RenderWindow& window, BaseCar car) {

        if (hasTurned(car)) {
            messageDisplay.displayMessage(window, L"Машина поворачивает", 425);
        }
        else {
            messageDisplay.displayMessage(window, L"Поворот закончен", 425);
        }
        
    }
    
    void solidLine(sf::RenderWindow& window, BaseCar car) {
        std::vector<Point> circlePoints = getCirclePoints(car.getCarXCord(), car.getCarYCord(), 50, 360); 
        if (areAnyPointsInside(circlePoints, 0, 160, 200, 225)) {
            messageDisplay.displayMessage(window, L"Наезд на линию разметки", 400);
        }
        if (areAnyPointsInside(circlePoints, 320, 480, 200, 225)) {
            messageDisplay.displayMessage(window, L"Наезд на линию разметки", 400);
        }
        if (areAnyPointsInside(circlePoints, 640, 800, 200, 225)) {
            messageDisplay.displayMessage(window, L"Наезд на линию разметки", 400);
        }

    }
    void handleInput(bool &ruleMark, bool &ruleWrongDrive, bool &ruleTurn) { 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            if (ruleMark) {
                ruleMark = false;
            }
            else {
                ruleMark = true;
            }
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (ruleWrongDrive) {
                ruleWrongDrive = false;
            }
            else {
                ruleWrongDrive = true;
            }
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            if (ruleTurn) {
                ruleTurn = false;
            }
            else {
                ruleTurn = true;
            }
        }

    }


private:
    MessageDisplay messageDisplay;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Car Movement"); 
    BaseCar car;
    Road road;
    RoadMarking mark;
    RuleTracker rule;
    bool ruleMark = true, ruleWrongDrive = true, ruleTurn = true;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
           car.moveForward(0.1f);
           
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            car.carRotate(-0.1f);
           
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            car.carRotate(0.1f);
           
        }
        window.clear();
        road.drawRoad(window);
        mark.drawMark(window);
        car.drawCar(window);
        rule.handleInput(ruleMark,ruleWrongDrive,ruleTurn);
        if (ruleMark) {
            rule.solidLine(window, car);
        }
        if (ruleTurn) {
            rule.turn(window, car);
        }
        if (ruleWrongDrive) {
            rule.wrongMovement(window, car);
        }
        window.display();

    }

    return 0;
}