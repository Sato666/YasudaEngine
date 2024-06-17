//-------------------------------------------------------------------------------------
// Class : MoveLight
// 親Class   :   GameObject
// 
// lightのゲームオブジェクトクラス
// 
// Yasuda.Aoto  2023.09.13
//-------------------------------------------------------------------------------------
#pragma once
#include "gameobject.h"

class MoveLight :
    public GameObject
{
public:
    void GameObjInit() override;
};

