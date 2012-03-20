#ifndef EVENTSCORE_H
#define EVENTSCORE_H

#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>

#include "Event.h"

class EventsCore
{
    public:
        EventsCore(sf::RenderWindow* pwin);
        ~EventsCore();

        void ExecutePoll();
        bool Check();
    protected:
    private:
    sf::RenderWindow* win;
    std::list<Event*> EventList;
};

#endif // EVENTSCORE_H
