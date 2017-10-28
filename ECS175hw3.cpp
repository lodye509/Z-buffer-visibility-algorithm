#include <GL/glut.h>
#include <GL/glu.h>
//#include <OpenGL/gl.h> // for Xcode
//#include <OpenGL/glu.h>
//#include <GLUT/glut.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;

float *pixel;
void display();
const int width = 200, length = 200;
float plane[3];//Store A,B,C for z-buffer plane equation
float zBuff[length][width];

void init (void)
{
    glClearColor (1.0, 1.0, 0.0, 1.0);  // Set display-window color to white.
    glMatrixMode (GL_PROJECTION);       // Set projection parameters.
    gluOrtho2D (0.0, 200.0, 0.0, 200.0);
}
void setPixel(int x, int y, float z, float r, float g, float b)
{
	if(z>zBuff[y][x])
	{
		zBuff[y][x] = z;
		pixel[3*y*width+x*3+0] = r;
		pixel[3*y*width+x*3+1] = g;
		pixel[3*y*width+x*3+2] = b;
	}
}
void zPlane(float x[],float y[], float z[])
{
	float x1 = x[0];
	float x2 = x[1];
	float x3 = x[2];
	float y1 = y[0];
	float y2 = y[1];
	float y3 = y[2];
	float z1 = z[0];
	float z2 = z[1];
	float z3 = z[2];
	float temp = x1*(y2*z3-y3*z2)-y1*(x2*z3-x3*z2)+z1*(x2*y3-x3*y2);
	float A=(y3*z2-y2*z3+y1*z3-y3*z1+y2*z1-y1*z2)/temp;
	float B=(x2*z3-x3*z2+x3*z1-x1*z3+x1*z2-x2*z1)/temp;
	float C=(x3*y2-x2*y3+x1*y3-x3*y1+x2*y1-x1*y2)/temp;
	plane[0]=A;
	plane[1]=B;
	plane[2]=C;
}
float getZ(float x, float y)
{
	float z=0;
	z = (-plane[0]*x-plane[1]*y-1)/plane[2];
	//cout<<z<<endl;
	return z;
}

void triangleDraw(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
//void triangleDraw(float x0, float y0, float x1, float y1, float x2, float y2, float r, float g, float b)
{
	float xArray[3];
	xArray[0]=x0;
	xArray[1]=x1;
	xArray[2]=x2;
	float yArray[3];
	yArray[0]=y0;
	yArray[1]=y1;
	yArray[2]=y2;
	float zArray[3];
	zArray[0]=z0;
	zArray[1]=z1;
	zArray[2]=z2;

	//To get the A,B,C for z-buffer plane equation
	zPlane(xArray, yArray, zArray); 

	/////////////////////Sort vertexs//////////////////////////
	float xTemp[3], yTemp[3], zTemp[3];
	for(int i=0;i<3;i++)
	{
		xTemp[i]=xArray[i];
		yTemp[i]=yArray[i];
		zTemp[i]=zArray[i];
	}

	//Find smallest y
	float temp=yArray[0];
	int n=0;
	for(int i=1;i<3;i++)
	{
		if (yArray[i]<temp)
		{
			temp=yArray[i];
			n=i;
		}
	}
	yArray[0] = yTemp[n];
	xArray[0] = xTemp[n];
	zArray[0] = zTemp[n];
	float yMin = temp;
	float xMin = xTemp[n];

	for(int i=0, a=0;i<3;i++)
	{
		if (temp<yTemp[i])
		{
		    a++;
			yArray[a] = yTemp[i];
			xArray[a] = xTemp[i];
			zArray[a] = zTemp[i];
			temp=yTemp[i];
		}
		else if(yTemp[i]>=yMin)
		{
			if(xTemp[i] != xMin)
			{
			    float t1,t2,t3;
			    t1=yArray[a];
			    t2=xArray[a];
				t3=zArray[a];
                yArray[a] = yTemp[i];
                xArray[a] = xTemp[i];
				zArray[a] = zTemp[i];
				yArray[a+1] = t1;
				xArray[a+1] = t2;
				zArray[a+1] = t3;
				a++;
				yTemp[n]=yMin;
			}
		}
	}
	/////////////////////Sort vertexs End//////////////////////////

    float y = yArray[0];
    float xLeft = xArray[0], xRight = xArray[0];
    float m1 = (y1-y0)/(x1-x0);
    float m2 = (y2-y1)/(x2-x1);
    float m3 = (y2-y0)/(x2-x0);
    
    //Draw vertex
    setPixel(x0, y0, z0, r, g, b);
    setPixel(x1, y1, z1, r, g, b);
    setPixel(x2, y2, z2, r, g, b);
    
    if(yArray[0] != yArray[1])
    {
        while(y < yArray[2])
        {
            y++;
            if(y != yArray[1])
            {
                xLeft = xLeft+ 1/m1;
                xRight = xRight + 1/m3;
                
				for(int i=xLeft;i<=xRight;i++)
				{
					setPixel(i, y, getZ(i,y), r, g, b);
				}
            }
            else if(y == yArray[1])
            {
                xLeft = xLeft+ 1/m1;
                xRight = xRight + 1/m3;
                for(int i=xLeft;i<=xRight;i++)
				{
					setPixel(i, y, getZ(i,y), r, g, b);
				}
                m1 = m2;
            }
        }
    }
    else if(yArray[0] == yArray[1])
    {
        xRight = x1;
        for(int i=x0;i<=xRight;i++)
		{
			setPixel(i, y, getZ(i,y), r, g, b);
		}
        while(y < yArray[2])
        {
            y++;
            xLeft = xLeft+ 1/m1;
            xRight = xRight + 1/m3;
            for(int i=xLeft;i<=xRight;i++)
			{
				setPixel(i, y, getZ(i,y), r, g, b);
			}
        }
    }
}

void display (void)
{
	 glClear (GL_COLOR_BUFFER_BIT);  // Clear display window.
	 glLoadIdentity();

     glDrawPixels(width, length, GL_RGB, GL_FLOAT, pixel);
	 glFlush ( );
}
int main(int argc,char** argv)
{
	for(int i=0;i<length;i++)
	{
		for(int j=0;j<width;j++)
		{
			zBuff[i][j] = -INFINITE;
		}
	}
	pixel = new float[3*width*length];
    glutInit (&argc, argv);                         // Initialize GLUT.
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);   // Set display mode.
    glutInitWindowPosition (400, 200);   // Set top-left display-window position.
    glutInitWindowSize (width, length);      // Set display-window width and height.
    glutCreateWindow ("ECS 175 Assigtment 3"); // Create display window.
	cout<<"OK!"<<endl;
	//           x0   y0  z0  x1   y1   z1 x2   y2   z2 r  g    b
	//triangleDraw(30,  10, 3,  10,  70,  3, 50,  90,  6, 0, 0.3, 1); //Blue triangle
	//triangleDraw(45,  30, 5,  30,  120, 5, 195, 130, 4, 1, 0.8, 0); //Yellow triangle
	triangleDraw(100, 70, 3,  80,  120, 3, 120, 140, 6, 1, 0,   1);
    init ( );                            // Execute initialization procedure.
    glutDisplayFunc (display);       // Send graphics to display window.
    glutMainLoop ( );                    // Display everything and wait.
    return 0;
}