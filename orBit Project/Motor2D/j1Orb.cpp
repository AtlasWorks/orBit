#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
//#include "j1Map.h"
#include "j1Orb.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1PathFinding.h"
#include "j1Scene.h"

j1Orb::j1Orb() : j1Module()
{
	name.create("orb");


}

// Destructor
j1Orb::~j1Orb()
{}

// Called before render is available
bool j1Orb::Awake(pugi::xml_node& config)
{
	LOG("Loading orb");
	bool ret = true;

	animation.create(config.child("animation").child_value());
	Texture.create(config.child("texture").child_value());

	iddle = App->player->LoadAnimation(animation.GetString(), "iddle");
	disappear = App->player->LoadAnimation(animation.GetString(), "disappear");
	appear = App->player->LoadAnimation(animation.GetString(), "appear");

	//iddle->speed = animSpeed;
	//disappear->speed = animSpeed;
	//appear->speed = animSpeed;
	iddle->speed = 0.15f;
	disappear->speed = 0.15f; //is not working loading from var
	appear->speed = 0.15f;
	appear->loop = false;
	disappear->loop = false;

	OrbX = config.child("collider").attribute("x").as_int();
	OrbY = config.child("collider").attribute("y").as_int();
	Orbwidth = config.child("collider").attribute("width").as_int();
	Orbheight = config.child("collider").attribute("height").as_int();
	OffsetY = config.child("offsetY").attribute("value").as_int();
	OffsetX = config.child("offsetX").attribute("value").as_int();
	animSpeed = config.child("animSpeed").attribute("value").as_float();
	orbSpeed = config.child("orbSpeed").attribute("value").as_float();
	maxTime = config.child("maxtime").attribute("value").as_int();

	return ret;
}

// Called before the first frame
bool j1Orb::Start()
{
	bool ret = true;

	CurrentAnimation = iddle;
	if (orbText == nullptr)
		orbText = App->tex->Load(Texture.GetString()); //orb

	orbcollider = App->coll->AddCollider({ OrbX,OrbY,Orbwidth,Orbheight }, COLLIDER_ORB, this);

	orbRect = { (int)App->player->pos.x, (int)App->player->pos.y-15,Orbwidth,Orbheight };
	return ret;
}

// Called each loop iteration
bool j1Orb::PreUpdate()
{

	if (CurrentAnimation->Finished() && once && !finishedAppearing)
	{
		haveOrb = true;
		CurrentAnimation = appear;
		if (orbcolliderMoving == nullptr) 
			AddCollider();
	}

	if (haveOrb) {
		 iddle->speed = 0.15f;

		orbcolliderMoving->SetPos(orbRect.x, orbRect.y);
	}

	if (haveOrb && !finishedAppearing && CurrentAnimation->Finished())
	{
		finishedAppearing = true;
		
	}

	if (finishedAppearing && haveOrb)
	{
		CurrentAnimation = iddle;
		
	}
	Movement();
	return true;
}

// Called each loop iteration
bool j1Orb::Update(float dt)
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN )
	{
		if (haveOrb) //if i have the orb-> disappear
		{
			haveOrb = false;
			orbcolliderMoving->to_delete=true;
		}
		else // if i dont have it ->appear
		{
			haveOrb = true;
			once = true;
			finishedAppearing=false;
			CurrentAnimation = appear;
			orbcollider->to_delete = true;
		}
		AddCollider();
		PreUpdate();
			Movement();
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && haveOrb)
	{
		timer = true;
		shoot = true;

	}
	

	return ret;
}

// Called each loop iteration
bool j1Orb::PostUpdate()
{
	bool ret = true;

	if (!haveOrb && App->scene->firstStage)
	App->render->Blit(orbText, OrbX, OrbY, &CurrentAnimation->GetCurrentFrame()); //orb
	
	else if (haveOrb)
	{
		App->render->Blit(orbText,orbRect.x,orbRect.y, &CurrentAnimation->GetCurrentFrame());
		
	}
	return ret;
}

// Called before quitting
bool j1Orb::CleanUp()
{
	LOG("Freeing orb");
	App->tex->UnLoad(orbText);

	/*if (playercollider != nullptr)
		playercollider = nullptr;*/

	return true;
}


bool j1Orb::Save(pugi::xml_node &config) const
{
	bool ret = true;

	config.append_child("once").append_attribute("value") = once;
	//config.append_child("secondStage").append_attribute("value") = secondStage;

	return ret;
}

bool j1Orb::Load(pugi::xml_node &config)
{

	bool ret = true;
	once = config.child("once").attribute("value").as_bool();
	


	return ret;
}

void j1Orb::OnCollision(Collider * c1, Collider * c2)
{
	
	

	 if (c2->type == COLLIDER_ORB || c1->type == COLLIDER_ORB && App->orb->once == false) //orb
	{
		once = true;
		CurrentAnimation = App->orb->disappear;
		orbcollider->to_delete = true;
		orbcolliderMoving = App->coll->AddCollider({ (int)App->player->pos.x - OffsetY, (int)App->player->pos.y - OffsetY, Orbwidth,
			Orbheight }, COLLIDER_ORB, this);
	}
	
	 if ((c2->type == COLLIDER_PLATFORM || c1->type == COLLIDER_PLATFORM) && shoot) //orb
	 {
		 touchedSomething = true;
	 }

	 if ((c2->type == COLLIDER_FLOOR || c1->type == COLLIDER_FLOOR) && shoot) //orb
	 {
		 touchedSomething = true;
	 }
	
}

void j1Orb::AddCollider() {
	
	if (!haveOrb && App->scene->firstStage)
		orbcollider = App->coll->AddCollider({ OrbX,OrbY,Orbwidth,Orbheight }, COLLIDER_ORB, this);
	
	else if (haveOrb && orbcolliderMoving==nullptr)

	orbcolliderMoving = App->coll->AddCollider({ (int)App->player->pos.x - OffsetY, (int)App->player->pos.y - OffsetY,
		Orbwidth, Orbheight }, COLLIDER_ORB, this);

	



}


void j1Orb::Movement() 
{
	//ball movement
	if (haveOrb)
	{
		if (timer)
		{
			start_time = SDL_GetTicks();
			timer = false;
		}

		if (!shoot && !touchedSomething)
		{
			orbRect.y = (int)App->player->pos.y - OffsetY;
			if (App->player->going_right)
			{
				orbRect.x = (int)App->player->pos.x - OffsetX;
				shootright = true;
			}
			else if (App->player->going_left)
			{
				orbRect.x = (int)App->player->pos.x + App->player->playercollider->rect.w;
				shootright = false;
			}
			else
			{
				orbRect.x = (int)App->player->pos.x - OffsetX;
				shootright = true;
			}

		}

		if (shoot && touchedSomething == false)
		{

			if (shootright)
			{
				orbRect.x += orbSpeed; //speed
			}
			else
			{
				orbRect.x -= orbSpeed; //speed
			}
			timePassed = SDL_GetTicks();
			timePassed -= start_time;
		}


		// 3 seconds and the ball reappear near player
		if (timePassed > maxTime && shoot) //max time
		{
			touchedSomething = true;
		}


		if (shoot && touchedSomething)
		{
			//maybe disappear collider?
			CurrentAnimation = disappear;
			finishedAppearing = false;
			shoot = false;
			touchedSomething /*= timer = shootright  */ = false;
		}
	}
}