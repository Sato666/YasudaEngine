//-------------------------------------------------------------------------------------
// Class : Model
// 親Class   :   GameObject
// 
// Modelのゲームオブジェクトクラス
// 
// Yasuda.Aoto  2023.12.09
//-------------------------------------------------------------------------------------
#pragma once
#include "gameobject.h"

class Model :
    public GameObject
{
private:

public:
    void GameObjInit() override;
};