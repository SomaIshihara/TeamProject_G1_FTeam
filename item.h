//==========================================
//
//アイテムプログラムのヘッダ[item.h]
//Author:飯田洲暉
//
//==========================================
#ifndef _ITEM_H_
#define _ITEM_H_

//アイテムの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 move;			//位置
	D3DXVECTOR3 rot;			//角度
	int			RespawnDelay;	//リスポーンするまでの時間
	int			DespawnLimit;	//消えるリミット
	float		fAlpha;			//アルファ値
	bool        buse;			//使用しているかどうか
}Item;

//プロトタイプ宣言
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(void);
void CollisionIP(int nPlayerNum);//アイテムとプレイヤーの当たり判定

#endif