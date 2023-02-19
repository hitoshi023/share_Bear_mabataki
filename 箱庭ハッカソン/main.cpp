//=============================================================================
//
// ���C������ [main.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "shadow.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "tree.h"
#include "collision.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "cloud.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"Team11_hakoniwa"		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

void CheckHit(void);


//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;


#ifdef _DEBUG
int		g_CountFPS;							// FPS�J�E���^
char	g_DebugStr[2048] = WINDOW_NAME;		// �f�o�b�O�����\���p

#endif


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// �E�B���h�E�̍����W
		CW_USEDEFAULT,																		// �E�B���h�E�̏���W
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// �E�B���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// �X�V����
				Draw();				// �`�揈��

#ifdef _DEBUG	// �f�o�b�O�ł̎������\������
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// ����\��߂�

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	InitRenderer(hInstance, hWnd, bWindow);

	InitLight();

	InitCamera();

	InitFade();

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	//// �t�B�[���h�̏�����
	//InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	//// �ǂ̏�����
	//InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, XM_PI, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	//// ��(�����p�̔�����)
	//InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, XM_PI, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();

	// �؂̏���������
	InitTree();

	// �o���b�g�̏���������
	InitBullet();

	// �X�R�A�̏���������
	InitScore();

	// �_�ƃh�A�̏���������
	InitCloud();

	// �T�E���h�̏���������
	InitSound(hWnd);

	PlaySound(SOUND_LABEL_BGM_Pride);

	// ���C�g��L����
	SetLightEnable(true);

	// �w�ʃ|���S�����J�����O
	SetCullingMode(CULL_MODE_BACK);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	// �t�F�[�h�̏I������
	UninitFade();

	// �T�E���h�̏I������
	UninitSound();

	// �X�R�A�̏I������
	UninitScore();

	// �o���b�g�̏I������
	UninitBullet();

	// �؂̏I������
	UninitTree();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();

	// �e�̏I������
	UninitShadow();

	// �ǂ̏I������
	//UninitMeshWall();

	// �n�ʂ̏I������
	//UninitMeshField();

	// �J�����̏I������
	UninitCamera();

	//���͂̏I������
	UninitInput();

	// �����_���[�̏I������
	UninitRenderer();

	// �_�ƃh�A�̏I������
	UninitCloud();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// ���͂̍X�V����
	UpdateLight();

	// ���͂̍X�V����
	UpdateInput();

	// �t�F�[�h�̍X�V����
	UpdateFade();

	// �J�����X�V
	UpdateCamera();

	// �n�ʏ����̍X�V
	//UpdateMeshField();

	// �Ǐ����̍X�V
	//UpdateMeshWall();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �G�l�~�[�̍X�V����
	UpdateEnemy();

	// �؂̍X�V����
	UpdateTree();

	// �o���b�g�̍X�V����
	UpdateBullet();

	// �e�̍X�V����
	UpdateShadow();

	//// �����蔻��
	//CheckHit();

	// �X�R�A�̍X�V����
	UpdateScore();

	//�@�_�ƃh�A�̍X�V����
	UpdateCloud();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@�N���A
	Clear();

	{
		SetViewPort(TYPE_FULL_SCREEN);
		XMFLOAT3 pos = GetCamAT()->pos;
		//pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���

		// �J���������_�̐ݒ�
		GetCamera()->at = pos;

		SetCamera();

		// �n�ʂ̕`�揈��
		//DrawMeshField();

		// �e�̕`�揈��
		DrawShadow();

		// �v���C���[�̕`�揈��
		DrawPlayer();

		// �G�l�~�[�̕`�揈��
		DrawEnemy();

		// �ǂ̕`�揈��
		//DrawMeshWall();

		// �؂̕`�揈��
		DrawTree();

		// �o���b�g�̕`�揈��
		DrawBullet();

		// �_�ƃh�A�̕`�揈��
		DrawCloud();

		

		// 2D�\���p
		{
			// ���C�g�EZ��r����
			SetLightEnable(false);
			SetDepthEnable(false);

			// ���D��x�������Ō��߂Ęg�Ƃ��̕`��̏��Ԃ����߂�

			// �X�R�A�̕`�揈��
			//DrawScore();

			DrawFade();

			// ���C�g�EZ��r�L��
			SetLightEnable(true);
			SetDepthEnable(true);
		}
	}

	//// �f�o�b�O�p���_
	//{
	//	SetViewPort(TYPE_FULL_SCREEN);
	//	//XMFLOAT3 pos = GetPlayer()->pos;
	//	//pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
	//	//SetCameraAT(pos);
	//	SetCamera();

	//	// �n�ʂ̕`�揈��
	//	DrawMeshField();

	//	// �e�̕`�揈��
	//	DrawShadow();

	//	// �v���C���[�̕`�揈��
	//	DrawPlayer();

	//	// �G�l�~�[�̕`�揈��
	//	DrawEnemy();

	//	// �ǂ̕`�揈��
	//	DrawMeshWall();

	//	// �؂̕`�揈��
	//	DrawTree();

	//	// �o���b�g�̕`�揈��
	//	DrawBullet();

	//	// 2D�\���p
	//	{
	//		// ���C�g�EZ��r����
	//		SetLightEnable(false);
	//		SetDepthEnable(false);

	//		// ���D��x�������Ō��߂Ęg�Ƃ��̕`��̏��Ԃ����߂�

	//		// �X�R�A�̕`�揈��
	//		DrawScore();

	//		// ���C�g�EZ��r�L��
	//		SetLightEnable(true);
	//		SetDepthEnable(true);
	//	}
	//}

	//// �o���b�g���_
	//{
	//	SetViewPort(TYPE_UP_RIGHT_SCREEN);
	//	XMFLOAT3 pos = GetBullet()->pos;
	//	pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
	//	SetCameraAT(pos);
	//	SetCamera();

	//	// �n�ʂ̕`�揈��
	//	DrawMeshField();

	//	// �e�̕`�揈��
	//	DrawShadow();

	//	// �v���C���[�̕`�揈��
	//	DrawPlayer();

	//	// �G�l�~�[�̕`�揈��
	//	DrawEnemy();

	//	// �ǂ̕`�揈��
	//	DrawMeshWall();

	//	// �؂̕`�揈��
	//	DrawTree();

	//	// �o���b�g�̕`�揈��
	//	DrawBullet();

	//	// 2D�\���p
	//	{
	//		// ���C�g�EZ��r����
	//		SetLightEnable(false);
	//		SetDepthEnable(false);

	//		// ���D��x�������Ō��߂Ęg�Ƃ��̕`��̏��Ԃ����߂�

	//		// �X�R�A�̕`�揈��
	//		DrawScore();

	//		// ���C�g�EZ��r�L��
	//		SetLightEnable(true);
	//		SetDepthEnable(true);
	//	}
	//}

	//// �G�l�~�[���_
	//{
	//	SetViewPort(TYPE_DOWN_LEFT_SCREEN);
	//	ENEMY* enemy = GetEnemy();
	//	XMFLOAT3 pos = enemy->pos;
	//	pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
	//	SetCameraAT(pos);
	//	SetCamera();

	//	// �n�ʂ̕`�揈��
	//	DrawMeshField();

	//	// �e�̕`�揈��
	//	DrawShadow();

	//	// �v���C���[�̕`�揈��
	//	DrawPlayer();

	//	// �G�l�~�[�̕`�揈��
	//	DrawEnemy();

	//	// �ǂ̕`�揈��
	//	DrawMeshWall();

	//	// �؂̕`�揈��
	//	DrawTree();

	//	// �o���b�g�̕`�揈��
	//	DrawBullet();

	//	// 2D�\���p
	//	{
	//		// ���C�g�EZ��r����
	//		SetLightEnable(false);
	//		SetDepthEnable(false);

	//		// ���D��x�������Ō��߂Ęg�Ƃ��̕`��̏��Ԃ����߂�

	//		// �X�R�A�̕`�揈��
	//		DrawScore();

	//		// ���C�g�EZ��r�L��
	//		SetLightEnable(true);
	//		SetDepthEnable(true);
	//	}
	//}

	//// �c���[���_
	//{
	//	SetViewPort(TYPE_DOWN_RIGHT_SCREEN);
	//	XMFLOAT3 pos = GetTree()->pos;
	//	pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
	//	SetCameraAT(pos);
	//	SetCamera();

	//	// �n�ʂ̕`�揈��
	//	DrawMeshField();

	//	// �e�̕`�揈��
	//	DrawShadow();

	//	// �v���C���[�̕`�揈��
	//	DrawPlayer();

	//	// �G�l�~�[�̕`�揈��
	//	DrawEnemy();

	//	// �ǂ̕`�揈��
	//	DrawMeshWall();

	//	// �؂̕`�揈��
	//	DrawTree();

	//	// �o���b�g�̕`�揈��
	//	DrawBullet();

	//	// 2D�\���p
	//	{
	//		// ���C�g�EZ��r����
	//		SetLightEnable(false);
	//		SetDepthEnable(false);

	//		// ���D��x�������Ō��߂Ęg�Ƃ��̕`��̏��Ԃ����߂�

	//		// �X�R�A�̕`�揈��
	//		DrawScore();

	//		// ���C�g�EZ��r�L��
	//		SetLightEnable(true);
	//		SetDepthEnable(true);
	//	}
	//}


	//SetViewPort(TYPE_FULL_SCREEN);

	//// 2D�\���p
	//{
	//	// ���C�g�EZ��r����
	//	SetLightEnable(false);
	//	SetDepthEnable(false);

	//	// ���D��x�������Ō��߂Ęg�Ƃ��̕`��̏��Ԃ����߂�

	//	// �X�R�A�̕`�揈��
	//	DrawScore();

	//	// ���C�g�EZ��r�L��
	//	SetLightEnable(true);
	//	SetDepthEnable(true);
	//}


#ifdef _DEBUG
	// �f�o�b�O�\��
	DrawDebugProc();
#endif

	// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();
}


long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif



//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	
	ENEMY* enemy = GetEnemy();		// �G�l�~�[�̃|�C���^�[��������
	PLAYER* player = GetPlayer();	// �v���C���[�̃|�C���^�[��������

	// �G�ƃv���C���[�L����
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//�G�̗L���t���O���`�F�b�N����
		if (enemy[i].use == false)
			continue;

		//BC�̓����蔻��
		if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size))
		{
			// �G�L�����N�^�[�͓|�����
			enemy[i].use = false;
			ReleaseShadow(enemy[i].shadowIdx);

			if (player->use == false) return;

			// �X�R�A�𑫂�
			AddScore(50);
		}
	}


	// �v���C���[�̒e�ƓG
	BULLET* bullet = GetBullet();
	for (int b = 0; b < MAX_BULLET; b++)
	{
		//�G�̗L���t���O���`�F�b�N����
		if (bullet[b].use == false) continue;

		//BC�̓����蔻��
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy[i].use == false) continue;

			if (CollisionBC(bullet[b].pos, enemy[i].pos, bullet[b].fWidth, enemy[i].size))
			{
				// �G�L�����N�^�[�͓|�����
				enemy[i].use = false;
				ReleaseShadow(enemy[i].shadowIdx);
				bullet[b].use = false;
				ReleaseShadow(bullet[b].shadowIdx);

				AddScore(100);

				break;
				// �X�R�A�𑫂�
			}
		}
	}

	// �v���C���[�̒e�Ɩ�
	TREE* tree = GetTree();
	for (int b = 0; b < MAX_BULLET; b++)
	{
		//�G�̗L���t���O���`�F�b�N����
		if (bullet[b].use == false) continue;

		//BC�̓����蔻��
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy[i].use == false) continue;

			if (CollisionBC(bullet[b].pos, enemy[i].pos, bullet[b].fWidth, enemy[i].size))
			{
				// �G�L�����N�^�[�͓|�����
				enemy[i].use = false;
				ReleaseShadow(enemy[i].shadowIdx);
				bullet[b].use = false;
				ReleaseShadow(bullet[b].shadowIdx);

				// �X�R�A�𑫂�
			}
		}
	}


	//// �G�l�~�[���S�����S�������ԑJ��
	//int enemy_count = 0;
	//for (int i = 0; i < MAX_ENEMY; i++)
	//{
	//	if (enemy[i].use == false) continue;
	//	enemy_count++;
	//}

	//// �G�l�~�[���O�C�H
	//if (enemy_count == 0)
	//{
	//	// SetFade(FADE_OUT, MODE_RESULT);

	//}

}

