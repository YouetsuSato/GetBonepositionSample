#include "Weapon.h"
#include "Engine\Model.h"


Weapon::Weapon(GameObject* parent)
	:GameObject(parent)	,hWeapon_(-1)
{
}

void Weapon::Initialize()
{
	
	hWeapon_ = Model::Load("oden.fbx");
	//transform_.rotate_.x = 90;
	transform_.scale_ = { 0.5,0.5,0.5 };
	//transform_.position_ = { 0, 1, 0 };

	
}

void Weapon::SetWpos(const Transform& tr)
{
	//localPos.Calclation();
	//transform_. localPos.GetWorldMatrix()
}


void Weapon::Update()
{
	
}

void Weapon::Draw()
{
	Model::SetTransform(hWeapon_, transform_);
	Model::Draw(hWeapon_);
}


void Weapon::Release()
{
}

