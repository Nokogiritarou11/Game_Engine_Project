#pragma once

#include <d3d11.h>
#include "skinned_mesh.h"
#include <unordered_map>
#include <vector>
#include <stdio.h>
#include <locale.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>
using namespace std;
using namespace DirectX;

HRESULT load_texture_from_file(const wchar_t* TexName, ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* g_TEXTURE2D_DESC);
HRESULT load_Model_from_text(const char* file_pass, const char* fbx_filename, skinned_mesh::meshes* meshes);