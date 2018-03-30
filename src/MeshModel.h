#pragma once
#include"ModelDataFactory.h"
#include"FbxModelLoader.h"
#include"ModelData.h"
#include"CameraClass.h"
#include"ShaderClass.h"

namespace K_Graphics {

	//���f���N���X�B�t�@�N�g���[���琶�Y���ꂽ���f���f�[�^���󂯎���ď�����
	//�u�`��v�Ƃ��������Ȃ̂ŁA�`�掞�̓��f���f�[�^�ȊO�̕K�v�ȏ����O����n�����̂Ƃ���
	class MeshModel {
	public:
		MeshModel(ModelDatas* data);
		~MeshModel();

		bool Initialize(ModelDatas* data);
		void Finalize();
		void SetAnimation(const std::string& animationName, bool playOnce, bool isLoop, bool isInterporation, int interpolationFrames);
		void SetTexture(Texture* texture, int arrayIndex, int materialIndex);
		void SetSpeed(float speed);

		void UpdateAnimation();
		void Draw(ShaderClass* shader);
		void InstanceDraw(int numInstance, ShaderClass* shader);

	private:
		void SetBone(int arrayIndex, ShaderClass* shader);
		void DrawBuffers(int arrayIndex, ShaderClass* shader);

	private:
		ModelDatas * data;
		Texture*  boneTexture;
		bool isBoneProcessed;

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	//3D���f���̕`�������܂Ƃ߂��N���X
	class MeshObject {
	public:
		MeshObject(MeshModel* model);
		~MeshObject();

		void SetBoneAnimation(const std::string& animationName, bool playOnce, bool isLoop, bool isInterporation = true, int interpolationFrames = 5);
		void SetSpeed(float speed);

		void UpdateAnimation();

		void Draw(CameraClass* camera, ShaderClass* shader, K_Math::Vector3& position, K_Math::Vector3& rotation, K_Math::Vector3& scale);
		void InstanceDraw(CameraClass* camera, ShaderClass* shader, int numDraw, K_Math::Vector3& position, K_Math::Vector3& rotation, K_Math::Vector3& scale);

	protected:
		void SetMatrix(CameraClass* camera, ShaderClass* shader, K_Math::Vector3& position, K_Math::Vector3& rotation, K_Math::Vector3& scale);
		MeshModel* drawModel;
	};

	//2D�X�v���C�g�`�������܂Ƃ߂��N���X
	//1�ӂ̒���1.0f�̔|���S�����X�v���C�g���ۂ��g�k�AUV���W���w�肷�邱�Ƃ��ł���

	class SpriteObject {
	public:
		SpriteObject(Texture* texture, float controlPointX = 0.0f, float controlPointY = 0.0f);
		~SpriteObject();

		bool SetTexture(Texture* texture);
		void Draw2D(CameraClass* camera, ShaderClass* shader, K_Math::Box2D& src, K_Math::Box2D& draw, float rotation);
		void Draw3D(CameraClass* camera, ShaderClass* shader, K_Math::Box2D& src, K_Math::Vector3& position, K_Math::Vector3& rotation, K_Math::Vector3& scale);

	public:
		K_Math::Vector2 controlPoint;
	protected:
		MeshModel * drawModel;
		void SetMatrix(CameraClass* camera, ShaderClass* shader, K_Math::Vector3& position, K_Math::Vector3& rotation, K_Math::Vector3& scale, bool billBoard);
		Texture* cullentTexture;
	};

}
