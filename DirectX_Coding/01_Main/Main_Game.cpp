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
	// TODO

	// 해당 인터페이스 포인터를 복사하거나 할 때, 레퍼런스 카운트를 1증가
	// 생명 주기가 애매해 지는 타이밍을 방지하기 위해
	// m_device->AddRef();

	// 레퍼런스 카운트를 1 줄여주고, 0이 되면, 그 객체가 내부적으로 알아서 소멸됨
	// m_device->Release();

	// 해당 수동으로 생성, 해제를 하는 일이 번거롭고, 무식한 작업이기에
	// 스마트 포인터와 마찬가지로 자동으로 관리해주는게 있으면 좋을것 같다.
	// -> ComPtr<>
}

void Main_Game::Update()
{
}

void Main_Game::Render()
{
}
