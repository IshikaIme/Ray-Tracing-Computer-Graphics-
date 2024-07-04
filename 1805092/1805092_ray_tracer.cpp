#include <bits/stdc++.h>

#include <windows.h>  


#include "1805092_classes.hpp"
#include "GL/glut.h"
#include <cmath>
#include <vector>


#define PI acos(-1)



int GL_VIEW_ANGLE = 80;
int WindowWidth = 500;
int WindowHeight = 500;	
GLfloat eyex = 0, eyey = -50, eyez = 50;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;
int disNearPlane, disFarPlane, fovY, AspectRatio;



vector <Object *> objects;
vector <PointLight> pointlights;
vector <Spotlight> spotlights;
int ImageWidth = 0;
int NumRecursion = 0;
int checkerBoardWidth=0;


Vector pos(0,-160,60);   // position of the eye
Vector l(0,1,0);     // look/forward direction
Vector r(1,0,0);      // right direction
Vector u(0,0,1);      // up direction

int imageCount=0;

/* Initialize OpenGL Graphics */



// capture(){
// initialize bitmap image and set background color
// planeDistance = (windowHeight/2.0) /
// tan(viewAngle/2.0)
// topleft = eye + l*planeDistance - r*windowWidth/2 +
// u*windowHeight/2
// 6 of 16
// du = windowWidth/imageWidth
// dv = windowHeight/imageHeight
// topleft = topleft + r*(0.5*du) - u*(0.5*dv)
// int nearest;
// double t, minT;
// for i=1:imageWidth
// for j=1:imageHeight
// calculate curPixel using topleft,r,u,i,j,du,dv
// cast ray from eye to (curPixel-eye) direction
// double *color = new double[3]
// for each object, o in objects
// t = o.intersect(ray, dummyColor, 0)
// update t so that it stores min +ve value
// save the nearest object, on
// minT = on->intersect(ray, color, 1)
// update image pixel (i,j)
// save image // The 1st image you capture after running
// the program should be named Output_11.bmp, the 2nd image
// you capture should be named Output_12.bmp and so on.
void capture()
{
	
	bitmap_image image(ImageWidth, ImageWidth);
	for (int i = 0; i < ImageWidth; i++)
	{
		for (int j = 0; j < ImageWidth; j++)
		{
			image.set_pixel(i, j, 0, 0, 0);
		}
	}

	double planeDistance = (WindowHeight / 2.0) / tan(((fovY *PI) / 180.0 )/ 2.0);

	Vector topLeft= pos + (l * planeDistance) - (r * (WindowWidth / 2.0)) + (u * (WindowHeight / 2.0));

	double du = (double)WindowWidth / ImageWidth;
	double dv = (double)WindowHeight / ImageWidth;


	topLeft = topLeft + (r * (0.5 * du)) - (u * (0.5 * dv));

	int nearest;
	double t, minT;
	

	for (int i = 0; i < ImageWidth; i++)
	{
		for (int j = 0; j< ImageWidth; j++)
		{
			nearest = -1;
			minT = INFINITY;
			Vector currentPixel = topLeft + (r * (i * du)) - (u * (j * dv));

			Ray ray(pos, (currentPixel - pos));
			
			Color color;

			for (int i = 0; i < objects.size(); i++)
			{
				t = objects[i]->intersect(ray, color, 0);
				if (t > 0 && t < minT)
				{
					minT = t;
					nearest = i;
				}
			}

			if (nearest != -1)
			{
				minT = objects[nearest]->intersect(ray, color, 1);
			}
			color.normalize();
			image.set_pixel(i, j, color.r * 255, color.g * 255, color.b * 255);
		}
	}
		
	imageCount++;
	string s = "Output"+to_string(imageCount) +".bmp";
	image.save_image(s);
	image.clear();
	cout<<"Image" <<imageCount<<" Captured"<<endl;
}







/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,1);
    glEnd();
}



void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
 
}

/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    r.x= l.y * u.z - l.z * u.y;
    r.y =l.z * u.x - l.x * u.z;
    r.z =l.x * u.y - l.y * u.x;
    gluLookAt(pos.x,pos.y,pos.z,
              pos.x+l.x,pos.y+l.y,pos.z+l.z,
              u.x,u.y,u.z);
    // draw
    drawAxes();
    for (Object *each : objects)
	{
		each->draw();
	}

	for (PointLight each : pointlights)
	{
		each.draw();
	}


	for (Spotlight each : spotlights)
	{
		each.draw();
	}


    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix

    // gluPerspective(80.0f, 1.0, 1.0f, 1000.0f);
    gluPerspective(fovY, AspectRatio, disNearPlane, disFarPlane);
}




void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 3*(3.14/180);
	switch(key){

		case '1':
			r.x = r.x*cos(rate)+l.x*sin(rate);
			r.y = r.y*cos(rate)+l.y*sin(rate);
			r.z = r.z*cos(rate)+l.z*sin(rate);

			l.x = l.x*cos(rate)-r.x*sin(rate);
			l.y = l.y*cos(rate)-r.y*sin(rate);
			l.z = l.z*cos(rate)-r.z*sin(rate);

			r.normalize();
			l.normalize();
			u.normalize();
			break;

        case '2':
			r.x = r.x*cos(-rate)+l.x*sin(-rate);
			r.y = r.y*cos(-rate)+l.y*sin(-rate);
			r.z = r.z*cos(-rate)+l.z*sin(-rate);

			l.x = l.x*cos(-rate)-r.x*sin(-rate);
			l.y = l.y*cos(-rate)-r.y*sin(-rate);
			l.z = l.z*cos(-rate)-r.z*sin(-rate);

			r.normalize();
			l.normalize();
			u.normalize();
			break;

        case '3':
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-l.x*sin(rate);
			u.y = u.y*cos(rate)-l.y*sin(rate);
			u.z = u.z*cos(rate)-l.z*sin(rate);

			r.normalize();
			l.normalize();
			u.normalize();
			break;

        case '4':
			l.x = l.x*cos(-rate)+u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-l.x*sin(-rate);
			u.y = u.y*cos(-rate)-l.y*sin(-rate);
			u.z = u.z*cos(-rate)-l.z*sin(-rate);

			r.normalize();
			l.normalize();
			u.normalize();
			break;

        case '5':
			u.x = u.x*cos(rate)+r.x*sin(rate);
			u.y = u.y*cos(rate)+r.y*sin(rate);
			u.z = u.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-u.x*sin(rate);
			r.y = r.y*cos(rate)-u.y*sin(rate);
			r.z = r.z*cos(rate)-u.z*sin(rate);

			r.normalize();
			l.normalize();
			u.normalize();
			break;

        case '6':
			u.x = u.x*cos(-rate)+r.x*sin(-rate);
			u.y = u.y*cos(-rate)+r.y*sin(-rate);
			u.z = u.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-u.x*sin(-rate);
			r.y = r.y*cos(-rate)-u.y*sin(-rate);
			r.z = r.z*cos(-rate)-u.z*sin(-rate);

			r.normalize();
			l.normalize();
			u.normalize();
			break;

		case '0':
			r.normalize();
			l.normalize();
			u.normalize();
			capture();
			break;

	
		default:
			break;
	}
	glutPostRedisplay();
}


void specialKeyListener(int key, int x,int y)
{
	switch(key){
		case GLUT_KEY_UP:		
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:		
			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;

		case GLUT_KEY_RIGHT:
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_LEFT :
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
	glutPostRedisplay();
}

void TakeInputLoad()
{
	//ofstream out;
	//out.open("output.txt");

	
    
    Object *object;
	string typeOfObject;

	ifstream in;
	in.open("test.txt");

	in>>disNearPlane>>disFarPlane>>fovY>>AspectRatio;   
   // out<<disNearPlane<<' '<< disFarPlane<<' '<< fovY<<' '<< AspectRatio<<endl;

	in>> NumRecursion;
	//out<< NumRecursion<<endl;

	in>> ImageWidth;
	//out<< ImageWidth<<endl;	

	int checkerBoardWidth;
	//out<< checkerBoardWidth<<endl;

	in>> checkerBoardWidth;
	double checkerAmbient, checkerDiffuse,  checkerReflection;
	in>> checkerAmbient>> checkerDiffuse>>  checkerReflection;
	//out<< checkerAmbient<<' '<< checkerDiffuse<< ' '<<  checkerReflection<<endl;	

	Object* fl = new Floor(800, checkerBoardWidth);
    fl->setcoEfficients(checkerAmbient, checkerDiffuse,0, checkerReflection);
	fl->setShine(0);
	objects.push_back(fl);
	//out<<" floor created "<<endl;
	int numobjects;
	in >> numobjects;
	cout<<"num of object ="<< numobjects<<endl;	
    
    for ( int i=0; i<numobjects; i++)
    {
        in>> typeOfObject;

        if(typeOfObject== "sphere")
        {
            Vector center;
            in>> center;
            double radius;
            in>> radius;
            object = new Sphere(center, radius);
			Color c;
            in>> c;
			object->setColor(c);

			double ambient, diffuse, specular, reflection;
            in>> ambient>> diffuse>> specular>> reflection;
			object->setcoEfficients(ambient, diffuse, specular, reflection);
			int shine;
            in>>shine;
			object->setShine(shine);

            objects.push_back(object);
            cout<< " sphere created "<<endl;
        }

		else if(typeOfObject== "cube")
        {
            Vector bottomLowerLeft;
            in>> bottomLowerLeft;
            double side;
            in>> side;
            object = new Cube(bottomLowerLeft, side);
			Color c;
            in>> c;
			object->setColor(c);

			double ambient, diffuse, specular, reflection;
            in>> ambient>> diffuse>> specular>> reflection;
			object->setcoEfficients(ambient, diffuse, specular, reflection);
			int shine;
            in>>shine;
			object->setShine(shine);

            ///objects.push_back(object);

			//lower base
			Vector A = bottomLowerLeft;
			Vector B = bottomLowerLeft + Vector(side, 0, 0);
			Vector C = bottomLowerLeft + Vector(0,0,side);
			Vector D = C + Vector(side, 0, 0);

			//upper base
			Vector G = bottomLowerLeft + Vector(0, side, 0);
			Vector E = D + Vector(0, side, 0);
			Vector F = B + Vector(0, side, 0);
			Vector H = C + Vector(0, side, 0);

			//triangles
			Triangle *t1 = new Triangle(A, B, C);
			Triangle *t2 = new Triangle(B, C, D);
			Triangle *t3 = new Triangle(C, D, H);
			Triangle *t4 = new Triangle(D, H, E);
			Triangle *t5 = new Triangle(A, B, G);
			Triangle *t6 = new Triangle(B, G, F);
			Triangle *t7 = new Triangle(G, F, E);
			Triangle *t8 = new Triangle(G, E, H);
			Triangle *t9 = new Triangle(A, C, G);
			Triangle *t10 = new Triangle(C, G, H);
			Triangle *t11 = new Triangle(B, D, F);
			Triangle *t12 = new Triangle(D, F, E);


			t1->setColor(c);
			t2->setColor(c);
			t3->setColor(c);
			t4->setColor(c);
			t5->setColor(c);
			t6->setColor(c);
			t7->setColor(c);
			t8->setColor(c);
			t9->setColor(c);
			t10->setColor(c);
			t11->setColor(c);
			t12->setColor(c);


			objects.push_back(t1);
			objects.push_back(t2);
			objects.push_back(t3);
			objects.push_back(t4);
			objects.push_back(t5);
			objects.push_back(t6);
			objects.push_back(t7);
			objects.push_back(t8);
			objects.push_back(t9);
			objects.push_back(t10);
			objects.push_back(t11);
			objects.push_back(t12);



            cout<< " Cube created "<<endl;
        }

		else if(typeOfObject== "pyramid")
        {
            Vector lowest;
            in>> lowest;
            double width, height;
            in>> width>> height;
            object = new Pyramid(lowest, width, height);
			Color clr;
            in>> clr;
			object->setColor(clr);

			double ambient, diffuse, specular, reflection;
            in>> ambient>> diffuse>> specular>> reflection;
			object->setcoEfficients(ambient, diffuse, specular, reflection);
			int shine;
            in>>shine;
			object->setShine(shine);

           // objects.push_back(object);
          

			Triangle *t1 = new Triangle(lowest, lowest + Vector(width, 0, 0), lowest + Vector(width, width, 0));
			Triangle *t2 = new Triangle(lowest, lowest + Vector( 0, width , 0), lowest + Vector(width, width,  0));
			Triangle *t3 = new Triangle(lowest, lowest + Vector(width/2, width/2, height), lowest + Vector(width, 0, 0));
			Triangle *t4 = new Triangle(lowest + Vector(width, 0, 0), lowest + Vector(width, width, 0), lowest + Vector(width/2, width/2, height));
			Triangle *t5 = new Triangle(lowest + Vector(width, width, 0), lowest + Vector(0, width, 0), lowest + Vector(width/2, width/2, height));
			Triangle *t6 = new Triangle(lowest , lowest + Vector(0, width, 0), lowest + Vector(width/2, width/2, 1));
			
			t1->setColor(clr);
			t2->setColor(clr);	
			t3->setColor(clr);
			t4->setColor(clr);
			t5->setColor(clr);
			t6->setColor(clr);

			objects.push_back(t1);
			objects.push_back(t2);
			objects.push_back(t3);
			objects.push_back(t4);
			objects.push_back(t5);
			objects.push_back(t6);

			cout<< " Pyramid created "<<endl;

        }
        else
		{
			cout << " Object Type '" << typeOfObject << "' is not implemented " << endl;
			break;
		}
    }
		int normalLightNo;
		in>> normalLightNo;
		for(int light=0; light<normalLightNo; light++)
		{
			Vector lightPos;
			in>> lightPos;
			double falloff;
			in>> falloff;
			PointLight l(lightPos, falloff);
			pointlights.push_back(l);

			cout<<"PointLight created"<<endl;

		}
		
		int SpotLightNo;
		in>> SpotLightNo;
		for(int light=0; light<SpotLightNo; light++)
		{
			Vector lightPos;
			in>> lightPos;
			double falloff;
			in>> falloff;
			Vector dir;
			in>>dir;
			double cutoff;
			in>>cutoff;
			Spotlight l(lightPos,  falloff,dir, cutoff);
			spotlights.push_back(l);

			cout<<"SpotLight created"<<endl;

		}



}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    TakeInputLoad();
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(WindowWidth, WindowHeight);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("Ray Tracing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop

    for (int i=0; i < objects.size(); i++) {
		delete objects[i];
	}

	vector<Object*>().swap(objects);
	vector<PointLight>().swap(pointlights);
	vector<Spotlight>().swap(spotlights);

	//cout<<"Hiiiiiiiiiii" <<endl;
    return 0;
}
