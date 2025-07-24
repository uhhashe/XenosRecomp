#include "shader.h"
#include "shader_recompiler.h"
#include "fxc_compiler.h"

static std::unique_ptr<uint8_t[]> readAllBytes(const char* filePath, size_t& fileSize)
{
    FILE* file = fopen(filePath, "rb");
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    auto data = std::make_unique<uint8_t[]>(fileSize);
    fread(data.get(), 1, fileSize, file);
    fclose(file);
    return data;
}

static void writeAllBytes(const char* filePath, const void* data, size_t dataSize)
{
    FILE* file = fopen(filePath, "wb");
    fwrite(data, 1, dataSize, file);
    fclose(file);
}

int main(int argc, char** argv)
{
    std::filesystem::path input(argv[1]);
    std::filesystem::path output(argv[2]);
    const char* includeInput = argv[3];

    size_t includeSize = 0;
    auto includeData = readAllBytes(includeInput, includeSize);
    std::string_view include(reinterpret_cast<const char*>(includeData.get()), includeSize);

    for (auto& inputFile : std::filesystem::directory_iterator(input))
    {
        if ((inputFile.path().extension() == ".xvu") || (inputFile.path().extension() == ".xpu"))
        {
            fmt::println("{}", inputFile.path().string());

            auto outputFile = (output / inputFile.path().filename()).string();
            outputFile[outputFile.size() - 3] = 'w';

            ShaderRecompiler recompiler;
            size_t fileSize;
            recompiler.recompile(readAllBytes(inputFile.path().string().c_str(), fileSize).get(), include);
            ID3DBlob* blob = FxcCompiler::compile(recompiler.out, recompiler.isPixelShader);
            if (blob != nullptr)
            {
                writeAllBytes(outputFile.c_str(), blob->GetBufferPointer(), blob->GetBufferSize());
                blob->Release();
            }

            //writeAllBytes((outputFile + ".hlsl").c_str(), recompiler.out.data(), recompiler.out.size());
        }
    }

    return 0;
}
