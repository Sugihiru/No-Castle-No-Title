#ifndef MOVING_THING_HPP
#define MOVING_THING_HPP

#include <SFML/Graphics.hpp>
#include "ILivingThing.hpp"


class MovingThing: ILivingThing
{
public:
    virtual ~MovingThing(){};
};

#endif // MOVING_THING_HPP
