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
	HWND m_hWnd;
	uint32 m_width = 0;
	uint32 m_height = 0;

private:
	// DX
	// COM(Component Object Model)이란
	// Platform-independent, Distributed, Object-oriented
	// COM객체들 -> 예) ID3D11Device
	// 해당 인터페이스들은 순수가상함수로 만들어져 있으며, 우리는 해당 함수를 가져와 사용함
	// 해당 함수의 대한 구현부는 OS가 제공한다. ( dll파일 형식으로 COM 구현체를 가지고 있음 )

	// 과정들
	// 1. COM 런타임 초기화 및 객체 생성 요청
	//		1) COM 라이브러리 초기화 -> COM 객체를 사용하고자 하는 각 스레드에 처음으로 초기화가 이뤄진다.(CoInitializeEx)
	//		2) COM 객체 생성 요청 -> COM 객체가 실제로 필요해지는 시점에 해당 객체 생성을 요청한다. (CoCreateInstance)

	// 2. Windows COM 런타임 (ole32.dll)의 역할
	//		1) 레지스트리 조회  -> CoCreateInstance 함수 (ole32.dll 내부에 있음)는 프로그램에서 요청한 해당 COM 객체의 CLSID(class ID, 인터페이스를 구현하고 있는 실제 C++ 클래스의 고유 식별자[GUID])를 가지고 Windows 레지스트리( Windows 운영체제의 중앙 설정 데이터베이스)를 조회한다.
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
	//		3) 그 이후에는 해당 인터페이스의 함수를 호출시켜 동작시킨다.


	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
};

