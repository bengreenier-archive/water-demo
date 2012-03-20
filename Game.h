#ifndef GAME_H
#define GAME_H

#include "Physics/PhysicsCore.h"

class Game
{
    public:
        Game(PhysicsCore* physicsCore);
        ~Game();
    protected:
    private:
        PhysicsCore* pCore;
};

#endif // GAME_H
