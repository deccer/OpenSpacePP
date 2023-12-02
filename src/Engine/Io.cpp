#include <Engine/Io.hpp>

#include <format>
#include <fstream>

namespace Io
{

std::expected<std::string, std::string> ReadTextFromFile(std::string_view filePath)
{
    std::ifstream file(filePath.data(), std::ios::ate);
    if (file.bad())
    {
        return std::unexpected(std::format("Io: File {} does not exist", filePath));
    }
    auto fileSize = file.tellg();
    if (fileSize == 0)
    {
        return std::unexpected(std::format("Io: File {} is empty", filePath));
    }

    std::string result(fileSize, '\0');
    file.seekg(0);
    file.read((char*)result.data(), result.size());
    return result;
}

}