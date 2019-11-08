#include "Math/vector4.h"
#include "Math/matrix44.h"
#include <vector>
#include <assert.h>


class Joint
{
public:
	Joint();
	~Joint();

	void SetName(std::string name);
	std::string GetName();

	void SetJointMatrix(Math::matrix44 mat);
	Math::matrix44& GetJointMatrix();

	void SetBaseMatrix(Math::matrix44 mat);
	Math::matrix44& GetBaseMatrix();

	void SetRotation(Math::matrix44 & rot);
	Math::matrix44 & GetRot();

	void SetPosition(Math::matrix44 & pos);
	Math::matrix44 & GetPos();

	void SetScale(Math::matrix44 & scale);
	Math::matrix44 & GetScale();

	void SetParent(Joint* parent);
	Joint* GetParent();

	void SetIndices(int index, int parentIndex);
	int GetParentIndex();

	void SetSelected(bool val);
	bool IsSelected();

	void AppendChildJoint(Joint* childJoint);
	std::vector<Joint*>& GetChildJoints();

	void RotateJoint(Math::matrix44 rotMatrix);
	void UpdateTransform();
	void SendTransformToChildren(Math::matrix44 transMat);
	void TranslateBaseMatrix(Math::matrix44 transMat);
	//void CreateMeshVertexBuffer(std::vector<JointLine>& vertices);
	void TranslateChildren(Math::matrix44 transMat);
	//void TranslateJoint(Math::matrix44& transMat);
	void ResetSelfAndChildren();
	void ComputeBaseMatrix();

	std::string name;

	int index;
	int parentIndex;

	Math::matrix44 position;
	Math::matrix44 rotation;
	Math::matrix44 scale;
	Math::matrix44 superBaseMatrix;
	Math::matrix44 translatedMatrix;
	Math::matrix44 jointMatrix;
	
	bool selected;

	Joint* parentJoint;

	std::vector<Joint*> childJoints;


};

inline void Joint::SetSelected(bool val)
{
	this->selected = val;
}

inline bool Joint::IsSelected()
{
	return this->selected;
}

inline void Joint::SetName(std::string name)
{
	this->name = name;
}

inline std::string Joint::GetName()
{
	return this->name;
}

inline void Joint::SetJointMatrix(Math::matrix44 mat)
{
	this->jointMatrix = mat;
}

inline Math::matrix44& Joint::GetJointMatrix()
{
	return this->jointMatrix;
}

inline void Joint::SetRotation(Math::matrix44 & rot)
{
	this->rotation = rot;
}

inline Math::matrix44 & Joint::GetRot()
{
	return this->rotation;
}

inline void Joint::SetPosition(Math::matrix44 & pos)
{
	this->position = pos;
}

inline Math::matrix44 & Joint::GetPos()
{
	return this->position;
}

inline void Joint::SetScale(Math::matrix44 & scale)
{
	this->scale = scale;
}

inline Math::matrix44 & Joint::GetScale()
{
	return this->scale;
}

inline void Joint::SetParent(Joint* parent)
{
	this->parentJoint = parent;
	parent->AppendChildJoint(this);
}

inline Joint* Joint::GetParent()
{
	return this->parentJoint;
}

inline void Joint::SetIndices(int index, int parentIndex)
{
	this->index = index;
	this->parentIndex = parentIndex;
}

inline int Joint::GetParentIndex()
{
	return this->parentIndex;
}

inline void Joint::AppendChildJoint(Joint* childJoint)
{
	assert(childJoint); // ERROR: child joint invalid
	this->childJoints.push_back(childJoint);
}

inline std::vector<Joint*>& Joint::GetChildJoints()
{
	return this->childJoints;
}

inline Math::matrix44 &Joint::GetBaseMatrix()
{
	return this->translatedMatrix;
}

inline void Joint::SetBaseMatrix(Math::matrix44 mat)
{
	this->translatedMatrix = mat;
}
