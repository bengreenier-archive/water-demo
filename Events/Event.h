#ifndef EVENT_H
#define EVENT_H

#include <SFML/Graphics.hpp>

class Event
{
    public:
        typedef bool(*EventFunction)(sf::Event evt);
        Event();
        ~Event();
        sf::Event::EventType check;
        sf::Event::EventType condition;
        EventFunction result;
    protected:
    private:
};

#endif // EVENT_H
