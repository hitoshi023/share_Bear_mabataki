//=============================================================================
//
// メイン処理 [main.cpp]
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
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"Team11_hakoniwa"		// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

void CheckHit(void);


//*****************************************************************************
// グローバル変数:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;


#ifdef _DEBUG
int		g_CountFPS;							// FPSカウンタ
char	g_DebugStr[2048] = WINDOW_NAME;		// デバッグ文字表示用

#endif


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
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
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// ウィンドウの左座標
		CW_USEDEFAULT,																		// ウィンドウの上座標
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// 更新処理
				Draw();				// 描画処理

#ifdef _DEBUG	// デバッグ版の時だけ表示する
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// 分解能を戻す

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
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
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	InitRenderer(hInstance, hWnd, bWindow);

	InitLight();

	InitCamera();

	InitFade();

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	//// フィールドの初期化
	//InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	//// 壁の初期化
	//InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, XM_PI, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	//// 壁(裏側用の半透明)
	//InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, XM_PI, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// エネミーの初期化
	InitEnemy();

	// 木の初期化処理
	InitTree();

	// バレットの初期化処理
	InitBullet();

	// スコアの初期化処理
	InitScore();

	// 雲とドアの初期化処理
	InitCloud();

	// サウンドの初期化処理
	InitSound(hWnd);

	PlaySound(SOUND_LABEL_BGM_Pride);

	// ライトを有効化
	SetLightEnable(true);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	// フェードの終了処理
	UninitFade();

	// サウンドの終了処理
	UninitSound();

	// スコアの終了処理
	UninitScore();

	// バレットの終了処理
	UninitBullet();

	// 木の終了処理
	UninitTree();

	// エネミーの終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	// 影の終了処理
	UninitShadow();

	// 壁の終了処理
	//UninitMeshWall();

	// 地面の終了処理
	//UninitMeshField();

	// カメラの終了処理
	UninitCamera();

	//入力の終了処理
	UninitInput();

	// レンダラーの終了処理
	UninitRenderer();

	// 雲とドアの終了処理
	UninitCloud();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力の更新処理
	UpdateLight();

	// 入力の更新処理
	UpdateInput();

	// フェードの更新処理
	UpdateFade();

	// カメラ更新
	UpdateCamera();

	// 地面処理の更新
	//UpdateMeshField();

	// 壁処理の更新
	//UpdateMeshWall();

	// プレイヤーの更新処理
	UpdatePlayer();

	// エネミーの更新処理
	UpdateEnemy();

	// 木の更新処理
	UpdateTree();

	// バレットの更新処理
	UpdateBullet();

	// 影の更新処理
	UpdateShadow();

	//// 当たり判定
	//CheckHit();

	// スコアの更新処理
	UpdateScore();

	//　雲とドアの更新処理
	UpdateCloud();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファクリア
	Clear();

	{
		SetViewPort(TYPE_FULL_SCREEN);
		XMFLOAT3 pos = GetCamAT()->pos;
		//pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている

		// カメラ注視点の設定
		GetCamera()->at = pos;

		SetCamera();

		// 地面の描画処理
		//DrawMeshField();

		// 影の描画処理
		DrawShadow();

		// プレイヤーの描画処理
		DrawPlayer();

		// エネミーの描画処理
		DrawEnemy();

		// 壁の描画処理
		//DrawMeshWall();

		// 木の描画処理
		DrawTree();

		// バレットの描画処理
		DrawBullet();

		// 雲とドアの描画処理
		DrawCloud();

		

		// 2D表示用
		{
			// ライト・Z比較無効
			SetLightEnable(false);
			SetDepthEnable(false);

			// ↓優先度を自分で決めて枠とかの描画の順番を決める

			// スコアの描画処理
			//DrawScore();

			DrawFade();

			// ライト・Z比較有効
			SetLightEnable(true);
			SetDepthEnable(true);
		}
	}

	//// デバッグ用視点
	//{
	//	SetViewPort(TYPE_FULL_SCREEN);
	//	//XMFLOAT3 pos = GetPlayer()->pos;
	//	//pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている
	//	//SetCameraAT(pos);
	//	SetCamera();

	//	// 地面の描画処理
	//	DrawMeshField();

	//	// 影の描画処理
	//	DrawShadow();

	//	// プレイヤーの描画処理
	//	DrawPlayer();

	//	// エネミーの描画処理
	//	DrawEnemy();

	//	// 壁の描画処理
	//	DrawMeshWall();

	//	// 木の描画処理
	//	DrawTree();

	//	// バレットの描画処理
	//	DrawBullet();

	//	// 2D表示用
	//	{
	//		// ライト・Z比較無効
	//		SetLightEnable(false);
	//		SetDepthEnable(false);

	//		// ↓優先度を自分で決めて枠とかの描画の順番を決める

	//		// スコアの描画処理
	//		DrawScore();

	//		// ライト・Z比較有効
	//		SetLightEnable(true);
	//		SetDepthEnable(true);
	//	}
	//}

	//// バレット視点
	//{
	//	SetViewPort(TYPE_UP_RIGHT_SCREEN);
	//	XMFLOAT3 pos = GetBullet()->pos;
	//	pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている
	//	SetCameraAT(pos);
	//	SetCamera();

	//	// 地面の描画処理
	//	DrawMeshField();

	//	// 影の描画処理
	//	DrawShadow();

	//	// プレイヤーの描画処理
	//	DrawPlayer();

	//	// エネミーの描画処理
	//	DrawEnemy();

	//	// 壁の描画処理
	//	DrawMeshWall();

	//	// 木の描画処理
	//	DrawTree();

	//	// バレットの描画処理
	//	DrawBullet();

	//	// 2D表示用
	//	{
	//		// ライト・Z比較無効
	//		SetLightEnable(false);
	//		SetDepthEnable(false);

	//		// ↓優先度を自分で決めて枠とかの描画の順番を決める

	//		// スコアの描画処理
	//		DrawScore();

	//		// ライト・Z比較有効
	//		SetLightEnable(true);
	//		SetDepthEnable(true);
	//	}
	//}

	//// エネミー視点
	//{
	//	SetViewPort(TYPE_DOWN_LEFT_SCREEN);
	//	ENEMY* enemy = GetEnemy();
	//	XMFLOAT3 pos = enemy->pos;
	//	pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている
	//	SetCameraAT(pos);
	//	SetCamera();

	//	// 地面の描画処理
	//	DrawMeshField();

	//	// 影の描画処理
	//	DrawShadow();

	//	// プレイヤーの描画処理
	//	DrawPlayer();

	//	// エネミーの描画処理
	//	DrawEnemy();

	//	// 壁の描画処理
	//	DrawMeshWall();

	//	// 木の描画処理
	//	DrawTree();

	//	// バレットの描画処理
	//	DrawBullet();

	//	// 2D表示用
	//	{
	//		// ライト・Z比較無効
	//		SetLightEnable(false);
	//		SetDepthEnable(false);

	//		// ↓優先度を自分で決めて枠とかの描画の順番を決める

	//		// スコアの描画処理
	//		DrawScore();

	//		// ライト・Z比較有効
	//		SetLightEnable(true);
	//		SetDepthEnable(true);
	//	}
	//}

	//// ツリー視点
	//{
	//	SetViewPort(TYPE_DOWN_RIGHT_SCREEN);
	//	XMFLOAT3 pos = GetTree()->pos;
	//	pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている
	//	SetCameraAT(pos);
	//	SetCamera();

	//	// 地面の描画処理
	//	DrawMeshField();

	//	// 影の描画処理
	//	DrawShadow();

	//	// プレイヤーの描画処理
	//	DrawPlayer();

	//	// エネミーの描画処理
	//	DrawEnemy();

	//	// 壁の描画処理
	//	DrawMeshWall();

	//	// 木の描画処理
	//	DrawTree();

	//	// バレットの描画処理
	//	DrawBullet();

	//	// 2D表示用
	//	{
	//		// ライト・Z比較無効
	//		SetLightEnable(false);
	//		SetDepthEnable(false);

	//		// ↓優先度を自分で決めて枠とかの描画の順番を決める

	//		// スコアの描画処理
	//		DrawScore();

	//		// ライト・Z比較有効
	//		SetLightEnable(true);
	//		SetDepthEnable(true);
	//	}
	//}


	//SetViewPort(TYPE_FULL_SCREEN);

	//// 2D表示用
	//{
	//	// ライト・Z比較無効
	//	SetLightEnable(false);
	//	SetDepthEnable(false);

	//	// ↓優先度を自分で決めて枠とかの描画の順番を決める

	//	// スコアの描画処理
	//	DrawScore();

	//	// ライト・Z比較有効
	//	SetLightEnable(true);
	//	SetDepthEnable(true);
	//}


#ifdef _DEBUG
	// デバッグ表示
	DrawDebugProc();
#endif

	// バックバッファ、フロントバッファ入れ替え
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
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	
	ENEMY* enemy = GetEnemy();		// エネミーのポインターを初期化
	PLAYER* player = GetPlayer();	// プレイヤーのポインターを初期化

	// 敵とプレイヤーキャラ
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//敵の有効フラグをチェックする
		if (enemy[i].use == false)
			continue;

		//BCの当たり判定
		if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size))
		{
			// 敵キャラクターは倒される
			enemy[i].use = false;
			ReleaseShadow(enemy[i].shadowIdx);

			if (player->use == false) return;

			// スコアを足す
			AddScore(50);
		}
	}


	// プレイヤーの弾と敵
	BULLET* bullet = GetBullet();
	for (int b = 0; b < MAX_BULLET; b++)
	{
		//敵の有効フラグをチェックする
		if (bullet[b].use == false) continue;

		//BCの当たり判定
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			//敵の有効フラグをチェックする
			if (enemy[i].use == false) continue;

			if (CollisionBC(bullet[b].pos, enemy[i].pos, bullet[b].fWidth, enemy[i].size))
			{
				// 敵キャラクターは倒される
				enemy[i].use = false;
				ReleaseShadow(enemy[i].shadowIdx);
				bullet[b].use = false;
				ReleaseShadow(bullet[b].shadowIdx);

				AddScore(100);

				break;
				// スコアを足す
			}
		}
	}

	// プレイヤーの弾と木
	TREE* tree = GetTree();
	for (int b = 0; b < MAX_BULLET; b++)
	{
		//敵の有効フラグをチェックする
		if (bullet[b].use == false) continue;

		//BCの当たり判定
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			//敵の有効フラグをチェックする
			if (enemy[i].use == false) continue;

			if (CollisionBC(bullet[b].pos, enemy[i].pos, bullet[b].fWidth, enemy[i].size))
			{
				// 敵キャラクターは倒される
				enemy[i].use = false;
				ReleaseShadow(enemy[i].shadowIdx);
				bullet[b].use = false;
				ReleaseShadow(bullet[b].shadowIdx);

				// スコアを足す
			}
		}
	}


	//// エネミーが全部死亡したら状態遷移
	//int enemy_count = 0;
	//for (int i = 0; i < MAX_ENEMY; i++)
	//{
	//	if (enemy[i].use == false) continue;
	//	enemy_count++;
	//}

	//// エネミーが０匹？
	//if (enemy_count == 0)
	//{
	//	// SetFade(FADE_OUT, MODE_RESULT);

	//}

}

