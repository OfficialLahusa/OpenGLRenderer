#include "ObjLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

void ObjLoader::clearMemory() {
	m_positions.clear();
	m_normals.clear();
	m_textureCoordinates.clear();
	m_finalVerts.clear();
}

ObjFile ObjLoader::loadObj(const std::string & filepath) {
	ObjFile mesh;

	std::ifstream file(filepath);
	if (!file.good()) {
		std::cerr << "[Error] Path " << filepath << " not valid. Please contact a developer.\n";
		return mesh;
	}
	else {

		std::string line;
		std::istringstream ss(line);

		while (getline(file, line)) {

			if (line.find("v ") == 0) {			//Pos found
				float x, y, z;
				ss >> x >> y >> z;
				m_positions.push_back({ x, y, z });
				//std::cout << line << std::endl;
			}
			else if (line.find("vn ") == 0) {	//Normal found
				float x, y, z;
				ss >> x >> y >> z;
				m_normals.push_back({ x, y, z });
				//std::cout << line << std::endl;
			}
			else if (line.find("vt ") == 0) {	//TexChoord found
				float u, v;
				ss >> u >> v;
				m_textureCoordinates.push_back({ u, v });
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
					return ObjFile();
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
							return ObjFile();
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
			} else {	//Nothing supported / Comment found -> skip
				//std::cout << line << std::endl;
			}
			
		}

		std::cout << "\nPosition Count: " << m_positions.size();
		std::cout << "\nNormal Count: " << m_normals.size();
		std::cout << "\nTexture Coordinate Count: " << m_textureCoordinates.size();
		std::cout << "\nFinal Vertex Count: " << m_finalVerts.size() << std::endl;

		//Create VA and VB

		VertexBufferLayout layout();
		VertexBuffer vb();
		VertexArray va;

		return mesh;
	}
}
