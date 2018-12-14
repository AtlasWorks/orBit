#include "j1Orb.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "j1Player.h"


j1Orb::j1Orb() : j1Entity("Orb", entity_type::ORB)
{

}

j1Orb::~j1Orb()
{

}

bool j1Orb::Start()
{
	LOG("Loading Slime");

	Orbinfo = manager->GetOrbData();

	entitycollrect = Orbinfo.OrbRect;
	colliding_offset = Orbinfo.colliding_offset;

	entitycoll = App->coll->AddCollider(entitycollrect, COLLIDER_TYPE::COLLIDER_ORB, (j1Module*)manager);

	touched = false;
	

	CurrentAnimation = Orbinfo.fly;
	Orbinfo.fly->speed = Orbinfo.animationspeed;
	Orbinfo.disappear->loop = false;
	Orbinfo.disappear->speed = Orbinfo.animationspeed;

	

	position.x = -1;
	position.y = -1;

	entitycoll->SetPos(position.x, position.y);

	entitystate = IDLE;


	if (spritesheet == nullptr)
		spritesheet = App->tex->Load(Orbinfo.Texture.GetString());

	entityID = App->entities->entityID;

	return true;
}

bool j1Orb::Update(float dt)
{
	
	entitycoll->SetPos(position.x, position.y);

	return true;
}

bool j1Orb::PostUpdate(float dt)
{
	bool ret = true;

	

	//if ((position.x)*App->win->GetScale() >= -App->render->camera.x && (position.x)*App->win->GetScale() <= -App->render->camera.x + App->render->camera.w)
	//{
	//	//check for player nearby

	//	if (!App->scene->player->god_mode &&
	//		App->scene->player->Future_position.x > position.x - Orbinfo.areaofaction &&
	//		App->scene->player->Future_position.x < position.x + Orbinfo.areaofaction &&
	//		App->scene->player->Future_position.y < position.y + Orbinfo.areaofaction &&
	//		App->scene->player->Future_position.y > position.y - Orbinfo.areaofaction)
	//	{
	//		
	//			CurrentAnimation = Orbinfo.fly;
	//			entitystate = IDLE;
	//			
	//	

	//	}
	//	else
	//	{
	//		

	//		
	//	}
	//}


	//Blitting orb
	App->render->Blit(spritesheet, position.x /*- Orbinfo.printingoffset.x*/, position.y /*+ Orbinfo.printingoffset.y*/, &CurrentAnimation->GetCurrentFrame(dt));

	return ret;
}



void j1Orb::OnCollision(Collider * c1, Collider * c2)
{
	bool lateralcollision = true;


	if (c2->type == COLLIDER_TYPE::COLLIDER_ORB || c2->type == COLLIDER_TYPE::COLLIDER_PLAYER )
	{
		if (touched == false)
		{
			App->scene->player->orbs_number += 1;
			if (entitycoll != nullptr)
			{
				entitycoll->to_delete = true;
				CurrentAnimation = Orbinfo.disappear;
			}
			touched = true;
		}
	}

	

}


void j1Orb::UpdateMovement(float dt)
{

}



bool j1Orb::Load(pugi::xml_node &config)
{
	bool ret = true;

	if (entityID == Orbinfo.orbID)
	{
		touched = config.child("Orb1").child("touched").attribute("value").as_bool();
	}
	else if (entityID == Orbinfo.orbID2)
	{
		touched = config.child("Orb2").child("touched").attribute("value").as_bool();
	}
	else if (entityID == Orbinfo.orbID3)
	{
		touched = config.child("Orb3").child("touched").attribute("value").as_bool();
	}



	return ret;
}

bool j1Orb::Save(pugi::xml_node &config) const
{
	if (entityID == Orbinfo.orbID)
	{
		config.append_child("Orb1").append_child("touched").append_attribute("value") = touched;
		
	}
	else if (entityID == Orbinfo.orbID2)
	{
		config.append_child("Orb2").append_child("touched").append_attribute("value") = touched;

	}
	else if (entityID == Orbinfo.orbID3)
	{
		config.append_child("Orb3").append_child("touched").append_attribute("value") = touched;

	}

	return true;
}

bool j1Orb::CleanUp()
{
	bool ret = true;

	App->tex->UnLoad(spritesheet);

	if (entitycoll != nullptr)
		entitycoll = nullptr;

	return ret;
}

void j1Orb::FixedUpdate(float dt)
{
	PostUpdate(dt);
}

void j1Orb::LogicUpdate(float dt)
{
	Update(dt);

}
