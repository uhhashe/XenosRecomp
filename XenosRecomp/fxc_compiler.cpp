#include "fxc_compiler.h"

ID3DBlob* FxcCompiler::compile(const std::string& shaderSource, bool compilePixelShader)
{
    ID3DBlob* code = nullptr;
    ID3DBlob* errorMsgs = nullptr;

    HRESULT result = D3DCompile(
        shaderSource.data(),
        shaderSource.size(),
        nullptr,
        nullptr,
        nullptr,
        "main",
        compilePixelShader ? "ps_3_0" : "vs_3_0",
        0,
        0,
        &code,
        &errorMsgs);

    if (FAILED(result) && errorMsgs != nullptr) 
        fputs(reinterpret_cast<const char*>(errorMsgs->GetBufferPointer()), stderr);

    if (errorMsgs != nullptr)
        errorMsgs->Release();

    return code;
}
