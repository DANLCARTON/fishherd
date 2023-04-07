#include <stdlib.h>
#include <vector>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include <cmath>
#include <iostream>

const float PI2 = 6.28f; //                              BEST VALUES :
float SPEED = 0.01; //                                    0.01
float TURN_FACTOR = 1; //                                 1
const unsigned int FISH_NUMBER = 80; //                   80
float ALIGNMENT_RADIUS = 0.5; //                          0.5
float SEPARATION_RADIUS = 0.25; //                        0.25
float COHESION_RADIUS = 0.75; //                          0.75  
const float MIN_SEPARATION_RADIUS = 0.125; //             0.125
float WALLS_RADIUS = 0.5; //                              0.5
bool DRAW_PARAMS = false; //                              false

float ALIGNMENT_STRENGTH = 0.0024; //                     0.0024
float SEPARATION_STRENGTH = 0.0048; //                    0.0048
float MOUSE_FISH_SEPARATION_STRENGTH = 0; //              0.0032
float WALLS_STRENGTH = 0.0032; //                         0.0032
float COHESION_STRENGTH = 0.005; //                       0.005
float TAIL_SIZE = 0.04; //                                0.04

// - - - - - - C L A S S   F I S H - - - - - -

class Fish {

    private:

        glm::vec2 m_position;
        double m_angle;
        double m_speed;

    public:

        unsigned int id;

        Fish();
        Fish(glm::vec2 position, double angle, double speed, unsigned int id) {this->m_position = position; this->m_angle = angle; this->m_speed = speed; this->id = id;};
        ~Fish() = default;

        glm::vec2 position() {return this->m_position;}
        void position(glm::vec2 newPosition) {m_position = newPosition;}
        double angle() {return this->m_angle;}
        void angle(double newAngle) {m_angle = newAngle;}
        double speed() {return this->m_speed;}
        void speed(double newSpeed) {m_speed = newSpeed;}

        void move();
        void turn(float direction);
        void draw(p6::Context& ctx);
};

// - - - - - - P A R A M E R T E S   G U I - - - - - -

void displayGUI () {
    ImGui::Begin("ACT ON THE FISH HERD");
    ImGui::SliderFloat("Size", &TAIL_SIZE, 0, 1);
    ImGui::SliderFloat("Speed", &SPEED, 0, 0.03);
    ImGui::SliderFloat("Turn Factor", &TURN_FACTOR, 0.01, 1);
    ImGui::SliderFloat("Separation Radius", &SEPARATION_RADIUS, 0, 1);
    ImGui::SliderFloat("Separation strength", &SEPARATION_STRENGTH, 0, 0.0196);
    ImGui::SliderFloat("Alignment Radius", &ALIGNMENT_RADIUS, 0, 1);
    ImGui::SliderFloat("Alignment strength", &ALIGNMENT_STRENGTH, 0, 0.0196);
    ImGui::SliderFloat("Cohesion Radius", &COHESION_RADIUS, 0.001, 1);
    ImGui::SliderFloat("Cohesion strength", &COHESION_STRENGTH, 0, 0.0196);
    ImGui::SliderFloat("Walls radius", &WALLS_RADIUS, 0, 1);
    ImGui::SliderFloat("Walls strength", &WALLS_STRENGTH, 0, 0.0196);
    ImGui::SliderFloat("Mouse strength", &MOUSE_FISH_SEPARATION_STRENGTH, 0, 0.0196);
    if (ImGui::Button("Show Parameters"))
            DRAW_PARAMS = !DRAW_PARAMS;
    ImGui::End();
}

// - - - - - - F O N C T I O N S   D E   B A S E - - - - - -

int sign(float value) {
    return value >= 0 ? 1 : -1;
}

double getInTheCircle(double value) {
    if (value < 0) while (value < 0) value += (M_PI*2);
    else while (value > (M_PI*2)) value -= (M_PI*2);
    return value;
}

double colorValue(double angle, float param) {
    double value = angle+param;
    value = getInTheCircle(value);
    if (value > (M_PI*2)/2) value = (M_PI*2)-value;
    return value/((M_PI*2)/2);
}

void Fish::move() {
    glm::vec2 temp(
        this->position()[0]+(std::cos(this->angle())*(this->speed())), 
        this->position()[1]+(std::sin(this->angle())*(this->speed()))
        );
    this->m_position = temp;
}

void Fish::turn(float direction) {
    this->m_angle += (direction*TURN_FACTOR);
    // this->angle(getInTheCircle(this->angle()));
}

void drawParams(Fish fish, p6::Context &ctx) {
    ctx.stroke = p6::Color{1.f, 0, 0, .5f};
    ctx.circle(p6::Center{fish.position()}, p6::Radius{SEPARATION_RADIUS});
    ctx.stroke = p6::Color{0, 1.f, 0, .5f};
    ctx.circle(p6::Center{fish.position()}, p6::Radius{ALIGNMENT_RADIUS});
    ctx.stroke = p6::Color(0, 0, 0, .5f);
    ctx.circle(p6::Center{fish.position()}, p6::Radius{MIN_SEPARATION_RADIUS});
    ctx.stroke = p6::Color(0, 0, 1.f, .5f);
    ctx.circle(p6::Center{fish.position()}, p6::Radius{COHESION_RADIUS});
}

void Fish::draw(p6::Context& ctx) {
    float r = colorValue(this->angle(), 0.f);
    float g = colorValue(this->angle(), (M_PI*2)/3.f);
    float b = colorValue(this->angle(), 2.f*(M_PI*2)/3.f);
    ctx.stroke = p6::Color{r, g, b, 1.f};
    ctx.circle(p6::Center{this->position()}, p6::Radius{0.03f});
    if (DRAW_PARAMS) drawParams(*this, ctx);;
    ctx.line(this->position(), glm::vec2 {
        this->position()[0]-std::cos(this->angle())*this->speed()*8,
        this->position()[1]-std::sin(this->angle())*this->speed()*8
    });

}

// - - - - - - G R O S S E S   F O N C T I O N S - - - - - -

float distance(Fish fish1, Fish fish2) {
    float distanceX = fish1.position()[0] - fish2.position()[0];
    float distanceY = fish1.position()[1] - fish2.position()[1];
    float distance = std::sqrt(distanceX*distanceX + distanceY*distanceY);
    return distance;
}

void Alignment(Fish currentFish, Fish &otherFish) {
    float dist = distance(currentFish, otherFish);
    float direction = currentFish.angle()-otherFish.angle();
    if (SEPARATION_RADIUS < dist  && dist < ALIGNMENT_RADIUS) otherFish.turn((2/dist)*direction*ALIGNMENT_STRENGTH);
}

void Separation(Fish &currentFish, Fish &otherFish) {
    float dist = distance(currentFish, otherFish);
    float direction = currentFish.angle()-otherFish.angle();
    direction = sign(direction);
    if (dist < SEPARATION_RADIUS) {
        if (currentFish.id == 1000) otherFish.turn(-(1/(20*dist))*direction*MOUSE_FISH_SEPARATION_STRENGTH);
        else otherFish.turn(-(1/(20*dist))*direction*SEPARATION_STRENGTH);
    }
    if (dist < MIN_SEPARATION_RADIUS) {
        currentFish.turn(1/(20*dist)*direction*SEPARATION_STRENGTH);
    }
}

void Cohesion(Fish &currentFish, std::vector<Fish> &fishHerd) {
    double averageDirection = 0;
    double fishesInTheRadius = 0;
    for (Fish &fish : fishHerd) {
        if (distance(currentFish, fish) < COHESION_RADIUS) {
            averageDirection += getInTheCircle(fish.angle());
            ++fishesInTheRadius;
        } 
    }
    averageDirection = averageDirection/fishesInTheRadius;
    currentFish.turn((averageDirection-getInTheCircle(currentFish.angle()))*COHESION_STRENGTH);
}

// - - - - - - G E S T I O N   D E S   M U R S - - - - - -

void avoidWall(Fish &fish, double distance, const int wall) {
    double dir = getInTheCircle(fish.angle());
    if (wall == 0) { // top wall
        if (dir < (M_PI*2)/4 || dir > (3*(M_PI*2)/4)) dir = -1;
        else dir = 1;
    } else if (wall == 1) { // bottom wall
        if (dir < (M_PI*2)/4 || dir > (3*(M_PI*2)/4)) dir = 1;
        else dir = -1;
    } else if (wall == 2) { // right wall
        if (dir < (M_PI*2)/2) dir = 1;
        else dir = -1;
    } else { // left wall
        if (dir < (M_PI*2)/2) dir = -1;
        else dir = 1;
    }
    fish.turn(1/(20*distance)*dir*WALLS_STRENGTH);
}

void wallSeparation(Fish &fish, const p6::Context &ctx) {
    double topWallDistance = 1-fish.position()[1];
    double bottomWallDistance = fish.position()[1]+1;
    double rightWallDistance = ctx.aspect_ratio()-fish.position()[0];
    double leftWallDistance = fish.position()[0]+ctx.aspect_ratio();
    if (topWallDistance < WALLS_RADIUS) avoidWall(fish, topWallDistance, 0);
    if (bottomWallDistance < WALLS_RADIUS) avoidWall(fish, bottomWallDistance, 1);
    if (rightWallDistance < WALLS_RADIUS) avoidWall(fish, rightWallDistance, 2);
    if (leftWallDistance < WALLS_RADIUS) avoidWall(fish, leftWallDistance, 3);
}

void passThrough(Fish &fish, const p6::Context &ctx) {
    if (fish.position()[0] >= ctx.aspect_ratio() || fish.position()[0] <= -ctx.aspect_ratio()) {
        glm::vec2 pos (fish.position()[0]*-1, fish.position()[1]);
        fish.position(pos);
    }
    if (fish.position()[1] >= 1 || fish.position()[1] <= -1) {
        glm::vec2 pos (fish.position()[0], fish.position()[1]*-1);
        fish.position(pos);
    }
}

// - - - - - - C R E E R   L E S   B O I D S - - - - - -

std::vector<Fish> createHerd(const unsigned int fishNumber, const p6::Context &ctx) {
    std::vector<Fish> fishHerd;
    for (unsigned int i = 0; i < fishNumber; ++i) {
        glm::vec2 temp(p6::random::number(-ctx.aspect_ratio(), ctx.aspect_ratio()), p6::random::number(-1, 1));
        double angle = p6::random::number(0.f, (M_PI*2));
        //double angle = 0.0;
        // std::cout << i << " : " << temp[0] << "|" << temp[1] << ", " << angle << std::endl; 
        fishHerd.push_back(Fish(temp, angle, SPEED, i));
    }
    return fishHerd;
}

// - - - - - - M A I N - - - - - -

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "FISHHERD"}};
    ctx.maximize_window();



    std::vector<Fish> herd = createHerd(FISH_NUMBER, ctx);
    Fish mouseFish = Fish(ctx.mouse(), p6::random::number(0, (M_PI*2)), 0, 1000);


    float r = p6::random::number(0, .5);
    float g = p6::random::number(0, .5);
    float b = p6::random::number(0, .5);
   
    // Declare your infinite update loop.
    ctx.update = [&]() {

        displayGUI();

        mouseFish.position(ctx.mouse());
        mouseFish.angle(p6::random::number(-(M_PI*2), (M_PI*2)));
        mouseFish.draw(ctx);
        
        ctx.stroke_weight = 0.f;
        ctx.fill = {r, g, b, TAIL_SIZE};
        ctx.rectangle(p6::FullScreen{});
        // ctx.circle(p6::Center{ctx.mouse()},p6::Radius{0.05f});
        ctx.stroke_weight = 0.01f;

        for (Fish &fish : herd) {
            fish.draw(ctx);
            fish.move();
            for (Fish &otherFish : herd) {
                if (fish.id != otherFish.id) {
                    Alignment(fish, otherFish);
                    Separation(fish, otherFish);
                    Separation(mouseFish, otherFish);
                    wallSeparation(fish, ctx);
                }
            }
            fish.speed(SPEED);
            Cohesion(fish, herd);
            passThrough(fish, ctx);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}