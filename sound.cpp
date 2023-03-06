//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : 石原颯馬
//
//=============================================================================
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SOUND_LOOP		(-1)		//ループする
#define SOUND_ONCE		(0)			//１回かぎり

//*****************************************************************************
// サウンド情報の構造体定義
//*****************************************************************************
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
	float Volume;		// ボリューム
	bool bPlay;			//再生するかどうか
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// マスターボイス
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX][NUM_DUPLICATION] = { {} };	// ソースボイス
BYTE *g_apDataAudio[SOUND_LABEL_MAX][NUM_DUPLICATION] = { {} };	// オーディオデータ
DWORD g_aSizeAudio[SOUND_LABEL_MAX][NUM_DUPLICATION] = { {} };	// オーディオデータサイズ
bool g_bPlay = true;											// 再生するかどうか

// サウンドの情報
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{ "data/SOUND/BGM/Title.wav",				SOUND_LOOP, 0.5f,true },		// タイトル音
	{ "data/SOUND/BGM/Tutorial.wav",			SOUND_LOOP, 0.5f,true },		// チュートリアル音
	{ "data/SOUND/BGM/Game000.wav",				SOUND_LOOP, 0.5f,true },		// ゲーム音
	{ "data/SOUND/BGM/Result.wav",				SOUND_LOOP, 0.5f,true },		// リザルト音
	{ "data/SOUND/SE/Uribou.wav",				SOUND_ONCE, 0.5f,true },		// コントローラー接続SE
	{ "data/SOUND/SE/Title_Decide.wav",			SOUND_ONCE, 0.6f,true },		// タイトルの決定SE
	{ "data/SOUND/SE/Collision.wav",			SOUND_ONCE, 0.6f,true },		// プレイヤー同士の衝突音SE
	{ "data/SOUND/SE/Energy.wav",				SOUND_ONCE, 0.6f,true },		// プレイヤー1のゲージ充填音
	{ "data/SOUND/SE/Energy.wav",				SOUND_ONCE, 0.6f,true },		// プレイヤー2のゲージ充填音
	{ "data/SOUND/SE/Energy.wav",				SOUND_ONCE, 0.6f,true },		// プレイヤー3のゲージ充填音
	{ "data/SOUND/SE/Energy.wav",				SOUND_ONCE, 0.6f,true },		// プレイヤー4のゲージ充填音
	{ "data/SOUND/SE/Drop000.wav",				SOUND_ONCE, 0.6f,true },		// プレイヤーの落下音
	{ "data/SOUND/SE/GrassDash000.wav",			SOUND_ONCE, 0.6f,true },		// プレイヤーのダッシュ音
	{ "data/SOUND/SE/HipDrop_Attack_002.wav",	SOUND_ONCE, 0.6f,true },		// プレイヤーのヒップドロップ音
	{ "data/SOUND/SE/Jamp001.wav",				SOUND_ONCE, 0.6f,true },		// プレイヤーのジャンプ音
	{ "data/SOUND/SE/PauseDecision000.wav",		SOUND_ONCE, 0.6f,true },		// ポーズの決定音
	{ "data/SOUND/SE/PauseSelection000.wav",	SOUND_ONCE, 0.6f,true },		// ポーズの選択音
	{ "data/SOUND/SE/PauseTransition000.wav",	SOUND_ONCE, 0.6f,true },		// ポーズの遷移音
	{ "data/SOUND/SE/Roar000.wav",				SOUND_ONCE, 0.6f,true },		// ポーズの咆哮音
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			// オーディオデータ読み込み
			hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound][nCntDupl], &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			g_apDataAudio[nCntSound][nCntDupl] = (BYTE*)malloc(g_aSizeAudio[nCntSound][nCntDupl]);
			hr = ReadChunkData(hFile, g_apDataAudio[nCntSound][nCntDupl], g_aSizeAudio[nCntSound][nCntDupl], dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// ソースボイスの生成
			hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound][nCntDupl], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// バッファの値設定
			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
			buffer.AudioBytes = g_aSizeAudio[nCntSound][nCntDupl];
			buffer.pAudioData = g_apDataAudio[nCntSound][nCntDupl];
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopCount = g_aSoundInfo[nCntSound].nCntLoop;

			// オーディオバッファの登録
			g_apSourceVoice[nCntSound][nCntDupl]->SubmitSourceBuffer(&buffer);
		}

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoice[nCntSound] != NULL)
			{
				// 一時停止
				g_apSourceVoice[nCntSound][nCntDupl]->Stop(0);

				// ソースボイスの破棄
				g_apSourceVoice[nCntSound][nCntDupl]->DestroyVoice();
				g_apSourceVoice[nCntSound][nCntDupl] = NULL;

				// オーディオデータの開放
				free(g_apDataAudio[nCntSound][nCntDupl]);
				g_apDataAudio[nCntSound][nCntDupl] = NULL;
			}
		}
	}
	
	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	// XAudio2オブジェクトの開放
	if(g_pXAudio2 != NULL)
	{
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label/*, int nNumSound*/)
{
	int nNumSound = 0;

	//ラベルの再生がON		かつ		再生がON
	if (g_aSoundInfo[label].bPlay == true && g_bPlay == true)
	{
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[label][nNumSound];
		buffer.pAudioData = g_apDataAudio[label][nNumSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundInfo[label].nCntLoop;

		// 状態取得
		g_apSourceVoice[label][nNumSound]->GetState(&xa2state);

		//音量調整
		g_apSourceVoice[label][nNumSound]->SetVolume(g_aSoundInfo[label].Volume);

		// 再生中
		if (xa2state.BuffersQueued != 0)
		{
			// 一時停止
			g_apSourceVoice[label][nNumSound]->Stop(0);

			// オーディオバッファの削除
			g_apSourceVoice[label][nNumSound]->FlushSourceBuffers();
		}

		// オーディオバッファの登録
		g_apSourceVoice[label][nNumSound]->SubmitSourceBuffer(&buffer);

		// 再生
		g_apSourceVoice[label][nNumSound]->Start(0);
	}

	return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
	{
		// 状態取得
		g_apSourceVoice[label][nCntDupl]->GetState(&xa2state);
		g_apSourceVoice[label][nCntDupl]->SetVolume(g_aSoundInfo[label].Volume);

		if (xa2state.BuffersQueued != 0)
		{// 再生中
		 // 一時停止
			g_apSourceVoice[label][nCntDupl]->Stop(0);

			// オーディオバッファの削除
			g_apSourceVoice[label][nCntDupl]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void StopSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoice[nCntSound][nCntDupl] != NULL)
			{
				// 一時停止
				g_apSourceVoice[nCntSound][nCntDupl]->Stop(0);
			}
		}
	}
}

//=============================================================================
// セグメント一時停止(ラベル指定)
//=============================================================================
void PauseSound(SOUND_LABEL label) 
{
	for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
	{
		XAUDIO2_VOICE_STATE xa2state;

		// 状態取得
		g_apSourceVoice[label][nCntDupl]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
		 // 一時停止
			g_apSourceVoice[label][nCntDupl]->Stop(0);
		}
	}
}

//=============================================================================
// セグメント再開(ラベル指定)
//=============================================================================
void RestartSound(SOUND_LABEL label)
{
	for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
	{
		XAUDIO2_VOICE_STATE xa2state;

		// 状態取得
		g_apSourceVoice[label][nCntDupl]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 一時停止中
		 // 再生
			g_apSourceVoice[label][nCntDupl]->Start(0);
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}