#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"


j1Player::j1Player()
{
	name.create("player");

}

j1Player::~j1Player()
{

}

bool j1Player::Start()
{

	LOG("Loading player");

	playercollider = App->coll->AddCollider({ 0, 0, 19, 36 }, COLLIDER_PLAYER, this);

	Velocity.x = 2.0f;
	Velocity.y = 0.0f;
	pos.x = 180;
	pos.y = 0.0f;

	gravity = -1.0f;
	playercolliding = false;

	jump_force = 7.5f;
	max_speed_y = 10.0f;
	stateplayer = IDLE;
	must_fall = false;
	double_jump = true;

	return true;
}

bool j1Player::Update(float dt)
{
	//Check if player is Falling or jumping

	/*if (must_fall)
	{
		playercolliding = false;
	}*/

	if (Velocity.y < 0 && stateplayer == JUMPING)
	{
		stateplayer = FALLING;
	}

	if (playercolliding == false && stateplayer == IDLE)
	{
		stateplayer = FALLING;
	}

	//Horizontal Movement 

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		Velocity.x = 2.0f;
		pos.x = pos.x - Velocity.x;
		going_left = true;
		going_right = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		Velocity.x = 2.0f;
		pos.x = pos.x + Velocity.x;
		going_right = true;
		going_left = false;
	}


	//Vertical Movement


	if (!must_fall)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && playercolliding == true)
		{
			Velocity.y = jump_force;
			stateplayer = JUMPING;
			playercolliding = false;
		}
	}

	if (stateplayer == JUMPING)
	{
		must_fall = false;
		if (double_jump == true && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && Velocity.y!=jump_force)
		{
			Velocity.y = jump_force/1.5f;
			pos.y -= Velocity.y;
			double_jump = false;
		}
	
		Velocity.y += gravity / 2;
		pos.y -= Velocity.y;

	}

	if (stateplayer == FALLING)
	{
		must_fall = false;

		if (double_jump == true && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && Velocity.y != jump_force)
		{
			Velocity.y = jump_force/1.5f;
			pos.y -= Velocity.y;
			double_jump = false;
		}

		Velocity.y += gravity / 2;
		pos.y -= Velocity.y;

	}

	//Limit maximum y axis velocity
	if (Velocity.y < -max_speed_y)
		Velocity.y = -max_speed_y;


	//Set player pos, prevent surpassing colliders 
	playercollider->SetPos(pos.x, pos.y);

	App->coll->Update(1.0f);

	playercollider->SetPos(pos.x, pos.y);

	//If no ground, free fall
	if (must_fall)
	{
		pos.y -= gravity*3.0f;
	}

	return true;
}

bool j1Player::PostUpdate()
{
	bool ret = true;

	App->render->DrawQuad(playercollider->rect, 0, 255, 0);

	return ret;
}

void j1Player::OnCollision(Collider * c1, Collider * c2)
{
	bool lateralcollision = true;

	if (c1->rect.y + c1->rect.h == c2->rect.y)
	{
		lateralcollision = false;
	}

	float aux = c1->rect.y; //pos.y

		if (c1->type == COLLIDER_FLOOR || c2->type == COLLIDER_FLOOR)
		{
		
			if (stateplayer != JUMPING && stateplayer != FALLING)
			{
				Velocity.y = 0.0f;
				stateplayer = IDLE;
			}
			// c2 ==COLLIDER_FLOOR 
			
				if (stateplayer != JUMPING)
					c1->rect.y = c2->rect.y - c1->rect.h;


				if (going_right)
				{
					//stopping player if lateral col0lision
					if (lateralcollision)
					{
						if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + 3 )
						{
							Velocity.x = 0.0f;
							if (stateplayer != JUMPING)
								c1->rect.y = aux;
							c1->rect.x = c2->rect.x - c1->rect.w;
						}
					}	
					else if (!lateralcollision && must_fall == false)
						stateplayer = IDLE;
				}
				//going left
				if (going_left)
				{
					if (lateralcollision)
					{
						if (c1->rect.x >= c2->rect.x + c2->rect.w - 3 && c1->rect.x <= c2->rect.x + c2->rect.w) //c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - 3
						{                                                                                                    //c2->rect.x + c2->rect.w <= c1->rect.x && c2->rect.x + c2->rect.w <= c1->rect.x - 3
							Velocity.x = 0.0f;                                                                               //c2->rect.x + c2->rect.w - 3 <= c1->rect.x && c2->rect.x + c2->rect.w >= c1->rect.x
							if (stateplayer != JUMPING)
								c1->rect.y = aux;
							c1->rect.x = c2->rect.x + c2->rect.w;
						}
					}
					else if (!lateralcollision && must_fall == false)
						stateplayer = IDLE;
				}
		  }


		pos.x = c1->rect.x;
		pos.y = c1->rect.y;
	
	playercolliding = true;
	double_jump = true;
	must_fall = false;
}

bool j1Player::Load(pugi::xml_node &)
{
	return true;
}

bool j1Player::Save(pugi::xml_node &) const
{
	return true;
}