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

	iddle->speed = 0.05f;
	disappear->speed = 0.1f;
	appear->speed = 0.05f;

	OrbX = config.child("collider").attribute("x").as_int();
	OrbY = config.child("collider").attribute("y").as_int();
	Orbwidth = config.child("collider").attribute("width").as_int();
	Orbheight = config.child("collider").attribute("height").as_int();

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

	if (App->orb->CurrentAnimation->Finished() && once)
	{
		App->orb->haveOrb = true;
		CurrentAnimation = appear;
		
	}

	if (haveOrb) {
		iddle->speed = 0.01f;

		orbcolliderMoving->SetPos(orbRect.x, orbRect.y);
	}

	if (haveOrb && finishedAppearing == false && CurrentAnimation->Finished())
		finishedAppearing = true;

	if (finishedAppearing && haveOrb)
	{
		CurrentAnimation = iddle;
	}

	orbRect.y = (int)App->player->pos.y - 15;
	if (App->player->going_right)
	{
		orbRect.x = (int)App->player->pos.x - 10;
	}
	else 
	{
		orbRect.x = (int)App->player->pos.x + App->player->playercollider->rect.w;
	}

	

	return true;
}

// Called each loop iteration
bool j1Orb::Update(float dt)
{
	bool ret = true;
	//if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && firstStage == false) //can only press during second stage. goes to first stage
	//{
	//	change_scene(StageList.start->data->GetString());
	//	firstStage = true;
	//	secondStage = false;
	//}

	//if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && secondStage == false) //can only press during first stage. goes to second stage
	//{
	//	change_scene(StageList.start->next->data->GetString());
	//	firstStage = false;
	//	secondStage = true;
	//}

	//if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) //test sounds
	//{
	//	App->audio->PlayFx(App->audio->doublejumpfx, 0);
	//}

	//if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) //reload stage1
	//{

	//	change_scene(StageList.start->data->GetString());
	//	firstStage = true;
	//	secondStage = false;
	//}

	//if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) // beginning of current level
	//{
	//	if (firstStage)
	//	{
	//		change_scene(StageList.start->data->GetString());
	//		firstStage = true;
	//		secondStage = false;
	//	}
	//	else if (secondStage)
	//	{
	//		change_scene(StageList.start->next->data->GetString());
	//		firstStage = false;
	//		secondStage = true;
	//	}
	//}

	//if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN) //audio down
	//{
	//	App->audio->ChangeVolume_music(10);
	//	App->audio->ChangeVolume_fx(10);
	//	LOG("volume up");
	//}

	//if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) //audio up
	//{
	//	App->audio->ChangeVolume_music(-10);
	//	App->audio->ChangeVolume_fx(-10);
	//	LOG("volume down");
	//}

	//if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)//load
	//{

	//	bool ret = App->LoadGame("save_game.xml");
	//}

	//if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) //save
	//	App->SaveGame("save_game.xml");

	//if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//	App->render->camera.y += 2;

	//if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//	App->render->camera.y -= 2;

	

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

	/*config.append_child("firstStage").append_attribute("value") = firstStage;
	config.append_child("secondStage").append_attribute("value") = secondStage;
*/
	return ret;
}

bool j1Orb::Load(pugi::xml_node &config)
{

	bool ret = true;
	
	


	return ret;
}

void j1Orb::OnCollision(Collider * c1, Collider * c2)
{
	if (c2->type == COLLIDER_ORB || c1->type == COLLIDER_ORB)
	{

		App->orb->CurrentAnimation = App->orb->disappear;

		if (App->orb->CurrentAnimation->Finished())
		{
			App->orb->haveOrb = true;
		}


	}
}

void j1Orb::AddCollider() {
	
	if (!haveOrb && App->scene->firstStage)
		orbcollider = App->coll->AddCollider({ OrbX,OrbY,Orbwidth,Orbheight }, COLLIDER_ORB, this);
	
	else if (haveOrb)
	orbcolliderMoving = App->coll->AddCollider({ (int)App->player->pos.x - 15, (int)App->player->pos.y - 15,
		Orbwidth, Orbheight }, COLLIDER_ORB, this);


}