#ifndef WATERCONTENT_H
#define WATERCONTENT_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <queue>
#include <iostream>

class WaterContent
{
    public:
        WaterContent(std::queue<b2Body*> in,int depth=20);
        ~WaterContent();
        void ScrollAndUpdate();
        sf::ConvexShape* Shape;//these make up the content
        bool firstTime;
        std::vector<b2Body*> Bodies;//these are the "top" of the water. surface edge
    protected:
    private:
        float mpp;
        int depth;
};

#endif // WATERCONTENT_H
