#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "math/Vector3.h"

using Microsoft::WRL::ComPtr;

struct Vertex {
    Vector3 position;
    Vector3 color;
};

class Mesh {
public:
    Mesh();
    ~Mesh();

    bool Initialize(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void Render(ID3D11DeviceContext* context);

    static Mesh CreateCube(ID3D11Device* device, const Vector3& color = Vector3(1.0f, 1.0f, 1.0f));
    static Mesh CreateQuad(ID3D11Device* device, const Vector3& color = Vector3(1.0f, 1.0f, 1.0f));

private:
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;

    UINT m_indexCount;
    UINT m_stride;
    UINT m_offset;
};
