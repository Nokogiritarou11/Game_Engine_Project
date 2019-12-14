#include "Resource_Manager.h"
#include <tchar.h>
#include "misc.h"
#include <sstream>
#include <WICTextureLoader.h>
#include <d3dcompiler.h>
#include <memory>
#include <functional>
#include <iostream>
#include <fstream>
#include "DxSystem.h"

HRESULT load_texture_from_file(const wchar_t* TexName, ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* g_TEXTURE2D_DESC)
{
	// UNIT.04
	HRESULT hr = S_OK;
	ComPtr<ID3D11Resource> resource;

	// UNIT.05
	static unordered_map<wstring, ComPtr<ID3D11ShaderResourceView>> cache;
	auto it = cache.find(TexName);
	if (it != cache.end())
	{
		//it->second.Attach(*shader_resource_view);
		*shader_resource_view = it->second.Get();
		(*shader_resource_view)->AddRef();
		(*shader_resource_view)->GetResource(resource.GetAddressOf());
	}
	else
	{
		// UNIT.04
		hr = CreateWICTextureFromFile(DxSystem::Device.Get(), TexName, resource.GetAddressOf(), shader_resource_view);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		// UNIT.05
		cache.insert(make_pair(TexName, *shader_resource_view));
	}

	// UNIT.04
	ComPtr<ID3D11Texture2D> texture2d;
	hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	texture2d->GetDesc(g_TEXTURE2D_DESC);

	return hr;
}

HRESULT load_Model_from_text(const char* file_pass, const char* fbx_filename, skinned_mesh::meshes* meshes)
{
	HRESULT hr = S_OK;
	skinned_mesh::meshes skin_meshes;

	static vector<skinned_mesh::meshes> stock_meshes;
	static unordered_map<string, int> cache;
	string fullpass = (string)file_pass + (string)fbx_filename;
	auto it = cache.find(fullpass);

	if (it != cache.end())
	{
		*meshes = stock_meshes[it->second];
		return S_OK;
	}

	//ファイルから読み込み
	{
		ifstream in_f(fullpass);
		_ASSERT_EXPR_A(in_f.is_open(), "File not found");
		string _s;
		getline(in_f, _s);
		int mesh_size = stoi(_s);
		skin_meshes.resize(mesh_size);
		for (int i = 0;i < mesh_size;i++)
		{
			vector<skinned_mesh::vertex> vertices; // Vertex buffer
			vector<u_int> indices; // Index buffer

			getline(in_f, _s);
			int vertex_size = stoi(_s);
			vertices.resize(vertex_size);
			for (int j = 0;j < vertex_size;j++)
			{
				in_f >> vertices[j].position.x >> vertices[j].position.y >> vertices[j].position.z;
				in_f.ignore();

				in_f >> vertices[j].normal.x >> vertices[j].normal.y >> vertices[j].normal.z;
				in_f.ignore();

				in_f >> vertices[j].texcoord.x >> vertices[j].texcoord.y;
				in_f.ignore();

				for (int k = 0;k < MAX_BONE_INFLUENCES;k++)
				{
					if (k < MAX_BONE_INFLUENCES - 1)
					{
						getline(in_f, _s, ' ');
					}
					else
					{
						getline(in_f, _s);
					}
					vertices[j].bone_weights[k] = stof(_s);
				}

				for (int k = 0;k < MAX_BONE_INFLUENCES;k++)
				{
					if (k < MAX_BONE_INFLUENCES - 1)
					{
						getline(in_f, _s, ' ');
					}
					else
					{
						getline(in_f, _s);
					}
					vertices[j].bone_indices[k] = stoi(_s);
				}
			}

			getline(in_f, _s);
			int index_size = stoi(_s);
			indices.resize(index_size);
			for (int j = 0;j < index_size;j++)
			{
				if (j < index_size - 1)
				{
					getline(in_f, _s, ' ');
				}
				else
				{
					getline(in_f, _s);
				}
				indices[j] = stoi(_s);
			}

			getline(in_f, _s);
			int subset_size = stoi(_s);
			skin_meshes[i].subsets.resize(subset_size);
			for (int j = 0;j < subset_size;j++)
			{
				getline(in_f, _s);
				skin_meshes[i].subsets[j].index_start = stoi(_s);

				getline(in_f, _s);
				skin_meshes[i].subsets[j].index_count = stoi(_s);

				in_f >> skin_meshes[i].subsets[j].diffuse.color.x >> skin_meshes[i].subsets[j].diffuse.color.y >> skin_meshes[i].subsets[j].diffuse.color.z >> skin_meshes[i].subsets[j].diffuse.color.w;
				in_f.ignore();

				//シェーダー読んでるところ
				getline(in_f, _s);

				getline(in_f, _s);
				if (_s != "")
				{
					string Texpass = (string)file_pass + _s;
					setlocale(LC_ALL, "japanese");
					wchar_t Texname[MAX_PATH] = { 0 };
					size_t ret = 0;
					mbstowcs_s(&ret, Texname, MAX_PATH, Texpass.c_str(), _TRUNCATE);
					D3D11_TEXTURE2D_DESC g_TEXTURE2D_DESC = {};
					load_texture_from_file(Texname, &skin_meshes[i].subsets[j].diffuse.shader_resource_view, &g_TEXTURE2D_DESC);
				}
			}

			for (int row = 0; row < 4; row++)
			{
				for (int column = 0; column < 4; column++)
				{
					if (row == 3 && column == 3)
					{
						getline(in_f, _s);
					}
					else
					{
						getline(in_f, _s, ' ');
					}
					skin_meshes[i].global_transform.m[row][column] = stof(_s);
				}
			}

			getline(in_f, _s);
			int skeletal_animation_size = stoi(_s);
			skin_meshes[i].skeletal_animation.resize(skeletal_animation_size);
			for (int j = 0;j < skeletal_animation_size;j++)
			{
				getline(in_f, _s);
				int skeletal_size = stoi(_s);
				skin_meshes[i].skeletal_animation[j].resize(skeletal_size);
				getline(in_f, _s);
				skin_meshes[i].skeletal_animation[j].sampling_time = stof(_s);
				for (int k = 0;k < skeletal_size;k++)
				{
					getline(in_f, _s);
					int bone_size = stoi(_s);
					skin_meshes[i].skeletal_animation[j].at(k).resize(bone_size);
					for (int l = 0; l < bone_size; l++)
					{
						for (int row = 0; row < 4; row++)
						{
							for (int column = 0; column < 4; column++)
							{
								if (row == 3 && column == 3)
								{
									getline(in_f, _s);
								}
								else
								{
									getline(in_f, _s, ' ');
								}
								skin_meshes[i].skeletal_animation[j].at(k).at(l).transform.m[row][column] = stof(_s);;
							}
						}
					}
				}
			}

			// 頂点バッファの生成
			{
				D3D11_BUFFER_DESC bd = {};
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(skinned_mesh::vertex) * vertices.size();
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;
				bd.StructureByteStride = 0;
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = &vertices[0];				// 頂点のアドレス
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;
				hr = DxSystem::Device->CreateBuffer(&bd, &InitData, skin_meshes[i].vertex_buffer.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}

			// インデックスバッファの生成
			{
				D3D11_BUFFER_DESC bd = {};
				//bd.Usage = D3D11_USAGE_DEFAULT;
				bd.Usage = D3D11_USAGE_IMMUTABLE;
				bd.ByteWidth = sizeof(u_int) * indices.size();
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;
				bd.StructureByteStride = 0;
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = &indices[0];				// 頂点のアドレス
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;
				hr = DxSystem::Device->CreateBuffer(&bd, &InitData, skin_meshes[i].index_buffer.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
			}
		}
	}
	*meshes = skin_meshes;
	stock_meshes.push_back(skin_meshes);
	cache.insert(make_pair(fullpass, stock_meshes.size() - 1));
	return hr;
}