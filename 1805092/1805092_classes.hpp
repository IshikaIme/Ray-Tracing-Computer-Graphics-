#ifndef _1805092_CLASSES_HPP
#define _1805092_CLASSES_HPP
#include<bits/stdc++.h>
#include "bitmap_image.hpp"
#include "GL/glut.h"
#define PI acos(-1)
using namespace std;

struct point;
class Color;
class Ray;
class Vector;
class Object;
class Sphere;
class Triangle;
class PointLight;
class Spotlight;

extern int NumRecursion;

extern vector<Object *> objects;
extern vector<PointLight> pointlights;
extern vector<Spotlight> spotlights;

#endif // MY_HEADER_HPP

struct point {
    public:
        double x, y, z;
};


class Color
{
    public:
        double r,g,b;
        Color()
        {
            r= 0.0;
            g= 0.0;
            b= 0.0;
        }

        Color(double rx, double gx, double bx) {r=rx; g=gx; b=bx; }
		
		Color operator*(const double value)
		{
			return Color(r * value, g * value, b * value);
		}

		Color operator*(const Color color)
		{
			return Color(r * color.r, g * color.g, b * color.b);
		}
		Color operator+(const Color color)
		{
			return Color(r + color.r, g + color.g, b + color.b);
		}

        friend istream& operator>>(istream &input, Color &c)
        {
            input >> c.r >> c.g >> c.b;
            return input;
        };


        friend  ostream& operator<<(ostream& output, const Color& p)
        {
            output << p.r << ' ' << p.g << ' ' << p.b ;
            return output;
        }
		
		void normalize()
		{
			this->r = min(1.0, this->r);
			this->g = min(1.0, this->g);
			this->b = min(1.0, this->b);

			this->r = max(0.0, this->r);
			this->g = max(0.0, this->g);
			this->b = max(0.0, this->b);
		}


 
};





class Vector
{
    public:
        double x,y,z,n;
        
        Vector()
        {
            x = y = z = 0.0;
            n = 1.0;
        }

        Vector(double a, double b, double c)
        {
            x=a;
            y=b;
            z=c;
            n= 1.0;
        }

        Vector(double a, double b, double c, double n)
        {
            x= a;
            y= b;
            z= c;
            n= n;
        }


        Vector(const Vector &p)
        {
            x= p.x;
            y= p.y;
            z= p.z;
            n= p.n;

        }

        Vector normalize()
        {
            double length = sqrt(x*x+y*y+z*z);
            x= x/length;
            y= y/length;
            z= z/length;
            Vector p(x,y,z);
            return p;

        }



        Vector operator+ (Vector b)
        {

            double sumx, sumy, sumz;
            sumx= x + b.x;
            sumy = y + b.y;
            sumz= z + b.z;
            Vector sum( sumx, sumy, sumz);
            return sum;
        }


        Vector operator- (Vector b)
        {

            double rx, ry, rz;
            rx= x - b.x;
            ry = y - b.y;
            rz= z - b.z;
            Vector r( rx, ry, rz);
            return r;
        }

        double operator* (Vector a)
        {

            double rx, ry, rz;
            rx= x *  a.x;
            ry = y *  a.y;
            rz= z*  a.z;
            
            return rx + ry + rz;
        }


        Vector operator* (double a)
        {

            double rx, ry, rz;
            rx= x * a;
            ry = y * a;
            rz= z * a;
            Vector r( rx, ry, rz);
            return r;
        }


        Vector  cross(Vector b)  {
            Vector temp(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
            return temp;
        }


        string print(Vector a)  {
            string str="";
            str=to_string(a.x);;
            return str;

        }



        friend  ostream& operator<<(ostream& output, const Vector& p)
        {
            output << p.x << ' ' << p.y << ' ' << p.z ;
            return output;
        }


        friend istream& operator>>(istream &input, Vector &p)
        {
            input >> p.x >> p.y >> p.z;
            return input;
        };


};

class Light
{
	public:
	Vector position;
	Color color;
	double fallParam;

	Light(Vector pos, double falloff)
	{
		position = pos;
		color = Color(1, 1, 1);
		fallParam = falloff;	
	}
	Light(){}

	virtual void draw()	{}


};

class PointLight : public Light
{
	public:
	PointLight() {}
	PointLight(Vector pos, double f)
	{
		position = pos;
		fallParam = f;
		color = Color(1, 1, 1);
		
	}
	

	void draw() override
	{
		glColor3d(1,1,1);
		glPushMatrix();
		{
			glTranslatef(position.x, position.y, position.z);
			glutSolidSphere(1.0, 50, 50);
		}
		glPopMatrix();
	}


};


class Spotlight : public Light
{
	public:
	Vector direction;
	double cutoffangle, fall;

	Spotlight(Vector pos, double falloff, Vector dir, double ang)
	{
		position = pos;
		direction = dir;
		fall= falloff;
		cutoffangle = ang;
		color = Color(1, 1, 1);
	}
	Spotlight() {}

	void draw() override
	{
		glColor3d(1,1,1);
		glPushMatrix();
		{
			glTranslatef(position.x, position.y, position.z);
			glutSolidSphere(1.0, 40, 40);
		}
		glPopMatrix();

		
		glBegin(GL_LINES);
		{
			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++)
					for (int k = -1; k <= 1; k++)
					{
						glTranslatef(position.x, position.y, position.z);
						glVertex3f(position.x + i * 2.0, position.y + j * 2.0, position.z + k * 2.0);
					}
		}
		glEnd();
	}
};



class Ray
{
    public:
        Vector start;
        Vector dir; 

    Ray(Vector start_, Vector dir_){
        start = start_;
        dir = dir_;
        dir.normalize();    
    }
    
    Ray (const Ray &r){
        start = r.start;
        dir = r.dir;
        dir.normalize();    
    }
	Ray(){}
};



class Object
{
public:
	double length;
	double coEfficient[4];
	Color clr;
	int shine;
	Vector referencePoint;
	

	Object(Vector referencePoint_, double length_, Color clr_, int shine_)
	{
		referencePoint = referencePoint_;
		length = length_;
		clr = clr_;
		shine = shine_;
	}

	Object(Object &object)
	{
		referencePoint = object.referencePoint;
		length = object.length;
		clr = object.clr;
		shine = object.shine;
	}
	
//need to override these functions in child classes
	Object() {}
	virtual void draw() {}

	void setColor(Color color)
	{
		clr = color;
	}

	void setcoEfficients(double ambient, double diffuse, double specular, double reflection)
	{
		coEfficient[0] = ambient;
		coEfficient[1] = diffuse;
		coEfficient[2] = specular;
		coEfficient[3] = reflection;
	}

	void setShine(int sh)
	{
		shine = sh;
	}

	

	virtual double intersect(Ray &r, Color &color, int level)
	{
		double minT = getMinimumT(r);

		if (level == 0) return minT;

		Vector intersectionPoint = r.start + r.dir * minT;

		
		double ambient = coEfficient[0];
		 color = getColorAtAPos(intersectionPoint) * ambient;
		// color.normalize();

        color = getColorAtAPos(intersectionPoint) ;
		color.normalize();
		Vector normal = getNormalAt(intersectionPoint);
		normal.normalize();

		if ((normal * r.dir)< 0) 
		{
			normal = normal * (-1);
		}


		//pointlight

		//cout<<pointlights.size()<<endl;

		for (int i=0; i<pointlights.size(); i++)
		{
			PointLight templight= pointlights[i];	
				
			Vector lightDir = templight.position - intersectionPoint;
			lightDir.normalize();

			Vector lightPosition = intersectionPoint + lightDir * 0.0001;

			Ray RayLight(lightPosition, lightDir);

			double t, minTInitial = INFINITY;
			Color tempColor;
		
			for (Object *tempObject : objects)
			{
				t = tempObject->intersect(RayLight, tempColor, 0);
				if (t > 0 && t < minTInitial)
				{
					minTInitial = t;
				}
			}

			//scaling_factor = exp(-distance*distance*S.falloff); lambert += toSource.dot(N)*scaling_factor;

			if ( minT < minTInitial)
			{
				double scaling_factor = exp(- minTInitial * minTInitial * templight.fallParam);
				double lambertValue = max( lightDir * normal , 0.0)  *scaling_factor;

				
				Vector Raay = normal * 2.0 * (normal * lightDir) - lightDir;
				Raay.normalize();
				double phongValue = max(pow(  r.dir* Raay  , shine ), 0.0) * scaling_factor; 

			//phong += pow( R’.dot(toSource), shininess)*scaling_factor
				
				double diffused= coEfficient[2];
				double specular = coEfficient[1];

				color = color +  getColorAtAPos(intersectionPoint) *templight.color * diffused*  lambertValue ;
				color.normalize();

				color = color + templight.color * specular *  phongValue;
				color.normalize();
			}
		}

		//spotlight

		for (int i=0; i<spotlights.size(); i++)
		{
			Spotlight templight = spotlights[i];	
			Vector lightDir = templight.direction;
			lightDir.normalize();

			Vector intersectinglight = intersectionPoint - templight.position;
			intersectinglight.normalize();

			double angle = ((double) (acos(lightDir * intersectinglight))*180)/PI;
				

			if (angle > templight.cutoffangle) continue;

			lightDir = templight.position - intersectionPoint;
			lightDir.normalize();

			Vector lightPosition = intersectionPoint + lightDir * 0.0001;
			Ray Raylight(lightPosition, lightDir);

			Color tempColor;
			double  minTInitial = INFINITY;
			double t = INFINITY;

			for (int index=0; index<objects.size(); index++)
			{
				Object * tempObject = objects[index];
				t = tempObject->intersect(Raylight, tempColor, 0);
				if (t > 0 && t < minTInitial)
				{
					minTInitial = t;
				}
			}

			if ( minT < minTInitial)
			{
			
				double scaling_factor = exp(- minTInitial * minTInitial * templight.fallParam);
				double lambertValue = max( lightDir * normal , 0.0)  *scaling_factor;

				
				Vector Raay = normal * 2.0 * (normal * lightDir) - lightDir;
				Raay.normalize();
				double phongValue = max(pow(  r.dir* Raay  , shine ), 0.0) * scaling_factor; 


				Color intersectingColor = getColorAtAPos(intersectionPoint);

				double diffuse= coEfficient[2];
				double specular = coEfficient[1];
				color = color + intersectingColor * templight.color * lambertValue * diffuse;
				color.normalize();

				color = color + templight.color * phongValue * specular;
				color.normalize();
			}
		}


//recursive reflection

		if (level >= NumRecursion) return minT;
	

		Vector reflectedRayDirection = r.dir - normal * 2.0 * (normal * r.dir);
		reflectedRayDirection.normalize();

		
		Ray reflectedRay((intersectionPoint + reflectedRayDirection * 0.0001), reflectedRayDirection);


		Color reflectedColor;
		int nearest = -1;
		double tRef, tRefInitial = INFINITY;

		for (int i = 0; i < objects.size(); i++)
		{
			tRef = objects[i]->intersect(reflectedRay, reflectedColor, 0);
			if (tRef > 0 && tRef< tRefInitial)
			{
				tRefInitial = tRef;
				nearest = i;
			}
		}

		double reflection = coEfficient[3];

		if (nearest != -1)
		{
			tRefInitial = objects[nearest]->intersect(reflectedRay, reflectedColor, level + 1);
			color = color + reflectedColor * reflection;
			color.normalize();
		}
		return minT;
	}
	virtual double getMinimumT(Ray &ray) {return -1.0;}
	

	virtual Color getColorAtAPos(Vector &intersectionPoint){	return clr;}
	
	virtual Vector getNormalAt(Vector &intersectionPoint)
	{
		Vector v;
		return v;
	}




};






class Floor : public Object
{
public:
	int tileNumbers;
	double floorWidth,tileWidth;
	
	

	//constructors
	Floor(double floorW, double tileW)
	{
		
		floorWidth = floorW;
		tileWidth = tileW;
		tileNumbers= floorW / tileW;
		referencePoint = Vector(-floorW / 2, -floorW / 2, 0);
		length = tileWidth;
	}
	Floor() {}

	
	//draw function
	void draw() override
	{
		
		for (int i = 0; i < tileNumbers; i++)
		{
			for (int j = 0; j < tileNumbers; j++)
			{
				if ((i + j) % 2 == 0)
				{
					glColor3d(0, 0, 0);
				}
				else
				{
					glColor3d(255, 255, 255);
				}

				glBegin(GL_QUADS);
				{
					glVertex3f(referencePoint.x + j * tileWidth,referencePoint.y + tileWidth * i, referencePoint.z);
					glVertex3f(referencePoint.x + (j + 1) * tileWidth,referencePoint.y + tileWidth * i, referencePoint.z);
					glVertex3f(referencePoint.x + (j + 1) * tileWidth,referencePoint.y + tileWidth * (i + 1), referencePoint.z);
					glVertex3f(referencePoint.x + j * tileWidth, referencePoint.y + tileWidth * (i + 1), referencePoint.z);
				}
				glEnd();
			}
		}
	}

	Color getColorAtAPos(Vector &intersectionPoint) override
	{
        int tilePosX = (int) ( (intersectionPoint.x - referencePoint.x) / tileWidth );
        int tilePosY = (int) ( (intersectionPoint.y - referencePoint.y) / tileWidth );


		if ((tilePosX + tilePosY) % 2 == 0)
		{
			return Color(0, 0, 0);
		}
		else
		{
			return Color(1, 1, 1);
		}
		
	}

	double getMinimumT(Ray &r)
	{
		

		double t = (-1.0) * ((r.start.z /  r.dir.z));
		Vector intersectionPoint = r.start + r.dir * t;

		if (intersectionPoint.x < referencePoint.x || intersectionPoint.x > -referencePoint.x|| intersectionPoint.y < referencePoint.y || intersectionPoint.y > -referencePoint.y)
		{
			return -1;
		}


		return t;
	}

	Vector getNormalAt(Vector &intersectionPoint) override
	{
		return Vector(0, 0, 1);
	}

	
};

class Sphere : public Object{
    public:
    Sphere(Vector center, double radius){
            referencePoint = center;
            length = radius;
    }
    void draw(){
    // write codes for drawing sphere
        glColor3f(clr.r, clr.g, clr.b);
        glPushMatrix();
        {
            glTranslatef(referencePoint.x, referencePoint.y, referencePoint.z);
            glutSolidSphere(length, 100, 100);
            // drawSphere(length, 100, 100);
        }   
        glPopMatrix();
    }

 double getMinimumT(Ray &r)
	{
		Vector origin = r.start - referencePoint;

		double a = 1;
		double b = (r.dir * origin) * 2.0;
		double c = (origin * origin) - (length * length) ;

		double determinant = b * b - 4 * a * c;
		if (determinant < 0)
		{
			return -1;
		}

		double d = sqrt(determinant);
		double t1 = (-b - d) / (2 * a);
		double t2 = (-b + d) / (2 * a);

		if (t1 < 0 && t2 < 0) return -1;

		if (t1 > 0) return t1;
		
		if (t2 > 0) return t2;

		return -1;
	}

	Vector getNormalAt(Vector &intersectionPoint) override
	{
		Vector normal = intersectionPoint - referencePoint;
		normal.normalize();
		return normal;
	}
};







class Triangle : public Object
{
public:
	Vector a, b, c;

	Triangle() {}
	Triangle(Vector _a, Vector _b, Vector _c)
	{
		this->a = _a;
		this->b = _b;
		this->c = _c;
	}

	void draw()
	{
		glColor3d(clr.r, clr.g, clr.b);
		glBegin(GL_TRIANGLES);
		{
			glVertex3d(a.x, a.y, a.z);
			glVertex3d(b.x, b.y, b.z);
			glVertex3d(c.x, c.y, c.z);
		}
		glEnd();
	}
	Vector getNormalAt(Vector &intPoint) override
	{
		Vector normal = (b - a).cross(c - a);
		normal.normalize();
		return normal;
	}

	Color getColorAtAPos(Vector &intPoint) override
	{
		return clr;
	}

	double getMinimumT(Ray &ray)
	{
		Vector AC = c - a;
		Vector AB = b - a;
		

		Vector vertex = (ray.dir).cross(AC);
		double A, F,U, V, temp;
		A = AB * vertex;

		
		if ((A > -0.0000001) && (A < 0.0000001))  return -1;
		

		F = 1.0 / A;
		Vector end = ray.start - a;

		U = F * (end * vertex);
		if (U < 0.0 || U > 1.0)  return -1;
	

		Vector endingVec = end.cross(AB);
		V = F * (ray.dir * endingVec);
		if (V < 0.0 || (U + V) > 1.0) return -1;
		
		temp = F * (AC * endingVec);
		if (temp > 0.0000001) return temp;
	

		return -1;
	}

	
};


class Cube : public Object
{
	public:
		Vector Opposite, Midpoint;
	Cube() {}

	Cube(Vector bottomlower, double side)
	{
		length = side;
		Opposite.x= bottomlower.x + length;
		Opposite.y= bottomlower.y + length;
		Opposite.z= bottomlower.z + length;

		Midpoint.x= (bottomlower.x + Opposite.x) / 2;
		Midpoint.y= (bottomlower.y + Opposite.y) / 2;
		Midpoint.z= (bottomlower.z + Opposite.z) / 2;

		referencePoint = Midpoint;
		
	}
	void draw()
	{
		glColor3f(clr.r, clr.g, clr.b);
		glPushMatrix();
		{
			glTranslatef(referencePoint.x, referencePoint.y, referencePoint.z);
			glutSolidCube(length);
		}
		glPopMatrix();
	}

};



class Pyramid : public Object
{
	public:
		double width, height;
		Vector lowest;
		
		Pyramid() {}
		Pyramid(Vector lowestPoint, double w, double h)
		{
			width = w;
			height = h;
			referencePoint = lowestPoint;
			lowest = lowestPoint;

		}

	
		void draw()
		{
			glColor3f(clr.r, clr.g, clr.b);

			glPushMatrix();
			glTranslatef(referencePoint.x, referencePoint.y, referencePoint.z);

			glPushMatrix();
			glScalef(width, width, height);
			glBegin(GL_QUADS);
			
				glVertex3f(0, 0, 0);
				glVertex3f(1, 0, 0);
				glVertex3f(1, 1, 0);
				glVertex3f(0, 1, 0);
			
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glScalef(width, width, height);
			glBegin(GL_TRIANGLES);
			
				glVertex3f(0, 0, 0);
				glVertex3f(1, 0, 0);
				glVertex3f(0.5, 0.5, 1);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glScalef(width, width, height);
			glBegin(GL_TRIANGLES);
			
				glVertex3f(1, 0, 0);
				glVertex3f(1, 1, 0);
				glVertex3f(0.5, 0.5, 1);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glScalef(width, width, height);
			glBegin(GL_TRIANGLES);
			
				glVertex3f(0, 1, 0);
				glVertex3f(1, 1, 0);
				glVertex3f(0.5, 0.5, 1);
			glEnd();
			glPopMatrix();
			
			glPopMatrix();
		}

		
};









