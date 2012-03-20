#ifndef WATERCHAIN_H
#define WATERCHAIN_H

#include <Box2D/Box2D.h>
#include <vector>
#include <queue>
#include <string>
#include "Screen/ScreenCore.h"
#include "Physics/PhysicsCore.h"

class WaterChain
{
    public:
        WaterChain(ScreenCore* score,PhysicsCore* core,PhysicsCore::Position one, PhysicsCore::Position two,int points);
        ~WaterChain();
        std::queue<b2Body*> GetChainedBodies();
    protected:
    private:
        float ppm;
        struct mbody{int num; b2Body* body; std::string state;};

        std::queue<b2BodyDef*> m_bdefs;
        std::queue<b2FixtureDef*> m_fdefs;
        std::queue<b2DistanceJointDef*> m_rdefs;
        std::vector<mbody*>m_bodies;
        //vectors for the components of the chain

        PhysicsCore* m_core;
        ScreenCore* m_s_core;
        b2ChainShape* m_chain;
        b2Body* m_body;
        b2BodyDef* m_bodydef;
        b2FixtureDef* m_fixturedef;
        PhysicsCore::Position pos1,pos2;//starts at 1, ends at 2
        void AddNode(PhysicsCore::Position pos,int size,PhysicsCore::Context,int num);//add nodes along the way, at set intervals, at set sizes, with set joints
        void FinalAddNode(PhysicsCore::Position pos,int size,PhysicsCore::Context context,int num,PhysicsCore::Position cpos);
        void LinkBodies();//create the joints, and joint them

};

#endif // WATERCHAIN_H
