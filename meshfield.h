//==========================================
//
//メッシュフィールドのヘッダ[meshfield.h]
//Author:飯田洲暉
//
//==========================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_


//メッシュフィールドの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//フィールドの位置
	D3DXVECTOR3 rot;			//フィールドの向き
	float fLength_X;			//X軸の幅（半分）
	float fLength_Z;			//Y軸の幅（半分）
	int nBlock_X;				//X軸の分割数
	int nBlock_Z;				//Y軸の分割数
}MESHFIELD;

//プロトタイプ宣言
void InitMeshfield(void);
void UninitMeshfield(void);
void UpdateMeshfield(void);
void DrawMeshfield(void);

#endif 