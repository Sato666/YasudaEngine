//-------------------------------------------------------------------------------------
// Class : CAnimationData
// 
// Assimpを使ってアニメーションのデータをとってくる
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
	// このアニメーションのパス名
	std::string m_filename;

	const aiScene* m_AiScene = nullptr;			// assimp scene

	std::unordered_map<std::string, const aiScene*> m_Animation;

public:
	// アニメーションデータ読み込み
	void LoadAnimation(const std::string filename, const std::string name);

	// 指定した名前のアニメーションを取得する
	aiAnimation* GetAnimation(const char* name, int idx);

};