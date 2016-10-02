#include<iostream>
#include<cstdio>
#include<cassert>
#include<cstring>
#include<cfloat>
#include "image.h"
#include "scene_parser.h" 
#include "ray.h"
#include "light.h"
#include "camera.h"
#include "glCanvas.h"
#include<gl/glut.h>

using namespace std;

char *input_file = NULL;
int width = 200;
int height = 200;
int bounces = 0;
char *output_file = NULL;  //the formal output rendered image
float depth_min = 0; 
float depth_max = 1;
char *depth_file = NULL;  //image which indicate the depth
char *normal_file = NULL;  //image which indicate normal vector
bool gui = false, ginter = false, backflag = false, shadow = false;
float tst, pst, weight = 0.01f;
SceneParser *scene;  //pointer to load the scene file

class RayTracer
{
public:
	RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadow); //initialize raytracer class
	~RayTracer();
	void renderfunction();  //render the image
	Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight,float indexOfRefraction, Hit &hit);  //for every pixel, send a trace ray and return the computed pixel color
	Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming);  //compute the mirror direction for reflective material
	bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming,float index_i, float index_t, Vec3f &transmitted);  //compute the refraction direction for transparent material
private:
	SceneParser *scene;
	int maxbounces,lightc;
	float cutoff;  //cutoff light weight
	bool shadow;
	Image *image;
	Camera* camera;
	Object3D *group;
	Light **light;
	Vec3f ambientco, backco;// ambient color and background color
	float cos, dist;
	Vec3f dir, col, normal;
	float w = width, h = height, t;
};

RayTracer::RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadow) 
	:maxbounces(max_bounces), cutoff(cutoff_weight), shadow(shadow)
{
	scene = s;
	lightc = scene->getNumLights();
	image = new Image(width, height);
	camera = scene->getCamera();
	group = scene->getGroup();
	light = new Light*[lightc];
	for (int i = 0; i < lightc; i++)
		light[i] = scene->getLight(i);
	ambientco = scene->getAmbientLight(), backco = scene->getBackgroundColor();
	w = width;
	h = height;
}
RayTracer::~RayTracer()
{
}
void RayTracer::renderfunction()
{
	Ray ray;
	Hit hit;
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		{
			ray = camera->generateRay(Vec2f(x / w, y / h));  //generate ray from this pixel's position
			hit.set(100000, NULL, Vec3f(0, 0, 0), ray);//record the minimum t value in hit and we can get the correct material to render
			if (group->intersect(ray, hit, camera->getTMin()))  //use this ray to intersect the model and get the minimum t value
			{
				image->SetPixel(x, y, this->traceRay(ray, camera->getTMin(), 0, 1, hit.getMaterial()->i_refraction, hit));
				//compute the pixel color and set it to the image
			}
			else
			{
				image->SetPixel(x, y, scene->getBackgroundColor());
			}
		}
	image->SaveTGA(output_file);
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) //recursively call this function to compute the ray color.
{
	Vec3f objc = hit.getMaterial()->getDiffuseColor();
	Vec3f thecolor = objc*ambientco;
	Vec3f hitnormal = hit.getNormal();
	Material *hitmat = hit.getMaterial();
	hitnormal.Normalize();
	Vec3f hitpoint = hit.getIntersectionPoint();
	for (int i = 0; i < lightc; i++)  //for every light, compute the shading color.
	{
		Vec3f dir1,color1;
		float dist1;
		light[i]->getIllumination(hitpoint, dir1, color1, dist1);
		Ray tolight(hitpoint, dir1);
		Hit tolighthit;
		tolighthit.set(10000, NULL, Vec3f(0, 0, 0), tolight);
		if (shadow)
		{
			if (!group->intersect(tolight, tolighthit, tmin)) //if the ray from intersection point to the light will hit a object, this point discard this light's color
				thecolor += hitmat->Shade(ray, hit, dir1, color1, backflag); 
		}
		else
		{
			thecolor += hitmat->Shade(ray, hit, dir1, color1, backflag);
		}
	}
	if (bounces < maxbounces)  //if bounce times over the maximum, end the recursion.
	{
		float reflweight = weight*hitmat->reflectiveColor.Length();  //if the light weight smaller than cutoff weight, discard it.
		float refrweight = weight*hitmat->transparentColor.Length();
		if (reflweight>cutoff)
		{
			Ray m_ray(hitpoint, this->mirrorDirection(hitnormal, ray.getDirection()));  //generate ray in the mirror directon.
			Hit m_hit;
			m_hit.set(100000, NULL, Vec3f(0, 0, 0), m_ray);
			if (group->intersect(m_ray, m_hit, tmin))
			{
				thecolor += hitmat->reflectiveColor*this->traceRay(m_ray, tmin, bounces + 1, reflweight, m_hit.getMaterial()->i_refraction, m_hit);
				//recursively call this trace function to get the reflection color.
			}
			else
			{
				thecolor += hitmat->reflectiveColor*scene->getBackgroundColor();
			}
		}
		if (refrweight > cutoff)
		{
			Vec3f transdir, transnormal=hitnormal;
			float indexrefra=indexOfRefraction;
			if (hitnormal.Dot3(ray.getDirection()) > 0)
			{
				indexrefra = 1.0f / indexOfRefraction;
				transnormal.Negate();
			}
			if (this->transmittedDirection(transnormal, ray.getDirection(), 1, indexrefra, transdir))  //generate ray in the refraction direction.
			{
				Ray t_ray(hitpoint, transdir);
				Hit t_hit;
				t_hit.set(100000, NULL, Vec3f(0, 0, 0), t_ray);
				if (group->intersect(t_ray, t_hit, tmin))
				{
					thecolor += hitmat->transparentColor*this->traceRay(t_ray, tmin, bounces + 1, refrweight, t_hit.getMaterial()->i_refraction, t_hit);
					//recursively call this trace function to get the refraction color.
				}
				else
				{
					thecolor += hitmat->transparentColor*scene->getBackgroundColor();
				}
			}
		}
	}
	return thecolor;
}

Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &incoming)  //return the mirror direction
{
	Vec3f outray;
	outray = incoming - 2 * normal.Dot3(incoming)*normal;
	outray.Normalize();
	return outray;
}

bool RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted)// compute the refractive direction vector
{
	float indexr = index_i / index_t;
	Vec3f incoming1 = incoming*(-1.0f);
	float d = 1 - pow(indexr, 2)*(1 - pow(normal.Dot3(incoming1), 2));
	if (d < 0) return false;
	else transmitted = (indexr*(normal.Dot3(incoming1)) - sqrt(d))*normal - indexr*incoming1;
	transmitted.Normalize();
	return true;
}

int main(int argc, char *argv[])
{
	//command line for this course's all assignments, some may be not used in this assignment.
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth")) {
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp(argv[i], "-normals")) {
			i++; assert(i < argc);
			normal_file = argv[i];
		}
		else if (!strcmp(argv[i], "-shade_back")) {
			backflag = true;
		}
		else if (!strcmp(argv[i], "-gui")) {
			gui = true;
		}
		else if (!strcmp(argv[i], "-tessellation")) {
			i++; assert(i < argc);
			tst = atof(argv[i]);
			i++; assert(i < argc);
			pst = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-gouraud")) {
			ginter = true;
		}
		else if (!strcmp(argv[i], "-shadows")) {
			shadow = true;
		}
		else if (!strcmp(argv[i], "-bounces")) {
			i++; assert(i < argc);
			bounces = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight")) {
			i++; assert(i < argc);
			weight = atof(argv[i]);
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}
	//glDisable(GL_LIGHTING);
	scene=new SceneParser(input_file, tst, pst, ginter);
	RayTracer *raytracer = new RayTracer(scene, bounces, weight, shadow);
	
	raytracer->renderfunction();
}