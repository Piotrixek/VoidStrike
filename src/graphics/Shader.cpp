#include "Shader.h"
#include <iostream>

Shader::Shader() {
}

Shader::~Shader() {
}

bool Shader::LoadVertexShader(ID3D11Device* device, const std::wstring& filename) {
    ComPtr<ID3DBlob> errorBlob;
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        m_vertexShaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr)) {
        if (errorBlob) {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        }
        return false;
    }

    hr = device->CreateVertexShader(
        m_vertexShaderBlob->GetBufferPointer(),
        m_vertexShaderBlob->GetBufferSize(),
        nullptr,
        m_vertexShader.GetAddressOf()
    );

    return SUCCEEDED(hr);
}

bool Shader::LoadPixelShader(ID3D11Device* device, const std::wstring& filename) {
    ComPtr<ID3DBlob> errorBlob;
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        m_pixelShaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr)) {
        if (errorBlob) {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        }
        return false;
    }

    hr = device->CreatePixelShader(
        m_pixelShaderBlob->GetBufferPointer(),
        m_pixelShaderBlob->GetBufferSize(),
        nullptr,
        m_pixelShader.GetAddressOf()
    );

    return SUCCEEDED(hr);
}

void Shader::Bind(ID3D11DeviceContext* context) {
    context->IASetInputLayout(m_inputLayout.Get());
    context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}
