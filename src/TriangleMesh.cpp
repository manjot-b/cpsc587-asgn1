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
    boost::char_separator<char> space(" ");
    boost::char_separator<char> slash("/");

    ifstream infile(objFilePath);
    if (!infile)
    {
        cerr << "Could not open file: " << objFilePath << endl;
    }
    string line;

    vector<glm::vec3> tmpVerts;
    vector<glm::vec3> tmpNorms;
    vector<glm::vec3> indices;

    while (getline(infile, line))
    {
        // Remove comments from a line
        size_t comment_start = line.find_first_of(comment);
        line = line.substr(0, comment_start);
        if (line.empty())
            continue;

        boost::tokenizer< boost::char_separator<char> > tokens(line, space);
        
        auto it = tokens.begin();
        float data[] = {0, 0, 0, 0};
        uint dataCount = 0;
        string type = *it;

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
        else if (*it == "f")
        {
            type = *it;
            it++;
            for (; it != tokens.end(); it++)
            {
                boost::tokenizer< boost::char_separator<char> > index(*it, slash);
                dataCount = 0;
                auto indIt = index.begin();

                for (; indIt != index.end(); indIt++)
                {
                    if (*indIt == "")   // index not specified
                    {
                        data[dataCount] = 0xffffffff;   // will be greater than size of array
                        dataCount++;
                    }
                    else 
                    {
                        data[dataCount] = stoi(*indIt) - 1;
                        dataCount++;
                    }
                }

                if (data[0] < tmpVerts.size())
                {
                    vertices_.push_back(tmpVerts[data[0]].x);
                    vertices_.push_back(tmpVerts[data[0]].y);
                    vertices_.push_back(tmpVerts[data[0]].z);
                }
                // NOTE: when taking care of tex coord, it will have index 1 and norms will have 2
                if (data[1] < tmpNorms.size())
                {
                    normals_.push_back(tmpNorms[data[1]].x);
                    normals_.push_back(tmpNorms[data[1]].y);
                    normals_.push_back(tmpNorms[data[1]].z);
                }


            }
        }
        else 
        {
            continue;
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
    
}
