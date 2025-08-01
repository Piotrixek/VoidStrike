#include "Renderer.h"
#include "Mesh.h"
#include <iostream>

Renderer::Renderer() : m_width(0), m_height(0) {
}

Renderer::~Renderer() {
}

bool Renderer::Initialize(HWND hwnd, int width, int height) {
    m_width = width;
    m_height = height;

    if (!CreateDeviceAndSwapChain(hwnd, width, height)) return false;
    if (!CreateRenderTargetView()) return false;
    if (!CreateDepthStencilView(width, height)) return false;
    if (!CreateConstantBuffer()) return false;

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    m_context->RSSetViewports(1, &viewport);

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = TRUE;
    rasterizerDesc.DepthClipEnable = TRUE;

    m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerState.GetAddressOf());
    m_context->RSSetState(m_rasterizerState.Get());

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());
    SetDefaultDepthStencilState();

    return true;
}

bool Renderer::CreateDeviceAndSwapChain(HWND hwnd, int width, int height) {
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevels,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        nullptr,
        m_context.GetAddressOf()
    );

    return SUCCEEDED(hr);
}

bool Renderer::CreateRenderTargetView() {
    ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
    return SUCCEEDED(hr);
}

bool Renderer::CreateDepthStencilView(int width, int height) {
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ComPtr<ID3D11Texture2D> depthStencilBuffer;
    HRESULT hr = m_device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf());
    if (FAILED(hr)) return false;

    hr = m_device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, m_depthStencilView.GetAddressOf());
    return SUCCEEDED(hr);
}

bool Renderer::CreateConstantBuffer() {
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(ConstantBuffer);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.GetAddressOf());
    return SUCCEEDED(hr);
}

void Renderer::Clear(float r, float g, float b, float a) {
    float clearColor[4] = { r, g, b, a };
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}

void Renderer::Present() {
    m_swapChain->Present(1, 0);
}

void Renderer::Resize(int width, int height) {
    if (width == 0 || height == 0 || (width == m_width && height == m_height)) return;

    m_width = width;
    m_height = height;

    m_context->OMSetRenderTargets(0, nullptr, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();

    m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

    CreateRenderTargetView();
    CreateDepthStencilView(width, height);

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_context->RSSetViewports(1, &viewport);
}

void Renderer::SetMatrices(const Matrix4& world, const Matrix4& view, const Matrix4& projection) {
    m_cbData.world = world.transpose();
    m_cbData.view = view.transpose();
    m_cbData.projection = projection.transpose();
}

void Renderer::UpdateConstantBuffer() {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = m_context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr)) {
        memcpy(mappedResource.pData, &m_cbData, sizeof(ConstantBuffer));
        m_context->Unmap(m_constantBuffer.Get(), 0);
    }

    m_context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
}

ComPtr<ID3D11InputLayout> Renderer::CreateInputLayoutForShader(Shader* shader) {
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    ComPtr<ID3D11InputLayout> inputLayout;
    HRESULT hr = m_device->CreateInputLayout(
        layout,
        2,
        shader->GetVertexShaderBlob()->GetBufferPointer(),
        shader->GetVertexShaderBlob()->GetBufferSize(),
        inputLayout.GetAddressOf()
    );

    if (SUCCEEDED(hr)) {
        return inputLayout;
    }

    return nullptr;
}

void Renderer::SetDefaultDepthStencilState() {
    m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
}
