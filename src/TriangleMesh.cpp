#include "TriangleMesh.h"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>
#include <glm/glm.hpp>

using namespace std;

TriangleMesh::TriangleMesh(const char* objFilePath)
: Mesh(GL_TRIANGLES)
{
    loadFromObjFile(objFilePath);
}

TriangleMesh::TriangleMesh(const float* data, unsigned int size)
: Mesh(GL_TRIANGLES, data, size)
{

}

void TriangleMesh::loadFromObjFile(const char* objFilePath)
{
    string comment = "#";

    ifstream infile(objFilePath);
    if (!infile)
    {
        cerr << "Could not open file: " << objFilePath << endl;
    }
    string line;

    vector<glm::vec3> tmpVerts;
    vector<glm::vec3> tmpNorms;

    while (getline(infile, line))
    {
        // Remove comments from a line
        size_t comment_start = line.find_first_of(comment);
        line = line.substr(0, comment_start);
    
        boost::char_separator<char> space(" ");
        boost::tokenizer< boost::char_separator<char> > tokens(line, space);
        
        auto it = tokens.begin();
        float data[] = {0, 0, 0, 0};
        uint dataCount = 0;
        string type;

        if (*it == "v")
        {
            type = *it;
            it++;
            for (; it != tokens.end(); it++)
            {
                data[dataCount] = stof(*it);
                dataCount++;
            }
        }
        else if (*it == "vn")
        {
            type = *it;
            it++;
            for (; it != tokens.end(); it++)
            {
                data[dataCount] = stof(*it);
                dataCount++;
            }
        }

        if (dataCount < 2)
        {
            cerr << "Need at least 2 compenents of data per vertex" << endl;
        }

        if (type == "v")
            tmpVerts.push_back( glm::vec3(data[0], data[1], data[2]) );
        else if (type == "vn")
            tmpNorms.push_back( glm::vec3(data[0], data[1], data[2]) );
    }
    for (auto v : tmpVerts)
    {
        cout << v.x << " " << v.y << " " << v.z << endl;
    }
}
