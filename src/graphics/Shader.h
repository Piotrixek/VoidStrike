#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Shader {
public:
    Shader();
    ~Shader();

    bool LoadVertexShader(ID3D11Device* device, const std::wstring& filename);
    bool LoadPixelShader(ID3D11Device* device, const std::wstring& filename);

    void Bind(ID3D11DeviceContext* context);

    ID3D11VertexShader* GetVertexShader() const { return m_vertexShader.Get(); }
    ID3D11PixelShader* GetPixelShader() const { return m_pixelShader.Get(); }
    ID3DBlob* GetVertexShaderBlob() const { return m_vertexShaderBlob.Get(); }

    void SetInputLayout(ComPtr<ID3D11InputLayout> layout) { m_inputLayout = layout; }

private:
    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3DBlob> m_vertexShaderBlob;
    ComPtr<ID3DBlob> m_pixelShaderBlob;
    ComPtr<ID3D11InputLayout> m_inputLayout;
};
