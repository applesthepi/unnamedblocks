#include "shader.hpp"

// void ShaderRegistry::ReloadAllShaders()
// {
//     m_shaderUnlocalizedNames.clear();
    
//     for (u8 i = 0; i < m_shaders.size(); i++)
//         delete m_shaders[i];

//     m_shaders.clear();

//     LoadShader("dots", "res/shaders/dots");
// }

// bool ShaderRegistry::LoadShader(const std::string& name, const std::string& path)
// {
//     m_mutex.lock();

//     sf::Shader* shader = new sf::Shader();
//     if (!shader->loadFromFile(path + ".vs", path + ".fs"))
//     {
//         Logger::Error("failed to load shader; shader \"" + path + "\" does not exist!");
        
//         m_mutex.unlock();
//         return false;
//     }
    
//     m_shaders.push_back(shader);
//     m_shaderUnlocalizedNames.push_back(name);

//     m_mutex.unlock();
//     return true;
// }

// bool ShaderRegistry::ReplaceShader(const std::string& name, const std::string& path)
// {
//     m_mutex.lock();

//     for (unsigned int i = 0; i < m_shaderUnlocalizedNames.size(); i++)
//     {
//         if (m_shaderUnlocalizedNames[i] == name)
//         {
//             sf::Shader* shader = new sf::Shader();
//             if (!shader->loadFromFile(path + ".vs", path + ".fs"))
//             {
//                 Logger::Error("failed to load shader; shaders \"" + path + "\" does not exist!");
                
//                 m_mutex.unlock();
//                 return false;
//             }

//             delete m_shaders[i];
//             m_shaders[i] = shader;

//             m_mutex.unlock();
//             return true;
//         }
//     }

//     m_mutex.unlock();
//     return false;
// }

// sf::Shader* ShaderRegistry::GetShader(const std::string& name)
// {
//     m_mutex.lock();

//     for (unsigned int i = 0; i < m_shaderUnlocalizedNames.size(); i++)
//     {
//         if (m_shaderUnlocalizedNames[i] == name)
//         {
//             sf::Shader* ref = m_shaders[i];

//             m_mutex.unlock();
//             return ref;
//         }
//     }

//     m_mutex.unlock();
//     return nullptr;
// }

// std::vector<std::string> ShaderRegistry::m_shaderUnlocalizedNames;
// std::vector<sf::Shader*> ShaderRegistry::m_shaders;
// std::mutex ShaderRegistry::m_mutex;