#include "config.h"
#include "SkeletonParser.h"
#include <vector>
#include <sstream>
#include "Joint.h"
#include "Skeleton.h"
#include <assert.h>
#include "tinyxml2.h"

using namespace tinyxml2;

SkeletonParser::SkeletonParser()
{

}

SkeletonParser::~SkeletonParser()
{

}

Skeleton * SkeletonParser::LoadSkeletonFromFile(const char* filename)
{
	Skeleton* skeleton = nullptr;
	XMLDocument doc;
	XMLError err = doc.LoadFile(filename);
	if(err != XML_SUCCESS)
	{
		return skeleton;
	}

	XMLElement* characterNode = doc.FirstChildElement("Nebula3")->FirstChildElement("Model")->FirstChildElement("CharacterNodes")->FirstChildElement("CharacterNode");
	XMLElement* jointElement = characterNode->FirstChildElement("Joint");
		
	if(!jointElement)
	{
		assert(false);	// ERROR: file doesn't have the proper structure for this parser
	}	
	
	int numJoints = 1;
	skeleton = new Skeleton();
	while (true)
	{
		jointElement = jointElement->NextSiblingElement("Joint");
		if(!jointElement)
		{
			break;
		}
		numJoints++;		
	}
	
	std::vector<Joint*> joints;	
	
	for(int i = 0; i < numJoints; i++)
	{
		joints.push_back(new Joint());
	}
	
	jointElement = characterNode->FirstChildElement("Joint");
	
	for(int i = 0; i < int(joints.size()); i++)
	{
		//handle current joint		
		Joint* joint;
		
		const char* name = jointElement->Attribute("name");
		
		int index = jointElement->IntAttribute("index");
		int parent = jointElement->IntAttribute("parent");
		joint = joints[index];

		const char* pos = jointElement->Attribute("position");
		const char* rot = jointElement->Attribute("rotation");
		const char* scale = jointElement->Attribute("scale");
		
		Math::vector4 posVec = this->StringToVec4(pos);
		Math::vector4 rotVec = this->StringToVec4(rot);
		Math::vector4 scaleVec = this->StringToVec4(scale);

		joint->SetName(name);
		joint->SetIndices(index, parent);
		if (parent >= 0)
		{
			joint->SetParent(joints[parent]);
		}

		Math::matrix44 trans = Math::matrix44::transMat(posVec);
		joint->SetPosition(trans);

		Math::matrix44 rotation = Math::matrix44::quaternionToMatrix(rotVec);
		joint->SetRotation(rotation);

		Math::matrix44 scaleMat = Math::matrix44::scaleMatFromVect(scaleVec);
		joint->SetScale(scaleMat);

		joint->ComputeBaseMatrix();
if (joint->GetName() == "Hip")
{
	int i = 0;
}
		printf("Loaded values \n");

		printf("Name: %s \n",  name);
		printf("pos: x: %f, y: %f z: %f w: %f \n", posVec[0], posVec[1], posVec[2], posVec[3]);
		printf("rot: 1: %f, 2: %f 3: %f 4: %f \n", rotVec[0], rotVec[1], rotVec[2], rotVec[3]);
		printf("scale: 1: %f, 2: %f 3: %f 4: %f \n", scaleVec[0], scaleVec[1], scaleVec[2], scaleVec[3]);
		printf("\n");

		//get next sibling and run again
		jointElement = jointElement->NextSiblingElement("Joint");
	}
	
	skeleton->SetJoints(joints);

	printf("\n Loaded values \n");

	for (int i = 0; i < int(skeleton->GetJoints().size()); i++)
	{
		printf("joint %s has position %f, %f, %f \n",
			skeleton->GetJoints()[i]->GetName().c_str(),
			skeleton->GetJoints()[i]->GetBaseMatrix().getPos()[0],
			skeleton->GetJoints()[i]->GetBaseMatrix().getPos()[1],
			skeleton->GetJoints()[i]->GetBaseMatrix().getPos()[2]);
	}

	Math::matrix44 temp;


	skeleton->GetRootJoint()->TranslateBaseMatrix(temp);

	return skeleton;
}

void SkeletonParser::SplitString(const char* string, char delim, std::vector<float>& elements)
{
	std::stringstream ss;

	ss.str(string);

	std::string item;
	
	while (std::getline(ss, item, delim))
	{
		elements.push_back(float(atof(item.c_str())));
	}
}

Math::vector4 SkeletonParser::StringToVec4(const char* str)
{
	std::vector<float> elements;
	this->SplitString(str, ',', elements);
	return this->ElementsToVec4(elements);
}

Math::vector4 SkeletonParser::ElementsToVec4(std::vector<float>& elements)
{
	Math::vector4 vec;
	for (int i = 0; i < int(elements.size()); i++)
	{
		vec[i] = elements[i];
	}
	return vec;
}