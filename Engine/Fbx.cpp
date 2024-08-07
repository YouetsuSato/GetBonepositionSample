#include "Fbx.h"
#include "Direct3D.h"
#include "FbxParts.h"


Fbx::Fbx():_animSpeed(0)
{
}

Fbx::~Fbx()
{
	for (int i = 0; i < parts_.size(); i++)
	{
		delete parts_[i];
	}
	parts_.clear();

	pFbxScene_->Destroy();
	pFbxManager_->Destroy();
}

HRESULT Fbx::Load(std::string fileName)
{
	// FBX�̓ǂݍ���
	pFbxManager_ = FbxManager::Create();
	pFbxScene_ = FbxScene::Create(pFbxManager_, "fbxscene");
	FbxString FileName(fileName.c_str());
	FbxImporter *fbxImporter = FbxImporter::Create(pFbxManager_, "imp");
	
	if (!fbxImporter->Initialize(FileName.Buffer(), -1, pFbxManager_->GetIOSettings()))
	{
		//���s
		return E_FAIL;
	}

	fbxImporter->Import(pFbxScene_);
	fbxImporter->Destroy();
	FbxGeometryConverter geometryConverter(pFbxManager_);
	geometryConverter.Triangulate(pFbxScene_, true, true);
	geometryConverter.RemoveBadPolygonsFromMeshes(pFbxScene_);

#pragma region SplitMesh
	//geometryConverter.SplitMeshesPerMaterial(pFbxScene_, true);

	//meshCount = pFbxScene_->GetSrcObjectCount<FbxMesh>();
	//matCount = pFbxScene_->GetSrcObjectCount<FbxSurfacePhong>();
	//std::vector<FbxSurfacePhong*> matlist;
	//for (int i = 0; i < matCount; i++)
	//{
	//		FbxSurfaceMaterial* pMaterial = pFbxScene_->GetSrcObject<FbxSurfaceMaterial>(i);
	//		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;
	//		matlist.push_back(pPhong);
	//}
	//std::vector<FbxMesh*> meshlist;
	//for (int i = 0; i < meshCount; i++)
	//{
	//	FbxMesh* pMesh = pFbxScene_->GetSrcObject<FbxMesh>(i);
	//	pMesh->GetNode();
	//	meshlist.push_back(pMesh);
	//}
#pragma endregion SplitMesh


	// �A�j���[�V�����̃^�C�����[�h�̎擾
	_frameRate = pFbxScene_->GetGlobalSettings().GetTimeMode();

	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	//�J�����g�f�B���N�g�����t�@�C�����������ꏊ�ɕύX
	char dir[MAX_PATH];
	_splitpath_s(fileName.c_str(), nullptr, 0, dir, MAX_PATH, nullptr, 0, nullptr, 0);
	SetCurrentDirectory(dir);

	//���[�g�m�[�h���擾����
	FbxNode* rootNode = pFbxScene_->GetRootNode();

	//�����̎q���̐��𒲂ׂ�
	int childCount = rootNode->GetChildCount();

	//1���`�F�b�N
	for (int i = 0; childCount > i; i++)
	{
		CheckNode(rootNode->GetChild(i), &parts_);
	}

	//pFbxScene_->GetSrcObjectCount<FbxSurfacePhong>();
	//std::vector<FbxMesh*> meshList;
	//
	//int meshCount = pFbxScene_->GetSrcObjectCount<FbxMesh>();
	//for (int i = 0; i < meshCount; ++i)
	//{
	//	// <���������ꂾ���őS�Ẵ��b�V���f�[�^���擾�ł���>
	//	FbxMesh* mesh = pFbxScene_->GetSrcObject<FbxMesh>(i);
	//	std::string name = mesh->GetName();
	//	//m_fbxMeshNames.push_back(name);
	//	//m_fbxMeshes.insert({ mesh, name });
	//	meshList.push_back(mesh);
	//}

	//�J�����g�f�B���N�g�������̈ʒu�ɖ߂�
	SetCurrentDirectory(defaultCurrentDir);

	return S_OK;
}

void Fbx::CheckNode(FbxNode * pNode, std::vector<FbxParts*>* pPartsList)
{
	//���̃m�[�h�ɂ̓��b�V����񂪓����Ă��邾�낤���H
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();
	if (attr != nullptr && attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		//�p�[�c��p��
		FbxParts* pParts = new FbxParts(this);
		pParts->Init(pNode);

		//�p�[�c���𓮓I�z��ɒǉ�
		pPartsList->push_back(pParts);
	}

	//�q�m�[�h�ɂ��f�[�^�����邩���I�I
	{
		//�q���̐��𒲂ׂ�
		int childCount = pNode->GetChildCount();

		//��l���`�F�b�N
		for (int i = 0; i < childCount; i++)
		{
			CheckNode(pNode->GetChild(i), pPartsList);
		}
	}
}

void Fbx::Release()
{
}

XMFLOAT3 Fbx::GetBonePosition(std::string boneName)
{
	XMFLOAT3 position = XMFLOAT3(0, 0, 0);
	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBonePosition(boneName, &position))
			break;
	}
	return position;
}

XMFLOAT3 Fbx::GetAnimBonePosition(std::string boneName)
{
	XMFLOAT3 position = XMFLOAT3(0, 0, 0);
	for (int i = 0; i < parts_.size(); i++)
	{
		if (parts_[i]->GetBonePositionAtNow(boneName, &position))
			break;
	}
	return position;
}


void Fbx::Draw(Transform& transform, int frame)
{
	Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);

	//�p�[�c��1���`��
	for (int k = 0; k < parts_.size(); k++)
	{
		// ���̏u�Ԃ̎����̎p���s��𓾂�
		FbxTime     time;
		time.SetTime(0, 0, 0, frame, 0, 0, _frameRate);

		//�X�L���A�j���[�V�����i�{�[���L��j�̏ꍇ
		if (parts_[k]->GetSkinInfo() != nullptr)
		{
			parts_[k]->DrawSkinAnime(transform, time);
		}

		//���b�V���A�j���[�V�����̏ꍇ
		else
		{
			parts_[k]->DrawMeshAnime(transform, time, pFbxScene_);
		}
	}
}


//���C�L���X�g�i���C���΂��ē����蔻��j
void Fbx::RayCast(RayCastData * data)
{
	//���ׂẴp�[�c�Ɣ���
	for (int i = 0; i < parts_.size(); i++)
	{
		parts_[i]->RayCast(data);
	}
}