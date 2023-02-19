//=============================================================================
//
// �J�������� [camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	POS_X_CAM			(-150.0f)			// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM			(70.0f)			// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM			(-150.0f)		// �J�����̏����ʒu(Z���W)

//#define	POS_X_CAM		(0.0f)			// �J�����̏����ʒu(X���W)
//#define	POS_Y_CAM		(200.0f)		// �J�����̏����ʒu(Y���W)
//#define	POS_Z_CAM		(-400.0f)		// �J�����̏����ʒu(Z���W)


#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(10000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE_CAMERA	(2.0f)										// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)								// �J�����̉�]��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CameraRotation(XMFLOAT3 CamAT_pos, float Cam_posY, float len, float degPerSec, float degStart, int time);
void InitCamGrobal(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA			g_Camera;		// �J�����f�[�^
static CAM_AT			g_CamAT;		// �J���������_�f�[�^

static int				g_ViewPortType = TYPE_FULL_SCREEN;

static int				g_MoveCnt;		// �J�E���^

// �J�����p���`��Ԉړ��e�[�u��
static INTERPOLATION_DATA g_MoveTbl0[] = {
	//���W									��]��							�g�嗦							����
	// 
		// �J�������W�Œ�{�����_��] 6
/*00*/	{ XMFLOAT3(-900.0f, 500.0f, -800.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	359 },
/*01*/	{ XMFLOAT3(-900.0f, 500.0f, -800.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

		// �����_�Œ�{�J�������W��] 6 12
/*02*/	{ XMFLOAT3(900.0f, 30.0f, -200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	359 },
/*03*/	{ XMFLOAT3(780.0f, 30.0f, -200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

		// ��E�������i 6 18
/*04*/	{ XMFLOAT3(-90.0f, 220.0f, -1200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 1.0f),	359 },
/*05*/	{ XMFLOAT3(-90.0f, 120.0f, 300.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

		// ���̏o�^�C�����v�X 15 33
/*06*/	{ XMFLOAT3(750.0f, 200.0f, -900.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	899 },
/*07*/	{ XMFLOAT3(750.0f, 200.0f, -900.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

		// �Ƃɓ���H 6 39/ 6 45
/*08*/	{ XMFLOAT3(1000.0f, 200.0f, -200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	360 },
/*09*/	{ XMFLOAT3(1000.0f, 200.0f, 880.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	359 },
/*10*/	{ XMFLOAT3(1000.0f, 200.0f, 880.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

		// �{�[�g
/*11*/	{ XMFLOAT3(750.0f, 200.0f, -1200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	359 },
/*12*/	{ XMFLOAT3(750.0f, 200.0f, -1200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

};

// �J���������_�p���`��Ԉړ��e�[�u��
static INTERPOLATION_DATA g_MoveTbl1[] = {
	//���W									��]��							�g�嗦							����

	// ��]
	{ XMFLOAT3(-500.0f, 400.0f, 500.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 1.0f),	359 },
	{ XMFLOAT3(500.0f, 300.0f, -600.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

	// ��]
	{ XMFLOAT3(0.0f, 30.0f, 0.0f),			XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 1.0f),	359 },
	{ XMFLOAT3(0.0f, 30.0f, 0.0f),			XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

	// ��
	{ XMFLOAT3(-90.0f, 50.0f, -500.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f),	359 },
	{ XMFLOAT3(-90.0f, 50.0f, 600.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

	// ���̏o�^�C�����v�X��
	{ XMFLOAT3(200.0f, 300.0f, 200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f),	899 },
	{ XMFLOAT3(700.0f, 300.0f, 200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

	// ��
	{ XMFLOAT3(1000.0f, 200.0f, 200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f),	360 },
	{ XMFLOAT3(1000.0f, 250.0f, 1200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 1.0f),	30 },
	{ XMFLOAT3(1000.0f, 250.0f, 1200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 1.0f),	329 },
	{ XMFLOAT3(1000.0f, 150.0f, 1200.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },
			
	// �{�[�g
	{ XMFLOAT3(750.0f, 150.0f, -900.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f),	359 },
	{ XMFLOAT3(850.0f, 150.0f, -900.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(2.0f, 2.0f, 1.0f),	1 },

};

static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,

};


//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = { -900.0f, 500.0f, -800.0f };
	g_CamAT.pos = { 0.0f, 50.0f, 0.0f };
	g_Camera.at  = { 0.0f, 0.0f, 0.0f };
	g_Camera.up  = { 0.0f, 1.0f, 0.0f };
	g_Camera.rot = { 0.0f, 0.0f, 0.0f };
	g_Camera.canMove = true;
	g_CamAT.use = true;

	// ���_�ƒ����_�̋������v�Z
	//float vx, vz;
	//vx = g_Camera.pos.x - g_Camera.at.x;
	//vz = g_Camera.pos.z - g_Camera.at.z;
	//g_Camera.len = sqrtf(vx * vx + vz * vz);
	g_Camera.len = 500.0f;

	// ���`��Ԃœ������Ă݂�
	g_Camera.time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Camera.tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Camera.tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_CamAT.time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_CamAT.tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_CamAT.tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_MoveCnt = 0;

	// �r���[�|�[�g�^�C�v�̏�����
	g_ViewPortType = TYPE_FULL_SCREEN;
}


//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{


	// �ړ�����
	{	// ���`��Ԃ̏���
		int nowNo = (int)g_CamAT.time;			// �������ł���e�[�u���ԍ������o���Ă���
		int maxNo = g_CamAT.tblMax;				// �o�^�e�[�u�����𐔂��Ă���
		int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
		INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_CamAT.tblNo];	// �s���e�[�u���̃A�h���X���擾

		XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
		XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
		XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

		XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
		XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
		XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

		float nowTime = g_CamAT.time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

		Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
		Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
		Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

		// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
		XMStoreFloat3(&g_CamAT.pos, nowPos + Pos);

		//// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
		//XMStoreFloat3(&g_CamAT.rot, nowRot + Rot);

		//// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
		//XMStoreFloat3(&g_CamAT.scl, nowScl + Scl);
		//g_CamAT.w = TEXTURE_WIDTH * g_CamAT.scl.x;
		//g_CamAT.h = TEXTURE_HEIGHT * g_CamAT.scl.y;

		// frame���g�Ď��Ԍo�ߏ���������
		g_CamAT.time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
		if ((int)g_CamAT.time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
		{
			g_CamAT.time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
		}
	}

	{	// ���`��Ԃ̏���
		int nowNo = (int)g_Camera.time;			// �������ł���e�[�u���ԍ������o���Ă���
		int maxNo = g_Camera.tblMax;				// �o�^�e�[�u�����𐔂��Ă���
		int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
		INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Camera.tblNo];	// �s���e�[�u���̃A�h���X���擾

		XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
		XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
		XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

		XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
		XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
		XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

		float nowTime = g_Camera.time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

		Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
		Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
		Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

		// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
		XMStoreFloat3(&g_Camera.pos, nowPos + Pos);

		// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
		XMStoreFloat3(&g_Camera.rot, nowRot + Rot);

		//// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
		//XMStoreFloat3(&g_Camera.scl, nowScl + Scl);
		//g_Camera.w = TEXTURE_WIDTH * g_Camera.scl.x;
		//g_Camera.h = TEXTURE_HEIGHT * g_Camera.scl.y;


		// frame���g�Ď��Ԍo�ߏ���������
		g_Camera.time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
		if ((int)g_Camera.time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
		{
			g_Camera.time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
		}

		switch ((int)g_Camera.time)
		{
		case 2:
			CameraRotation(XMFLOAT3{ 900.0f, 250.0f, 780.0f }, 400.0f, 1000.0f, 6.0f, 100.0f, 360);
			break;
		case 4:
			g_Camera.up.x -= 0.001f;
		case 0:
		case 11:
			g_MoveCnt++;
			if (g_MoveCnt + 50 == 360)
				SetFade(FADE_OUT);
			break;
		case 9:
			g_MoveCnt++;
			if (g_MoveCnt <= 30)
			{
				g_CamAT.pos.x += sin(g_MoveCnt * XM_2PI / 360) * 250;
				g_CamAT.pos.y += 5.0f;
				g_CamAT.pos.z += cos(g_MoveCnt * XM_2PI / 360) * 150;
			}
			else if (g_MoveCnt > 30)
			{
				g_CamAT.pos.x += sin(g_MoveCnt * XM_2PI / 360) * 280;
				g_CamAT.pos.y -= 5.0f;
				g_CamAT.pos.z += cos(g_MoveCnt * XM_2PI / 360) * 150;
			}

			if (g_MoveCnt + 50 == 360)
				SetFade(FADE_OUT);
			break;
		case 6:
			g_MoveCnt++;
			if (g_MoveCnt + 50 == 899)
				SetFade(FADE_OUT);
			break;
		case 3: 
			g_Camera.up.x = 0.2f;
			InitCamGrobal();
			break;
		case 5: 
			g_Camera.up.x = 0.0f;
		case 7:
		case 10:
		case 12:
			InitCamGrobal();
			break;

		default:
			break;
		}

	}

#ifdef _DEBUG

	if (GetKeyboardPress(DIK_Z))
	{// ���_����u���v
		g_Camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_Camera.rot.y > XM_PI)
		{
			g_Camera.rot.y -= XM_PI * 2.0f;
		}

		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_C))
	{// ���_����u�E�v
		g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_Camera.rot.y < -XM_PI)
		{
			g_Camera.rot.y += XM_PI * 2.0f;
		}

		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_Y))
	{// ���_�ړ��u��v
		g_Camera.pos.y += VALUE_MOVE_CAMERA;
	}

	if (GetKeyboardPress(DIK_N))
	{// ���_�ړ��u���v
		g_Camera.pos.y -= VALUE_MOVE_CAMERA;
	}

	if (GetKeyboardPress(DIK_Q))
	{// �����_����u���v
		g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_Camera.rot.y < -XM_PI)
		{
			g_Camera.rot.y += XM_PI * 2.0f;
		}

		g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_E))
	{// �����_����u�E�v
		g_Camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_Camera.rot.y > XM_PI)
		{
			g_Camera.rot.y -= XM_PI * 2.0f;
		}

		g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_T))
	{// �����_�ړ��u��v
		g_Camera.at.y += VALUE_MOVE_CAMERA;
	}

	if (GetKeyboardPress(DIK_B))
	{// �����_�ړ��u���v
		g_Camera.at.y -= VALUE_MOVE_CAMERA;
	}

	if (GetKeyboardPress(DIK_U))
	{// �߂Â�
		g_Camera.len -= VALUE_MOVE_CAMERA;
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_M))
	{// �����
		g_Camera.len += VALUE_MOVE_CAMERA;
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}

	// �J�����������ɖ߂�
	if (GetKeyboardPress(DIK_R))
	{
		UninitCamera();
		InitCamera();
	}

#endif



#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Camera Pos X:%f,Y:%f,Z:%f UP x:%f y:%f z:%f", g_Camera.pos.x, g_Camera.pos.y, g_Camera.pos.z,
																g_Camera.up.x, g_Camera.up.y, g_Camera.up.z);

	if (GetKeyboardPress(DIK_LEFT))
	{	// ���ֈړ�
		g_Camera.pos.x -= 2.0f;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{	// ���ֈړ�
		g_Camera.pos.x += 2.0f;
	}
	if (GetKeyboardPress(DIK_UP))
	{	// ���ֈړ�
		g_Camera.pos.z += 2.0f;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{	// ���ֈړ�
		g_Camera.pos.z -= 2.0f;
	}
	if (GetKeyboardPress(DIK_LSHIFT))
	{
		if (GetKeyboardPress(DIK_LEFT))
		{	// ���ֈړ�
			g_Camera.pos.x -= 10.0f;
		}
		if (GetKeyboardPress(DIK_RIGHT))
		{	// ���ֈړ�
			g_Camera.pos.x += 10.0f;
		}
		if (GetKeyboardPress(DIK_UP))
		{	// ���ֈړ�
			g_Camera.pos.z += 10.0f;
		}
		if (GetKeyboardPress(DIK_DOWN))
		{	// ���ֈړ�
			g_Camera.pos.z -= 10.0f;
		}
	}
	if (GetKeyboardPress(DIK_SPACE))
	{	// ���ֈړ�
		g_Camera.pos.y += 2.0f;
	}
	if (GetKeyboardPress(DIK_N))
	{	// ���ֈړ�
		g_Camera.pos.y -= 2.0f;
	}
	if (GetKeyboardPress(DIK_1))
	{	// ���ֈړ�
		g_Camera.up.x += 0.005f;
	}
	if (GetKeyboardPress(DIK_2))
	{	// ���ֈړ�
		g_Camera.up.x -= 0.005f;
	}

	if (GetKeyboardPress(DIK_A))
	{	// ���ֈړ�
		g_Camera.at.x -= 2.0f;
	}
	if (GetKeyboardPress(DIK_D))
	{	// ���ֈړ�
		g_Camera.at.x += 2.0f;
	}
	if (GetKeyboardPress(DIK_W))
	{	// ���ֈړ�
		g_Camera.at.z += 2.0f;
	}
	if (GetKeyboardPress(DIK_S))
	{	// ���ֈړ�
		g_Camera.at.z -= 2.0f;
	}


#endif
}


//=============================================================================
// �J�����̍X�V
//=============================================================================
void SetCamera(void) 
{
	// �r���[�}�g���b�N�X�ݒ�
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Camera.pos), XMLoadFloat3(&g_Camera.at), XMLoadFloat3(&g_Camera.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Camera.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Camera.mtxInvView, mtxInvView);


	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&g_Camera.mtxProjection, mtxProjection);

	SetShaderCamera(g_Camera.pos);
}


//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}

//=============================================================================
// �J���������_�̎擾
//=============================================================================
CAM_AT *GetCamAT(void) 
{
	return &g_CamAT;
}

//=============================================================================
// �r���[�|�[�g�̐ݒ�
//=============================================================================
void SetViewPort(int type)
{
	ID3D11DeviceContext *g_ImmediateContext = GetDeviceContext();
	D3D11_VIEWPORT vp;

	g_ViewPortType = type;

	// �r���[�|�[�g�ݒ�
	switch (g_ViewPortType)
	{
	case TYPE_FULL_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_LEFT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_RIGHT_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
		vp.TopLeftY = 0;
		break;

	case TYPE_UP_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_DOWN_HALF_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
		break;

	case TYPE_UP_LEFT_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		break;

	case TYPE_UP_RIGHT_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
		vp.TopLeftY = 0;
		break;

	case TYPE_DOWN_LEFT_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
		break;

	case TYPE_DOWN_RIGHT_SCREEN:
		vp.Width = (FLOAT)SCREEN_WIDTH / 2;
		vp.Height = (FLOAT)SCREEN_HEIGHT / 2;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (FLOAT)SCREEN_WIDTH / 2;
		vp.TopLeftY = (FLOAT)SCREEN_HEIGHT / 2;
		break;
	}
	g_ImmediateContext->RSSetViewports(1, &vp);

}


int GetViewPortType(void)
{
	return g_ViewPortType;
}



// �J�����̎��_�ƒ����_���Z�b�g
void SetCameraAT(XMFLOAT3 pos)
{
	// �J�����̒����_��ݒ�
	g_Camera.at = pos;

	// �J�����̎��_���J������Y����]�ɑΉ������Ă���
	g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
	g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;

}

void InitCamGrobal(void)
{
	g_MoveCnt = 0;

}

// �~��ɃJ�����̍��W�𓮂���
void CameraRotation(XMFLOAT3 CamAT_pos, float Cam_posY, float len, float degPerSec, float degStart, int time)
{
	g_MoveCnt++;

	// �J�����̒����_��ݒ�
	g_CamAT.pos = CamAT_pos;

	g_Camera.pos.x = CamAT_pos.x + len * cos(g_MoveCnt * XM_PI / 10800 * degPerSec + XM_PI /180 * degStart);	// ��b��deg����]����
	g_Camera.pos.z = CamAT_pos.z - len * sin(g_MoveCnt * XM_PI / 10800 * degPerSec + XM_PI /180 * degStart);	// ��b��deg����]����
	g_Camera.pos.y = Cam_posY;

	if (g_MoveCnt + 50 == time)
		SetFade(FADE_OUT);
	if (g_MoveCnt == time)
		InitCamGrobal();
}
