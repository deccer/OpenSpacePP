#pragma once

#include <expected>
#include <string_view>

namespace Io
{
    std::expected<std::string, std::string> ReadTextFromFile(std::string_view filePath);
}