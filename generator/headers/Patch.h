#ifndef __PATCH_H__
#define __PATCH_H__

#include <vector>
#include <map>
#include "Point.h"
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

 class Patch{
                int nPatchs;
                int nPoints;
                int tessellation;
                vector<Point> controlPoints;
                map<int,vector<int> > patchs;

        public:
                Patch();
                Patch(vector<Point> p);
                void multMatrixVector(float *m, float *v, float *res);
                float getTangent(float tu, float tv, float m[4][4] , float p[4][4], int deriv);
                void getPatchPoints(int patch, vector<Point>* points, vector<Point>* textureList, vector<Point>* normalList);
                Point* getPoint(float ta, float tb, float coordenadasX[4][4], float coordenadasY[4][4], float coordenadasZ[4][4]);
                void parserPatchFile(string filename);
                Patch(int tess, string filename);
                void geradorModeloBezier(vector<Point> *vert, vector<Point> *normal, vector<float> *text);


};

#endif
