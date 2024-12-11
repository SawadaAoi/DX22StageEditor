/* ========================================
	DX22Base/
	------------------------------------
	�R���|�[�l���g�X�V���p�w�b�_
	------------------------------------
	�����F�R���|�[�l���g�̍X�V�������`
	------------------------------------
	ComponentOrder.h
========================================== */
#pragma once

// ===�񋓒�`===========
enum E_UpdateOrder
{
	OrderTransform,			// �g�����X�t�H�[��	
	OrderGroundRayCast,		// �n��(���C�L���X�g)	
	OrderRigidBody,			// ����
	OrderCollisionBase,		// �����蔻����
	OrderCollisionSphere,	// �����蔻��(��)
	OrderCollisionAABB,		// �����蔻��(AABB)
	OrderCollisionOBB,		// �����蔻��(OBB)
	OrderGround,			// �n��
	OrderModelStatic,		// ���f��(�ÓI)
	OrderModelAnim,			// ���f��(�A�j���[�V����)
	OrderPlayerControl,		// �v���C���[����
	OrderBillboard,			// �r���{�[�h
	OrderGeometry,			// �W�I���g��
	OrderCameraBase,		// �J�������
	OrderCameraDebug,		// �J����(�f�o�b�O)	
	OrderLightBase,			// ���C�g���
};