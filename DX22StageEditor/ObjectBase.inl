/* ========================================
	DX22Base/
	------------------------------------
	�I�u�W�F�N�g�pinl�t�@�C��
	------------------------------------
	��inl�t�@�C���̓w�b�_�t�@�C���ɋL�ڂ���Ă���̂Ɠ���
�@�@�w�b�_�[�t�@�C���̏璷����h�����߂Ɏg�p
	------------------------------------
	ObjectBase.inl
========================================== */


/* ========================================
	�R���|�[�l���g�ǉ��֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�ɃR���|�[�l���g��ǉ�����
	-------------------------------------
	�ߒl�F�ǉ������R���|�[�l���g�̃|�C���^
=========================================== */
template<typename T>
inline T* ObjectBase::AddComponent()
{
	// ���ɓ�����ނ̃R���|�[�l���g������ꍇ�͒ǉ����Ȃ�
	if (GetComponent<T>() != nullptr)
	{
		return nullptr;
	}

	auto pComponent = std::make_unique<T>(this);	// �R���|�[�l���g�𐶐�
 	int nOrder		= pComponent->GetUpdateOrder(); // �X�V�������擾


	// �}���ʒu��T��(�擪���珇�ɒT��)
	auto it = m_pComponents.begin();
	for (; it != m_pComponents.end(); it++)
	{
		// �X�V���������������ɑ}��
		if (nOrder < static_cast<ComponentBase*>((*it).get())->GetUpdateOrder())
		{
			break; // �}���ʒu���������烋�[�v�𔲂���
		}
	}

	// �R���|�[�l���g��}�����A���̈ʒu�̃C�e���[�^��ێ�
	auto insertedIt = m_pComponents.insert(it, std::move(pComponent));

	// ����������
	insertedIt->get()->Init();

	// �}�����ꂽ�R���|�[�l���g�̃|�C���^��Ԃ�
	return static_cast<T*>(insertedIt->get());
}

/* ========================================
	�R���|�[�l���g�擾�֐�
	-------------------------------------
	���e�F�w�肵����ނ̃R���|�[�l���g���擾����
	-------------------------------------
	�ߒl�F�擾�����R���|�[�l���g�̃|�C���^
=========================================== */
template<typename T>
inline T* ObjectBase::GetComponent()
{
	// �����R���|�[�l���g�z���T��
	for (auto& pComponent : m_pComponents)
	{
		// �w�肵����ނ̃R���|�[�l���g�����������ꍇ�͂��̃|�C���^��Ԃ�
		if (pComponent->GetTypeID() == T::GetStaticTypeID())
		{
			return static_cast<T*>(pComponent.get());
		}
	}

	return nullptr; // ������Ȃ������ꍇ��nullptr��Ԃ�
}

/* ========================================
	�R���|�[�l���g�폜�֐�
	-------------------------------------
	���e�F�w�肵����ނ̃R���|�[�l���g���폜����
=========================================== */
template<typename T>
inline void ObjectBase::RemoveComponent()
{
	for (auto it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
	{
		if ((*it)->GetTypeID() == T::GetStaticTypeID())
		{
			(*it)->Uninit(); // �I������
			m_pComponents.erase(it); // �z�񂩂�R���|�[�l���g�폜
			return;
		}
	}

}
