#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <GL/glfw.h>
#include "Object.h"
#include "vec3f.h"

using namespace std;

Object::Object(){

}

Object::Object(string filename)
{
	vector<float> vertexVec, 
				  textureVec,
				  normalVec;
	vector<int> indiceVec;

	vector<int> fVert,
				fTex,
				fNorm;

	float xf, yf, zf;
	int   v,uv,n;


	ifstream inFile;
	inFile.open(filename.c_str());

	inFile.exceptions ( ifstream::failbit | ifstream::badbit | ifstream::goodbit | ifstream::eofbit );
	try
	{
		if (!inFile)
		{
			cout << "File not loaded";
			exit(1);
		}

		while(inFile.good())
		{//----------------------------
			
			char line[256];
			inFile.getline(line,256);

			string str = string(line);
			int counter = 0;
			float position[3] = {0,0,0};
			float facePosition[4] = {0,0,0,0};


			// VERTEX LIST ----------------------------------------//
			if((str.find_first_of("v") == 0) &&(str.find_first_of("n") != 1) && (str.find_first_of("t") != 1))
			{

					for(int i=0; i < str.length(); i++) //loop through line, store ' ' adress to position[i].
					{
							if(str[i] == ' ')
							{
								i++; //remove first space, store only valid data.
								position[counter] = i;
								counter++;
							}
					}
	
					if (position[0] != NULL || position[1] != NULL || position[2] != NULL) 
					{//simple check - if no data, don't store.

						xf = atof(str.substr(position[0], position[1]-position[0]).c_str()); 
						yf = atof(str.substr(position[1], position[2]-position[1]).c_str());
						zf = atof(str.substr(position[2], str.find('v')-position[2]).c_str());
						//calculate lenght of each X,Y,Z - find/skip last character "v" 
						// - store as xf, yf, zf.
						
						vertexVec.push_back(xf);
						vertexVec.push_back(yf);
						vertexVec.push_back(zf);

					}

			}

			// NORMAL LIST ----------------------------------------//
			if((str.find_first_of("v") == 0) &&(str.find_first_of("n") == 1) && (str.find_first_of("t") != 1))
			{

					for(int i=0; i < str.length(); i++) //loop through line, store ' ' adress to position[i].
					{
							if(str[i] == ' ')
							{
								i++; //remove first space, store only valid data.
								position[counter] = i;
								counter++;
							}
					}
	
					if (position[0] != NULL || position[1] != NULL || position[2] != NULL) 
					{//simple check - if no data, don't store.

						xf = atof(str.substr(position[0], position[1]-position[0]).c_str()); 
						yf = atof(str.substr(position[1], position[2]-position[1]).c_str());
						zf = atof(str.substr(position[2], str.find('vn')-position[2]).c_str());
						//calculate lenght of each X,Y,Z - find/skip last character "v" 
						// - store as xf, yf, zf.

						normalVec.push_back(xf);
						normalVec.push_back(yf);
						normalVec.push_back(zf);

					}

			}

			// TEXCOORD LIST ----------------------------------------//
			if((str.find_first_of("v") == 0) &&(str.find_first_of("n") != 1) && (str.find_first_of("t") == 1))
			{

					for(int i=0; i < str.length(); i++) //loop through line, store ' ' adress to position[i].
					{
							if(str[i] == ' ')
							{
								i++; //remove first space, store only valid data.
								position[counter] = i;
								counter++;
							}
					}
	
					if (position[0] != NULL || position[1] != NULL || position[2] != NULL) 
					{//simple check - if no data, don't store.

						xf = atof(str.substr(position[0], position[1]-position[0]).c_str()); 
						yf = atof(str.substr(position[1], position[2]-position[1]).c_str());
						zf = atof(str.substr(position[2], str.find('vt')-position[2]).c_str());
						//calculate lenght of each X,Y,Z - find/skip last character "v" 
						// - store as xf, yf, zf.
						
						textureVec.push_back(xf);
						textureVec.push_back(yf);
						textureVec.push_back(zf);

					}

			}

			// FACES LIST ----------------------------------------//
			if (str.find_first_of("f") == 0)
			{

					for(int i=0; i < str.length(); i++)
					{
							if(str[i] == ' ')
							{
								i++;
								if (counter < 3)
									facePosition[counter] = i;

								counter++;							
							}
					}
					replace(str.begin(), str.end(), '/', ' ');

					string sub[4];
					sub[0] = str.substr(facePosition[0], facePosition[1]-facePosition[0]).c_str();
					sub[1] = str.substr(facePosition[1], facePosition[2]-facePosition[1]).c_str();
					sub[2] = str.substr(facePosition[2], str.find('f') - facePosition[2]).c_str();

					stringstream ss (stringstream::in | stringstream::out);

					ss << sub[0]; 
					ss >> v >> uv >> n;
					fVert.push_back(v);
					fTex.push_back(uv);
					fNorm.push_back(n);

					ss << sub[1];
					ss >> v >> uv >> n;
					fVert.push_back(v);
					fTex.push_back(uv);
					fNorm.push_back(n);

					ss << sub[2];
					ss >> v >> uv >> n;
					fVert.push_back(v);
					fTex.push_back(uv);
					fNorm.push_back(n);
					
			}


		}//----------------------------
	}
	catch (ifstream::failure e)
	{
		//cout << e.what();
	}

	// MAKE ARRAYS ----------------------------------------//


	//for (int i = 0; i < vertexVec.size(); i++)
	//{
	//	vertices.push_back(vertexVec[i]);
	//}

	for (int i = 0; i < fVert.size(); i++)
	{
		indices.push_back(fVert[i]-1);
	}

	for (int i = 0; i < fVert.size(); i++)
	{
		vertices.push_back(vertexVec[ (fVert[i]-1)*3 ]);
		vertices.push_back(vertexVec[ (fVert[i]-1)*3 + 1]);
		vertices.push_back(vertexVec[ (fVert[i]-1)*3 + 2]);
	}


	for (int i = 0; i < fTex.size(); i++)
	{
		texcoords.push_back(textureVec[ (fTex[i]-1)*3 ]);
		texcoords.push_back(textureVec[ (fTex[i]-1)*3 + 1]);
		texcoords.push_back(textureVec[ (fTex[i]-1)*3 + 2]);
	}

	

	for (int i = 0; i < fNorm.size(); i++)
	{
		normals.push_back(normalVec[ (fNorm[i]-1)*3 ]);
		normals.push_back(normalVec[ (fNorm[i]-1)*3 + 1]);
		normals.push_back(normalVec[ (fNorm[i]-1)*3 + 2]);
	}





	
}

void Object::createBoundingBox()
{
	min.setX(10000);
	min.setY(10000);
	min.setZ(10000);

	max.setX(-10000);
	max.setY(-10000);
	max.setZ(-10000);

	for (int i = 0; i < vertices.size(); i+=3)
	{
		if (min.getX() > vertices[i])	min.setX(vertices[i]);
		if (min.getY() > vertices[i+1])	min.setY(vertices[i+1]);
		if (min.getZ() > vertices[i+2])	min.setZ(vertices[i+2]);

		if (max.getX() < vertices[i])	max.setX(vertices[i]);
		if (max.getY() < vertices[i+1])	max.setY(vertices[i+1]);
		if (max.getZ() < vertices[i+2])	max.setZ(vertices[i+2]);
	}
}


void Object::DrawObject()
{
	// Draw the scene
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glNormalPointer(GL_FLOAT, 0, normals.data());
	//glTexCoordPointer(3, GL_FLOAT, 0, texcoords.data());
	//glVertexPointer(3, GL_FLOAT, 0, vertices.data());

	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);

	//cout << texcoords.size() << " " << normals.size() << " " << vertices.size() << " " << indices.size() << endl;

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < indices.size(); i++)
	{ 
		glTexCoord3f(texcoords[i*3], texcoords[i*3 + 1], texcoords[i*3 + 2]);
		glNormal3f(normals[i*3], normals[i*3 + 1], normals[i*3 + 2]);
		glVertex3f(vertices[i*3], vertices[i*3 +  1], vertices[i*3 + 2]);
	}
	glEnd();

}