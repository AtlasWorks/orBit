#ifndef __j1ORB_H__
#define __j1ORB_H__

#include "j1Module.h"
#include "Animation.h"

struct SDL_Texture;
class SDL_Rect;

class j1Orb : public j1Module
{
public:

	j1Orb();

	// Destructor
	virtual ~j1Orb();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//call to save stage info
	bool Save(pugi::xml_node&config) const;

	//call to load file
	bool Load(pugi::xml_node&config);

	void OnCollision(Collider* c1, Collider* c2);
	void AddCollider();

public:

	int Orbwidth = NULL;
	int Orbheight = NULL;
	int OrbX = NULL;
	int OrbY = NULL;

	Collider * orbcollider = nullptr;
	Collider * orbcolliderMoving = nullptr;

	SDL_Texture* orbText = nullptr;

	Animation* CurrentAnimation = nullptr;
	Animation* iddle = nullptr;
	Animation* disappear = nullptr;
	Animation* appear = nullptr;

	bool haveOrb = false; // true when player have orb

	bool finishedAppearing = false; // true when the the animation on appearing is done
	bool once = false;  //one time touching the collider to obtain the orb
	

private:
	p2SString animation = nullptr;
	p2SString Texture = nullptr;
};

#endif // __j1ORB_H__