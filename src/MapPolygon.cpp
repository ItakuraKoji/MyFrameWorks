#include"MapPolygon.h"

////////
//public
////
MapPolygon::MapPolygon() {
	mfbx_manager = 0;
	mfbx_scene = 0;
}
MapPolygon::~MapPolygon() {
	Finalize();
}

//�z��̃[���N���A
bool MapPolygon::Initialize() {
	m_polygonStack.clear();
	m_polygonStack.shrink_to_fit();
	return true;
}
//���
void MapPolygon::Finalize() {
	auto begin = m_polygonStack.begin();
	auto end = m_polygonStack.end();
	std::vector<PolygonData>::iterator it;
	for (it = begin; it < end; ++it) {
		delete[](*it).polygon;
	}
	if (mfbx_manager) {
		mfbx_manager->Destroy();
	}
}

bool MapPolygon::LoadModel(const char *filename) {
	if(!InitializeFBX(filename)) {
		return false;
	}

	if (!LoadFBXNodeRecursive(mfbx_scene->GetRootNode())) {
		return false;
	}
	return true;
}

//�ۗL����|���S���𔻒肷��
bool MapPolygon::isHitPolygon(int index, Hit::Segment segment, Vector3f &crossPoint) {
	Vector3f p[3];
	//�|���S���f�[�^���璸�_�𔲂��o���i�O�p�|���S���Ȃ̂łR�j
	for (int i = 0; i < 3; ++i) {
		float x = m_polygonStack[0].polygon[index].point[i].x();
		float y = m_polygonStack[0].polygon[index].point[i].y();
		float z = m_polygonStack[0].polygon[index].point[i].z();

		p[i] = Vector3f(x, y, z);
	}

	Hit::Segment seg(segment.firstPoint, Vector3f(0.0f, 1.0f, 0.0f));
	Hit::Capsule cap(seg, 1.0f);
	Vector3f normal = Hit::GetPolygonNormal(p[0], p[1], p[2]);
	bool result = Hit::IsHitPolygonAndCapsule(cap, p[0], p[1], p[2], crossPoint);
	while (Hit::IsHitPolygonAndCapsule(cap, p[0], p[1], p[2], crossPoint)) {
		cap.segment.firstPoint -= normal / 100.0f;
	}
	crossPoint = cap.segment.firstPoint - seg.firstPoint;
	if (index == 28 || index == 31) {
		std::cout << "Distance : " << Hit::GetDistanceSegmentAndPolygon(cap.segment, p[0], p[1], p[2]) << std::endl;
	}

	return result;

	return Hit::HitSegmentAndPolygon(segment, p[0], p[1], p[2], crossPoint);
}

//�|���S�������l��
int MapPolygon::GetNumFace() {
	return m_numFace;
}

//bullet�ɒn�`�̎O�p���b�V�������̂Ƃ��Ēǉ�
void MapPolygon::setCollisionWorld(BulletPhysics *physics) {
	for (int i = 0; i < this->m_numFace; ++i) {
		btScalar point[3 * 3];
		for (int k = 0; k < 3; ++k) {
			point[k * 3 + 0] = m_polygonStack[0].polygon[i].point[k].x();
			point[k * 3 + 1] = m_polygonStack[0].polygon[i].point[k].y();
			point[k * 3 + 2] = m_polygonStack[0].polygon[i].point[k].z();
		}
		
		btCollisionShape* collision = new btConvexHullShape(point, 3, 3 * sizeof(btScalar));
		btScalar mass(0.0f);
		btTransform trans;
		trans = btTransform::getIdentity();
		trans.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
		trans.setOrigin(btVector3(0.0f, 0.0f, 0.0f));

		btDefaultMotionState *mt = new btDefaultMotionState(trans);
		btRigidBody::btRigidBodyConstructionInfo info(mass, mt, collision, btVector3(0.0f, 0.0f, 0.0f));
		btRigidBody *rigid = new btRigidBody(info);
		physics->AddRigidBody(rigid);
	}
}



////////
//private
////
bool MapPolygon::InitializeFBX(const char* filename) {
	FbxImporter *importer;
	mfbx_manager = FbxManager::Create();
	if (!mfbx_manager) {
		return false;
	}
	importer = FbxImporter::Create(mfbx_manager, "");
	if (!importer) {
		return false;
	}
	mfbx_scene = FbxScene::Create(mfbx_manager, "");
	if (!mfbx_scene) {
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
		int polygonCount = mesh->GetPolygonVertexIndex(i);
		int *vertexIndex = mesh->GetPolygonVertices();

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

