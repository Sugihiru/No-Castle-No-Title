#ifndef I_LIVING_THING_HPP
#define I_LIVING_THING_HPP

class ILivingThing
{
public:
    virtual ~ILivingThing(){};

    virtual bool isDead() = 0;
};

#endif // I_LIVING_THING_HPP
