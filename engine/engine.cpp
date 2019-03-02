﻿#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define _USE_MATH_DEFINES

#include "tinyxml2.h"
#include "engine.h"

using namespace std;
using namespace tinyxml2;

void drawPrimitives(vector<Point> points) {
    glBegin(GL_TRIANGLES);
    for (const Point pt : points)  {
        glVertex3f(pt.x, pt.y, pt.z);
    }
    glEnd();
}

int readPointsFile(string filename)
{
	Point p;
	string l, t;
	ifstream file(filename);
	int i;

	if (!file.is_open()) {
		cout << "Unable to open file: " << filename << "." << endl; return -1;
	}
	else
	{
		while (!file.eof())
		{
			getline(file, l);
			stringstream ss(l.c_str());

			if (l.c_str() != NULL)
			{
				i = 0;
				while (getline(ss, t, ','))
				{
					if (i == 0)
						p.x = stof(t);
					else if (i == 1)
						p.y = stof(t);
					else
						p.z = stof(t);
					i++;
				}
				//Points.push_back(pt); (Depende do sitio onde for a funcao)
			}
		}
		//Points.pop_back(); // last line is blank
		file.close();
	}
	return 0;
}

int loadXMLfile(string filename)
{
	

	XMLDocument xmlDoc;
    XMLNode *pRoot;
    XMLElement *pElement, *pListElement;
    XMLError eResult = xmlDoc.LoadFile(filename.c_str());
    
    if (eResult == XML_SUCCESS)
    {
        pRoot = xmlDoc.FirstChild();
        if (pRoot != nullptr)
        {
            pElement = pRoot->FirstChildElement("models");
            
            if (pElement != nullptr)
            {
                pListElement = pElement->FirstChildElement("model");
                
                while (pListElement != nullptr)
                {
                    string file;
                    file = pListElement->Attribute("file");
                    
                    if (!file.empty() && readPointsFile(file) == -1)
                        return -1;
                    
                    pListElement = pListElement->NextSiblingElement("model");
                }
            }
        }
    }
    else
    {
        cout << "Unable to open file: " << filename << "." << endl;
        return -1;
    }
    return 0;
}

void MenuAjuda() {
	cout << "#_____________________________ HELP _____________________________#" << endl;
	cout << "|                                                                |" << endl;
	cout << "| Usage: ./engine {XML FILE}                                     |" << endl;
	cout << "|                 [-h]                                           |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   FILE:                                                        |" << endl;
	cout << "| Specify a path to an XML file in which the information about   |" << endl;
	cout << "| the models you wish to create are specified                    |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   MOVE:                                                        |" << endl;
	cout << "|   w: Move your position forward                                |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   s: Move your position back                                   |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   a: Move your position to the left                            |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   d: Move your position to the right                           |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   ↑ (mouse) : Rotate your view up                              |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   ↓ (mouse) : Rotate your view down                            |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   ← (mouse) : Rotate your view to the left                     |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   → (mouse) : Rotate your view to the right                    |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   FORMAT:                                                      |" << endl;
	cout << "|   v: Change the figure format into points                      |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   l: Change the figure format into lines                       |" << endl;
	cout << "|                                                                |" << endl;
	cout << "|   p: Fill up the figure                                        |" << endl;
	cout << "#________________________________________________________________#" << endl;
}

void specialKey (int key, int a, int b)
{
    (void)a;(void)b;
    switch (key)
    {
        case GLUT_KEY_UP:
            if (beta < (M_PI / 2 - step))
                beta += step;
            break;

        case GLUT_KEY_DOWN:
            if (beta > -(M_PI / 2 - step))
                beta -= step;
            break;

        case GLUT_KEY_LEFT:
            alpha -= step;
            break;

        case GLUT_KEY_RIGHT:
            alpha += step;
            break;

        case GLUT_KEY_F1:
            radius -= step;
            break;

        case GLUT_KEY_F2:
            radius += step;
            break;

        default:
            return;
    }
    glutPostRedisplay();
}

void renderScene(void)
{
    // bg color
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(radius*cos(beta)*sin(alpha), radius*sin(beta), radius*cos(beta)*cos(alpha),
              0.0, 0.0, 0.0,
              0.0f, 1.0f, 0.0f);
    //drawPrimitives(points);

    // End of frame
    glutSwapBuffers();
}

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
	int read;
    // put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);

	if (argc < 2) {
		cout << "Invalid input. Use -h if you need some help." << endl;
		return 0;
	}
	else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help")) {
		MenuAjuda();
		return 0;
	}
	else
		 read = loadXMLfile(argv[1]);

	
	if (read) {
		// put callback registration here
		glutDisplayFunc(renderScene);
		glutReshapeFunc(changeSize);
		glutIdleFunc(renderScene);
		glutSpecialFunc(specialKey);
	}

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main loop
    glutMainLoop();

    return 1;
}
