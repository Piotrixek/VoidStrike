#include "Mesh.h"

Mesh::Mesh() : m_indexCount(0), m_stride(sizeof(Vertex)), m_offset(0) {
}

Mesh::~Mesh() {
}

bool Mesh::Initialize(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    // vertex Buffer
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(vertices.size());
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices.data();

    HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf());
    if (FAILED(hr)) return false;

    // index Buffer
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size());
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices.data();

    hr = device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf());
    if (FAILED(hr)) return false;

    m_indexCount = static_cast<UINT>(indices.size());

    return true;
}

void Mesh::Render(ID3D11DeviceContext* context) {
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_stride, &m_offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexed(m_indexCount, 0, 0);
}

Mesh Mesh::CreateCube(ID3D11Device* device, const Vector3& color) {
    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, -0.5f}, color},
        {{ 0.5f, -0.5f, -0.5f}, color},
        {{ 0.5f,  0.5f, -0.5f}, color},
        {{-0.5f,  0.5f, -0.5f}, color},
        {{-0.5f, -0.5f,  0.5f}, color},
        {{ 0.5f, -0.5f,  0.5f}, color},
        {{ 0.5f,  0.5f,  0.5f}, color},
        {{-0.5f,  0.5f,  0.5f}, color}
    };

    std::vector<unsigned int> indices = {
        0, 2, 1, 0, 3, 2, // Front Face
        1, 6, 5, 1, 2, 6, // Right Face
        5, 7, 4, 5, 6, 7, // Back Face
        4, 3, 0, 4, 7, 3, // Left Face
        3, 6, 2, 3, 7, 6, // Top Face
        0, 5, 4, 0, 1, 5  // Bottom Face
    };

    Mesh mesh;
    mesh.Initialize(device, vertices, indices);
    return mesh;
}

Mesh Mesh::CreateQuad(ID3D11Device* device, const Vector3& color) {
    std::vector<Vertex> vertices = {
        {{-1.0f, -1.0f, 0.0f}, color},
        {{ 1.0f, -1.0f, 0.0f}, color},
        {{ 1.0f,  1.0f, 0.0f}, color},
        {{-1.0f,  1.0f, 0.0f}, color}
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0
    };

    Mesh mesh;
    mesh.Initialize(device, vertices, indices);
    return mesh;
}
