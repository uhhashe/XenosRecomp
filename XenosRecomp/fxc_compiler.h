#pragma once

struct FxcCompiler
{
    static ID3DBlob* compile(const std::string& shaderSource, bool compilePixelShader);
};
