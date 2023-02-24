//==========================================
//
//モデルプログラムのヘッダ[model.h]
//Author:石原颯馬
//
//==========================================
#ifndef _MODEL_H_
#define _MODEL_H_

//マクロ
#define MAX_PARTS	(10)	//パーツ使用最大数

//配置モデル列挙
typedef enum
{
	OBJECTTYPE_FENCE = 0,
	OBJECTTYPE_MAX
} OBJECTTYPE;

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
	MOTIONTYPE_ACTION,		//アクション
	MOTIONTYPE_JUMP,		//ジャンプ
	MOTIONTYPE_LANDING,		//着地
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
	D3DXVECTOR3 pos;		//モーション設定した位置（オブジェクトの場合こちらのみ使用）
	D3DXVECTOR3 rotOffset;	//モーション設定前の向き（オフセット）
	D3DXVECTOR3 rot;		//モーション設定した向き（オブジェクトの場合こちらのみ使用）
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	LPDIRECT3DTEXTURE9 apTexture[MAX_TEXTURE];	//テクスチャポインタ
	int mIdxModelParent;	//親モデルインデックス
	bool bUse;				//使用の有無
} Parts;

//モデル構造体
typedef struct
{
	Parts aParts[MAX_PARTS];	//パーツ構造体
	char aModelFileName[MAX_PARTS][MAX_PATH_STR];
	float fMove;	//移動量
	float fJump;	//ジャンプ量
	float fRadius;	//半径（未使用）
	int nNumParts;	//パーツ数
} Model;

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
Model GetAnimal(ANIMAL animal);
Model GetXObject(OBJECTTYPE objtype);

#endif // !_MODEL_H_
