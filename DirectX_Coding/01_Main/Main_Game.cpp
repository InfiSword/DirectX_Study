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

	
}

void Main_Game::Update()
{
}

void Main_Game::Render()
{
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

		// 
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		
	}

	// ����̽��� �����, �� ������ SwapChain�� �����.
	// �Ѳ����� ���� ����� ������ �Լ�
	//D3D11CreateDeviceAndSwapChain(
	//	nullptr,
	//	D3D_DRIVER_TYPE_HARDWARE,	// �츮�� �ϵ������ �׷���ī�带 ����ϰڴ�.
	//	nullptr,
	//	0,
	//	nullptr,	// D3D_FEATURE_LEVEL ( �ش� DX������ �ش��ϴ� ��ɵ��� �����ؾ� �Ѵٴ� ���� ����ϰ�, �ǳ��ִ� ������, �Է��� ���ϰ� null�� �дٸ�, ���� ������ �� �ִ� ���� ���� �ϳ��� ����ֱ� ������ ��ǻ� null�� �ֵ� ��
	//	0,
	//	D3D11_SDK_VERSION,	// ���� 11���� ���� �� ����� ��� ���ϰ� ���� ȯ���� dll ������ ���� ȣȯ�Ǵ��� Ȯ���ϱ� ���� ���� ���� ��



	//)
}
