//=============================================================================
//
// �J�������� [camera.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct CAMERA
{
	XMFLOAT4X4			mtxView;		// �r���[�}�g���b�N�X
	XMFLOAT4X4			mtxInvView;		// �r���[�}�g���b�N�X
	XMFLOAT4X4			mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	
	XMFLOAT3			pos;			// �J�����̎��_(�ʒu)
	XMFLOAT3			at;				// �J�����̒����_
	XMFLOAT3			up;				// �J�����̏�����x�N�g��
	XMFLOAT3			rot;			// �J�����̉�]
	
	float				len;			// �J�����̎��_�ƒ����_�̋���

	float				time;			// ���`��ԗp
	int					tblNo;			// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;			// ���̃e�[�u���̃f�[�^��

	bool				canMove;		// �����邩�̃t���O

};

struct CAM_AT
{
	XMFLOAT3			pos;			// �J�����̒����_(�ʒu)

	float				time;			// ���`��ԗp
	int					tblNo;			// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;			// ���̃e�[�u���̃f�[�^��

	bool				use;			// �J���������_�̎g�p�t���O
};


enum {
	TYPE_FULL_SCREEN,
	TYPE_LEFT_HALF_SCREEN,
	TYPE_RIGHT_HALF_SCREEN,
	TYPE_UP_HALF_SCREEN,
	TYPE_DOWN_HALF_SCREEN,
	TYPE_UP_LEFT_SCREEN,
	TYPE_UP_RIGHT_SCREEN,
	TYPE_DOWN_LEFT_SCREEN,
	TYPE_DOWN_RIGHT_SCREEN,
	TYPE_NONE,

};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

CAMERA* GetCamera(void);
CAM_AT* GetCamAT(void);

void SetViewPort(int type);
int GetViewPortType(void);

void SetCameraAT(XMFLOAT3 pos);