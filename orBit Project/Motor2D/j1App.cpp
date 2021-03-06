#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Fonts.h"
#include "j1Gui.h"

#include "Brofiler/Brofiler.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	coll = new j1Collision();
	entities = new j1EntityManager();
	pathfinding = new j1PathFinding();
	font = new j1Fonts();
	gui = new j1Gui();



	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(entities);
	AddModule(coll);
	AddModule(map);
	AddModule(pathfinding);
	AddModule(font);
	AddModule(gui);

	// scene last
	AddModule(scene);


	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = LoadConfig(config_file, "config.xml");

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());

		framerate_cap = app_config.attribute("framerate_cap").as_uint();

		if (framerate_cap > 0.0f)
		{
			capped_ms = 1000.0f / framerate_cap;
		}
	}

	if(ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	PERF_START(ptimer);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	startup_time.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{

	BROFILER_CATEGORY("App_Update", Profiler::Color::AntiqueWhite);

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file, const char* path) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file(path);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();

	if (App->scene->Activate_Ingamemenu
		|| App->scene->Activate_MainMenu
		|| App->scene->Activate_Credits
		|| App->scene->Activate_InGameSettings
		|| App->scene->Activate_MainMenuSettings)
	{
		//App->win->SetScale(1);
		on_GamePause = true;
		dt = 0.0f;
		
	}
	else
	{
		if (scene->taketime == true)
		{
			scene->timeBeingPaused.Stop();
			scene->timeAccumulated = scene->timeBeingPaused.getTotalTimeofPaused();
			scene->sceneTimer.changePausedtime(scene->timeAccumulated);
		}
		App->win->SetScale(2);
		on_GamePause = false;
		scene->taketime = false;
		
	}

	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if(want_to_save == true)
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();

	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000.0f)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0.0f;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	// FPS / average FPS / MS of the last frame (Cap on/off + Vsync on/off)
	if (cap_on && render->Vsync)
	{
		sprintf_s(title, 256, "FPS: %i /Average FPS: %.2f / Ms of the last frame: %u / FPS Cap: True / Vsync: True / Timer:%f ",
			frames_on_last_update, avg_fps, last_frame_ms, scene->sceneTimer.ReadSec());
	}
	else if (cap_on && !render->Vsync)
	{
		if (on_GamePause)
		{
			sprintf_s(title, 256, "FPS: %i /Average FPS: %.2f / Ms of the last frame: %u / FPS Cap: True / Vsync: False / Timer:%u ",
				frames_on_last_update, avg_fps, last_frame_ms, scene->timeWhenPaused);
		}
		else
			sprintf_s(title, 256, "FPS: %i /Average FPS: %.2f / Ms of the last frame: %u / FPS Cap: True / Vsync: False / Timer:%f ",
				frames_on_last_update, avg_fps, last_frame_ms, scene->sceneTimer.ReadSec());
	}
	else if (!cap_on && render->Vsync)
	{
		sprintf_s(title, 256, "FPS: %i /Average FPS: %.2f / Ms of the last frame: %u / FPS Cap: False / Vsync: True ",
			frames_on_last_update, avg_fps, last_frame_ms);
	}
	else if (!cap_on && !render->Vsync)
	{
		sprintf_s(title, 256, "FPS: %i /Average FPS: %.2f / Ms of the last frame: %u / FPS Cap: False / Vsync: False ",
			frames_on_last_update, avg_fps, last_frame_ms);
	}
	else
	{
		sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
			avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
	}

	App->win->SetTitle(title);

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		j1PerfTimer t;
		if (cap_on)
			SDL_Delay(capped_ms - last_frame_ms);
		//Debug purpose
		//LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate(dt);
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	PERF_START(ptimer);

	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
bool j1App::LoadGame(const char* file)
{
	bool ret = true;
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	load_game.create("save_game.xml");

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.GetString());
	if (result == NULL)
	{
		ret = false;
	}
	want_to_load = true;
	
	return ret;
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
	save_game.create(file);
}

// ---------------------------------------
void j1App::GetSaveGames(p2List<p2SString>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool j1App::LoadGameNow()
{
	bool ret = false;
	
	load_game.create("save_game.xml");

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.GetString());

	if(result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.GetString());

		root = data.child("game_state");

		p2List_item<j1Module*>* item = modules.start;
		ret = true;

		while(item != NULL && ret == true)
		{
			ret = item->data->Load(root.child(item->data->name.GetString()));
			item = item->next;
		}

		data.reset();
		if(ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : "unknown");
	}
	else
	{
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());
		ret = false;
	}
		

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	save_game.create("save_game.xml");

	LOG("Saving Game State to %s...", save_game.GetString());

	// xml object where we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	p2List_item<j1Module*>* item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Save(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if(ret == true)
	{
		data.save_file(save_game.GetString());
		LOG("... finished saving", save_game.GetString());
	}
	else
		LOG("Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}