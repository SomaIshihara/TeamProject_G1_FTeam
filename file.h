//==========================================
//
//ファイル読み込みプログラムのヘッダ[file.h]
//Author:石原颯馬
//
//==========================================
#ifndef _FILE_H_
#define _FILE_H_

#include "main.h"
#include "model.h"

//マクロ
//なし

//コード名
//共通
//読み込み開始・終了
#define CODE_SCRIPT				"SCRIPT"			//スクリプト開始
#define CODE_END_SCRIPT			"END_SCRIPT"		//スクリプト終了

//オリジナル
//敵配置
#define CODE_SETENEMY			"SETENEMY"			//敵配置(使用パラメータ:pos,rot,nameIdx,life)
#define CODE_END_SETENEMY		"END_SETENEMY"		//敵配置終了

//パラメータ
#define CODE_NAMEIDX			"NAMEIDX"			//名前の番号
#define CODE_LIFE				"LIFE"				//体力

//モデルビューワー・モーションビューワー用
//モデルビューワ
//テクスチャ数
#define CODE_NUM_TEXTURE		"NUM_TEXTURE"		//テクスチャ数読み取り
//テクスチャファイル名
#define CODE_TEXTURE_FILENAME	"TEXTURE_FILENAME"	//テクスチャファイルパス読み取り
//モデル数
#define CODE_NUM_MODEL			"NUM_MODEL"			//モデル数読み取り
//モデルファイル名
#define CODE_MODEL_FILENAME		"MODEL_FILENAME"	//モデルファイルパス読み取り
//カメラ情報
#define CODE_CAMERASET			"CAMERASET"			//カメラの設定開始
#define CODE_END_CAMERASET		"END_CAMERASET"		//カメラの設定終了
//ライト情報
#define CODE_LIGHTSET			"LIGHTSET"			//ライト設定開始
#define CODE_END_LIGHTSET		"END_LIGHTSET"		//ライト設定終了
//空情報
#define CODE_SKYSET				"SKYSET"			//空設定開始
#define CODE_END_SKYSET			"END_SKYSET"		//空設定終了
//山情報
#define CODE_MOUNTAINSET		"MOUNTAINSET"		//山設定開始
#define CODE_END_MOUNTAINSET	"END_MOUNTAINSET"	//山設定終了
//地面情報
#define CODE_FIELDSET			"FIELDSET"			//地面設定開始
#define CODE_END_FIELDSET		"END_FIELDSET"		//地面設定終了
//壁情報
#define CODE_WALLSET			"WALLSET"			//壁設定開始
#define CODE_END_WALLSET		"END_WALLSET"		//壁設定終了
//モデル情報
#define CODE_MODELSET			"MODELSET"			//モデル配置開始
#define CODE_END_MODELSET		"END_MODELSET"		//モデル配置終了
//ビルボード情報
#define CODE_BILLBOARDSET		"BILLBOARDSET"		//ビルボード配置開始
#define CODE_END_BILLBOARDSET	"END_BILLBOARDSET"	//ビルボード配置終了
//プレイヤー情報
#define CODE_PLAYERSET			"PLAYERSET"			//プレイヤー配置開始
#define CODE_END_PLAYERSET		"END_PLAYERSET"		//プレイヤー配置終了

//モーションビューワー
//初期
#define CODE_NUM_MODEL			"NUM_MODEL"			//モデル数
#define CODE_MODEL_FILENAME		"MODEL_FILENAME"	//モデルのファイルパス
#define CODE_CHARACTERSET		"CHARACTERSET"		//キャラクター情報
#define CODE_END_CHARACTERSET	"END_CHARACTERSET"

//パーツ設定
#define CODE_PARTSSET			"PARTSSET"			//パーツ設定開始
#define CODE_END_PARTSSET		"END_PARTSSET"		//パーツ設定終了

//モーション設定
#define CODE_MOTIONSET			"MOTIONSET"			//モーション設定開始
#define CODE_END_MOTIONSET		"END_MOTIONSET"		//モーション設定終了
//キーセット設定
#define CODE_KEYSET				"KEYSET"			//キーセット設定開始
#define CODE_END_KEYSET			"END_KEYSET"		//キーセット設定終了
//キー設定
#define CODE_KEY				"KEY"				//キー設定開始
#define CODE_END_KEY			"END_KEY"			//キー設定終了

//パラメータ類（オリジナル・MV共通部分あり）
//モデル・モーション共通
#define CODE_POS				"POS"				//位置
#define CODE_ROT				"ROT"				//向き
//モデルビューワー用
#define CODE_REF				"REF"				//注視点
#define CODE_DIRECTION			"DIRECTION"			//方向
#define CODE_DIFFUSE			"DIFFUSE"			//拡散光
#define CODE_TEXTYPE			"TEXTYPE"			//テクスチャ種類
#define CODE_MOVE				"MOVE"				//移動量
#define CODE_BLOCK				"BLOCK"				//マス数
#define CODE_SIZE				"SIZE"				//1マス当たりの大きさ
#define CODE_TYPE				"TYPE"				//モデル類タイプ
#define CODE_STATE				"STATE"				//状態（何に使うか忘れた
#define CODE_COLLISION			"COLLISION"			//当たり判定（何に使うか忘れた
#define CODE_SHADOW				"SHADOW"			//影設定
#define CODE_ORIGIN				"ORIGIN"			//なにこれ
#define CODE_BLEND				"BLEND"				//合成の設定じゃないかな
#define CODE_MOTION_FILENAME	"MOTION_FILENAME"	//モーションのファイルパス
//モーションビューワー用
#define CODE_MOVE				"MOVE"				//移動量
#define CODE_JUMP				"JUMP"				//ジャンプ量
#define CODE_RADIUS				"RADIUS"			//半径
#define CODE_NUM_PARTS			"NUM_PARTS"			//パーツ数
#define CODE_INDEX				"INDEX"				//番号
#define CODE_PARENT				"PARENT"			//親モデルの番号
#define CODE_LOOP				"LOOP"				//ループするかの設定
#define CODE_NUM_KEY			"NUM_KEY"			//キー数
#define CODE_FRAME				"FRAME"				//フレーム数

//プロト
void InitFile(void);
void UninitFile(void);
void UpdateFile(void);
void LoadMapFile(void);
void LoadMotionFile(void);
void GetMotionInfo(MOTION_INFO *pMotionInfo);

#endif