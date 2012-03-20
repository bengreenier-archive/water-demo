#include "WaterContent.h"
WaterContent::WaterContent(std::queue<b2Body*> in,int depth)
{
    //ctor
    mpp = 50.0f;
    firstTime=true;
    WaterContent::depth = depth;
    while(in.size()>0)
    {
        Bodies.push_back(in.front());
        in.pop();
    }

    Shape = new sf::ConvexShape();
    //Shape->setFillColor(sf::Color(0,0,255,0));

    std::cout<<"WaterContent Created Successfully.\n";
}

WaterContent::~WaterContent()
{
    //dtor
}

void WaterContent::ScrollAndUpdate()
{
    //scroll all bodies, get world points, convert to pixels, update shape.
    Shape->setPointCount(Bodies.size()+2);
    Shape->setOrigin(0,0);
    //Shape->setPosition(Bodies[0]->GetPosition().x*mpp,Bodies[0]->GetPosition().y*mpp);
    for (int i=0;i<Bodies.size();i++)
    {
        b2Vec2 tpos = Bodies[i]->GetPosition();
        sf::Vector2f actualpos;
        actualpos.x = tpos.x*mpp;
        actualpos.y = tpos.y*mpp;
        Shape->setPoint(i,actualpos);
    }
        Shape->setPoint(Bodies.size(),sf::Vector2f(Shape->getPoint(Bodies.size()-1).x,Shape->getPoint(Bodies.size()-1).y+depth));
        Shape->setPoint(Bodies.size()+1,sf::Vector2f(Shape->getPoint(0).x,Shape->getPoint(0).y+depth));
        Shape->setFillColor(sf::Color(0,0,255,0));
        Shape->setOutlineThickness(2);
        Shape->setOutlineColor(sf::Color(255,0,0));

}
