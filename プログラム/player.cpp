//=============================================================================
//
// ���f������ [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "light.h"
#include "bullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	MODEL_PLAYER		"data/MODEL/cone.obj"			// �ǂݍ��ރ��f����
//#define	MODEL_PLAYER_PARTS	"data/MODEL/torus.obj"			// �ǂݍ��ރ��f����

#define		MODEL_PLAYER			"data/MODEL/bear_body.obj"		// ��
#define		MODEL_PLAYER_PARTS_H	"data/MODEL/bear_head.obj"		// ��
#define		MODEL_PLAYER_PARTS_M	"data/MODEL/bear_mouse.obj"		// ��
#define		MODEL_PLAYER_PARTS_E	"data/MODEL/bear_eye.obj"		// ��
#define		MODEL_PLAYER_PARTS_RA	"data/MODEL/bear_Rarm.obj"		// �E�r
#define		MODEL_PLAYER_PARTS_LA	"data/MODEL/bear_Larm.obj"		// ���r
#define		MODEL_PLAYER_PARTS_RL	"data/MODEL/bear_Rleg.obj"		// �E��
#define		MODEL_PLAYER_PARTS_LL	"data/MODEL/bear_Lleg.obj"		// ����
#define		MODEL_PLAYER_PARTS_TAIL	"data/MODEL/bear_tail.obj"		// ����



#define		VALUE_MOVE			(2.0f)							// �ړ���
#define		VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define		PLAYER_SHADOW_SIZE	(1.0f)							// �e�̑傫��
#define		PLAYER_OFFSET_Y		(15.0f)							// �v���C���[�̑��������킹��

#define		PLAYER_PARTS_MAX	(8)								// �v���C���[�̃p�[�c�̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Player;						// �v���C���[

static PLAYER		g_Parts[PLAYER_PARTS_MAX];		// �v���C���[�̃p�[�c�p


// �v���C���[�̊K�w�A�j���[�V�����f�[�^
// �v���C���[�̓������E�ɓ������Ă���A�j���f�[�^
//static INTERPOLATION_DATA move_tbl_right[] = {	// pos, rot, scl, frame
//	{ XMFLOAT3(20.0f, 20.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
//	{ XMFLOAT3(20.0f, 20.0f, 0.0f), XMFLOAT3(XM_PI / 2, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 180 },
//
//};

static INTERPOLATION_DATA move_tbl_head[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

//static INTERPOLATION_DATA move_tbl_left[] = {	// pos, rot, scl, frame
//	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),         XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
//	{ XMFLOAT3(-20.0f, 10.0f, 0.0f), XMFLOAT3(XM_PI / 2, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 180 },
//
//};

static INTERPOLATION_DATA move_tbl_mouse[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 20, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_eye[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(0.0f, 30.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 0.1f, 1.0f), 10 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_Rarm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 12,  0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 12, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },

};

static INTERPOLATION_DATA move_tbl_Larm[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 12, 0.0f,0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 12,  0.0f,0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },

};

static INTERPOLATION_DATA move_tbl_Rleg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 12, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 12,  0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },

};

static INTERPOLATION_DATA move_tbl_Lleg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(XM_PI / 12,  0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-XM_PI / 12, 0.0f, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },

};

static INTERPOLATION_DATA move_tbl_tail[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI / 12, 0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI / 12,  0.0f),      XMFLOAT3(1.0f, 1.0f, 1.0f), 25 },

};



static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	move_tbl_head,
	move_tbl_Rarm,
	move_tbl_Larm,
	move_tbl_Rleg,
	move_tbl_Lleg,
	move_tbl_tail,
	move_tbl_mouse,
	move_tbl_eye,

};



//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER, &g_Player.model);
	g_Player.load = true;

	g_Player.pos = { 0.0f, PLAYER_OFFSET_Y, 0.0f };
	g_Player.rot = { 0.0f, 0.0f, 0.0f };
	g_Player.scl = { 0.2f, 0.2f, 0.2f };

	g_Player.spd = 0.0f;			// �ړ��X�s�[�h�N���A
	g_Player.size = PLAYER_SIZE;	// �����蔻��̑傫��

	g_Player.use = true;

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�



	// �K�w�A�j���[�V�����p�̏���������
	g_Player.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������

	// �p�[�c�̏�����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use = false;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);	// �e�̌��_����̃I�t�Z�b�g���W
		g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);	// �����̉�]���
		g_Parts[i].scl = XMFLOAT3(0.2f, 0.2f, 0.2f);	// �����̊g��k��

		// �e�q�֌W
		g_Parts[i].parent = &g_Player;		// �� �����ɐe�̃A�h���X������
	//	g_Parts[�r].parent= &g_Player;		// �r��������e�͖{�́i�v���C���[�j
	//	g_Parts[��].parent= &g_Paerts[�r];	// �w���r�̎q���������ꍇ�̗�

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
		g_Parts[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Parts[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Parts[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
		g_Parts[i].load = 0;
	}

	// ���̐ݒ�
	g_Parts[0].use = true;
	g_Parts[0].parent = &g_Player;			// �e���Z�b�g
	g_Parts[0].tblNo = 0;					// �Đ�����A�j���f�[�^�̐퓬�A�h���X���Z�b�g
	g_Parts[0].tblMax = sizeof(move_tbl_head) / sizeof(INTERPOLATION_DATA);
	g_Parts[0].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_H, &g_Parts[0].model);

	// �E�r�̐ݒ�
	g_Parts[1].use = true;
	g_Parts[1].parent = &g_Player;			// �e���Z�b�g
	g_Parts[1].tblNo = 1;					// �Đ�����A�j���f�[�^�̐퓬�A�h���X���Z�b�g
	g_Parts[1].tblMax = sizeof(move_tbl_Rarm) / sizeof(INTERPOLATION_DATA);
	g_Parts[1].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_RA, &g_Parts[1].model);

	// �E�r�̐ݒ�
	g_Parts[2].use = true;
	g_Parts[2].parent = &g_Player;			// �e���Z�b�g
	g_Parts[2].tblNo = 2;					// �Đ�����A�j���f�[�^�̐퓬�A�h���X���Z�b�g
	g_Parts[2].tblMax = sizeof(move_tbl_Larm) / sizeof(INTERPOLATION_DATA);
	g_Parts[2].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_LA, &g_Parts[2].model);

	// �E�r�̐ݒ�
	g_Parts[3].use = true;
	g_Parts[3].parent = &g_Player;			// �e���Z�b�g
	g_Parts[3].tblNo = 3;					// �Đ�����A�j���f�[�^�̐퓬�A�h���X���Z�b�g
	g_Parts[3].tblMax = sizeof(move_tbl_Rleg) / sizeof(INTERPOLATION_DATA);
	g_Parts[3].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_RL, &g_Parts[3].model);

	// �E�r�̐ݒ�
	g_Parts[4].use = true;
	g_Parts[4].parent = &g_Player;			// �e���Z�b�g
	g_Parts[4].tblNo = 4;					// �Đ�����A�j���f�[�^�̐퓬�A�h���X���Z�b�g
	g_Parts[4].tblMax = sizeof(move_tbl_Lleg) / sizeof(INTERPOLATION_DATA);
	g_Parts[4].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_LL, &g_Parts[4].model);

	// �E�r�̐ݒ�
	g_Parts[5].use = true;
	g_Parts[5].parent = &g_Player;			// �e���Z�b�g
	g_Parts[5].tblNo = 5;					// �Đ�����A�j���f�[�^�̐퓬�A�h���X���Z�b�g
	g_Parts[5].tblMax = sizeof(move_tbl_tail) / sizeof(INTERPOLATION_DATA);
	g_Parts[5].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_TAIL, &g_Parts[5].model);

	// ���̐ݒ�
	g_Parts[6].use = true;
	g_Parts[6].parent = &g_Player;			// �e���Z�b�g
	g_Parts[6].tblNo = 6;					// �Đ�����A�j���f�[�^�̐퓬�A�h���X���Z�b�g
	g_Parts[6].tblMax = sizeof(move_tbl_mouse) / sizeof(INTERPOLATION_DATA);
	g_Parts[6].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_M, &g_Parts[6].model);

	// �ڂ̐ݒ�
	g_Parts[7].use = true;
	g_Parts[7].parent = &g_Player;			// �e���Z�b�g
	g_Parts[7].tblNo = 7;					// �Đ�����A�j���f�[�^�̐퓬�A�h���X���Z�b�g
	g_Parts[7].tblMax = sizeof(move_tbl_eye) / sizeof(INTERPOLATION_DATA);
	g_Parts[7].load = 1;
	LoadModel(MODEL_PLAYER_PARTS_E, &g_Parts[7].model);



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// ���f���̉������
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = false;
	}

	// �p�[�c�̉������
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		if (g_Parts[i].load)
		{
			UnloadModel(&g_Parts[i].model);
			g_Parts[i].load = false;
		}
	}



}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

	// �ړ��������Ⴄ
	if (GetKeyboardPress(DIK_LEFT))
	{	// ���ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.dir = XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{	// �E�ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.dir = -XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_UP))
	{	// ��ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.dir = XM_PI;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{	// ���ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.dir = 0.0f;
	}


#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.rot.y = g_Player.dir = 0.0f;
		g_Player.spd = 0.0f;
	}
#endif


	//	// Key���͂���������ړ���������
	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y = g_Player.dir + cam->rot.y;

		// ���͂̂����������փv���C���[���������Ĉړ�������
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}

	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);

	// �e���ˏ���
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		SetBullet(g_Player.pos, g_Player.rot);
	}

	g_Player.spd *= 0.5f;

	if (g_Player.spd > 0.1f)
	{
		// �K�w�A�j���[�V����
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			// �g���Ă���Ȃ珈������
			if ((g_Parts[i].use == true) && (g_Parts[i].tblMax > 0))
			{	// ���`��Ԃ̏���
				int nowNo = (int)g_Parts[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Parts[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

				float nowTime = g_Parts[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
				Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

				// frame���g�Ď��Ԍo�ߏ���������
				g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
				if ((int)g_Parts[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Parts[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
				}

			}

		}
	}


	{	// �|�C���g���C�g�̃e�X�g
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = true;
		//SetLightData(1, light);
	}


#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Player:�� �� �� ���@Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	// 
	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);


	// ���f���`��
	DrawModel(&g_Player.model);



	// �p�[�c�̊K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		////// �p�[�c�̕`�揈�� //////
		// 1.�܂��̓p�[�c���g��SRT���s��
		// 2.�e���q�����`�F�b�N����
		// 3.�q���������ꍇ�A�e��mtzWorld�Ǝ�����mtxWorld���|���Z����
		// 4.���̌��ʁimtxWorld�j���g���Ď����i�p�[�c�j��`�悷��


		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
		{
			// �e��mtxWorld�͂ǂ��ɂ���̂�
			// g_Parts[i].parent->mtxWorld = g_Player.mtxWorld
			// g_Parts[i].prent = &g_Player
			// g_Player.mtxWorld

			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
																// ��
																// g_Player.mtxWorld���w���Ă���
		}

		// ����Ȃ�q�p�[�c�̂��߂ɁA������mtxWorld��ۑ����Ă���
		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
		if (g_Parts[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		// ���f���`��
		DrawModel(&g_Parts[i].model);

	}



	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

