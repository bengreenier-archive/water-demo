#ifndef PHYSICSCORE_H
#define PHYSICSCORE_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <queue>
#include <list>
#include "../WaterContent.h"
#include "../Screen/ScreenCore.h"
class PhysicsCore
{
    public:
        enum Context{Static,Dynamic};
        struct Position{int x,y;};
        struct Instance{b2Body* Body; sf::Transformable* Trans; sf::Drawable* Draw; b2BodyDef* BodyDef;};
        PhysicsCore(float timestep,int velocity,int position,int maxbodies,int gravx,int gravy);
        ~PhysicsCore();
        b2World* AccessWorld(){return world;}
        void Clear();
        void Update(ScreenCore* screen);
        void AddCircle(Context context,Position position,int radius,float density,float restitution,float friction,sf::Color color);
        void AddSquare(Context context,Position position,int width,int height,float density,float restitution,float friction,sf::Color color);
        void Add(PhysicsCore::Instance* in){AddList.push(in);}
        ScreenCore* SetScreen;
        static void ThreadUpdate(PhysicsCore* ref){ ref->Update(ref->SetScreen); }

        void RegisterJointDef(b2JointDef* def);
        void RegisterWaterContent(WaterContent* con);

    protected:
    private:

        b2Vec2 gravity;
        bool MiscClear;
        b2World* world;
        float step;
        int velo,pos,max;
        std::list<Instance*>List;
        std::list<WaterContent*>WaterList;
        std::queue<Instance*>AddList;
        std::queue<Instance*>RemoveList;

        std::queue<b2JointDef*>JointDefList;

};

#endif // CORE_H
