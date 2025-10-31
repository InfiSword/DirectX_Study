#pragma once
class Main_Game
{
public:
	Main_Game();
	~Main_Game();

public:
	void Init(HWND _hWnd);
	void Update();
	void Render();

private:
	void RenderBegin();
	void RenderEnd();
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

private:
	void CreateGeometry();
	void CreateInputLayout();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);
	
	void CreateVS();
	void CreatePS();

private:
	HWND m_hWnd;
	uint32 m_width = 0;
	uint32 m_height = 0;

private:
	// DX
	// COM(Component Object Model)�̶�
	// Platform-independent, Distributed, Object-oriented
	// COM��ü�� -> ��) ID3D11Device �������̽� ������ �޸� �ּ�
	// �ش� �������̽����� ���������Լ��� ������� ������, �츮�� �ش� �Լ��� ������ �����
	// �ش� �Լ��� ���� �����δ� OS�� �����Ѵ�. ( dll���� �������� COM ����ü�� ������ ���� )

	// ������
	// 1. COM ��Ÿ�� �ʱ�ȭ �� ��ü ���� ��û
	//		1) COM���� ���̺귯��(ole32.dll, DirectX ����) �ʱ�ȭ -> COM ��ü�� ����ϰ��� �ϴ� �� �����忡 ó������ �ʱ�ȭ�� �̷�����.(CoInitializeEx -> ole32.dll�� ���ԵǾ� ����)
	//		2) COM ��ü ���� ��û -> COM ��ü�� ������ �ʿ������� ������ �ش� ��ü ������ ��û�Ѵ�. (CoCreateInstance -> ole32.dll�� ���ԵǾ� ����)
	//		* ole32.dll�� COM�� �����ϴ� �ٽ� ���̺귯������, OLE ����� ����ϴ� ���̺귯���̴�.
	//		* COM��ü ����, ��ü �Ҹ�, ������ Ŭ������ �������̽� ��ȸ, ������ ������ �ش� ole32.dll�� �����Ѵ�. 

	// 2. Windows COM ��Ÿ�� (ole32.dll)�� ����
	//		1) ������Ʈ�� ��ȸ  -> CoCreateInstance �Լ��� ���α׷����� ��û�� �ش� COM ��ü�� CLSID(class ID, �������̽��� �����ϰ� �ִ� ���� C++ Ŭ������ ���� �ĺ���[GUID])�� ������ Windows ������Ʈ��( Windows �ü���� �߾� ���� �����ͺ��̽�)�� ��ȸ�Ѵ�.
	// �ش� ������Ʈ������ �� CLSID���� �ش� COM ��ü�� ���� ���� �ڵ尡 ��� DLL ������ ��ο� �̸��� ��Ȯ�� ��ϵǾ� �ִ�.
	// �ش� ������Ʈ�� ������ �ٷ� COM ��Ÿ���� � DLL ���Ͽ� ���α׷��� ��û�� COM �������̽� �Լ����� vtable(���� �Լ����� ���� ���� �ڵ��� �޸� �ּ�)�� �����ϴ� �ڵ尡 �ִ��� �ľ��ϴ� ���� ������ �Ѵ�.
	//		2) DLL �ε� -> o1e32.dll�� �ش� ������Ʈ������ ���� ������ �������� �����ڵ�(d3d11.dll)������ ã�� ���α׷��� ����Ǵ� �޸� ���� ������ �ε��Ѵ�.  

	// 3. ��ü ����
	//		1) d3d11.dll���� DllGetClassObject�̶� Ư�� �Լ��� ã�� ȣ���Ѵ�. �� ��, �ش� �Լ��� DLL�� COMŬ������ ��üȭ�� �� �ֵ��� ���ش�.
	//		2) �ش� �Լ��� ���ؼ�, Ư�� CLSID�� �ش��ϴ� Ŭ���� ���丮�� �����ϰ�, �� Ŭ���� ���丮�� IClassFactory �������̽� �����͸� ole32.dll���� ��ȯ�Ѵ�.
	//		3) ole32.dll�� ���� �� Ŭ���� ���丮�� CreateInstance �޼��带 ȣ���Ͽ�, �ش� CreateInstance ���ο��� �ش� ���α׷����� ȣ��� �������̽��� �����ϴ� ���� C++Ŭ������ ��ü�� �޸𸮿� �Ҵ��ϰ� �ʱ�ȭ�Ѵ�. ���⼭ �ش� ���� ������ Ŭ������ �ٷ� COM��ü�� ���̴�. �ᱹ, �ش� �������̽� �������� �޸𸮴� ���� ������ C++ Ŭ���� ��ü(COM ��ü)�� ����Ű�� �ȴ�.	
	//		4) �� ��, vtable�� �ش� Ŭ�������� 1���� �����ǰ� �Ǹ�, �ش� Ŭ������ ��ü���� vpointer�� �����, �ش� vpointer�� �ش� Ŭ������ vtable�� ����Ű�� �ȴ�. ���� � ��ü�� �����Լ��� ȣ���ϸ�, �ش� ��ü�� �޸𸮿��� vpointer�� �о��, vtable�� �����Ͽ�, ȣ���Ϸ��� �����Լ��� �ش��ϴ� �Լ� ������(�ش� ���� �Լ��� ������)�� �����´�. 
	
	// 4. �������̽� ������ ��ȯ�� ����
	//		1) CreateInstance�� �ش� ������ ��ü�� �ּ�, �� vpointer�� �ִ� �ּҸ� �ش� ���α׷��� ȣ���� �������̽� ������ Ÿ������ ĳ�����Ͽ� ole32.dll���� ��ȯ�Ѵ�.
	//		2) ole32.dll�� �� �����͸� �ٽ� �츮 ���α׷��� ������ �������̽� ������ ������ ����ȴ�.
	//		3) �� ���Ŀ��� �ش� �������̽��� �Լ��� ȣ����� �츮�� �ƴ� C++���ó�� ���۽�Ų��.


	// Device & SwapChain
	// ����̽� ���� ���� ������ �� ���
	ComPtr<ID3D11Device> m_device = nullptr;
	// ������ ���������ο� ������ �� �����
	ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;	
	
	// DXGI�̶�� ���� �ٲ����ʴ� ȭ��ó�� ����� ���� ������ ������
	// DX9, 11, 12 ������� DXGI�� ���������� ����� �� �����Ƿ� ���� ��������
	// SwapChain�̶�? -> ���� ���۸����� ������
	// �ĸ� ���ۿ� GPU�� ���� ����� �׸��� -> �� �� ����ڴ� ���� ���۸� ���� �ִ´�. -> ����� �� �׸���, �� �� �ش� GPU�� ��� �׸��� �� �׸� �ĸ� ���۸� ���ο� ���� ���۶�� �����ϰ�, ����Ͱ� �� ����(�ٲ�� �� ���� ���� ����)�� ǥ���ϵ��� �Ѵ�. -> ���ÿ� ������ ���� ���ۿ��� ���۴� �ٽ� ���� �׸��� �׸� �ĸ� ���۰� �ȴ�.
	// ���� ���۸��� �ٸ���, ���ſ� ȭ�� �����͸� �����ϴ� ��ſ� �����͸� �ٲ� ��ȯ�� �Ͼ�� ���̴�.
	ComPtr<IDXGISwapChain> m_swapChain = nullptr;

	// Render Target View
	// �ĸ� ���۸� ������
	ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;

	// Misc
	// ȭ���� �����ϴ� ����ü (ũ�� ����)
	D3D11_VIEWPORT m_viewport = {0};

	float m_clearColor[4] = { 0.f, 0.f, 0.f,0.f };

private:
	// Geometry
	vector<Vertex> m_vertices;
	ComPtr<ID3D11Buffer> m_vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;

	// VS -> ���ؽ� ���̴�
	ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	ComPtr<ID3DBlob> m_vsBlob = nullptr;

	// PS -> �ȼ� ���̴�
	ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
	ComPtr<ID3DBlob> m_psBlob = nullptr;

};

