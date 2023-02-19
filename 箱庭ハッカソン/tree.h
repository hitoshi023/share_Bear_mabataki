//=============================================================================
//
// �؏��� [tree.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	int life;

	XMFLOAT3	pos;			// �ʒu
	XMFLOAT3	scl;			// �X�P�[��
	MATERIAL	material;		// �}�e���A��
	float		fWidth;			// ��
	float		fHeight;		// ����
	int			nIdxShadow;		// �eID
	bool		bUse;			// �g�p���Ă��邩�ǂ���

} TREE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTree(void);
void UninitTree(void);
void UpdateTree(void);
void DrawTree(void);

int SetTree(XMFLOAT3 pos, float fWidth, float fHeight, XMFLOAT4 col);

TREE *GetTree(void);