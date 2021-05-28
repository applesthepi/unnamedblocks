#pragma once
#include "ModVin/ModCategories.h"

#include <Espresso/Espresso.h>
#include <mutex>
#include <shared_mutex>
#include <string>

class WindowHandler
{
public:
    WindowHandler(ModBlockPass* pass, sf::RenderWindow* window)
        :m_window(window), m_lastScroll(0.0)
    {
        m_textCallback = [](const sf::Event::TextEvent& ev)
        {
            Logger::Info(std::to_string(ev.unicode));
        };

        m_dealocationCallback = new std::function<void(ModBlockPass*)>();
        *m_dealocationCallback = [this](ModBlockPass*)
        {
            InputHandler::UnregisterTextCallback(&m_textCallback);
            RemoveAllRenderables();
        };

        InputHandler::RegisterTextCallback(&m_textCallback);
        pass->AddDeallocation(m_dealocationCallback);

        m_renderables.reserve(1000);
        m_renderableIndex.reserve(1000);
        m_renderableEnabled.reserve(1000);
    }

    sf::RenderWindow* GetWindow()
    {
        return m_window;
    }

    double PullScroll()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        double scroll = m_lastScroll;
        m_lastScroll = 0.0;

        return scroll;
    }

    void AddScroll(double scroll)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_lastScroll += scroll;
    }

    void AddRenderable(sf::Drawable* renderable, double* idx)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_renderables.push_back(renderable);
        m_renderableEnabled.push_back(false);
        
        *idx = m_renderableIndex.size();
        m_renderableIndex.push_back(idx);
    }

    void RemoveRenderable(double* idx, bool muteWarning = false)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        uint64_t uIdx = *idx;

		m_renderables.erase(m_renderables.begin() + uIdx);
		m_renderableIndex.erase(m_renderableIndex.begin() + uIdx);
		m_renderableEnabled.erase(m_renderableEnabled.begin() + uIdx);

        for (uint64_t i = uIdx; i < m_renderables.size(); i++)
        {
            *m_renderableIndex[i] = i;
        }

        if (!muteWarning)
            Logger::Warn("could not remove renderable that does not exist");
    }

    sf::Drawable* GetRenderable(const double* idx)
    {
        return m_renderables[(uint64_t)*idx];
    }

	void SetRenderableEnabled(uint64_t idx, bool enabled)
	{
        m_renderableEnabled[idx] = enabled;
	}

    // not locking because of performance
    void DrawAll()
    {
        for (uint64_t i = 0; i < m_renderables.size(); i++)
        {
            if (m_renderableEnabled[i])
                m_window->draw(*m_renderables[i]);
        }
    }
private:
    void RemoveAllRenderables()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        for (uint64_t i = 0; i < m_renderables.size(); i++)
            delete m_renderables[i];

        m_renderables.clear();
        m_renderableIndex.clear();
        m_renderableEnabled.clear();
    }

    std::mutex m_mutex;
    sf::RenderWindow* m_window;
    double m_lastScroll;

    std::function<void(const sf::Event::TextEvent&)> m_textCallback;
    std::function<void(ModBlockPass*)>* m_dealocationCallback;

    std::vector<sf::Drawable*> m_renderables;
    std::vector<double*> m_renderableIndex;
    std::vector<bool> m_renderableEnabled;
};

class BlockSystemCreateWindow : public ModBlock
{
public:
    const char* GetUnlocalizedName() const override;
    const char* GetCategory() const override;

    blockExecution PullExecuteDebug() const override;
    blockExecution PullExecuteRelease() const override;

    blockInitialization GetRuntimeGlobalPreInit() const override;
    
    const std::vector<BlockArgumentInitializer> GetArguments() const override;
};
