//==========================================
//
//���f���v���O�����̃w�b�_[model.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _MODEL_H_
#define _MODEL_H_

//�}�N��
#define MAX_PARTS	(32)	//�p�[�c�g�p�ő吔

//������ޗ񋓁i��U���̂����̂݁j
typedef enum
{
	ANIMAL_WILDBOAR = 0,	//���̂���
	ANIMAL_MAX
} ANIMAL;

//���[�V�����̎��
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	//�j���[�g����
	MOTIONTYPE_MOVE,		//�ړ�
	MOTIONTYPE_ACTION,		//�A�N�V�����iEC���g�p�E�s�N�g���񖢎����j
	MOTIONTYPE_JUMP,		//�W�����v�iEC���g�p�E�s�N�g���񖢎����j
	MOTIONTYPE_LANDING,		//���n�iEC���g�p�E�s�N�g���񖢎����j
	MOTIONTYPE_MAX
} MOTIONTYPE;

//�L�[�\����
typedef struct
{
	float mPosX;	//�ʒuX
	float mPosY;	//�ʒuY
	float mPosZ;	//�ʒuZ
	float mRotX;	//����X
	float mRotY;	//����Y
	float mRotZ;	//����Z
} KEY;

//�L�[���̍\����
typedef struct
{
	int mFrame;	//�Đ��t���[��
	KEY aKey[MAX_PARTS];	//���f���̃L�[�v�f
} KEY_INFO;

//���[�V������ԍ\����
typedef struct
{
	bool mLoop;	//���[�v���邩
	int mNumKey;//�L�[����
	KEY_INFO aKeyInfo[16];	//�L�[�̏��i16�͓K�X�ς��āj
} MOTION_INFO;

//���[�V�����\����
typedef struct
{
	//�A�j���[�V�����֌W
	MOTION_INFO aMotionInfo[MOTIONTYPE_MAX];	//���[�V�������
	int mNumMotion;								//���[�V�����̑���
	MOTIONTYPE motionType;						//���[�V�������
	bool mLoopMotion;							//���[�v���邩
	int mNumKey;								//�L�[����
	int mKey;									//���݂̃L�[No.
	int mCounterMotion;							//���[�V�����J�E���^�[
} Motion;

//�p�[�c�\����
typedef struct
{
	//�`��֌W
	LPD3DXMESH pMesh;		//���b�V��
	LPD3DXBUFFER pBuffMat;	//�}�e���A���|�C���^
	DWORD dwNumMatModel;	//�}�e��
	D3DXVECTOR3 posOffset;	//���[�V�����ݒ�O�̈ʒu�i�I�t�Z�b�g�j
	D3DXVECTOR3 pos;		//���[�V�����ݒ肵���ʒu
	D3DXVECTOR3 rotOffset;	//���[�V�����ݒ�O�̌����i�I�t�Z�b�g�j
	D3DXVECTOR3 rot;		//���[�V�����ݒ肵������
	D3DXMATRIX mtxWorld;	//���[���h�}�g��
	LPDIRECT3DTEXTURE9 apTexture[16];	//�e�N�X�`���|�C���^
	int mIdxModelParent;	//�e���f���C���f�b�N�X
	bool bUse;				//�g�p�̗L��
} Parts;

//���f���\����
typedef struct
{
	Parts aParts[MAX_PARTS];	//�p�[�c�\����
	//�ǉ�������̂��������牺�ɒǉ�
} Model;

//�v���g�^�C�v�錾
void InitModel(void);
void UninitModel(void);
Model GetModel(ANIMAL animal);

#endif // !_MODEL_H_
