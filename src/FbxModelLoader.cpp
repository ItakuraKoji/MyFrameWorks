#include"FbxModelLoader.h"

////////
//public
////
FbxModelLoader::FbxModelLoader() {
	this->fbxData = 0;
	this->bufferData = 0;
	this->materialData = 0;
	this->animationData = 0;
	this->boneData = 0;

	this->loaded = false;
}
FbxModelLoader::~FbxModelLoader() {
	if (this->fbxData) {
		delete this->fbxData;
	}
	if (this->bufferData) {
		delete this->bufferData;
	}
	if (this->materialData) {
		delete this->materialData;
	}
	if (this->animationData) {
		delete this->animationData;
	}
	if (this->boneData) {
		delete this->boneData;
	}

}

bool FbxModelLoader::LoadFBX(const std::string& fileName, TextureList* list) {
	this->fbxData       = new FbxData;
	this->bufferData    = new VertexData;
	this->materialData  = new MaterialData;
	this->animationData = new AnimationData;
	this->boneData      = new BoneData;

	this->textureList = list;

	if (!InitializeFBX(fileName)) {
		return false;
	}

	FbxNode *rootNode = this->fbxData->GetScene()->GetRootNode();
	if (!RecursiveNode(rootNode)) {
		return false;
	}
	this->loaded = true;
	return true;
}

//FBXポインタの譲渡
FbxData* FbxModelLoader::PassFbxData() {
	assert(this->loaded);
	FbxData* returnData = this->fbxData;
	this->fbxData = NULL;
	return returnData;
}
//頂点バッファ情報の譲渡
VertexData* FbxModelLoader::PassVertexBuffer() {
	assert(this->loaded);
	VertexData* returnData = this->bufferData;
	this->bufferData = NULL;
	return returnData;
}
//マテリアル情報の譲渡
MaterialData* FbxModelLoader::PassMaterialData() {
	assert(this->loaded);
	MaterialData* returnData = this->materialData;
	this->materialData = NULL;
	return returnData;
}
//アニメーション情報の譲渡
AnimationData* FbxModelLoader::PassAnimationData() {
	assert(this->loaded);
	AnimationData* returnData = this->animationData;
	this->animationData = NULL;
	return returnData;
}
//ボーン行列情報の譲渡
BoneData* FbxModelLoader::PassBoneData() {
	assert(this->loaded);
	BoneData* returnData = this->boneData;
	this->boneData = NULL;
	return returnData;
}

////////
//private
////
bool FbxModelLoader::RecursiveNode(FbxNode* node) {
	FbxNodeAttribute* attr;
	attr = node->GetNodeAttribute();

	if (attr != NULL) {
		if (attr->GetAttributeType() == FbxNodeAttribute::eMesh) {
			if (!LoadFbxMesh(node->GetMesh())) {
				return false;
			}
		}
	}
	//再起
	int numChild = node->GetChildCount();
	for (int i = 0; i < numChild; ++i) {
		if (!RecursiveNode(node->GetChild(i))) {
			return false;
		}
	}
	return true;
}

bool FbxModelLoader::LoadFbxMesh(FbxMesh* mesh) {
	int numVertex = mesh->GetControlPointsCount();
	int numFace = mesh->GetPolygonCount();
	int numUV = mesh->GetTextureUVCount();
	Vertex* vertex;

	if (numUV > numVertex) {
		vertex = new Vertex[numUV];
	}
	else {
		vertex = new Vertex[numVertex];
	}

	//頂点
	LoadVertex(mesh, vertex);
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//マテリアル
	FbxNode* node = mesh->GetNode();
	int numMaterial = node->GetMaterialCount();
	std::vector<Material> material;
	std::vector<GLuint> IBOs;
	LoadMaterial(mesh, material, IBOs);


	//ボーン
	PolygonTable *table = CreatePolygonTable(mesh, numVertex, numFace);
	this->LoadBones(mesh, vertex, table);

	if (numUV > numVertex) {
		numVertex = numUV;
	}
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(Vertex), vertex, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//textureUV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));
	//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (5 * sizeof(float)));
	//boneWeight
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (8 * sizeof(float)));
	//boneIndex
	glVertexAttribPointer(4, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (12 * sizeof(float)));


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	VertexBuffer buffer;
	buffer.VAO = VAO;
	buffer.VBO = VBO;
	buffer.IBOs = IBOs;
	buffer.numMaterial = numMaterial;
	buffer.numFace = numFace;
	this->bufferData->Add(buffer);

	delete[] vertex;
	delete[] table;
	return true;
}

////////
//private
////

//面ベースでメッシュ読み込み
void FbxModelLoader::LoadVertex(FbxMesh* mesh, Vertex* vertex) {
	int numVertex = mesh->GetControlPointsCount();
	int numFace = mesh->GetPolygonCount();
	int numUV = mesh->GetTextureUVCount();

	for (int i = 0; i < numFace; ++i) {
		//三角ポリゴンの一頂点ずつ格納
		for (int p = 0; p < 3; ++p) {
			int vertexIndex;
			//頂点インデックスを面から得る
			if (numUV > numVertex) {
				vertexIndex = mesh->GetTextureUVIndex(i, p, FbxLayerElement::eTextureDiffuse);
			}
			else {
				int polygonCount = mesh->GetPolygonVertexIndex(i);
				int *polygonVertex = mesh->GetPolygonVertices();
				vertexIndex = polygonVertex[polygonCount + p];
			}

			//頂点
			FbxVector4 *pCoord = mesh->GetControlPoints();
			int index = mesh->GetPolygonVertex(i, p);
			vertex[vertexIndex].position.x() = (float)pCoord[index][0];
			vertex[vertexIndex].position.y() = (float)pCoord[index][1];
			vertex[vertexIndex].position.z() = (float)pCoord[index][2];

			//法線
			FbxVector4 normal;
			mesh->GetPolygonVertexNormal(i, p, normal);
			vertex[vertexIndex].normal.x() = (float)normal[0];
			vertex[vertexIndex].normal.y() = (float)normal[1];
			vertex[vertexIndex].normal.z() = (float)normal[2];
			vertex[vertexIndex].normal.normalize();

			//UV
			if (!numUV) {
				continue;
			}
			FbxLayerElementUV *pUV;
			pUV = mesh->GetLayer(0)->GetUVs();
			if (pUV->GetMappingMode() == FbxLayerElementUV::eByPolygonVertex) {
				int uvIndex = mesh->GetTextureUVIndex(i, p, FbxLayerElement::eTextureDiffuse);
				FbxVector2 v2 = pUV->GetDirectArray().GetAt(uvIndex);
				vertex[vertexIndex].texcoord.x() = (float)v2[0];
				vertex[vertexIndex].texcoord.y() = (float)v2[1];
			}
		}
	}
	//UVタイプがeByControlPointの時
	FbxVector2 v2;
	FbxLayerElementUV *pUV;
	pUV = mesh->GetLayer(0)->GetUVs();
	if (numUV) {
		if (pUV->GetMappingMode() == FbxLayerElement::eByControlPoint) {
			pUV = mesh->GetLayer(0)->GetUVs();
			for (int k = 0; k < numUV; ++k) {
				v2 = pUV->GetDirectArray().GetAt(k);
				vertex[k].texcoord.x() = (float)v2[0];
				vertex[k].texcoord.y() = (float)v2[1];
			}
		}
	}
}

//マテリアル読み込み
void FbxModelLoader::LoadMaterial(FbxMesh* mesh, std::vector<Material>& material, std::vector<GLuint>& IBOs) {
	FbxNode* node = mesh->GetNode();
	int numMaterial = node->GetMaterialCount();
	int numVertex = mesh->GetControlPointsCount();
	int numFace = mesh->GetPolygonCount();
	int numUV = mesh->GetTextureUVCount();

	material.resize(numMaterial);
	IBOs.resize(numMaterial);

	for (int i = 0; i < numMaterial; ++i) {
		FbxSurfaceMaterial *pMaterial = node->GetMaterial(i);
		FbxClassId materialType = pMaterial->GetClassId();

		if (materialType.Is(FbxSurfacePhong::ClassId)) {
			FbxSurfacePhong *pPhong = (FbxSurfacePhong*)pMaterial;

			material[i].ambient(0) = (float)(pPhong->Ambient.Get()[0]);
			material[i].ambient(1) = (float)(pPhong->Ambient.Get()[1]);
			material[i].ambient(2) = (float)(pPhong->Ambient.Get()[2]);
			material[i].ambient(3) = 1.0f;

			material[i].diffuse(0) = (float)(pPhong->Diffuse.Get()[0]);
			material[i].diffuse(1) = (float)(pPhong->Diffuse.Get()[1]);
			material[i].diffuse(2) = (float)(pPhong->Diffuse.Get()[2]);
			material[i].diffuse(3) = 1.0f;

			material[i].specular(0) = (float)(pPhong->Specular.Get()[0]);
			material[i].specular(1) = (float)(pPhong->Specular.Get()[1]);
			material[i].specular(2) = (float)(pPhong->Specular.Get()[2]);
			material[i].specular(3) = 1.0f;

			//鏡面反射強度
			material[i].power = (float)(pPhong->Shininess.Get());

		}
		else if (materialType.Is(FbxSurfaceLambert::ClassId)) {
			FbxSurfaceLambert *pLambert = (FbxSurfaceLambert*)pMaterial;

			material[i].ambient(0) = (float)(pLambert->Ambient.Get()[0]);
			material[i].ambient(1) = (float)(pLambert->Ambient.Get()[1]);
			material[i].ambient(2) = (float)(pLambert->Ambient.Get()[2]);
			material[i].ambient(3) = 1.0f;

			material[i].diffuse(0) = (float)(pLambert->Diffuse.Get()[0]);
			material[i].diffuse(1) = (float)(pLambert->Diffuse.Get()[1]);
			material[i].diffuse(2) = (float)(pLambert->Diffuse.Get()[2]);
			material[i].diffuse(3) = 1.0f;

			material[i].specular(0) = 0.0f;
			material[i].specular(1) = 0.0f;
			material[i].specular(2) = 0.0f;
			material[i].specular(3) = 1.0f;

			//鏡面反射強度
			material[i].power = 1.0f;
		}

		//テクスチャ
		FbxProperty lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse, 0);
		FbxFileTexture *pTexture = FbxCast<FbxFileTexture>(lProperty.GetSrcObject<FbxFileTexture>(0));
		material[i].textureName = "";
		if (!pTexture) {
		}
		else {
			const char *fullName = pTexture->GetRelativeFileName();
			//最終的に使用するファイル名
			char fileName[120] = "";
			//Blenderから読み取った名前
			char name[100] = "";
			//拡張子
			char ext[10] = "";
			//ファイル名を取得(ファイル名と拡張子のみ)
			_splitpath_s(fullName, 0, 0, fileName, 120, name, 100, ext, 10);

			strcat_s(name, ext);
			strcat_s(fileName, name);


			if (strcmp(ext, ".tga") == 0) {
				material[i].textureName = fileName;
				
				if (!this->textureList->LoadTexture(fileName, fileName)) {
					material[i].textureName = "";
				}

			}
		}
		//インデックスバッファ
		int indexCount = 0;
		int *pIndex = new int[numFace * 3];
		for (int k = 0; k < numFace; ++k) {
			FbxLayerElementMaterial *material = mesh->GetLayer(0)->GetMaterials();

			int matId = material->GetIndexArray().GetAt(k);
			if (matId == i) {
				if (numUV > numVertex) {
					pIndex[indexCount] = mesh->GetTextureUVIndex(k, 0, FbxLayerElement::eTextureDiffuse);
					pIndex[indexCount + 1] = mesh->GetTextureUVIndex(k, 1, FbxLayerElement::eTextureDiffuse);
					pIndex[indexCount + 2] = mesh->GetTextureUVIndex(k, 2, FbxLayerElement::eTextureDiffuse);
				}
				else {
					pIndex[indexCount] = mesh->GetPolygonVertex(k, 0);
					pIndex[indexCount + 1] = mesh->GetPolygonVertex(k, 1);
					pIndex[indexCount + 2] = mesh->GetPolygonVertex(k, 2);
				}
				indexCount += 3;
			}
		}
		material[i].numFace = indexCount / 3;

		glGenBuffers(1, &IBOs[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOs[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexCount, pIndex, GL_STATIC_DRAW);
		delete[] pIndex;
	}
	this->materialData->Add(material);
}


//頂点がどのポリゴンに属するかを記録
FbxModelLoader::PolygonTable* FbxModelLoader::CreatePolygonTable(FbxMesh *mesh, int numVertex, int numFace) {
	PolygonTable *pt = new PolygonTable[numVertex];
	for (int vertex = 0; vertex < numVertex; ++vertex) {
		for (int face = 0; face < numFace; ++face) {
			for (int polygon123 = 0; polygon123 < 3; ++polygon123) {
				
				if (mesh->GetPolygonVertex(face, polygon123) != vertex) {
					continue;
				}
				pt[vertex].polygonIndex[pt[vertex].numPolygon] = face;
				pt[vertex].polygon123  [pt[vertex].numPolygon] = polygon123;
				++pt[vertex].numPolygon;
			
			}
		}
	}
	return pt;
}

bool FbxModelLoader::LoadBones(FbxMesh* mesh, Vertex* vertex, PolygonTable *table) {
	FbxDeformer *deformer = mesh->GetDeformer(0);
	if (!deformer) {
		//ボーンが存在しなかったらアニメーション関連のデータに別れを告げる
		delete this->animationData;
		delete this->boneData;
		this->animationData = 0;
		this->boneData = 0;
		return false;
	}
	FbxSkin *skin = static_cast<FbxSkin*>(deformer);

	int numBone = skin->GetClusterCount();
	std::vector<FbxCluster*> cluster;
	cluster.resize(numBone);
	for (int i = 0; i < numBone; ++i) {
		cluster[i] = skin->GetCluster(i);
	}

	int numUV     = mesh->GetTextureUVCount();
	int numVertex = mesh->GetControlPointsCount();

	if (numUV > numVertex) {
		for (int i = 0; i < numBone; ++i) {
			int *indices   = cluster[i]->GetControlPointIndices();
			int numIndices = cluster[i]->GetControlPointIndicesCount();
			double *weight = cluster[i]->GetControlPointWeights();

			for (int k = 0; k < numIndices; ++k) {
				for (int p = 0; p < table[indices[k]].numPolygon; ++p) {
					if (!weight[k]) {
						continue;
					}
					int polygonIndex = table[indices[k]].polygonIndex[p];
					int polygon123   = table[indices[k]].polygon123[p];
					int uvIndex      = mesh->GetTextureUVIndex(polygonIndex, polygon123, FbxLayerElement::eTextureDiffuse);
					for (int m = 0; m < 4; ++m) {
						if (vertex[uvIndex].boneWeight[m] > 0.0f) {
							if (vertex[uvIndex].boneIndex[m] == i) {
								break;
							}
							continue;
						}
						vertex[uvIndex].boneIndex[m] = i;
						vertex[uvIndex].boneWeight[m] = (float)weight[k];
						break;
					}
				}
			}
		}
	}
	else {
		//頂点ベースのモデルの場合
		for (int i = 0; i < numBone; ++i) {
			int *index = cluster[i]->GetControlPointIndices();
			double *weight = cluster[i]->GetControlPointWeights();
			int vertexCount = cluster[i]->GetControlPointIndicesCount();

			for (int k = 0; k < vertexCount; ++k) {
				for (int m = 0; m < 4; ++m) {
					if ((float)weight[k] < vertex[index[k]].boneWeight[m]) {
						continue;
					}
					vertex[index[k]].boneIndex[m] = i;
					vertex[index[k]].boneWeight[m] = (float)weight[k];
					break;
				}
			}
		}
	}

	//バインドポーズを取得
	std::vector<Bone> bone;
	bone.resize(numBone);
	for (int i = 0; i < numBone; ++i) {
		FbxAMatrix mat;
		cluster[i]->GetTransformLinkMatrix(mat);
		for (int x = 0; x < 4; ++x) {
			for (int y = 0; y < 4; ++y) {
				bone[i].bindMat(x + y * 4) = (float)mat.Get(y, x);
				bone[i].cluster = cluster[i];
			}
		}
	}
	this->boneData->Add(bone);

	return true;
}



bool FbxModelLoader::InitializeFBX(const std::string& fileName) {

	//マネージャーを生成しモデルデータをインポート
	FbxManager  *manager;
	FbxImporter *importer;
	FbxScene    *scene;
	manager  = FbxManager::Create();
	if (!manager) {
		return false;
	}

	importer = FbxImporter::Create(manager, "");
	scene    = FbxScene::Create(manager, "");

	this->fbxData->Add(manager, importer, scene);

	if (!importer || !scene) {
		return false;
	}

	//初期化とインポート
	if (!importer->Initialize(fileName.data())) {
		return false;
	}
	if (!importer->Import(scene)) {
		return false;
	}

	//面を三角化、余計な面も取り除く
	FbxGeometryConverter converter(manager);
	converter.Triangulate(scene, true);
	converter.RemoveBadPolygonsFromMeshes(scene);

	//アニメーション情報を取得、名前をキーにして保持
	int numAnim = importer->GetAnimStackCount();
	for (int i = 0; i < numAnim; ++i) {
		//取得
		FbxTakeInfo *take = importer->GetTakeInfo(i);
		AnimType anim;
		auto start = take->mLocalTimeSpan.GetStart();
		auto end = take->mLocalTimeSpan.GetStop();
		anim.animName = take->mName.Buffer();
		anim.animID = i;
		anim.startTime = (int)(start.Get() / FbxTime::GetOneFrameValue(FbxTime::eFrames120));
		anim.endTime = (int)(end.Get() / FbxTime::GetOneFrameValue(FbxTime::eFrames120));

		//追加
		this->animationData->Add(anim);
	}

	return true;
}


