#include"MapPolygon.h"

////////
//public
////
MapPolygon::MapPolygon() {
	mfbx_manager = 0;
	mfbx_scene = 0;
	Initialize();
}
MapPolygon::~MapPolygon() {
	Finalize();
}

//�z��̃[���N���A
bool MapPolygon::Initialize() {
	this->m_polygonStack.clear();
	this->m_polygonStack.shrink_to_fit();
	return true;
}
//���
void MapPolygon::Finalize() {
	for (auto it = this->m_polygonStack.begin(); it < this->m_polygonStack.end(); ++it) {
		delete[](*it).polygon;
	}

	if (this->mfbx_manager != nullptr) {
		this->mfbx_manager->Destroy();
	}
	if (this->collisionMesh != nullptr) {
		delete collisionMesh;
	}
}

bool MapPolygon::LoadModel(const char *filename) {
	if(!InitializeFBX(filename)) {
		return false;
	}

	if (!LoadFBXNodeRecursive(this->mfbx_scene->GetRootNode())) {
		return false;
	}
	return true;
}


//�|���S�������l��
int MapPolygon::GetNumFace() {
	return m_numFace;
}

//bullet�ɒn�`�̎O�p���b�V�������̂Ƃ��Ēǉ�
void MapPolygon::setCollisionWorld(BulletPhysics *physics) {
	std::vector<btVector3> vectices;
	for (int i = 0; i < this->m_numFace; ++i) {
		for (int k = 0; k < 3; ++k) {
			vectices.push_back(m_polygonStack[0].polygon[i].point[k]);
		}
	}

	this->collisionMesh = physics->CreateTriangleMesh(vectices.data(), this->m_numFace);
	btCollisionShape* collision = physics->CreateTriangleMeshShape(this->collisionMesh);
	physics->CreateRigidBody(collision, 0.0f, 1, btVector3(0.0f, 0.0f, 0.0f));
	//btCollisionObject* rigid = physics->CreateCollisionObject(collision, false, 1, btVector3(0.0f, 0.0f, 0.0f));
}



////////
//private
////
bool MapPolygon::InitializeFBX(const char* filename) {
	FbxImporter *importer;
	mfbx_manager = FbxManager::Create();
	if (mfbx_manager == nullptr) {
		return false;
	}
	importer = FbxImporter::Create(mfbx_manager, "");
	if (importer == nullptr) {
		return false;
	}
	mfbx_scene = FbxScene::Create(mfbx_manager, "");
	if (mfbx_scene == nullptr) {
		return false;
	}
	if (!importer->Initialize(filename)) {
		return false;
	}
	if (!importer->Import(mfbx_scene)) {
		return false;
	}
	//�ʂ��O�p���A�]�v�Ȗʂ���菜��
	FbxGeometryConverter converter(mfbx_manager);
	converter.Triangulate(mfbx_scene, true);
	converter.RemoveBadPolygonsFromMeshes(mfbx_scene);
	//�C���|�[�^�[�͂�������Ȃ�
	importer->Destroy();
	return true;
}

bool MapPolygon::LoadFBXNodeRecursive(FbxNode *node) {
	FbxNodeAttribute* attr;
	//�m�[�h�������擾
	attr = node->GetNodeAttribute();
	if (attr != NULL) {
		FbxNodeAttribute::EType type;
		type = attr->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh) {
			if (!LoadFBX(node->GetMesh())) {
				return false;
			}
		}
	}
	//�q������Ȃ�q�ōċN
	int numChild = node->GetChildCount();
	for (int i = 0; i < numChild; ++i) {
		FbxNode* child = node->GetChild(i);
		if (!LoadFBXNodeRecursive(child)) {
			return false;
		}
	}
	return true;
}

//FBX���璸�_�𔲂��o���A����p�Ȃ̂ł����܂Œ��_���W�̂�
bool MapPolygon::LoadFBX(FbxMesh *mesh) {
	int numFace = mesh->GetPolygonCount();
	int numVertex = mesh->GetControlPointsCount();

	//�|���S���̃f�[�^
	PolygonData data;
	data.polygon = new PolygonType[numFace];
	data.numPolygon = numFace;

	FbxVector4 *pCoord = mesh->GetControlPoints();

	//UV�x�[�X�̓ǂݍ��݂͑z�肵�Ȃ�
	for (int i = 0; i < numFace; ++i) {
		int  polygonCount = mesh->GetPolygonVertexIndex(i);
		int* vertexIndex = mesh->GetPolygonVertices();

		int polygonIndices[3] = {};
		polygonIndices[0] = vertexIndex[polygonCount];
		polygonIndices[1] = vertexIndex[polygonCount + 1];
		polygonIndices[2] = vertexIndex[polygonCount + 2];

		for (int k = 0; k < 3; ++k) {
			//�u�����_�[�̃��f���͂Ȃ����X�O�x�X���̂ō��W�������܂��ݒ肵�Ă���Ă�A���������
			int index = mesh->GetPolygonVertex(i, k);
			data.polygon[i].point[k][0] = (float)-pCoord[index][0];
			data.polygon[i].point[k][1] = (float)pCoord[index][1];
			data.polygon[i].point[k][2] = (float)pCoord[index][2];
		}
	}
	m_polygonStack.push_back(data);
	m_numFace = numFace;
	return true;
}

