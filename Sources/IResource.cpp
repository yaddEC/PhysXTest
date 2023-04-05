#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "IResource.h"
#include "Debug.h"
#include <unordered_map>
#include "ResourceManager.h"
#include "IResource.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <STB_Image/stb_image.h>

using namespace Resources;
//std::unordered_map<std::string, IResource*> ResourceManager::resources;

#pragma region IResource

void IResource::SetResourceId(unsigned int id)
{
	resourceId = id;
}
unsigned int IResource::GetResourceId() const
{
	return resourceId;
}

void IResource::SetResourcePath(const std::string& path)
{
	resourcePath = path;
}

std::string IResource::GetResourcePath() const
{
	return resourcePath;
}

bool IResource::GetIsLoaded()
{
	return isLoaded;
}

bool IResource::GetIsBinded()
{
	return isBinded;
}

#pragma endregion



#pragma region Model

Model::Model()
{
	vertexBuffer = {};
	indexBuffer = {};

	isLoaded = isBinded = false;
}

Model::~Model()
{
	vertexBuffer.clear();
	indexBuffer.clear();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Model::LoadResource()
{
	std::ifstream obj;
	obj.open(resourcePath);
	if (!obj.is_open()) 
		DEBUG_LOG("|| WARNING || Could not open \"%s\" file", resourcePath.c_str());

	std::vector<Vector3> v;
	std::vector<Vector3> vn;
	std::vector<Vector2> vt;
	unsigned int index = 0;

	bool normalsGave = false;

	Vector3 pos;
	Vector3 normal;
	Vector2 texture;
	
	char slash;
	unsigned int indexPos, indexTexture, indexNormal;
	Vertex newVertex;
	bool existsAlready;
	
	std::string line;
	while (std::getline(obj, line)) 
	{
		if (line[0] == 'v')
		{
			if (line[1] == ' ')
			{
				std::stringstream ss;
				ss.str(line.substr(2));
				ss >> pos.x >> pos.y >> pos.z;
				v.push_back(pos);
				
				
			}
			else if (line[1] == 't')
			{
				std::stringstream ss;
				ss.str(line.substr(3));
				ss >> texture.x >> texture.y;
				vt.push_back(texture);
			}
			else
			{
				std::stringstream ss;
				ss.str(line.substr(2));
				ss >> normal.x >> normal.y >> normal.z;
				vn.push_back(normal);
			}
		}
		else if(line[0] == 'f')
		{
			line = line.substr(2);
			std::stringstream ss;
			ss.str(line);

			for (int i = 0; i < 3; i++)
			{
				ss >> indexPos >> slash >> indexTexture >> slash >> indexNormal;
				if (indexNormal)
				{
					newVertex = { v[indexPos - 1], vn[indexNormal - 1], vt[indexTexture - 1] };
				}
				else
				{
					newVertex = { v[indexPos - 1], Vector3(0, 0, 0), vt[indexTexture - 1] };
				}
				

				existsAlready = false;
				for (unsigned int i = 0; i < vertexBuffer.size(); i++)
				{
					if (newVertex.position == vertexBuffer[i].position && newVertex.normal == vertexBuffer[i].normal && newVertex.textureUV == vertexBuffer[i].textureUV)
					{
						indexBuffer.push_back(i);
						existsAlready = true;
						break;
					}
				}
				if (!existsAlready)
				{
					vertexBuffer.push_back(newVertex);
					indexBuffer.push_back(index);
					index++;
				}
			}
		
		}
	}
	obj.close();	
	isLoaded = true;
}

void Resources::Model::CustomLoadResource(std::vector<Vertex> _vertexBuffer)
{
	bool existsAlready;
	int index = 0;
	for (Vertex v : _vertexBuffer)
	{
		existsAlready = false;
		for (unsigned int i = 0; i < vertexBuffer.size(); i++)
		{
			if (v.position == vertexBuffer[i].position && v.normal == vertexBuffer[i].normal && v.textureUV == vertexBuffer[i].textureUV)
			{
				indexBuffer.push_back(i);
				existsAlready = true;
				break;
			}
		}
		if (!existsAlready)
		{
			vertexBuffer.push_back(v);
			indexBuffer.push_back(index);
			index++;
		}
	}
	isLoaded = true;
}

void Model::UnloadResource()
{
}

void Resources::Model::BindResource()
{
	SetBuffersAndVAO();
	isBinded = true;
}

void Model::SetBuffersAndVAO()
{
	glGenBuffers(1, &VBO);

	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	//glNamedBufferData(VBO, sizeof(vertexBuffer), &vertexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexBuffer.size(), &vertexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexBuffer.size(), &indexBuffer[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

unsigned int Model::GetVAO()
{
	if (VAO)
		return VAO;
	return NULL;
}

#pragma endregion



#pragma region Texture

Texture::~Texture()
{
	UnloadResource();
}

void Texture::LoadResource()
{
	int nrChannels;

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(resourcePath.c_str(), &width, &height, &nrChannels, 0);

	isLoaded = true;
}


void Texture::UnloadResource()
{
	glDeleteTextures(1, &resourceId);
}

void Texture::BindResource()
{
	glGenTextures(1, &texKey);
	glBindTexture(GL_TEXTURE_2D, texKey);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glBindTextureUnit(resourceId, texKey);

	isBinded = true;
}

#pragma endregion



#pragma region Shader


void Shader::LoadResource()
{
}


void Shader::UnloadResource()
{
}

void Resources::Shader::BindResource()
{
	if (vertexSource != "" && fragSource != "")
	{
		SetVertexShader();
		SetFragmentShader();
		Link();
		isBinded = true;
	}
}

void Shader::LoadShader(std::string const& filename, Shader::ShaderType type)
{
	std::ifstream shaderFile;
	shaderFile.open(filename);
	if (!shaderFile.is_open())
		DEBUG_LOG("|| WARNING || Could not open \"%s\" file", filename.c_str());

	std::string shaderStr;
	std::string line;
	while (std::getline(shaderFile, line))
	{
		shaderStr += '\n' + line;
	}
	shaderFile.close();

	if(type == Shader::ShaderType::VERTEX)
		vertexSource = shaderStr;
	else
		fragSource = shaderStr;
}

bool Shader::SetVertexShader()
{
	const char* sourceChar = vertexSource.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &sourceChar, NULL);
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		DEBUG_LOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED")
		return false;
	}
	return true;
}

bool Shader::SetFragmentShader()
{
	const char* sourceChar = fragSource.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &sourceChar, NULL);
	glCompileShader(fragmentShader);
	int success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		DEBUG_LOG("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED")
		return false;
	}
	return true;
}

bool Shader::Link()
{
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
		DEBUG_LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED")
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return false;
}

int Shader::GetProgram()
{
	if(shaderProgram)
		return shaderProgram;
	return NULL;
}
void Shader::SetShaders(std::string const& vertexPath, std::string const& fragPath)
{
	if (ResourceManager::GetInstance().mulithreadLoading)
	{
		ThreadPool::AddTask([this, vertexPath]() mutable
			{
				LoadShader(vertexPath, Shader::ShaderType::VERTEX);
			});

		ThreadPool::AddTask([this, fragPath]() mutable
			{
				LoadShader(fragPath, Shader::ShaderType::FRAG);
			});
	}
	else
	{
		LoadShader(vertexPath, Shader::ShaderType::VERTEX);
		LoadShader(fragPath, Shader::ShaderType::FRAG);
	}
	
}
#pragma endregion


#pragma region CubeMap


void Resources::CubeMap::LoadResource()
{
	glGenTextures(1, &texKey);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texKey);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		std::string path = resourcePath + faces[i];
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	isLoaded = true;
	return;
}

void CubeMap::UnloadResource()
{
}

void CubeMap::BindResource()
{
}

void CubeMap::SetFaces(std::vector<std::string> cubeMapFaces)
{
	faces = cubeMapFaces;
}



#pragma endregion