//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "debugproc.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
float DtoR(float degree);
XMFLOAT3 RotZ(struct LIGHT* light);
XMFLOAT3 NorLightDir(struct LIGHT* light);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];

static FOG		g_Fog;

static	BOOL	g_FogEnable = false;

static int		colChangeCnt;
static int		sunMoveCnt;

int lightCount = 0;
int canUpdateCount = 0;
bool canUpdate = false;
#define LIGHT_PADDING	(10)

//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{

	//���C�g������
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		g_Light[i].Direction = XMFLOAT3( 0.0f, -1.0f, 0.0f );
		g_Light[i].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		g_Light[i].Ambient   = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		g_Light[i].Attenuation = 100.0f;	// ��������
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ���C�g�̃^�C�v
		g_Light[i].Enable = false;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	// ���s�����̐ݒ�i���E���Ƃ炷���j
	g_Light[0].Position = XMFLOAT3(0.0f, 50.0f, 0.0f);
	g_Light[0].Direction = NorLightDir(&g_Light[0]);		// ���̌���
	g_Light[0].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );	// ���̐F
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					// ���s����
	g_Light[0].Enable = true;									// ���̃��C�g��ON
	SetLight(0, &g_Light[0]);									// ����Őݒ肵�Ă���

	// �|�C���g���C�g�̐ݒ�
	//g_Light[1].Position = XMFLOAT3(0.0f, 300.0f, 0.0f);
	//g_Light[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//g_Light[1].Type = LIGHT_TYPE_POINT;
	//g_Light[1].degree = DtoR(10.0f);
	//g_Light[1].Enable = true;
	//g_Light[1].Attenuation = 550.0f;
	//SetLight(1, &g_Light[1]);


	// �t�H�O�̏������i���̌��ʁj
	g_Fog.FogStart = 100.0f;									// ���_���炱�̋��������ƃt�H�O��������n�߂�
	g_Fog.FogEnd   = 250.0f;									// �����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
	g_Fog.FogColor = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );		// �t�H�O�̐F
	SetFog(&g_Fog);
	SetFogEnable(g_FogEnable);		// ���̏ꏊ���`�F�b�N���� shadow

	colChangeCnt = 0;
	sunMoveCnt = 0;
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateLight(void)
{
	canUpdateCount++;
	if (canUpdateCount >= 1080) {
		canUpdate = true;
	}
	if (canUpdate) {
		lightCount++;
		if (lightCount >= LIGHT_PADDING) {
			lightCount = 0;
			g_Light[0].degree += 0.005f;
			g_Light[0].Position = RotZ(&g_Light[0]);
			g_Light[0].Direction = NorLightDir(&g_Light[0]);
			SetLight(0, &g_Light[0]);
		}
	}
	//colChangeCnt++;

	//if (colChangeCnt % 10 == 0)
	//{

	//	if (colChangeCnt % 3600 == 1080)
	//	{
	//		g_Light[0].Diffuse = XMFLOAT4{0.2f, 0.2f, 0.2f, 1.0f};
	//		SetLight(0, &g_Light[0]);

	//		// �_�����̐ݒ�
	//		g_Light[1].Position = XMFLOAT3(-200.0f, 0.0f, 0.0f);
	//		g_Light[1].Direction = XMFLOAT3(0.0f, -1.0f, -1.0f);		// ���̌���
	//		g_Light[1].Diffuse = XMFLOAT4(1.0f, 0.8f, 0.6f, 1.0f);	// ���̐F
	//		g_Light[1].Type = LIGHT_TYPE_POINT;						// ���s����
	//		g_Light[1].Enable = true;								// ���̃��C�g��ON
	//		SetLight(1, &g_Light[1]);								// ����Őݒ肵�Ă���
	//	}
	//	if (colChangeCnt % 3600 > 1080)
	//	{
	//		sunMoveCnt++;
	//		g_Light[0].Diffuse.x += 0.01f;	// R
	//		g_Light[0].Diffuse.y += 0.01f;	// G
	//		g_Light[0].Diffuse.z += 0.01f;	// B
	//		SetLight(0, &g_Light[0]);

	//		//g_Light[1].Position.x = cosf(sunMoveCnt * XM_PI / 180 + XM_PI) * 800;
	//		//g_Light[1].Position.y = sinf(sunMoveCnt * XM_PI / 180) * 800;
	//	}
	//}

#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Light Direction X:%f,Y:%f,Z:%f\n",
		g_Light[0].Direction.x, g_Light[0].Direction.y, g_Light[0].Direction.z);
	PrintDebugProc("Light Position X:%f,Y:%f,Z:%f\n",
		g_Light[0].Position.x, g_Light[0].Position.y, g_Light[0].Position.z);
#endif
}


//=============================================================================
// ���C�g�̐ݒ�
// Type�ɂ���ăZ�b�g���郁���o�[�ϐ����ς���Ă���
//=============================================================================
void SetLightData(int index, LIGHT *light)
{
	SetLight(index, light);
}


LIGHT *GetLightData(int index)
{
	return(&g_Light[index]);
}


//=============================================================================
// �t�H�O�̐ݒ�
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}


BOOL	GetFogEnable(void)
{
	return(g_FogEnable);
}


float DtoR(float degree) {
	return (3.14 / 180) * degree;
}


XMFLOAT3 RotZ(struct LIGHT* light) {
	XMFLOAT3 newPos;
	float radian = DtoR(light->degree);
	newPos.x = light->Position.x;
	newPos.y = cosf(radian) * light->Position.y - sinf(radian) * light->Position.z;
	newPos.z = sinf(radian) * light->Position.y + cosf(radian) * light->Position.z;
	return newPos;
}


//�@���s���C�g�̐��K��
XMFLOAT3 NorLightDir(struct LIGHT* light) {
	XMFLOAT3 dir;

	dir.x = 0.0f - light->Position.x;
	dir.y = 0.0f - light->Position.y;
	dir.z = 0.0f - light->Position.z;

	float distance = pow(pow(light->Position.x, 2) + pow(light->Position.y, 2) + pow(light->Position.z, 2), 0.5f);

	dir.x /= distance;
	dir.y /= distance;
	dir.z /= distance;
	
	return dir;
}