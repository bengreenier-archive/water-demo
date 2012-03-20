#ifndef SCREENCORE_H
#define SCREENCORE_H

#include "../Events/EventsCore.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <list>
#include <queue>
class ScreenCore
{
    public:
        ScreenCore(sf::RenderWindow* pwin);
        ~ScreenCore();
        void DrawAll();
        static void ThreadDraw(ScreenCore* ref)
        {
            sf::Context context;
            ref->win->setActive(true);
            while(1)
            {
                ref->STDMutex.lock();

                if (!ref->StopThreadDraw)
                {
                    ref->DrawAll();
                }
                else
                {
                    break;
                }
                ref->STDMutex.unlock();

            }

            ref->win->setActive(false);
            ref->GoingDownNow=true;
            //ref->win->close();
            return;
        }
        static void ThreadSTDMutexForce(ScreenCore* ref)
        {
            while(1)
            {
                ref->STDMutex.lock();

                if (ref->StopThreadDraw)
                {
                    ref->STDMutex.unlock();
                    return;
                }
                else
                    ref->StopThreadDraw=true;

                ref->STDMutex.unlock();
            }
        }
        void AddDrawable(sf::Drawable* in);
        void Clear();
        bool DeleteDrawable(sf::Drawable* in);
        sf::RenderWindow* AccessWindow();
        EventsCore* Events;
        void SetClearColor(sf::Color in);
        bool StopThreadDraw,GoingDownNow;
        sf::Mutex STDMutex;
    protected:
    private:
        sf::RenderWindow* win;
        sf::Mutex mutex,mdrmutex,mdqmutex;
        std::list<sf::Drawable*> DrawableList;
        sf::Color m_clear_color;
        bool m_MidDraw;
        std::queue<sf::Drawable*> m_MidDrawRemove,m_MidDrawQueue;
        std::string title;
};

#endif // CORE_H
