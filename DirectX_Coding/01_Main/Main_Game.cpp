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

	// �ش� �������̽� �����͸� �����ϰų� �� ��, ���۷��� ī��Ʈ�� 1����
	// ���� �ֱⰡ �ָ��� ���� Ÿ�̹��� �����ϱ� ����
	// m_device->AddRef();

	// ���۷��� ī��Ʈ�� 1 �ٿ��ְ�, 0�� �Ǹ�, �� ��ü�� ���������� �˾Ƽ� �Ҹ��
	// m_device->Release();

	// �ش� �������� ����, ������ �ϴ� ���� ���ŷӰ�, ������ �۾��̱⿡
	// ����Ʈ �����Ϳ� ���������� �ڵ����� �������ִ°� ������ ������ ����.
	// -> ComPtr<>
}

void Main_Game::Update()
{
}

void Main_Game::Render()
{
}
