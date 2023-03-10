/*==========================================================================================================================================================

リザルト画面処理[result.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _RESULT_H_
#define _RESULT_H_

void InitResult(void);		void InitResultObject(void);
void UninitResult(void);	void UninitResultObject(void);
void UpdateResult(void);	void UpdateResultObject(void);
void DrawResult(void);		void DrawResultObject(void);

void SkipResult(void);
void MoveSkipShutter(void);
void UpdateSkipUIVtx(void);
#endif