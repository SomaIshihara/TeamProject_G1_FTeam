//==========================================
//
//モデルプログラムのヘッダ[model.h]
//Author:石原颯馬
//
//==========================================
#ifndef _MODEL_H_
#define _MODEL_H_

//マクロ
#define MAX_PARTS	(32)	//パーツ使用最大数

//動物種類列挙（一旦いのししのみ）
typedef enum
{
	ANIMAL_WILDBOAR = 0,	//いのしし
	ANIMAL_MAX
} ANIMAL;

//モーションの種類
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	//ニュートラル
	MOTIONTYPE_MOVE,		//移動
	MOTIONTYPE_ACTION,		//アクション（EC未使用・ピクトさん未実装）
	MOTIONTYPE_JUMP,		//ジャンプ（EC未使用・ピクトさん未実装）
	MOTIONTYPE_LANDING,		//着地（EC未使用・ピクトさん未実装）
	MOTIONTYPE_MAX
} MOTIONTYPE;

//キー構造体
typedef struct
{
	float mPosX;	//位置X
	float mPosY;	//位置Y
	float mPosZ;	//位置Z
	float mRotX;	//向きX
	float mRotY;	//向きY
	float mRotZ;	//向きZ
} KEY;

//キー情報の構造体
typedef struct
{
	int mFrame;	//再生フレーム
	KEY aKey[MAX_PARTS];	//モデルのキー要素
} KEY_INFO;

//モーション状態構造体
typedef struct
{
	bool mLoop;	//ループするか
	int mNumKey;//キー総数
	KEY_INFO aKeyInfo[16];	//キーの情報（16は適宜変えて）
} MOTION_INFO;

//モーション構造体
typedef struct
{
	//アニメーション関係
	MOTION_INFO aMotionInfo[MOTIONTYPE_MAX];	//モーション情報
	int mNumMotion;								//モーションの総数
	MOTIONTYPE motionType;						//モーション種類
	bool mLoopMotion;							//ループするか
	int mNumKey;								//キー総数
	int mKey;									//現在のキーNo.
	int mCounterMotion;							//モーションカウンター
} Motion;

//パーツ構造体
typedef struct
{
	//描画関係
	LPD3DXMESH pMesh;		//メッシュ
	LPD3DXBUFFER pBuffMat;	//マテリアルポインタ
	DWORD dwNumMatModel;	//マテ数
	D3DXVECTOR3 posOffset;	//モーション設定前の位置（オフセット）
	D3DXVECTOR3 pos;		//モーション設定した位置
	D3DXVECTOR3 rotOffset;	//モーション設定前の向き（オフセット）
	D3DXVECTOR3 rot;		//モーション設定した向き
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	LPDIRECT3DTEXTURE9 apTexture[16];	//テクスチャポインタ
	int mIdxModelParent;	//親モデルインデックス
} Parts;

//モデル構造体
typedef struct
{
	Parts aParts[MAX_PARTS];	//パーツ構造体
	//追加するものがあったら下に追加
} Model;

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
Model GetModel(ANIMAL animal);

#endif // !_MODEL_H_
