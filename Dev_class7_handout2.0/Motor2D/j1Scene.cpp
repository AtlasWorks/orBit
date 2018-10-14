#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Player.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");


}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	for (pugi::xml_node stage = config.child("map_name"); stage; stage = stage.next_sibling("map_name"))
	{
		p2SString* StageName = new p2SString(stage.attribute("name").as_string());
		/*StageName->create(stage.attribute("name").as_string());*/
		StageList.add(StageName);
			App->map->numberStages++;
	}

	/*map_name = config.child("map_name").attribute("name").as_string();*/

	if (StageList.start->data->GetString() == NULL)
	{
		ret = false;
		LOG("stagelist is null");
	}
	camera1.x = config.child("startcamera1").attribute("x").as_int();
	camera1.y = config.child("startcamera1").attribute("y").as_int();
	camera2.x = config.child("startcamera2").attribute("x").as_int();
	camera2.y = config.child("startcamera2").attribute("y").as_int();
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;

	/*App->map->Load(map_name.GetString());*/

	//Loading map both maps

	p2List_item<p2SString*>* stageListItem;
	stageListItem = StageList.start;

	ret = App->map->Load(stageListItem->data->GetString(),App->map->data);
		
		if (ret == false)
		{
			LOG("issue loading First Stage");
			ret = false;
		}
		
	stageListItem = StageList.start->next;;

	ret = App->map->Load(stageListItem->data->GetString(), App->map->data2);

	if (ret == false)
	{
		LOG("issue loading Second Stage");
		ret = false;
	}


	//ret = App->map->Load(StageList.start->data->GetString());
	
	//loading music&positions depending in the order

	FirstStage = StageList.start->data->GetString();

	if (FirstStage =="stage1_TiledV017.tmx" )
	{
		App->render->camera.x = camera1.x;
		App->render->camera.y = camera1.y;
		App->player->pos.x = App->map->data.initpos.x;//App->player->initpos1.x;
		App->player->pos.y = App->map->data.initpos.y;//App->player->initpos1.y;
		p2SString stageMusic("%s%s", App->audio->musicfolder.GetString(), App->audio->SongNamesList.start->data->GetString());
		App->audio->PlayMusic(stageMusic.GetString());
	}
	else
	{
		App->render->camera.x = camera2.x;
		App->render->camera.y = camera2.y;
		App->player->pos.x = App->map->data2.initpos.x;//App->player->initpos1.x;
		App->player->pos.y = App->map->data2.initpos.y;//App->player->initpos1.y;
		p2SString stageMusic("%s%s", App->audio->musicfolder.GetString(), App->audio->SongNamesList.start->next->data->GetString());
		App->audio->PlayMusic(stageMusic.GetString());
	}

	// DO NOT ERASE
	/*MapLayer* layer;
	for (uint l = 0; l < App->map->data.layers.count(); l++)
	{
		layer = App->map->data.layers.At(l)->data;
		
		if (layer->name.)
		{
			p2SString stageMusic("%s%s", App->audio->musicfolder.GetString(), layer->properties.GetProperties("Song").GetString());
			App->audio->PlayMusic(stageMusic.GetString());
			
		}
	}
*/

	
		//colliderfloor = App->coll->AddCollider({ 0, 150, 1024, 100 }, COLLIDER_FLOOR, this);
		//colliderbox = App->coll->AddCollider({ 100, 120, 50, 30 }, COLLIDER_FLOOR, this);

		App->map->ColliderDrawer(App->map->data);

		//colliderfloor = App->coll->AddCollider({ 0, 150, 1024, 100 }, COLLIDER_FLOOR, this);
		//colliderbox = App->coll->AddCollider({ 100, 120, 50, 30 }, COLLIDER_FLOOR, this);
		//colliderbox = App->coll->AddCollider({ 151, 120, 50, 30 }, COLLIDER_FLOOR, this);
		//test = App->coll->AddCollider({ 400,50,20,20 }, COLLIDER_FLOOR, this);

		
	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && firstStage == false) //can only press during second stage. gos to first stage
	{	
			change_scene(StageList.start->data->GetString());
			firstStage = true;
			secondStage = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && secondStage== false) //can only press during first stage. gos to second stage
	{	
		change_scene(StageList.start->next->data->GetString());
		firstStage = false;
		secondStage = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) //reload stage1
	{
		
		change_scene(StageList.start->data->GetString());
		firstStage = true;
		secondStage = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) // beginning of current level
	{
		if (firstStage)
			{
				change_scene(StageList.start->data->GetString());
				firstStage = true;
				secondStage = false;
			}
		else if (secondStage)
			{
				change_scene(StageList.start->next->data->GetString());
				firstStage = false;
				secondStage = true;
			}
	}

	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN) //audio dwon
	{
		App->audio->ChangeVolume_music(10);
		App->audio->ChangeVolume_fx(10);
		LOG("volume up");
	}

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) //audio up
	{
		App->audio->ChangeVolume_music(-10);
		App->audio->ChangeVolume_fx(-10);
		LOG("volume down");
	}
	
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) //load
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) //save
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 5;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 5;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->render->camera.x += 5;
		App->map->paralaxRef[0] += App->map->speed[0];
		App->map->paralaxRef[1] += App->map->speed[1];
	}
	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->render->camera.x -= 5;
		App->map->paralaxRef[0] -= App->map->speed[0];
		App->map->paralaxRef[1] -= App->map->speed[1];
	}	

	int x, y;
	App->input->GetMousePosition(x, y);

	if (firstStage == true)
	{
		App->map->Draw(App->map->data);
		
		
		iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y, App->map->data);
		p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
			App->map->data.width, App->map->data.height,
			App->map->data.tile_width, App->map->data.tile_height,
			App->map->data.tilesets.count(),
			map_coordinates.x, map_coordinates.y);

		App->win->SetTitle(title.GetString());
	}
	else
	{
		App->map->Draw(App->map->data2);

		
		iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y, App->map->data2);
		p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
			App->map->data.width, App->map->data.height,
			App->map->data.tile_width, App->map->data.tile_height,
			App->map->data.tilesets.count(),
			map_coordinates.x, map_coordinates.y);

		App->win->SetTitle(title.GetString());
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	p2List_item<p2SString*>* item;
	item = StageList.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	StageList.clear();

	return true;
}


bool j1Scene::change_scene(const char* map_name) {
	
	bool ret = true;

	
	App->coll->CleanUp();
	App->player-> playercollider= App->coll->AddCollider(App->player->playercol, COLLIDER_PLAYER, App->player);

	
	if (FirstStage == map_name)
	{	
		App->render->camera.x = camera1.x;
		App->render->camera.y = camera1.y;
		/*App->player->pos.x = 560;
		App->player->pos.y = 180;*/
		App->player->pos.x = App->map->data.initpos.x;//App->player->initpos1.x;
		App->player->pos.y = App->map->data.initpos.y;//App->player->initpos1.y;
		App->map->ColliderDrawer(App->map->data);
		p2SString stageMusic("%s%s", App->audio->musicfolder.GetString(), App->audio->SongNamesList.start->data->GetString());//aqui deberia poder leer metadata
		App->audio->PlayMusic(stageMusic.GetString());
	}
	else
	{
		App->render->camera.x = camera2.x;
		App->render->camera.y = camera2.y;
		/*App->player->pos.x = 46;
		App->player->pos.y = 180;*/
		App->player->pos.x = App->map->data2.initpos.x;//App->player->initpos2.x;
		App->player->pos.y = App->map->data2.initpos.y;// App->player->initpos2.y;
		App->map->ColliderDrawer(App->map->data2);
		p2SString stageMusic("%s%s", App->audio->musicfolder.GetString(), App->audio->SongNamesList.start->next->data->GetString());//aqui leer metadata de direccion
		App->audio->PlayMusic(stageMusic.GetString());
	}
	
	return ret;
}


bool j1Scene::Save(pugi::xml_node &config) const
{
	bool ret = true;

	config.append_child("firstStage").append_attribute("value") = firstStage;
	config.append_child("secondStage").append_attribute("value") = secondStage;

	return ret;
}

bool j1Scene::Load(pugi::xml_node &config)
{

	bool ret = true;

	firstStage= config.child("firstStage").attribute("value").as_bool();
	secondStage= config.child("secondStage").attribute("value").as_bool();

	return ret;
}