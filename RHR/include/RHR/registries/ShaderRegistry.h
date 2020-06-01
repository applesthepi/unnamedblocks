#include <string>
#include <vector>
#include <SFML/Graphics/Shader.hpp>
#include <mutex>

class ShaderRegistry
{
public:
    static void ReloadAllShaders();
    static sf::Shader* GetShader(const std::string& name);
private:
    static bool LoadShader(const std::string& name, const std::string& path);
    static bool ReplaceShader(const std::string& name, const std::string& path);

    static std::vector<std::string> m_shaderUnlocalizedNames;
    static std::vector<sf::Shader*> m_shaders;
    static std::mutex m_mutex;
};