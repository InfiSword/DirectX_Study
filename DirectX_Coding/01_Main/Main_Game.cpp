#include "../99_Header/Main/pch.h"
#include "Main_Game.h"

Main_Game::Main_Game()
{
}

Main_Game::~Main_Game()
{
}

void Main_Game::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;
	m_width = WinSizeX;
	m_height = WinSizeY;

	// �ش� �������̽� �����͸� �����ϰų� �� ��, ���۷��� ī��Ʈ�� 1����
	// ���� �ֱⰡ �ָ��� ���� Ÿ�̹��� �����ϱ� ����
	// m_device->AddRef();

	// ���۷��� ī��Ʈ�� 1 �ٿ��ְ�, 0�� �Ǹ�, �� ��ü�� ���������� �˾Ƽ� �Ҹ��
	// m_device->Release();

	// �ش� �������� ����, ������ �ϴ� ���� ���ŷӰ�, ������ �۾��̱⿡
	// ����Ʈ �����Ϳ� ���������� �ڵ����� �������ִ°� ������ ������ ����.
	// -> ComPtr<>

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

void Main_Game::Update()
{
}

void Main_Game::Render()
{
	RenderBegin();

	RenderEnd();

}

void Main_Game::RenderBegin()
{
	// ������ ���������̴׿��� Output Merge -> OEM�ܰ�
	// �ش� �׸��� �׸� ��ȭ��(�ĸ� ����)�� �ش� ���ҽ��� ����
	// �ش� ĵ������ �׸��� �׸��� �̶��, ������ ����
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
	
	// �ش� �ڵ�� ĵ������ Ư�� �������� �����ϰ� ����� ����
	// �ĸ� ���ۿ��� ���� �����ӿ� �׷ȴ� �׸��� �״�� �����ֱ⿡, �̰�
	// �����ִ� ������ �Ѵ�.
	// ��, �ĸ� ���ۿ� �׸��� �ش� �������� ���� �о������ ��
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), m_clearColor);	

	// ĵ������ ��� ������ �׸��� �׸��� ����
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void Main_Game::RenderEnd()
{
	// [�ĸ�] -> [����]
	// �ĸ� ������ ������ ���� ���۷� ���ش޶� 
	// ���� ��ü(Swap)�� �����ϴ� �ܰ�, �ĸ� ���ۿ� �׸��� �ϼ��ϰ�,
	// �� �ĸ� ���۸� ����ڰ� ���� �ִ� ���� ���۷� �¹ٲٰ� �ȴ�.
	HRESULT hr =  m_swapChain->Present(1, 0);
	assert(SUCCEEDED(hr));
}

void Main_Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		// ����ü���� ����� ����(�ĸ� ����)�� �ȼ� ������ ������
		// ������ ������ ���ο� ���� ũ�⸦ �츮�� ������ â�� ũ��� �����ϰ� �����Ͽ� �ȼ� ��Ī���� ������ ������� ȭ�鿡 �� ������ ǥ�õǵ��� ��
		desc.BufferDesc.Width = m_width;
		desc.BufferDesc.Height = m_height;

		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		
		// ��Ƽ ���ø� -> ȭ���� �׸� ��, ��� ������ �Ͼ �� �ִ�. ( ������� ) -> �ذ� ����� �ϳ��� �츮�� ���� ȭ���� ũ�⸦ �� �� �������� ũ�� ���� ����, �װſ� ���� ���� ���� ���ϰ�, �ش� ������ ������ ���ϰ� �ȴٸ� ��������� �ش� ��� ������ ��ȭ���ִ� ����� �����Ѵ�. 
		// �̷� �� �ش� Count ���ڸ� �÷����� ���� ����� �� �ִ�.
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		// �ش� ���۸� ��� ����� ���ΰ�
		// -> GPU�� ������ ��ġ��, �������� � ����( ��� ���̴� ���� )�� �׷��� �ϴ��� �˾Ƴ��� ��,
		// �� ���� ������� �׷��ְڴ�. ( �ĸ� ���ۿ� ������ �ϰڴ�. -> ȭ���� �׸��� �뵵 -> ���� ü���� �ĸ� ����)
		// -> ������ ������������ ���� ��� ����� �ǰڴ�.
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// �ش� �ĸ� ���۸� �ϳ��� ����ϰڴ�. 
		desc.BufferCount = 1;
		desc.OutputWindow = m_hWnd;		
		desc.Windowed = true;

		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		
	}

	// ����̽��� �����, �� ������ SwapChain�� �����.
	// �Ѳ����� ���� ����� ������ �Լ�
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,	// �츮�� �ϵ������ �׷���ī�带 ����ϰڴ�.
		nullptr,
		0,
		nullptr,	// D3D_FEATURE_LEVEL ( �ش� DX������ �ش��ϴ� ��ɵ��� �����ؾ� �Ѵٴ� ���� ����ϰ�, �ǳ��ִ� ������, �Է��� ���ϰ� null�� �дٸ�, ���� ������ �� �ִ� ���� ���� �ϳ��� ����ֱ� ������ ��ǻ� null�� �ֵ� ��
		0,
		D3D11_SDK_VERSION,	// ���� 11���� ���� �� ����� ��� ���ϰ� ���� ȯ���� dll ������ ���� ȣȯ�Ǵ��� Ȯ���ϱ� ���� ���� ���� ��
		&desc,
		m_swapChain.GetAddressOf(),
		m_device.GetAddressOf(),
		nullptr,
		m_deviceContext.GetAddressOf()
	);

	assert(SUCCEEDED(hr));

}

void Main_Game::CreateRenderTargetView()
{
	HRESULT hr;

	// �ĸ���ۿ��� �ش� ���ҽ��� Texture2D�� ������ -> �� ĵ����
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());

	assert(SUCCEEDED(hr));

	// �ĸ� ���ۿ�, �ش� backBuffer ĵ������ �����ؼ�, ������ �׸� ��ȭ����
	// ����Ŵ
	m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());

	assert(SUCCEEDED(hr));

}

void Main_Game::SetViewport()
{
	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.Width = static_cast<float>(m_width);
	m_viewport.Height = static_cast<float>(m_height);
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;


}
