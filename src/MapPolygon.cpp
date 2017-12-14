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

//配列のゼロクリア
bool MapPolygon::Initialize() {
	m_polygonStack.clear();
	m_polygonStack.shrink_to_fit();
	return true;
}
//解放
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


//ポリゴン数を獲得
int MapPolygon::GetNumFace() {
	return m_numFace;
}

//bulletに地形の三角メッシュを剛体として追加
void MapPolygon::setCollisionWorld(BulletPhysics *physics) {
	for (int i = 0; i < this->m_numFace; ++i) {
		btScalar point[3 * 3];
		for (int k = 0; k < 3; ++k) {
			point[k * 3 + 0] = m_polygonStack[0].polygon[i].point[k].x();
			point[k * 3 + 1] = m_polygonStack[0].polygon[i].point[k].y();
			point[k * 3 + 2] = m_polygonStack[0].polygon[i].point[k].z();
		}
		
		btCollisionShape* collision = new btConvexHullShape(point, 3, 3 * sizeof(btScalar));
		physics->CreateCollisionObject(collision, btVector3(0.0f, 0.0f, 0.0f));
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
	//面を三角化、余計な面も取り除く
	FbxGeometryConverter converter(mfbx_manager);
	converter.Triangulate(mfbx_scene, true);
	converter.RemoveBadPolygonsFromMeshes(mfbx_scene);
	//インポーターはもういらない
	importer->Destroy();
	return true;
}

bool MapPolygon::LoadFBXNodeRecursive(FbxNode *node) {
	FbxNodeAttribute* attr;
	//ノード属性を取得
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
	//子がいるなら子で再起
	int numChild = node->GetChildCount();
	for (int i = 0; i < numChild; ++i) {
		FbxNode* child = node->GetChild(i);
		if (!LoadFBXNodeRecursive(child)) {
			return false;
		}
	}
	return true;
}

//FBXから頂点を抜き出す、判定用なのであくまで頂点座標のみ
bool MapPolygon::LoadFBX(FbxMesh *mesh) {
	int numFace = mesh->GetPolygonCount();
	int numVertex = mesh->GetControlPointsCount();

	//ポリゴンのデータ
	PolygonData data;
	data.polygon = new PolygonType[numFace];
	data.numPolygon = numFace;

	FbxVector4 *pCoord = mesh->GetControlPoints();

	//UVベースの読み込みは想定しない
	for (int i = 0; i < numFace; ++i) {
		int polygonCount = mesh->GetPolygonVertexIndex(i);
		int *vertexIndex = mesh->GetPolygonVertices();

		int polygonIndices[3] = {};
		polygonIndices[0] = vertexIndex[polygonCount];
		polygonIndices[1] = vertexIndex[polygonCount + 1];
		polygonIndices[2] = vertexIndex[polygonCount + 2];

		for (int k = 0; k < 3; ++k) {
			//ブレンダーのモデルはなぜか９０度傾くので座標軸をうまく設定してやってる、困ったやつめ
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

