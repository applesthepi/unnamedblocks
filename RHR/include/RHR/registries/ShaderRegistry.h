#include <string>
#include <vector>
#include <SFML/Graphics/Shader.hpp>
#include <mutex>

class ShaderRegistry
{
public:
    static void Initialize();
    static void ReloadAllShaders();
    static sf::Shader* GetShader(std::string unlocalizedName);
private:
    static bool LoadShader(std::string path, std::string unlocalizedName);
    static bool ReplaceShader(std::string unlocalizedName, std::string path);

    static std::vector<std::string>* m_shaderUnlocalizedNames;
    static std::vector<sf::Shader*>* m_shaders;
    static std::mutex m_mutex;
};