#pragma once
#include <stdio.h>
#include "Shader.h"

class Material
{
public:
	Material();
	~Material();

	DirectX::XMFLOAT4 color = { 1,1,1,1 };
private:

};