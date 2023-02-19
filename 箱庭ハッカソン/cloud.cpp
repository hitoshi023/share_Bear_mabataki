//=============================================================================
//
// �G�l�~�[���f������ [cloud.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "cloud.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_CLOUD			"data/MODEL/cloud.obj"		// �ǂݍ��ރ��f����
#define	MODEL_HEAD			"data/MODEL/bear_head.obj"
#define	MODEL_BODY			"data/MODEL/bear_body.obj"
#define	MODEL_LEFTHAND		"data/MODEL/bear_lefthand.obj"
#define	MODEL_RIGHTHAND		"data/MODEL/bear_righthand.obj"
#define	MODEL_LEFTLEG		"data/MODEL/bear_leftleg.obj"
#define	MODEL_RIGHTLEG		"data/MODEL/bear_rightleg.obj"
#define	MODEL_HANDGUN		"data/MODEL/handgun.obj"
#define MODEL_GRASS			"data/MODEL/garss.obj"
#define MODE_HOUSE			"data/MODEL/newhome.obj"
#define MODE_CLOUD			"data/MODEL/cloud.obj"
#define MODE_DOOR			"data/MODEL/door.obj"

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define CLOUD_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define CLOUD_OFFSET_Y		(7.0f)						// �G�l�~�[�̑��������킹��

#define CLOUD_SCALE		(1.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetCloud();

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CLOUD			g_Cloud[MAX_CLOUD];				// �G�l�~�[
static CLOUD			g_Door;

int g_Cloud_load = 0;
int g_Door_load = 0;

bool canOpenDoor = false;
int canOpenDoorCount = 0;

int makeCloudCount = 0;
#define MAKE_CLOUD_PADDING	(100)

int makeDoorCount = 0;

//	���˂̈ʒu
XMFLOAT3 chimneyPos = XMFLOAT3(1160.0f,700.0f,1012.0f);

//	�ړI�̍��W
XMFLOAT3 finalCloudPos = XMFLOAT3(chimneyPos.x + 100.0f, chimneyPos.y + 300.0f, chimneyPos.z);


//	�h�A�̈ʒu
XMFLOAT3 doorPos = XMFLOAT3(1040.0f, 114.0f, 385.0f);

//


//�@���`���(��)
static INTERPOLATION_DATA_CLOUD g_MoveTbl0[] = {
	//���W									��]��							�g�嗦						RGBA						����
	{ chimneyPos,						XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 1.0f),		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),				240 },
	{ finalCloudPos,					XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(10.0f, 10.0f, 10.0f),	XMFLOAT4(1.0f,1.0f,1.0f,0.0f),				60 },
};


static INTERPOLATION_DATA_CLOUD* g_MoveTblAdr[] =
{
	g_MoveTbl0,
};


static INTERPOLATION_DATA g_MoveTbl1[] = {
	//���W									��]��							�g�嗦							����
	{doorPos,	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(10.0f, 10.0f, 10.0f),	60 },
	{doorPos,	XMFLOAT3(0.0f, -1.57f, 0.0f),	XMFLOAT3(10.0f, 10.0f, 10.0f),	60 },

};

static INTERPOLATION_DATA* g_MoveTblAdr1[] =
{
	g_MoveTbl1,
};


//=============================================================================
// ����������
//=============================================================================
HRESULT InitCloud(void)
{
	for (int i = 0; i < MAX_CLOUD; i++)
	{
		LoadModel(MODEL_CLOUD, &g_Cloud[i].model);
		g_Cloud[i].load = true;

		g_Cloud[i].pos = XMFLOAT3(-50.0f , CLOUD_OFFSET_Y, 20.0f);
		g_Cloud[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Cloud[i].scl = XMFLOAT3(CLOUD_SCALE, CLOUD_SCALE, CLOUD_SCALE);

		g_Cloud[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Cloud[i].size = CLOUD_SIZE;	// �����蔻��̑傫��
		g_Cloud[i].count = 0;
		g_Cloud[i].diff = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Cloud[i].model, &g_Cloud[i].diffuse[i]);

		XMFLOAT3 pos = g_Cloud[i].pos;
		pos.y -= (CLOUD_OFFSET_Y - 0.1f);
		g_Cloud[i].shadowIdx = CreateShadow(pos, CLOUD_SHADOW_SIZE, CLOUD_SHADOW_SIZE);
		
		g_Cloud[i].use = false;		// true:�����Ă�

	}

	//	�h�A
	LoadModel(MODE_DOOR, &g_Door.model);
	g_Door.load = true;
	g_Door.pos = doorPos;
	g_Door.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Door.scl = XMFLOAT3(10.0f, 10.0f, 10.0f);

	g_Door.time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
	g_Door.tblNo = 0;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Door.tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	
	//g_Cloud[0].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
	//g_Cloud[0].tblNo = 0;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	//g_Cloud[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA_CLOUD);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCloud(void)
{

	for (int i = 0; i < MAX_CLOUD; i++)
	{
		if (g_Cloud[i].load)
		{
			UnloadModel(&g_Cloud[i].model);
			g_Cloud[i].load = false;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCloud(void)
{

	makeCloudCount++;
	if (makeCloudCount > MAKE_CLOUD_PADDING) {
		SetCloud();
		makeCloudCount = 0;
	}

	canOpenDoorCount++;
	if (canOpenDoorCount >= (35 * 60)) {
		canOpenDoor = true;
	}

		//�h�A�̐��`���
	if (canOpenDoor) {
		if (g_Door.tblMax > 0)	// ���`��Ԃ����s����H
		{	// ���`��Ԃ̏���
			int nowNo = (int)g_Door.time;			// �������ł���e�[�u���ԍ������o���Ă���
			int maxNo = g_Door.tblMax;				// �o�^�e�[�u�����𐔂��Ă���
			int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
			INTERPOLATION_DATA* tbl = g_MoveTblAdr1[g_Door.tblNo];	// �s���e�[�u���̃A�h���X���擾

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�


			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���


			float nowTime = g_Door.time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

			Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
			Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���


			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			XMStoreFloat3(&g_Door.pos, nowPos + Pos);

			// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Door.rot, nowRot + Rot);

			// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Door.scl, nowScl + Scl);
			//g_Door.w = TEXTURE_WIDTH * g_Door.scl.x;
			//g_Door.h = TEXTURE_HEIGHT * g_Door.scl.y;



			// frame���g�Ď��Ԍo�ߏ���������
			g_Door.time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
			if ((int)g_Door.time >= maxNo - 1)			// �o�^�e�[�u���Ō�܂ňړ��������H
			{
				//g_Door.time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
				g_Door.tblMax = 0;
				g_Door.use = false;
			}

		}
	}

	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_CLOUD; i++)
	{
		if (g_Cloud[i].use == true)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes
			SetModelDiffuse(&g_Cloud[i].model, 0, g_Cloud[i].diff);
			if (g_Cloud[i].tblMax > 0)	// ���`��Ԃ����s����H
			{	// ���`��Ԃ̏���
				int nowNo = (int)g_Cloud[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Cloud[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				INTERPOLATION_DATA_CLOUD* tbl = g_MoveTblAdr[g_Cloud[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�
				XMVECTOR nowDiff = XMLoadFloat4(&tbl[nowNo].diff);

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���
				XMVECTOR Diff = XMLoadFloat4(&tbl[nextNo].diff) - nowDiff; // RGBA

				float nowTime = g_Cloud[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
				Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���
				Diff *= nowTime;

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&g_Cloud[i].pos, nowPos + Pos);

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Cloud[i].rot, nowRot + Rot);

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Cloud[i].scl, nowScl + Scl);
				//g_Cloud[i].w = TEXTURE_WIDTH * g_Cloud[i].scl.x;
				//g_Cloud[i].h = TEXTURE_HEIGHT * g_Cloud[i].scl.y;

				// RGBA
				XMStoreFloat4(&g_Cloud[i].diff, nowDiff + Diff);

				// frame���g�Ď��Ԍo�ߏ���������
				g_Cloud[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
				if ((int)g_Cloud[i].time >= maxNo - 1)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					//g_Cloud[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
					g_Cloud[i].tblMax = 0;
					g_Cloud[i].use = false;
				}

			}
		}
	}




#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P))
	{
		// ���f���̐F��ύX�ł����I�������ɂ��ł����B
		for (int j = 0; j < g_Cloud[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Cloud[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// ���f���̐F�����ɖ߂��Ă���
		for (int j = 0; j < g_Cloud[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Cloud[0].model, j, g_Cloud[0].diffuse[j]);
		}
	}
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCloud(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_CLOUD; i++)
	{
		if (g_Cloud[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Cloud[i].scl.x, g_Cloud[i].scl.y, g_Cloud[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Cloud[i].rot.x, g_Cloud[i].rot.y + XM_PI, g_Cloud[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Cloud[i].pos.x, g_Cloud[i].pos.y, g_Cloud[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Cloud[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Cloud[i].model);
	}

	{

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Door.scl.x, g_Door.scl.y, g_Door.scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Door.rot.x, g_Door.rot.y + XM_PI, g_Door.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Door.pos.x, g_Door.pos.y, g_Door.pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Door.mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Door.model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
CLOUD *GetCloud()
{
	return &g_Cloud[0];
}

void SetCloud() {
	for (int i = 0; i < MAX_CLOUD; i++) {
		if (g_Cloud[i].use == false) {
			g_Cloud[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
			g_Cloud[i].tblNo = 0;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
			g_Cloud[i].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA_CLOUD);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
			g_Cloud[i].use = true;
			return;
		}
	}
}