#include "CurveMesh.h"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>
#include <glm/glm.hpp>

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

void CurveMesh::smooth(uint iterations)
{
    vector<float> midVertices;
    vector<float> newVertices;

    for (uint i = 0; i < iterations; i++)
    {
        for (uint i = 0; i < vertices_.size(); i += 3)
        {
            glm::vec3 current(vertices_[i], vertices_[i+1], vertices_[i+2]);
            uint nextIndex = (i + 3) % vertices_.size();     // wrap back around for last point
            glm::vec3 next(vertices_[nextIndex], vertices_[nextIndex + 1], vertices_[nextIndex + 2]);

            glm::vec3 average = (current + next) / 2.0f;
            
            midVertices.push_back(current.x);
            midVertices.push_back(current.y);
            midVertices.push_back(current.z);

            midVertices.push_back(average.x);
            midVertices.push_back(average.y);
            midVertices.push_back(average.z);
        }

        for (uint i = 0; i < midVertices.size(); i += 3)
        {
            glm::vec3 current(midVertices[i], midVertices[i+1], midVertices[i+2]);
            uint nextIndex = (i + 3) % midVertices.size();     // wrap back around for last point
            glm::vec3 next(midVertices[nextIndex], midVertices[nextIndex + 1], midVertices[nextIndex + 2]);

            glm::vec3 average = (current + next) / 2.0f;

            newVertices.push_back(average.x);
            newVertices.push_back(average.y);
            newVertices.push_back(average.z);
        }

        vertices_ = newVertices;
        midVertices.clear();
        newVertices.clear();
    }

    length_ = calcArcLength(); 
    arcLengthParameterization(length_);
}

float CurveMesh::calcArcLength()
{
    float length = 0;

    for (uint i = 0; i < vertices_.size(); i += 3)
    {
        glm::vec3 current(vertices_[i], vertices_[i+1], vertices_[i+2]);
        uint nextIndex = (i + 3) % vertices_.size();     // wrap back around for last point
        glm::vec3 next(vertices_[nextIndex], vertices_[nextIndex + 1], vertices_[nextIndex + 2]);
    
        length += glm::distance(current, next);
    }
    return length;
}

void CurveMesh::arcLengthParameterization(float length)
{
    uint divisions = 10000;
    deltaS_ = length / divisions;

    uint j = 0;
    uValues_.push_back( glm::vec3(vertices_[j], vertices_[j+1], vertices_[j+2]) ); // push back first point

    
    
    float accumDist = 0;
    while (j < vertices_.size())
    {
        glm::vec3 current(vertices_[j], vertices_[j+1], vertices_[j+2]);
        uint nextIndex = (j + 3) % vertices_.size();     // wrap back around for last point
        glm::vec3 next(vertices_[nextIndex], vertices_[nextIndex + 1], vertices_[nextIndex + 2]);
            
        float dist = glm::distance(next, current);
        // cout << dist << endl;
        if ( accumDist + dist < deltaS_)
        {
            accumDist += dist;
        }
        else 
        {
            uValues_.push_back( current );
            accumDist = 0;
        }

        j += 3;     // skip over x,y,z to next point
    }
}

const vector<glm::vec3>& CurveMesh::getUValues() const { return uValues_; }
float CurveMesh::getLength() const { return length_; }
float CurveMesh::getDeltaS() const { return deltaS_; }
