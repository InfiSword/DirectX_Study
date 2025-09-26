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
	void CreateDeviceAndSwapChain();

private:
	HWND m_hWnd;
	uint32 m_width = 0;
	uint32 m_height = 0;

private:
	// DX
	// COM(Component Object Model)이란
	// Platform-independent, Distributed, Object-oriented
	// COM객체들 -> 예) ID3D11Device 인터페이스 포인터 메모리 주소
	// 해당 인터페이스들은 순수가상함수로 만들어져 있으며, 우리는 해당 함수를 가져와 사용함
	// 해당 함수의 대한 구현부는 OS가 제공한다. ( dll파일 형식으로 COM 구현체를 가지고 있음 )

	// 과정들
	// 1. COM 런타임 초기화 및 객체 생성 요청
	//		1) COM관련 라이브러리(ole32.dll, DirectX 관련) 초기화 -> COM 객체를 사용하고자 하는 각 스레드에 처음으로 초기화가 이뤄진다.(CoInitializeEx -> ole32.dll에 포함되어 있음)
	//		2) COM 객체 생성 요청 -> COM 객체가 실제로 필요해지는 시점에 해당 객체 생성을 요청한다. (CoCreateInstance -> ole32.dll에 포함되어 있음)
	//		* ole32.dll은 COM을 관리하는 핵심 라이브러리이자, OLE 기능을 사용하는 라이브러리이다.
	//		* COM객체 생성, 객체 소멸, 구현부 클래스의 인터페이스 조회, 스레드 관리를 해당 ole32.dll이 관리한다. 

	// 2. Windows COM 런타임 (ole32.dll)의 역할
	//		1) 레지스트리 조회  -> CoCreateInstance 함수는 프로그램에서 요청한 해당 COM 객체의 CLSID(class ID, 인터페이스를 구현하고 있는 실제 C++ 클래스의 고유 식별자[GUID])를 가지고 Windows 레지스트리( Windows 운영체제의 중앙 설정 데이터베이스)를 조회한다.
	// 해당 레지스트리에는 각 CLSID별로 해당 COM 객체의 실제 구현 코드가 담긴 DLL 파일의 경로와 이름이 명확히 기록되어 있다.
	// 해당 레지스트리 정보가 바로 COM 런타임이 어떤 DLL 파일에 프로그램이 요청한 COM 인터페이스 함수들의 vtable(가상 함수들의 실제 구현 코드의 메모리 주소)을 구현하는 코드가 있는지 파악하는 지도 역할을 한다.
	//		2) DLL 로드 -> o1e32.dll은 해당 레지스트리에서 얻은 정보를 바탕으로 구현코드(d3d11.dll)파일을 찾아 프로그램이 실행되는 메모리 공간 안으로 로드한다.  

	// 3. 객체 생성
	//		1) d3d11.dll에서 DllGetClassObject이란 특정 함수를 찾아 호출한다. 이 때, 해당 함수는 DLL이 COM클래스를 객체화할 수 있도록 해준다.
	//		2) 해당 함수를 통해서, 특정 CLSID에 해당하는 클래스 팩토리를 생성하고, 이 클래스 팩토리의 IClassFactory 인터페이스 포인터를 ole32.dll에게 반환한다.
	//		3) ole32.dll은 이제 이 클래스 팩토리에 CreateInstance 메서드를 호출하여, 해당 CreateInstance 내부에서 해당 프로그램에서 호출된 인터페이스를 구현하는 실제 C++클래스의 객체를 메모리에 할당하고 초기화한다. 여기서 해당 실제 구현부 클래스가 바로 COM객체인 것이다. 결국, 해당 인터페이스 포인터의 메모리는 실제 구현부 C++ 클래스 객체(COM 객체)를 가리키게 된다.	
	//		4) 이 때, vtable를 해당 클래스마다 1개씩 생성되게 되며, 해당 클래스의 객체마다 vpointer가 생기고, 해당 vpointer가 해당 클래스의 vtable을 가리키게 된다. 만약 어떤 객체가 가상함수를 호출하면, 해당 객체의 메모리에서 vpointer를 읽어와, vtable에 접근하여, 호출하려는 가상함수에 해당하는 함수 포인터(해당 가상 함수의 구현부)를 가져온다. 
	
	// 4. 인터페이스 포인터 반환과 동작
	//		1) CreateInstance는 해당 생성된 객체의 주소, 즉 vpointer가 있는 주소를 해당 프로그램이 호출한 인터페이스 포인터 타입으로 캐스팅하여 ole32.dll에게 반환한다.
	//		2) ole32.dll은 이 포인터를 다시 우리 프로그램이 선언한 인터페이스 포인터 변수에 저장된다.
	//		3) 그 이후에는 해당 인터페이스의 함수를 호출시켜 우리가 아는 C++방식처럼 동작시킨다.


	// 디바이스 관련 무언갈 생성할 때 사용
	ComPtr<ID3D11Device> m_device = nullptr;
	// 렌더링 파이프라인에 연결할 때 사용함
	ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;	
	
	// DXGI이라는 자주 바뀌지않는 화면처리 기능을 따로 빼놓고 관리함
	// DX9, 11, 12 상관없이 DXGI는 공통적으로 사용할 수 있으므로 따로 빼놨구나
	// SwapChain이란? -> 더블 버퍼링같은 느낌임
	// 후면 버퍼에 GPU가 다음 장면을 그린다 -> 이 때 사용자는 전면 버퍼를 보고 있는다. -> 장면을 다 그리면, 이 때 해당 GPU는 방금 그림을 다 그린 후면 버퍼를 새로운 전면 버퍼라고 선언하고, 모니터가 이 버퍼(바뀌고 난 뒤의 전면 버퍼)를 표시하도록 한다. -> 동시에 기존에 전면 버퍼였던 버퍼는 다시 다음 그림을 그릴 후면 버퍼가 된다.
	// 더블 버퍼링과 다르게, 무거운 화면 데이터를 복사하는 대신에 포인터만 바꿔 교환이 일어나는 것이다.
	ComPtr<IDXGISwapChain> m_swapChain = nullptr;
};

