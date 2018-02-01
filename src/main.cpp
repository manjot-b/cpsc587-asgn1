#include <iostream>
#include <string>
#include "CurveMesh.h"

using namespace std;

int main(int argc, char* argv[])
{
    CurveMesh curve("rsc/roller_coaster.obj");
    for (unsigned int i = 0; i < curve.verticesCount(); i++ )
    {
        cout << curve.getData()[i] << "  ";
        if ((i+1) % 3 == 0)
            cout << endl;
    }
}