#include "CurveMesh.h"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>

using namespace std;

CurveMesh::CurveMesh(const char *objFilePath)
: Mesh(GL_LINE_LOOP)
{
    loadFromObjFile(objFilePath);
}

CurveMesh::CurveMesh(const float* data, unsigned int size) 
: Mesh(GL_LINE_LOOP, data, size)
{

}

void CurveMesh::loadFromObjFile(const char *objFilePath)
{
    string comment = "#";

    ifstream infile(objFilePath);
    if (!infile)
    {
        cerr << "Could not open file: " << objFilePath << endl;
    }
    string line;

    while (getline(infile, line))
    {
        // Remove comments from a line
        size_t comment_start = line.find_first_of(comment);
        line = line.substr(0, comment_start);

        if (line[0] == 'v') // found a vertex. Now read the 3 float on this line
        {
            boost::char_separator<char> space(" ");
            boost::tokenizer< boost::char_separator<char> > tokens(line, space);
            for (const auto& it : tokens)
            {
                if (it != "v")
                {
                    vertices_.push_back( stof(it) );
                }
            }
        }
    }
}
