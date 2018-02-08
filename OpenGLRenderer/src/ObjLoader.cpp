#include "ObjLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <array>

void ObjLoader::clearMemory() {
	m_positions.clear();
	m_normals.clear();
	m_textureCoordinates.clear();
	m_finalVerts.clear();
}

void ObjLoader::loadObj(fMeshData& outData, const std::string & filepath) {

	std::ifstream file(filepath);
	if (!file.good()) {
		std::cerr << "[Error] Path " << filepath << " not valid. Please contact a developer.\n";
		return;
	}
	else {

		std::string line;
		std::istringstream ss(line);

		while (getline(file, line)) {

			if (line.find("v ") == 0) {			//Pos found
				std::string segment;
				std::stringstream linestream(line);
				std::vector<std::string> seglist;
				while (std::getline(linestream, segment, ' ')) {
					seglist.push_back(segment);
				}
				seglist.erase(seglist.begin());
				//std::cout << seglist[0] << "," << seglist[1] << "," << seglist[2] << "\n";
				m_positions.push_back({ std::atof(seglist[0].c_str()), std::atof(seglist[1].c_str()), std::atof(seglist[2].c_str()) });
				//std::cout << line << std::endl;
			}
			else if (line.find("vn ") == 0) {	//Normal found
				std::string segment;
				std::stringstream linestream(line);
				std::vector<std::string> seglist;
				while (std::getline(linestream, segment, ' ')) {
					seglist.push_back(segment);
				}
				seglist.erase(seglist.begin());
				//std::cout << seglist[0] << "," << seglist[1] << "," << seglist[2] << "\n";
				m_normals.push_back({ std::atof(seglist[0].c_str()), std::atof(seglist[1].c_str()), std::atof(seglist[2].c_str()) });
				//std::cout << line << std::endl;
			}
			else if (line.find("vt ") == 0) {	//TexChoord found
				std::string segment;
				std::stringstream linestream(line);
				std::vector<std::string> seglist;
				while (std::getline(linestream, segment, ' ')) {
					seglist.push_back(segment);
				}
				seglist.erase(seglist.begin());
				//std::cout << seglist[0] << "," << seglist[1] << "\n";
				m_textureCoordinates.push_back({ std::atof(seglist[0].c_str()), std::atof(seglist[1].c_str()) });
				//std::cout << line << std::endl;
			}
			/*
				Whenever a face is found, the face is split up into its individual vertices and tested for triangulation.
				This is by far not the cleanest/most efficient procedure and will likely be changed in future versions
			*/
			else if (line.find("f ") == 0) {
				std::string segment;
				std::stringstream linestream(line);
				std::vector<std::string> seglist;
				while (std::getline(linestream, segment, ' ')) {
					seglist.push_back(segment);
				}
				seglist.erase(seglist.begin());
				if (seglist.size() != 3) {
					std::cerr << "[Error] Obj file (" << filepath << ") is not triangulated.\n";
					return;
				}
				else {
					for (std::string element : seglist) {
						std::string sgmt;
						std::stringstream stream(element);
						std::vector<std::string> sgmtlist;
						while (std::getline(stream, sgmt, '/')) {
							sgmtlist.push_back(sgmt);
						}
						if (sgmtlist.size() != 3) {
							std::cerr << "[Error] Obj file (" << filepath << ") is not using the \"v/vn/vt\"-Layout.\n";
							return;
						}
						else {
							int x, y, z;
							x = std::atoi(sgmtlist[0].c_str());
							y = std::atoi(sgmtlist[1].c_str());
							z = std::atoi(sgmtlist[2].c_str());
							m_finalVerts.push_back({ x, y, z });
						} 
					}
				}
			} 
			else {	//Nothing supported / Comment found -> skip
				//std::cout << line << std::endl;
			}
			
		}

		/*std::cout << "\nPosition Count: " << m_positions.size();
		std::cout << "\nNormal Count: " << m_normals.size();
		std::cout << "\nTexture Coordinate Count: " << m_textureCoordinates.size();
		std::cout << "\nFinal Vertex Count: " << m_finalVerts.size() << std::endl;*/

		//Create VA and VB
		
		std::vector<float> finalMeshData;
		std::vector<unsigned int> finalIndexBuffer;

		for (unsigned int i = 0; i < m_finalVerts.size(); i++) {
			glm::vec3 vert = m_finalVerts[i];
			//std::cout << i << "\n";

			//Vertex Position
			finalMeshData.push_back(m_positions[vert.x - 1].x);
			finalMeshData.push_back(m_positions[vert.x - 1].y);
			finalMeshData.push_back(m_positions[vert.x - 1].z);

			//std::cout << "Vertex Done" << "\n";

			//Vertex Normal
			finalMeshData.push_back(m_normals[vert.z - 1].x);
			finalMeshData.push_back(m_normals[vert.z - 1].y);
			finalMeshData.push_back(m_normals[vert.z - 1].y);

			//std::cout << "Normal Done" << "\n";
			//Vertex Texture Coordinates
			finalMeshData.push_back(m_textureCoordinates[vert.y - 1].x);
			finalMeshData.push_back(m_textureCoordinates[vert.y - 1].y);

			//std::cout << "TexCoord Done" << "\n";
			
			finalIndexBuffer.push_back(i);
		}

		outData.first = finalMeshData;
		outData.second = finalIndexBuffer;

		/*std::cout << "--------------------\nEND RESULT\n--------------------\n";
		std::cout << "Vertex Array Size: " << finalMeshData.size() << "x float" << std::endl;
		std::cout << "Index Buffer Size: " << finalIndexBuffer.size() << "x unsigned int" << std::endl;

		std::cout << "Works just fine!\n";*/
		clearMemory();
		finalMeshData.clear();
		finalIndexBuffer.clear();

		return;
	}
}

ObjFile::ObjFile(fMeshData data)
	: m_vb(&data.first[0], data.first.size() * sizeof(float))
	, m_ib(&data.second[0], data.second.size())
{
	m_vbl.Push<float>(3);
	m_vbl.Push<float>(3);
	m_vbl.Push<float>(2);
	m_va.AddBuffer(m_vb, m_vbl);
}

ObjFile::ObjFile(const std::string & filepath) {
	fMeshData data;
	ObjLoader ld;
	ld.loadObj(data, filepath);
	m_vb.load(&data.first[0], data.first.size() * sizeof(float));
	m_ib.load(&data.second[0], data.second.size());
	m_vbl.Push<float>(3);
	m_vbl.Push<float>(3);
	m_vbl.Push<float>(2);
	m_va.AddBuffer(m_vb, m_vbl);
}



ObjFile::ObjFile() {

}

ObjFile::~ObjFile() {

}
