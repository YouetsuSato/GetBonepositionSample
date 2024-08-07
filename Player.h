#pragma once
#include "Engine/GameObject.h"
#include "Weapon.h"

class Player :
    public GameObject
{
	int hSilly;
	Weapon* pWep;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	Player(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

