#include"bulletDebugDraw.h"

bulletDebugDraw::bulletDebugDraw() {
	btVector3 point[] = { btVector3(0.0f, 10.0f, 0.0f), btVector3(0.0f, -10.0f, 0.0f) };
	unsigned int index[] = { 0, 1 };

	//VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
	//VBO
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(btVector3), point, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer    (0, 3, GL_FLOAT, GL_FALSE, sizeof(btVector3), 0);
	//IBO
	glGenBuffers(1, &this->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(unsigned int), index, GL_STATIC_DRAW);

	glBindVertexArray(0);

}
bulletDebugDraw::~bulletDebugDraw() {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->IBO);
}

void bulletDebugDraw::drawLine(const btVector3& from, const btVector3& to,
	const btVector3& color) {
	btVector3 lineVertex[] = { from, to };

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(btVector3), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(btVector3), lineVertex);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void bulletDebugDraw::drawLine(const btVector3& from, const btVector3& to,
	const btVector3& fromColor, const btVector3& toColor) {

	/* from����to�ցA����`��
	from��to�ŌʂɐF�w�肳��Ă�� */

}

void bulletDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,
	btScalar distance, int lifeTime,
	const btVector3& color) {

	/* PointOnB�ɁA�Փ˓_��`�� */

}


void bulletDebugDraw::reportErrorWarning(const char* warningString) {

	/* �x���\�� */

}

void bulletDebugDraw::draw3dText(const btVector3& location, const char* textString) {

	/* �w���ԍ��W�ɕ�����\�� */

}


void bulletDebugDraw::setDebugMode(int debugMode) {

	/* �f�o�b�O���[�h�w�� */
	this->debug_mode = debugMode;

}

int bulletDebugDraw::getDebugMode() const {

	/* ���݂̃f�o�b�O���[�h��ԋp */
	return this->debug_mode;
}