#pragma once

#include<FBX\fbxsdk.h>
#include<Eigen\Core>
#include<bullet\btBulletDynamicsCommon.h>

#include<vector>
#include<iostream>

#include"BulletPhysics.h"

//����p���f���̃|���S���������N���X�A�`��͈�؂ł��Ȃ��B�s��ɂ���]�ɂ��Ή����Ă��Ȃ�
class MapPolygon {
public:
	//struct PolygonType{
	//	Eigen::Vector3f point[3];
	//};
	//struct PolygonData {
	//	int numPolygon;
	//	PolygonType* polygon;
	//};

	//bullet�p
	struct PolygonType {
		btVector3 point[3];
	};
	struct PolygonData {
		int numPolygon;
		PolygonType* polygon;
	};


public:
	MapPolygon();
	~MapPolygon();
	bool Initialize();
	bool LoadModel(const char *filename);
	void Finalize();
	int GetNumFace();

	void setCollisionWorld(BulletPhysics *physics);


private:
	bool InitializeFBX(const char* filename);
	bool LoadFBXNodeRecursive(FbxNode *node);
	bool LoadFBX(FbxMesh *mesh);


private:
	//����p�̒��_�z��
	std::vector<PolygonData> m_polygonStack;

	int m_numFace;
	FbxManager *mfbx_manager;
	FbxScene *mfbx_scene;
};
