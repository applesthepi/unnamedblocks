#include "ShaderRegistry.h"
#include "util.h"

#include <Espresso/Global.h>
#include <Cappuccino/Logger.h>
#include <Espresso/util.h>

void ShaderRegistry::Initialize()
{
    m_shaders = new std::vector<sf::Shader*>();
    m_shaderUnlocalizedNames = new std::vector<std::string>();
    
    if (!LoadShader("nvidia_dots", "dots"))
        Logger::Error("failed to load shader");
}

void ShaderRegistry::ReloadAllShaders()
{
    std::string prefix;
    if (Global::GpuType == GpuMan::INTEL)
        prefix = "intel_";
    else if (Global::GpuType == GpuMan::NVIDIA)
        prefix = "nvidia_";
    else
    {
        Logger::Error("failed to reload shaders; unknown gpu type");
        return;
    }
    
    for (unsigned int i = 0; i < m_shaderUnlocalizedNames->size(); i++)
        ReplaceShader((*m_shaderUnlocalizedNames)[i], prefix + (*m_shaderUnlocalizedNames)[i]);
}

bool ShaderRegistry::LoadShader(std::string path, std::string unlocalizedName)
{
    m_mutex.lock();

    sf::Shader* shader = new sf::Shader();
    if (!shader->loadFromFile(get_runtime_path() + "/res/shaders/" + path + ".vs", get_runtime_path() + "/res/shaders/" + path + ".fs"))
    {
        Logger::Error("failed to load shader; shaders \"" + path + "\" does not exist!");
        
        m_mutex.unlock();
        return false;
    }
    
    m_shaders->push_back(shader);
    m_shaderUnlocalizedNames->push_back(unlocalizedName);

    m_mutex.unlock();
    return true;
}

bool ShaderRegistry::ReplaceShader(std::string unlocalizedName, std::string path)
{
    m_mutex.lock();

    for (unsigned int i = 0; i < m_shaderUnlocalizedNames->size(); i++)
    {
        if ((*m_shaderUnlocalizedNames)[i] == unlocalizedName)
        {
            sf::Shader* shader = new sf::Shader();
            if (!shader->loadFromFile(get_runtime_path() + "/res/shaders/" + path + ".vs", get_runtime_path() + "/res/shaders/" + path + ".fs"))
            {
                Logger::Error("failed to load shader; shaders \"" + path + "\" does not exist!");
                
                m_mutex.unlock();
                return false;
            }

            delete (*m_shaders)[i];
            (*m_shaders)[i] = shader;
            (*m_shaderUnlocalizedNames)[i] = unlocalizedName;

            m_mutex.unlock();
            return true;
        }
    }

    m_mutex.unlock();
    return false;
}

sf::Shader* ShaderRegistry::GetShader(std::string unlocalizedName)
{
    m_mutex.lock();

    for (unsigned int i = 0; i < m_shaderUnlocalizedNames->size(); i++)
    {
        if ((*m_shaderUnlocalizedNames)[i] == unlocalizedName)
        {
            sf::Shader* ref = (*m_shaders)[i];

            m_mutex.unlock();
            return ref;
        }
    }

    m_mutex.unlock();
    return nullptr;
}

std::vector<std::string>* ShaderRegistry::m_shaderUnlocalizedNames;
std::vector<sf::Shader*>* ShaderRegistry::m_shaders;
std::mutex ShaderRegistry::m_mutex;
