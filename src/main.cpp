#include <stdlib.h>
#include <vector>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include <cmath>
#include <iostream>

const float PI2 = 6.28f;
const unsigned int FISH_NUMBER = 150;
bool SCREEN_ENCOUNTER_METHOD = false; 
const double ALIGNMENT_RADIUS = 0.4;
const double SEPARATION_RADIUS = 0.2;
bool DRAW_PARAMS = false;

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

// - - - - - - S U R C H A R G E S   D ' O P E R A T E U R S - - - - - -

// - - - - - - F O N C T I O N S   D E   B A S E - - - - - -

int sign(float value) {
    return value >= 0 ? 1 : -1;
}

double getInTheCircle(double value) {
    if (value < 0) while (value < 0) value += PI2;
    else while (value > PI2) value -= PI2;
    return value;
}

double colorValue(double angle, float param) {
    double value = angle+param;
    value = getInTheCircle(value);
    if (value > PI2/2) value = PI2-value;
    return value/(PI2/2);
}

void Fish::move() {
    glm::vec2 temp(
        this->position()[0]+(std::cos(this->angle())*(this->speed())), 
        this->position()[1]+(std::sin(this->angle())*(this->speed()))
        );
    this->m_position = temp;
    //this->m_angle+= p6::random::number(-0.1, 0.1);
    //this->m_angle += 0.05;
    //this->m_angle -= 0.01;
}

void Fish::turn(float direction) {
    this->m_angle += direction;
}

void drawParams(Fish fish, p6::Context &ctx) {
    ctx.stroke = p6::Color{1.f, 0, 0, .5f};
    ctx.circle(p6::Center{fish.position()}, p6::Radius{SEPARATION_RADIUS});
    ctx.stroke = p6::Color{0, 1.f, 0, .5f};
    ctx.circle(p6::Center{fish.position()}, p6::Radius{ALIGNMENT_RADIUS});
}

void Fish::draw(p6::Context& ctx) {
    float r = colorValue(this->angle(), 0.f);
    float g = colorValue(this->angle(), PI2/3.f);
    float b = colorValue(this->angle(), 2.f*PI2/3.f);
    ctx.stroke = p6::Color{r, g, b, 1.f};
    ctx.circle(p6::Center{this->position()}, p6::Radius{0.03f});
    if (DRAW_PARAMS) drawParams(*this, ctx);;
    ctx.line(this->position(), glm::vec2 {
        this->position()[0]-std::cos(this->angle())*this->speed()*8,
        this->position()[1]-std::sin(this->angle())*this->speed()*8
    });

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

void bounce(Fish &fish, const p6::Context &ctx) {
    if (fish.position()[0] >= ctx.aspect_ratio() || fish.position()[0] <= -ctx.aspect_ratio()) {
        fish.angle(PI2/2-fish.angle());
    }
    if (fish.position()[1] >= 1 || fish.position()[1] <= -1) {
        fish.angle(PI2-fish.angle());
    }
}

std::vector<Fish> createHerd(const unsigned int fishNumber, const p6::Context &ctx) {
    std::vector<Fish> fishHerd;
    for (unsigned int i = 0; i < fishNumber; ++i) {
        glm::vec2 temp(p6::random::number(-ctx.aspect_ratio(), ctx.aspect_ratio()), p6::random::number(-1, 1));
        double angle = p6::random::number(0.f, PI2);
        //double angle = 0.0;
        std::cout << i << " : " << temp[0] << "|" << temp[1] << ", " << angle << std::endl; 
        fishHerd.push_back(Fish(temp, angle, .005, i));
    }
    return fishHerd;
}

// - - - - - - G R O S S E S   F O N C T I O N S   L A - - - - - -

float distance(Fish fish1, Fish fish2) {
    float distanceX = fish1.position()[0] - fish2.position()[0];
    float distanceY = fish1.position()[1] - fish2.position()[1];
    float distance = std::sqrt(distanceX*distanceX + distanceY*distanceY);
    return distance;
}

void Alignment(Fish currentFish, Fish &otherFish) {
    float dist = distance(currentFish, otherFish);
    float direction = currentFish.angle()-otherFish.angle();
    if (SEPARATION_RADIUS < dist  && dist < ALIGNMENT_RADIUS) otherFish.turn((1/dist)*direction * 0.0002);
}

void Separation(Fish currentFish, Fish &otherFish) {
    float dist = distance(currentFish, otherFish);
    float direction = currentFish.angle()-otherFish.angle();
    direction = sign(direction);
    if (dist < SEPARATION_RADIUS) otherFish.turn(-(1/(20*dist))*direction*0.0004);
}

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


    float r = p6::random::number(0, .5);
    float g = p6::random::number(0, .5);
    float b = p6::random::number(0, .5);
   

    // Declare your infinite update loop.
    ctx.update = [&]() {
        
        ctx.stroke_weight = 0.01f;
        ctx.fill = {r, g, b, 0.04f};
        ctx.rectangle(p6::FullScreen{});
        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.05f}
        );

        for (Fish &fish : herd) {
            fish.draw(ctx);
            fish.move();
            for (Fish &otherFish : herd) {
                if (fish.id != otherFish.id) {
                    Alignment(fish, otherFish);
                    Separation(fish, otherFish);
                }
            }
            if (SCREEN_ENCOUNTER_METHOD) bounce(fish, ctx); 
            else passThrough(fish, ctx);
        }
    };



    // Should be done last. It starts the infinite loop.
    ctx.start();
}