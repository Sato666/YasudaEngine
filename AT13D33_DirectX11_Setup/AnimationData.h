//-------------------------------------------------------------------------------------
// Class : CAnimationData
// 
// Assimp���g���ăA�j���[�V�����̃f�[�^���Ƃ��Ă���
// 
// Yasuda.Aoto  2023.01.24
//-------------------------------------------------------------------------------------
#pragma once
#include	<vector>
#include	<string>
#include	<unordered_map>
#include	<WICTextureLoader.h>

#include	<assimp\Importer.hpp>
#include	<assimp\scene.h>
#include	<assimp\postprocess.h>
#include	<assimp/cimport.h>


class CAnimationData {
	// ���̃A�j���[�V�����̃p�X��
	std::string m_filename;

	const aiScene* m_AiScene = nullptr;			// assimp scene

	std::unordered_map<std::string, const aiScene*> m_Animation;

public:
	// �A�j���[�V�����f�[�^�ǂݍ���
	void LoadAnimation(const std::string filename, const std::string name);

	// �w�肵�����O�̃A�j���[�V�������擾����
	aiAnimation* GetAnimation(const char* name, int idx);

};