#include	<cassert>
#include "AnimationData.h"

void CAnimationData::LoadAnimation(const std::string filename, const std::string name)
{
	// importer解放される際　シーンも解放されるのでｓｔａｔｉｃにしてる
	static Assimp::Importer importer;

	// シーン情報を構築
	m_Animation[name] = importer.ReadFile(
		filename.c_str(),
		aiProcess_ConvertToLeftHanded);
	assert(m_Animation[name]);

	if (m_Animation[name] == nullptr) {
		printf("load error %s ( %s ) \n ", filename.c_str(), importer.GetErrorString());
	}
}

aiAnimation* CAnimationData::GetAnimation(const char* name, int idx)
{
	aiAnimation* animation = m_Animation[name]->mAnimations[idx];

	if (animation == nullptr)
	{
		return nullptr;
	}
	else
	{
		return animation;
	}
}
