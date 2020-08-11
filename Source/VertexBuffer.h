#pragma once

#include "D3D11System.h"

class VertexBuffer
{
public:
	VertexBuffer(const void* data, size_t bufferSize)
		:m_bufferSize(bufferSize)
	{
		bool init_vertexBuffer = initD3DVertexBuffer(data, bufferSize);
		assert(init_vertexBuffer);
	}
	~VertexBuffer() {}
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer) = delete;

public:
	ComPtr<ID3D11Buffer> getD3D11Buffer()
	{
		return m_d3dBuffer;
	}

	size_t getBufferSize()
	{
		return m_bufferSize;
	}

private:
	bool initD3DVertexBuffer(const void* data, size_t bufferSize)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = bufferSize;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = data;
		sd.SysMemPitch = bufferSize;
		sd.SysMemSlicePitch = 0;

		HRESULT result =
			D3D11System::Device()->CreateBuffer(&bd, &sd, &m_d3dBuffer);
		return SUCCEEDED(result);
	}

private:
	ComPtr<ID3D11Buffer> m_d3dBuffer;
	size_t m_bufferSize;


};
