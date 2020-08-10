#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <assert.h>

class D3D11CBuffer
{
public:
	D3D11CBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, size_t bufferSize)
		:r_device(device), r_context(context), m_bufferSize(bufferSize)
	{
		assert(bufferSize % 16 == 0);
		bool init_D3D11CBuffer = initD3D11Buffer();
		assert(init_D3D11CBuffer);
	}
	virtual ~D3D11CBuffer()
	{}
	D3D11CBuffer(const D3D11CBuffer&) = delete;
	D3D11CBuffer& operator=(const D3D11CBuffer&) = delete;

public:
	void transferData(void* data, size_t dataSize)
	{
		assert(dataSize > m_bufferSize);
		D3D11_MAPPED_SUBRESOURCE mp = {};
		r_context->Map(m_constBuffer.Get(), 0, D3D11_MAP_WRITE, D3D11_MAP_WRITE_DISCARD, &mp);
		memcpy_s(mp.pData, dataSize, data, dataSize);
		r_context->Unmap(m_constBuffer.Get(), 0);
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> getConstBuffer()
	{
		return m_constBuffer;
	}

private:
	bool initD3D11Buffer()
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = m_bufferSize;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.Usage = D3D11_USAGE_DYNAMIC;

		HRESULT result =
			r_device->CreateBuffer(&bd, NULL, &m_constBuffer);
		return SUCCEEDED(result);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Device> r_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> r_context;
	size_t m_bufferSize;
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constBuffer;

};
