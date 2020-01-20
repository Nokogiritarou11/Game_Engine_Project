#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <wrl.h>
#include <memory>
#include <vector>
#include <stdio.h>
#include <locale.h>
#include "Component.h"
#include "Texture.h"
#include "Material.h"

class Renderer : public Component
{
public:
	Renderer();
	~Renderer();

	bool enabled;
	Material material;

private:

};