#include "WaterChain.h"

WaterChain::WaterChain(ScreenCore* score,PhysicsCore* core,PhysicsCore::Position one, PhysicsCore::Position two,int points)
{
    //ctor
    ppm = 1/50.0f;
    m_core=core;
    m_s_core=score;
    pos1=one;
    pos2=two;

    //what im gunna need to do is add em all, to a spot shy of two, and then after add, pull two-body over to two, stretching the chain
    int totalPoints=points;
    int pointSize = 2;

    AddNode(one,pointSize,PhysicsCore::Static,0);


    PhysicsCore::Position tempPos;
    tempPos = two;
    two.x = two.x-(totalPoints*pointSize);
    //two.y = two.y-(totalPoints*pointSize);
    for (int i = 0; i <= totalPoints; i ++) //this allows horozontal water only
    {
       PhysicsCore::Position intervalpos;
       if (i>0)
       {
           intervalpos.x=(one.x+two.x)/totalPoints*i;
           intervalpos.y=(one.y);///totalPoints*i;
       }
       else //i==0
       {
           intervalpos.x=(one.x+two.x)/totalPoints;
           intervalpos.y=(one.y);///totalPoints;
       }
       if (i!=totalPoints)
       AddNode(intervalpos,pointSize,PhysicsCore::Dynamic,i+1);
    }

    FinalAddNode(two,pointSize,PhysicsCore::Static,17,tempPos);//this approach stretches the joints, but doesn't look great

    //LinkBodies();

    std::cout<<"WaterChain Created Successfully.\n";


}

WaterChain::~WaterChain()
{
    //dtor
}
void WaterChain::LinkBodies()
{
    for (int i=m_bodies.size()-1;i>0;i--)
    {
        b2MassData massData;
        m_bodies[i]->body->GetFixtureList()->GetMassData(&massData);
        b2MassData *newMass = new b2MassData();

        newMass->mass = massData.mass/20;
        newMass->I = massData.I;
        newMass->center = massData.center;

        m_bodies[i]->body->SetMassData(newMass);
        //tdata->mass = tdata->mass/2;
        //m_bodies[i]->body->SetMassData(tdata);
        b2DistanceJointDef* t = new b2DistanceJointDef();
        //std::cout<<"jointing "<<i<<" to "<<i-1<<" |"<<m_bodies[i]->state<<" to "<<m_bodies[i-1]->state<<" | "<<m_bodies[i]->num<<" to "<<m_bodies[i-1]->num<<"\n";
        t->Initialize(m_bodies[i]->body,m_bodies[i-1]->body,m_bodies[i]->body->GetWorldCenter(),m_bodies[i-1]->body->GetWorldCenter());
        t->collideConnected = true;
        m_core->RegisterJointDef(t);
    }
}

void WaterChain::AddNode(PhysicsCore::Position pos,int size,PhysicsCore::Context context,int num)
{
    b2BodyDef* bodyDef = new b2BodyDef;

    if (context == PhysicsCore::Dynamic)
    bodyDef->type = b2_dynamicBody;

    bodyDef->allowSleep = true;
    bodyDef->awake = true;
    bodyDef->bullet = false;

    //set the bodydef's position in box2d coords
    bodyDef->position.Set(pos.x*ppm,pos.y*ppm);

    //no angles for now, god.

    b2CircleShape* circle = new b2CircleShape;

    circle->m_radius = size * ppm;

    b2FixtureDef* fixtureDef = new b2FixtureDef;
    fixtureDef->shape = circle;
    //apply material info to the fixture
    fixtureDef->density = 0.01f;
    fixtureDef->friction = 0.5f;
    fixtureDef->restitution = 0.2f;

    m_bdefs.push(bodyDef);
    m_fdefs.push(fixtureDef);

    //m_bodies.push_back(m_core->AccessWorld()->CreateBody(m_bdefs.front()));  //no though, then this doesn't update the sfml via pCore. should add via AddList in PhysicsCore::
    PhysicsCore::Instance* t = new PhysicsCore::Instance();

    sf::CircleShape* shape = new sf::CircleShape(size);
       shape->setFillColor(sf::Color(0,0,255));
       shape->setPosition(pos.x,pos.y);
       shape->setOrigin(size,size);


    t->Body = m_core->AccessWorld()->CreateBody(m_bdefs.front());

    mbody* temp = new mbody;
    temp->body = t->Body;
    temp->num = num;
    if (context == PhysicsCore::Dynamic)
    temp->state = "Dynamic";
    else
    temp->state = "Static";
    m_bodies.push_back(temp);
    m_bodies.back()->body->CreateFixture(m_fdefs.front());
    t->Draw = shape;
    t->Trans = shape;

    m_core->Add(t);

    m_bdefs.pop();
    m_fdefs.pop();

}

void WaterChain::FinalAddNode(PhysicsCore::Position pos,int size,PhysicsCore::Context context,int num,PhysicsCore::Position cpos)
{
    AddNode(pos,size,context,num);
    LinkBodies();
    m_bodies.back()->body->SetTransform(b2Vec2(cpos.x*ppm,cpos.y*ppm),m_bodies.back()->body->GetAngle());//transform to cpos, with same angle
}

std::queue<b2Body*> WaterChain::GetChainedBodies()
{
    std::queue<b2Body*> t;
    for (int i=0;i<m_bodies.size();i++)
    {
    t.push(m_bodies[i]->body);
    }
    return t;
}
