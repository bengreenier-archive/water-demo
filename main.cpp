#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Physics/PhysicsCore.h"
#include "Screen/ScreenCore.h"
#include "Events/EventsCore.h"
#include "WaterChain.h"
#include "WaterContent.h"
int main()
{
    sf::Context context;

    sf::RenderWindow* rWindow = new sf::RenderWindow(sf::VideoMode(1920, 1080),"Water Demo");
    rWindow->setActive(false);

    ScreenCore* sCore = new ScreenCore(rWindow);
    PhysicsCore* pCore = new PhysicsCore(1.0f/60.0f,6,2,500,0,10);
    EventsCore* eCore = new EventsCore(rWindow);
    sCore->Events = eCore;

    sf::Thread screenloop(&ScreenCore::ThreadDraw,sCore);
    sf::Thread screenquit(&ScreenCore::ThreadSTDMutexForce,sCore);

    screenloop.launch();

sf::Clock clock,clock2;
clock.restart();
clock2.restart();
    while(1)
    {

       /* if (clock2.getElapsedTime().asSeconds()>=2.0f)
        {
            PhysicsCore::Position posy;
            posy.x = 110;
            posy.y = 110;
            pCore->AddCircle(PhysicsCore::Dynamic,posy,10,2,0.3f,0.002f,sf::Color::Green);
            clock2.restart();
        }*/

                sf::Event event;
                while (rWindow->pollEvent(event))
                {
                    if ((event.type == sf::Event::Closed)||((event.type == sf::Event::KeyReleased)&&(event.key.code == sf::Keyboard::Escape)))
                    {
                        screenquit.launch();
                        sf::Text* poweroff = new sf::Text("Initiating Shutdown, Please hold");
                        poweroff->setColor(sf::Color::White);
                        poweroff->setPosition(150,150);
                        sCore->AddDrawable(poweroff);
                    }
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        PhysicsCore::Position posd;
                        posd.x = sf::Mouse::getPosition().x;
                        posd.y = sf::Mouse::getPosition().y;
                        pCore->AddCircle(PhysicsCore::Dynamic,posd,10,2,0.3f,0.002f,sf::Color::Green);
                    }
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
                        PhysicsCore::Position pos;
                        pos.x = sf::Mouse::getPosition().x;
                        pos.y = sf::Mouse::getPosition().y;
                        pCore->AddSquare(PhysicsCore::Dynamic,pos,10,10,1,0.3f,0.002f,sf::Color::Blue);
                    }
                    if ((event.type == sf::Event::KeyReleased)&&(event.key.code == sf::Keyboard::C))
                    {
                        pCore->Clear();
                    }
                    if ((event.type == sf::Event::KeyReleased)&&(event.key.code == sf::Keyboard::W))
                    {
                        PhysicsCore::Position posa,posb;
                        posa.x=1;
                        posa.y=300;
                        posb.x=300;
                        posb.y=300;
                        WaterChain wchain(sCore,pCore,posa,posb,50); //attempt to waterchain make
                        pCore->RegisterWaterContent(new WaterContent(wchain.GetChainedBodies(),100));
                    }

                }

        //this is suposed to be more stable, brings one pCore update per second max.
        if (clock.getElapsedTime().asSeconds()>=0.01f)
        {
            pCore->Update(sCore);
            clock.restart();
        }

        //this says, windows going down, fuck this while loop
        if (sCore->GoingDownNow)
            break;

    }


    //this order is important!
    screenquit.wait();
    screenloop.wait();
    pCore->Clear();
    sCore->Clear();
    rWindow->setActive(false);
    rWindow->close();
    return 0;
}
