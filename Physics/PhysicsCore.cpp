#include "PhysicsCore.h"

PhysicsCore::PhysicsCore(float timestep,int velocity,int position,int maxbodies,int gravx,int gravy)
{
    //ctor
    //timestep,velocity,position,maxbodies,grav.x,grav.y
    step=timestep;
    velo=velocity;
    pos=position;
    max=maxbodies;
    gravity.x=(float)gravx;
    gravity.y=(float)gravy;
    world = new b2World(gravity);
    MiscClear=false;

}

PhysicsCore::~PhysicsCore()
{
    //dtor
}

void PhysicsCore::Update(ScreenCore* screen)
{
    //for conversion
    float mpp=50.0f;
    float rtod=180/b2_pi;
    //for constraining
    int winWidth = screen->AccessWindow()->getSize().x;
    int winHeight = screen->AccessWindow()->getSize().y;
    int minC = (-1 * max);
    int maxC = max;

   //std::cout<<world->GetBodyCount()<<"\n";

    while (AddList.size()>0)
    {
        List.push_back(AddList.front());
        //std::cout<<"Adding Drawable @ "<<AddList.front()->Trans->GetPosition().x<<","<<AddList.front()->Trans->GetPosition().y<<"\n";
        screen->AddDrawable(AddList.front()->Draw);
        AddList.pop();
    }

    while (JointDefList.size()>0)
    {
        world->CreateJoint(JointDefList.front());
        JointDefList.pop();
    }



    while (RemoveList.size()>0)
    {
        List.remove(RemoveList.front());
        world->DestroyBody(RemoveList.front()->Body);
        screen->DeleteDrawable(RemoveList.front()->Draw);
        RemoveList.pop();
    }

    if (MiscClear)
    {
        while (WaterList.size()>0)
        {
            screen->DeleteDrawable(WaterList.front()->Shape);
            WaterList.pop_front();
        }

        MiscClear=false;
    }

    world->Step(step,velo,pos);

    std::list<Instance*>::iterator it;
    for (it = List.begin(); it != List.end(); it++)
    {
        float newAngle = ((-1*(*it)->Body->GetAngle()) * rtod ) - (*it)->Trans->getRotation();
        float newX= (*it)->Body->GetPosition().x*mpp;
        float newY= (*it)->Body->GetPosition().y*mpp;
        //std::cout<<"Updating\t"<<newX<<","<<newY<<"\n";
        if (((newX > (minC))&&(newX < (winWidth+maxC)))&&((newY > (minC))&&(newY < (winHeight+maxC))))
        {
            (*it)->Trans->setPosition(newX,newY);
            (*it)->Trans->rotate(newAngle);
        }
        else
            RemoveList.push((*it));

    }


    //while doing this, should check List bodies if inside watercontent, if so, dampen or something
    std::list<WaterContent*>::iterator wit;
    for (wit= WaterList.begin(); wit != WaterList.end(); wit++)
    {
        (*wit)->ScrollAndUpdate();
        if ((*wit)->firstTime)
        {
            screen->AddDrawable((*wit)->Shape);
            (*wit)->firstTime = false;
        }

    }



}

void PhysicsCore::AddCircle(Context context,Position position,int radius,float density,float restitution,float friction,sf::Color color)
{
    float ppm = 1/50.0f;

    b2BodyDef bodyDef;
    if (context == PhysicsCore::Dynamic)
        bodyDef.type = b2_dynamicBody;

    bodyDef.allowSleep = true;
    bodyDef.awake = true;
    bodyDef.bullet = false;

    //set the bodydef's position in box2d coords
    bodyDef.position.Set(position.x*ppm,position.y*ppm);

    //no angles for now, god.

    b2CircleShape* circle = new b2CircleShape;

    circle->m_radius = radius * ppm;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = circle;
    //apply material info to the fixture
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    sf::CircleShape* circleShape = new sf::CircleShape(radius);
    circleShape->setPosition(position.x,position.y);
    circleShape->setOrigin(radius,radius);
    //no angles for now, god.
    circleShape->setFillColor(color);

    Instance* tI = new Instance;
    tI->Body = world->CreateBody(&bodyDef);//this is the WRONG way to do this, should be done in Update()
    tI->Body->CreateFixture(&fixtureDef);
    tI->Draw = circleShape;
    tI->Trans = circleShape;

    AddList.push(tI);

}

void PhysicsCore::AddSquare(Context context,Position position,int width,int height,float density,float restitution,float friction,sf::Color color)
{
    float ppm = 1/50.0f;

    b2BodyDef bodyDef;
    if (context == PhysicsCore::Dynamic)
        bodyDef.type = b2_dynamicBody;

    bodyDef.allowSleep = true;
    bodyDef.awake = true;
    bodyDef.bullet = false;

     //set the bodydef's position in box2d coords
    bodyDef.position.Set(position.x*ppm,position.y*ppm);

    //no angles for now, god.

    b2PolygonShape* dynamicBox = new b2PolygonShape;

    dynamicBox->SetAsBox((width/2)*ppm, (height/2)*ppm);

    b2FixtureDef fixtureDef;

    fixtureDef.shape = dynamicBox;

    //apply material info to the fixture
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    sf::RectangleShape* rectangle = new sf::RectangleShape(sf::Vector2f(width,height));
    rectangle->setPosition(position.x,position.y);
    rectangle->setOrigin(width/2,height/2);
    //no angles for now, god.
    rectangle->setFillColor(color);

    Instance* tI = new Instance;
    tI->Body = world->CreateBody(&bodyDef);//this is the WRONG way to do this, should be done in Update()
    tI->Body->CreateFixture(&fixtureDef);
    tI->Draw = rectangle;
    tI->Trans = rectangle;

    AddList.push(tI);
}

void PhysicsCore::Clear()
{
    std::list<Instance*>::iterator it;
    for (it = List.begin(); it != List.end(); it++)
    {
        RemoveList.push((*it));
    }

    MiscClear=true;
}

void PhysicsCore::RegisterJointDef(b2JointDef* def)
{
    JointDefList.push(def);
}

void PhysicsCore::RegisterWaterContent(WaterContent* con)
{
    WaterList.push_back(con);
}
