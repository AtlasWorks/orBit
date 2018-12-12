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


	

	CurrentAnimation = Orbinfo.fly;
	Orbinfo.fly->speed = Orbinfo.animationspeed;
	

	position.x = 0;
	position.y = 0;

	entitycoll->SetPos(position.x, position.y);

	entitystate = IDLE;


	if (spritesheet == nullptr)
		spritesheet = App->tex->Load(Orbinfo.Texture.GetString());



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
	/*bool lateralcollision = true;

	if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + 3)
	{
		lateralcollision = false;
	}

	if (c2->type == COLLIDER_TYPE::COLLIDER_FLOOR || c2->type == COLLIDER_TYPE::COLLIDER_PLATFORM && dead == false && !lateralcollision)
	{
		if (c1->rect.y + c1->rect.h > c2->rect.y && c1->rect.y + c1->rect.h<c2->rect.y + colliding_offset)
		{
			c1->rect.y = c2->rect.y - c1->rect.h;
		}

		must_fall = false;

		if (going_right)
		{
			going_right = true;
			entitystate = RIGHT;
			going_left = false;
		}
		else
		{
			entitystate = LEFT;
			going_left = true;
			going_right = false;
		}

		slimecolliding = true;

	}

	if (lateralcollision)
	{
		if (going_right)
		{
			entitystate = LEFT;
			going_left = true;
			going_right = false;
			c1->rect.x = c2->rect.x - c1->rect.w - 2.0f;
		}
		else
		{
			going_right = true;
			entitystate = RIGHT;
			going_left = false;
			c1->rect.x = c2->rect.x + c2->rect.w + 2.0f;
		}
		slimecolliding = true;

		position.x = c1->rect.x;
	}
*/
}


void j1Orb::UpdateMovement(float dt)
{

}



bool j1Orb::Load(pugi::xml_node &config) //must change
{
	bool ret = true;
	/*if (entityID == Orbinfo.RefID.x)
	{
		position.x = config.child("Entity4").child("Slimex").attribute("value").as_float();
		position.y = config.child("Entity4").child("Slimey").attribute("value").as_float();
	}
	else if (entityID == Orbinfo.RefID.y)
	{
		position.x = config.child("Entity5").child("Slimex").attribute("value").as_float();
		position.y = config.child("Entity5").child("Slimey").attribute("value").as_float();
	}
*/


	return ret;
}

bool j1Orb::Save(pugi::xml_node &config) const //must change
{
	/*if (entityID == Orbinfo.RefID.x)
	{
		config.append_child("Entity4").append_child("Slimex").append_attribute("value") = position.x;
		config.child("Entity4").append_child("Slimey").append_attribute("value") = position.y;
	}
	else if (entityID == Orbinfo.RefID.y)
	{
		config.append_child("Entity5").append_child("Slimex").append_attribute("value") = position.x;
		config.child("Entity5").append_child("Slimey").append_attribute("value") = position.y;
	}*/

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
