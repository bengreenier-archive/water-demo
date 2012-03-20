#include "ScreenCore.h"

ScreenCore::ScreenCore(sf::RenderWindow* pwin)
{
    //ctor
    win=pwin;
    m_clear_color = sf::Color(0,0,0);
    StopThreadDraw=false;
    GoingDownNow=false;
    m_MidDraw=false;

}

ScreenCore::~ScreenCore()
{
    //dtor
}

void ScreenCore::DrawAll()
{


    if(win->isOpen())
    {
        win->clear(m_clear_color);
        mutex.lock();
        if (DrawableList.size()>0)
        {
            m_MidDraw=true;
            std::list<sf::Drawable*>::iterator it;
            for ( it=DrawableList.begin() ; it != DrawableList.end(); it++ )
                win->draw(*(*it));

            m_MidDraw=false;
        }
        mdqmutex.lock();
        while (m_MidDrawQueue.size()>0)
            {
                DrawableList.push_back(m_MidDrawQueue.front());
                m_MidDrawQueue.pop();
            }
        mdqmutex.unlock();
        mdrmutex.lock();
        while (m_MidDrawRemove.size()>0)
            {
                DrawableList.remove(m_MidDrawRemove.front());
                m_MidDrawRemove.pop();
            }
        mdrmutex.unlock();
        mutex.unlock();
        win->display();
    }
}

void ScreenCore::AddDrawable(sf::Drawable* in)
{
    if (!m_MidDraw)
    {
        mutex.lock();
        DrawableList.push_back(in);
        mutex.unlock();
    }
    else
    {
        mdqmutex.lock();
        m_MidDrawQueue.push(in);
        mdqmutex.unlock();
    }

}

bool ScreenCore::DeleteDrawable(sf::Drawable* in)
{
    if (!m_MidDraw)
    {
        mutex.lock();
        DrawableList.remove(in);
        mutex.unlock();
    }
    else
    {
        mdrmutex.lock();
        m_MidDrawRemove.push(in);
        mdrmutex.unlock();
    }

}

sf::RenderWindow* ScreenCore::AccessWindow()
{
    return win;
}

void ScreenCore::SetClearColor(sf::Color in)
{
    m_clear_color = in;
}

void ScreenCore::Clear()//this wont always work
{
        mutex.lock();
        DrawableList.clear();
        mutex.unlock();
}
