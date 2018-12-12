#ifndef __j1ORB_H__
#define __j1ORB_H__

#include "j1Entity.h"

struct SDL_Texture;
struct Collider;
struct PathInfo;

struct OrbData {
	
	Animation* fly = nullptr;

	p2SString folder = nullptr;
	p2SString Texture = nullptr;

	SDL_Rect		OrbRect = { 0,0,0,0 };
	iPoint			printingoffset = { 0,0 };

	float			colliding_offset = 0;
	float			animationspeed = 0;
	
	int				areaofaction = 0;
	iPoint			RefID = { 0, 0 };

	iPoint			Position1 = { 0, 0 };
	iPoint			Position2 = { 0, 0 };
	iPoint			Position3 = { 0, 0 };
	iPoint			Position4 = { 0, 0 };
	iPoint			Position5 = { 0, 0 };
	iPoint			Position6 = { 0, 0 };

};

class j1Orb :public j1Entity
{
public:

	j1Orb();
	~j1Orb();

	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	// Called each loop iteration
	void FixedUpdate(float dt);

	// Called each logic iteration
	void LogicUpdate(float dt);


	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// --- Pathfinding ---


	void UpdateMovement(float dt);

	// ----------------------

public:


	OrbData Orbinfo;
	


};

#endif // __j1ORB_H__