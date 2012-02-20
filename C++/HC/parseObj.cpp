#include <stdio.h> // Needed only for sprintf()
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <GL/glfw.h>

using namespace std;


vector<float> vertexVec;
vector<float> textureVec;
vector<float> normalVec;
vector<int> indiceVec;

vector<int> fVert;
vector<int> fTex;
vector<int> fNorm;

float xf, yf, zf;
int   v,uv,n;


void parseObj(	string filename,	
				vector<GLfloat> &vertices, 
				vector<GLfloat> &texcoords, 
				vector<GLfloat> &normals, 
				vector<GLuint> &indices) 
{
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
					//sub[3] = str.substr(facePosition[3], str.find('f') - facePosition[3]).c_str();

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

					//ss << sub[3];
					//ss >> v >> uv >> n;
					//fVert.push_back(v);
					//fTex.push_back(uv);
					//fNorm.push_back(n);

					
			}


		}//----------------------------
	}
	catch (ifstream::failure e)
	{
		//cout << e.what();
	}

	// MAKE ARRAYS ----------------------------------------//


	
	//vertices	= *new GLfloat[vertexVec.size()];
	//normals		= *new GLfloat[fNorm.size()];
	//texcoords	= *new GLfloat[fTex.size()];
	//indices		= *new GLubyte[fVert.size()];

	//vertices = vertexVec;

	for (int i = 0; i < vertexVec.size(); i++)
	{
		vertices.push_back(vertexVec[i]);

		//vertices.push_back(vertexVec[ (fVert[i]-1)*3 ]);
		//vertices.push_back(vertexVec[ (fVert[i]-1)*3 + 1]);
		//vertices.push_back(vertexVec[ (fVert[i]-1)*3 + 2]);
		//cout << vertices[i] << " ";
	}
	//cout << endl;

	for (int i = 0; i < fTex.size(); i++)
	{
		texcoords.push_back(textureVec[ (fTex[i]-1)*3 ]);
		texcoords.push_back(textureVec[ (fTex[i]-1)*3 + 1]);
		texcoords.push_back(textureVec[ (fTex[i]-1)*3 + 2]);
		//cout << texcoords[i] << " ";
	}
	//cout << endl;
	

	for (int i = 0; i < fNorm.size(); i++)
	{
		normals.push_back(normalVec[ (fNorm[i]-1)*3 ]);
		normals.push_back(normalVec[ (fNorm[i]-1)*3 + 1]);
		normals.push_back(normalVec[ (fNorm[i]-1)*3 + 2]);
		//cout << normals[i] << " ";
	}
	//cout << endl;

	for (int i = 0; i < fVert.size(); i++)
	{
		indices.push_back(fVert[i]-1);
		//cout << (int)indices[i] << " ";
	}

	cout << texcoords.size() << " " << vertices.size() << " " << normals.size() << " " << indices.size() << endl;
	//cout << endl << "---------------------------------" << endl;
	
}


//int main(void)
//{
//	vector<GLfloat> vertices;
//	vector<GLfloat> texcoords;
//	vector<GLfloat> normals;
//	vector<GLubyte> indices;
//
//	string filename = "kub2.obj";
//	parseObj(filename, vertices, texcoords, normals, indices);
//
//	system("pause");
//}