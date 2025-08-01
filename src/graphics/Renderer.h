#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include "math/Matrix4.h"
#include "Shader.h"

using Microsoft::WRL::ComPtr;

struct ConstantBuffer {
    Matrix4 world;
    Matrix4 view;
    Matrix4 projection;
};

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Initialize(HWND hwnd, int width, int height);
    void Clear(float r = 0.1f, float g = 0.1f, float b = 0.1f, float a = 1.0f);
    void Present();
    void Resize(int width, int height);

    void SetMatrices(const Matrix4& world, const Matrix4& view, const Matrix4& projection);
    void UpdateConstantBuffer();

    ID3D11Device* GetDevice() const { return m_device.Get(); }
    ID3D11DeviceContext* GetContext() const { return m_context.Get(); }

    ComPtr<ID3D11InputLayout> CreateInputLayoutForShader(Shader* shader);
    void SetDefaultDepthStencilState();

    int GetWidth() const {
        return m_width;
    }

    int GetHeight() const {
        return m_height;
    }

private:
    bool CreateDeviceAndSwapChain(HWND hwnd, int width, int height);
    bool CreateRenderTargetView();
    bool CreateDepthStencilView(int width, int height);
    bool CreateConstantBuffer();

    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    ComPtr<ID3D11Buffer> m_constantBuffer;
    ComPtr<ID3D11RasterizerState> m_rasterizerState;
    ComPtr<ID3D11DepthStencilState> m_depthStencilState;

    ConstantBuffer m_cbData;
    int m_width;
    int m_height;
};
