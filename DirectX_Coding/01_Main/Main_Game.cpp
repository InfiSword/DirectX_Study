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

	// 해당 인터페이스 포인터를 복사하거나 할 때, 레퍼런스 카운트를 1증가
	// 생명 주기가 애매해 지는 타이밍을 방지하기 위해
	// m_device->AddRef();

	// 레퍼런스 카운트를 1 줄여주고, 0이 되면, 그 객체가 내부적으로 알아서 소멸됨
	// m_device->Release();

	// 해당 수동으로 생성, 해제를 하는 일이 번거롭고, 무식한 작업이기에
	// 스마트 포인터와 마찬가지로 자동으로 관리해주는게 있으면 좋을것 같다.
	// -> ComPtr<>

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();
}

void Main_Game::Update()
{
}

void Main_Game::Render()
{
	RenderBegin();
	
	// IA - VS - RS - PS - OW
	// 여기서 쉐이더 리소스나, 기타 필요 기능들을 연결해 줘야 함
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// IA
		// m_vertexBuffer.GetAddressOf() => GPU에 존재하는 버텍스 포인터
		m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

		m_deviceContext->IASetInputLayout(m_inputLayout
			.Get());

		// 점과 점을 어떻게 이어붙일지 정해줌 -> 삼각형으로 인지해달라.
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		// OM

		m_deviceContext->Draw(m_vertices.size(), 0);
	}
	
	
	RenderEnd();

}

void Main_Game::RenderBegin()
{
	// 렌더링 파이프라이닝에서 Output Merge -> OEM단계
	// 해당 그림을 그릴 도화지(후면 버퍼)에 해당 리소스를 제출
	// 해당 캔버스에 그림을 그릴것 이라는, 공식적 선언
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

	// 해당 코드는 캔버스를 특정 색상으로 깨끗하게 지우는 역할
	// 후면 버퍼에는 이전 프레임에 그렸던 그림이 그대로 남아있기에, 이걸
	// 지워주는 역할을 한다.
	// 즉, 후면 버퍼에 그림을 해당 색상으로 전부 밀어버리는 것
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), m_clearColor);

	// 캔버스의 어느 영역에 그림을 그릴지 설정
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void Main_Game::RenderEnd()
{
	// [후면] -> [전면]
	// 후면 버퍼의 내용을 전면 버퍼로 전해달라 
	// 버퍼 교체(Swap)를 실행하는 단계, 후면 버퍼에 그림을 완성하고,
	// 이 후면 버퍼를 사용자가 보고 있는 전면 버퍼로 맞바꾸게 된다.
	HRESULT hr = m_swapChain->Present(1, 0);
	assert(SUCCEEDED(hr));
}

void Main_Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		// 스왑체인이 사용할 버퍼(후면 버퍼)의 픽셀 단위를 정의함
		// 정의한 버퍼의 가로와 세로 크기를 우리가 설정한 창의 크기와 동일하게 설정하여 픽셀 매칭으로 렌더링 결과물이 화면에 꽉 차도록 표시되도록 함
		desc.BufferDesc.Width = m_width;
		desc.BufferDesc.Height = m_height;

		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// 멀티 샘플링 -> 화면을 그릴 때, 계단 현상이 일어날 수 있다. ( 엘리어싱 ) -> 해결 방법중 하나는 우리가 만들 화면의 크기를 몇 배 증가시켜 크게 만든 다음, 그거에 대한 보간 값을 정하고, 해당 값으로 색상을 정하게 된다면 상대적으로 해당 계단 현상을 완화해주는 방법이 존재한다. 
		// 이럴 때 해당 Count 숫자를 늘려보는 것을 고려할 수 있다.
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		// 해당 버퍼를 어떻게 사용할 것인가
		// -> GPU가 연산을 마치고, 최종적인 어떤 색상( 모든 셰이더 연산 )을 그려야 하는지 알아냈을 때,
		// 그 연산 결과물을 그려주겠다. ( 후면 버퍼에 렌더링 하겠다. -> 화면을 그리는 용도 -> 스왑 체인의 후면 버퍼)
		// -> 렌더링 파이프라인의 최종 출력 대상이 되겠다.
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// 해당 후면 버퍼를 하나만 사용하겠다. 
		desc.BufferCount = 1;
		desc.OutputWindow = m_hWnd;
		desc.Windowed = true;

		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	}

	// 디바이스를 만들고, 그 다음에 SwapChain을 만든다.
	// 한꺼번에 같이 만드는 느낌의 함수
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,	// 우리의 하드웨어인 그래픽카드를 사용하겠다.
		nullptr,
		0,
		nullptr,	// D3D_FEATURE_LEVEL ( 해당 DX버전에 해당하는 기능들을 지원해야 한다는 것을 명시하고, 건네주는 인자임, 입력을 안하고 null로 둔다면, 내가 지원할 수 있는 상위 버전 하나를 골라주기 때문에 사실상 null로 둬도 됨
		0,
		D3D11_SDK_VERSION,	// 버전 11에서 개발 시 사용한 헤더 파일과 실행 환경의 dll 버전이 서로 호환되는지 확인하기 위한 고유 정수 값
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

	// 후면버퍼에서 해당 리소스를 Texture2D로 내보냄 -> 빈 캔버스
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());

	assert(SUCCEEDED(hr));

	// 후면 버퍼에, 해당 backBuffer 캔버스를 지정해서, 다음에 그릴 도화지를
	// 가리킴
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

void Main_Game::CreateGeometry()
{
	// VertexData 
	// CPU가 사용하는 RAM에 우리가 그리려는 도형의 정보를 만듬
	{
		m_vertices.resize(3);

		m_vertices[0].Pos = Vec3{ -0.5f, -0.5f ,0 };
		m_vertices[0].color = Color{ 1.f, 0.f, 0.f, 0.f };

		m_vertices[1].Pos = Vec3{ 0.f,0.5f,0.f };
		m_vertices[1].color = Color{ 1.f, 0.f, 0.f, 0.f };

		m_vertices[2].Pos = Vec3{ 0.5f,-0.5f,0.f };
		m_vertices[2].color = Color{ 1.f, 0.f, 0.f, 0.f };

	}

	// VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		// GPU에서만 읽을 수 있는 데이터로 설정
		// 이 데이터는 한번 GPU로 보내면 절대 바뀌지 않기에, 최적화가능
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		// 정점버퍼로 데이터를 공급하는 역할로써 사용하겠다는 플레그 사용
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * m_vertices.size());

		// CPU에 있는 RAM메모리 데이터를 GPU에게 전달하기 위한 구조체
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		// GPU 버퍼를 생성할 때 초기화할 데이터가 어디에 있는지 알려줌
		data.pSysMem = m_vertices.data();

		// 정점으로 이뤄진 데이터 버퍼가 생성되고, 해당 버퍼는 VRAM에 생성된다.
		m_device->CreateBuffer(&desc, &data, m_vertexBuffer.GetAddressOf());
	}
}

void Main_Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0
		},
		{
			"COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0
		},
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	m_device->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf());
}

void Main_Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr);

	assert(SUCCEEDED(hr));
}

void Main_Game::CreateVS()
{
	// 표준 버텍스 쉐이더 버전사용
	LoadShaderFromFile(L"../DirectX_Coding/98_Shader/Default.hlsl", "VS", "vs_5_0", m_vsBlob);

	// blob의 정보들을 가지고 버텍스 쉐이더를 만들어준다.
	HRESULT hr = m_device->CreateVertexShader(m_vsBlob->GetBufferPointer(),
		m_vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());

	assert(SUCCEEDED(hr));
}

void Main_Game::CreatePS()
{ 
	LoadShaderFromFile(L"../DirectX_Coding/98_Shader/Default.hlsl", "PS", "ps_5_0", m_psBlob);

	// blob의 정보들을 가지고 픽셀 쉐이더를 만들어준다.
	HRESULT hr = m_device->CreatePixelShader(m_psBlob->GetBufferPointer(),
		m_psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());

	assert(SUCCEEDED(hr));
}
