#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Weapon.h"
#include "TestScene.h"
#include "dummyParent.h"

Player::Player(GameObject* parent)
	:GameObject(parent), hSilly(-1)
{
}

void Player::Initialize()
{
	
	dummyParent* p = Instantiate<dummyParent>(this->GetParent());
	hSilly = Model::Load("GolfDrive.fbx");

#pragma region DummyCode
	//hSilly = Model::Load("SillyDancing.fbx");
	//hSilly = Model::Load("Ch09_nonPBR.fbx");
	//Model::SetAnimFrame(hSilly, 0, 229, 1.0); //dance

	Model::SetAnimFrame(hSilly, 0, 203, 1.0); //golf
	//pWep = ((TestScene *)GetParent())->pWp;

	pWep = Instantiate<Weapon>(p);
	Transform tr1,tr2;
	
	//tr1.position_.x = Model::GetBonePosition(hSilly, "mixamorig6:RightHand").x;
	//tr1.position_.y = Model::GetBonePosition(hSilly, "mixamorig6:RightHand").y;
	//tr1.position_.z = Model::GetBonePosition(hSilly, "mixamorig6:RightHand").z;

	tr1.position_.x = Model::GetAnimBonePosition(hSilly, "mixamorig6:RightHand").x;
	tr1.position_.y = Model::GetAnimBonePosition(hSilly, "mixamorig6:RightHand").y;
	tr1.position_.z = Model::GetAnimBonePosition(hSilly, "mixamorig6:RightHand").z;
	pWep->SetPosition( tr1.position_ );

	tr2.position_.x = Model::GetAnimBonePosition(hSilly, "mixamorig6:RightHandMiddle").x;
	tr2.position_.y = Model::GetAnimBonePosition(hSilly, "mixamorig6:RightHandMiddle").y;
	tr2.position_.z = Model::GetAnimBonePosition(hSilly, "mixamorig6:RightHandMiddle").z;
#pragma endregion Œã‚Å–ß‚·

}

void Player::Update()
{
	transform_.rotate_.y +=1;
	
	pWep->SetPosition(Model::GetAnimBonePosition(hSilly, "mixamorig6:RightHand"));


}

void Player::Draw()
{
	transform_.scale_ = { 0.05,0.05,0.05 };
	transform_.position_ = { 0, -3.0, 0 };
	Model::SetTransform(hSilly, transform_);
	Model::Draw(hSilly);
}


void Player::Release()
{
}
