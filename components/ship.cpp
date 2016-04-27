#include "ship.h"

#include <components/sprite.h>

using sf::Ship;

Ship::Ship()
{
}

void Ship::configure()
{
    // Add some logic that runs when the left trigger changes position
    connect(engine, &Engine::leftTrigChanged, this, [this](double v)
    {
        throttle = v;
    });

    // Also the right joy is our intended rotation
    connect(engine, &Engine::rightJoyChanged, this, [this](QVector2D v)
    {
       steeringVector = v;
    });

    //Also attach our asset
    auto sprite = engine->spawn<sf::Sprite>(":assets/ship");
    child_nodes.append(sprite);
    sprite->scale = QVector3D(0.5,0.5,1);

    flame = engine->spawn<sf::Sprite>(":assets/flame");
    child_nodes.prepend(flame);
    flame->location = QVector3D(0,25,-1);
    flame->rotation = 180;
}

void Ship::render(float ticktime)
{
    SceneObject::render(ticktime);
}

void Ship::tick(float ticktime)
{
    //We adjust the size of our flame based on our throttle
    flame->scale = QVector3D(0.15*throttle,0.2*throttle,1);
    //Need to adjust location a little bit so that the flame looks like
    //it comes from the base
    flame->location = QVector3D(0,25 - 6*(1-throttle),-1);

    // Calculate acceleration
    double inputToAccelerationFactor = 300;
    // Take the trigger and create a vector down the Y axis
    QVector3D newAccel = QVector3D(0,throttle*inputToAccelerationFactor,0);
    // Then rotate it by our current rotation
    newAccel = QQuaternion::fromEulerAngles(0,0,rotation) * newAccel;
    // And set our acceleration to that
    acceleration = -newAccel;

    // Then do all the normal stuff
    SceneObject::tick(ticktime);

    // But also add "drag" so that we slow down if our
    // engines are off
    double bleedFactor = 0.6; //you lose 60% of your velocity every second
    velocity = velocity*(1-bleedFactor*ticktime);

    // And add a max velocity
    double maxVelocity = 300;
    double rat = velocity.length() / maxVelocity;
    if (rat > 1)
    {
        velocity.setX(velocity.x()*(1/rat));
        velocity.setY(velocity.y()*(1/rat));
    }
    //Don't get any weird non-planar movement
    velocity.setZ(0);

    // And add steering. Limit the rate of change
    // for realism
    if(steeringVector.length() > 0.7)
    {
        double maxDegreesChange = 60; //Per second
        double steeringStrength = 5;

        double joyangle = qRadiansToDegrees(qAtan2(steeringVector.y(), steeringVector.x())) - 270;
        while (joyangle < 0) joyangle += 360;
        double curangle = rotation;
        while (curangle < 0) curangle += 360;
        double delta = joyangle - curangle;
        while (delta < 0) delta += 360;
        if (delta > 180) {
            delta = -(180 - (delta-180));
        }
        delta = clamp(delta, -maxDegreesChange, maxDegreesChange);
        rotation = curangle+delta*ticktime*steeringStrength;
    }

}
