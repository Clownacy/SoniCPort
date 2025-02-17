#include "Object.h"

#include "Video.h"
#include "Level.h"
#include "LevelScroll.h"

#include "Object/Sonic.h"

#include <Macros.h>

#include <string.h>

//Object draw queue
struct SpriteQueue
{
	uint32_t size;
	Object *obj[0x3F];
} sprite_queue[8];

//Object indices
//#ifndef SCP_FIX_BUGS
//	#define Obj_Null ObjectFall //Thats right, all null objects point to ObjectFall
//#else
	void Obj_Null(Object *obj)
	{
		if (obj->respawn_index)
			objstate[obj->respawn_index] &= 0x7F;
		ObjectDelete(obj);
	}
//#endif
//Don't re-enable this until all objects are implemented
//...Trust me

void Obj_Sonic(Object *obj);
void Obj_TitleSonic(Object *obj);
void Obj_PSB(Object *obj);
void Obj_GHZBridge(Object *obj);
void Obj_Crabmeat(Object *obj);
void Obj_BuzzBomber(Object *obj);
void Obj_BuzzMissile(Object *obj);
void Obj_BuzzExplode(Object *obj);
void Obj_Ring(Object *obj);
void Obj_Explosion(Object *obj);
void Obj_Chopper(Object *obj);
void Obj_TitleCard(Object *obj);
void Obj_RingLoss(Object *obj);
void Obj_GameOverCard(Object *obj);
void Obj_GHZRock(Object *obj);
void Obj_Motobug(Object *obj);
void Obj_Spring(Object *obj);
void Obj_Newtron(Object *obj);
void Obj_GHZEdge(Object *obj);
void Obj_Credits(Object *obj);

static void (*object_func[])(Object*) = {
	/* ObjId_Null         */ NULL,
	/* ObjId_Sonic        */ Obj_Sonic,
	/* ObjId_02           */ Obj_Null,
	/* ObjId_03           */ Obj_Null,
	/* ObjId_04           */ Obj_Null,
	/* ObjId_05           */ Obj_Null,
	/* ObjId_06           */ Obj_Null,
	/* ObjId_07           */ Obj_Null,
	/* ObjId_08           */ Obj_Null,
	/* ObjId_09           */ Obj_Null,
	/* ObjId_0A           */ Obj_Null,
	/* ObjId_0B           */ Obj_Null,
	/* ObjId_0C           */ Obj_Null,
	/* ObjId_0D           */ Obj_Null,
	/* ObjId_TitleSonic   */ Obj_TitleSonic,
	/* ObjId_PSB          */ Obj_PSB,
	/* ObjId_10           */ Obj_Null,
	/* ObjId_GHZBridge    */ Obj_GHZBridge,
	/* ObjId_12           */ Obj_Null,
	/* ObjId_13           */ Obj_Null,
	/* ObjId_14           */ Obj_Null,
	/* ObjId_15           */ Obj_Null,
	/* ObjId_16           */ Obj_Null,
	/* ObjId_17           */ Obj_Null,
	/* ObjId_18           */ Obj_Null,
	/* ObjId_19           */ Obj_Null,
	/* ObjId_1A           */ Obj_Null,
	/* ObjId_1B           */ Obj_Null,
	/* ObjId_1C           */ Obj_Null,
	/* ObjId_1D           */ Obj_Null,
	/* ObjId_1E           */ Obj_Null,
	/* ObjId_Crabmeat     */ Obj_Crabmeat,
	/* ObjId_20           */ Obj_Null,
	/* ObjId_21           */ Obj_Null,
	/* ObjId_BuzzBomber   */ Obj_BuzzBomber,
	/* ObjId_BuzzMissile  */ Obj_BuzzMissile,
	/* ObjId_BuzzExplode  */ Obj_BuzzExplode,
	/* ObjId_Ring         */ Obj_Ring,
	/* ObjId_26           */ Obj_Null,
	/* ObjId_Explosion    */ Obj_Explosion,
	/* ObjId_Animal       */ Obj_Null,
	/* ObjId_29           */ Obj_Null,
	/* ObjId_2A           */ Obj_Null,
	/* ObjId_Chopper      */ Obj_Chopper,
	/* ObjId_2C           */ Obj_Null,
	/* ObjId_2D           */ Obj_Null,
	/* ObjId_2E           */ Obj_Null,
	/* ObjId_2F           */ Obj_Null,
	/* ObjId_30           */ Obj_Null,
	/* ObjId_31           */ Obj_Null,
	/* ObjId_32           */ Obj_Null,
	/* ObjId_33           */ Obj_Null,
	/* ObjId_TitleCard    */ Obj_TitleCard,
	/* ObjId_35           */ Obj_Null,
	/* ObjId_36           */ Obj_Null,
	/* ObjId_RingLoss     */ Obj_RingLoss,
	/* ObjId_38           */ Obj_Null,
	/* ObjId_GameOverCard */ Obj_GameOverCard,
	/* ObjId_3A           */ Obj_Null,
	/* ObjId_GHZRock      */ Obj_GHZRock,
	/* ObjId_3C           */ Obj_Null,
	/* ObjId_3D           */ Obj_Null,
	/* ObjId_3E           */ Obj_Null,
	/* ObjId_3F           */ Obj_Null,
	/* ObjId_Motobug      */ Obj_Motobug,
	/* ObjId_Spring       */ Obj_Spring,
	/* ObjId_Newtron      */ Obj_Newtron,
	/* ObjId_43           */ Obj_Null,
	/* ObjId_GHZEdge      */ Obj_GHZEdge,
	/* ObjId_45           */ Obj_Null,
	/* ObjId_46           */ Obj_Null,
	/* ObjId_47           */ Obj_Null,
	/* ObjId_48           */ Obj_Null,
	/* ObjId_49           */ Obj_Null,
	/* ObjId_4A           */ Obj_Null,
	/* ObjId_4B           */ Obj_Null,
	/* ObjId_4C           */ Obj_Null,
	/* ObjId_4D           */ Obj_Null,
	/* ObjId_4E           */ Obj_Null,
	/* ObjId_4F           */ Obj_Null,
	/* ObjId_50           */ Obj_Null,
	/* ObjId_51           */ Obj_Null,
	/* ObjId_52           */ Obj_Null,
	/* ObjId_53           */ Obj_Null,
	/* ObjId_54           */ Obj_Null,
	/* ObjId_55           */ Obj_Null,
	/* ObjId_56           */ Obj_Null,
	/* ObjId_57           */ Obj_Null,
	/* ObjId_58           */ Obj_Null,
	/* ObjId_59           */ Obj_Null,
	/* ObjId_5A           */ Obj_Null,
	/* ObjId_5B           */ Obj_Null,
	/* ObjId_5C           */ Obj_Null,
	/* ObjId_5D           */ Obj_Null,
	/* ObjId_5E           */ Obj_Null,
	/* ObjId_5F           */ Obj_Null,
	/* ObjId_60           */ Obj_Null,
	/* ObjId_61           */ Obj_Null,
	/* ObjId_62           */ Obj_Null,
	/* ObjId_63           */ Obj_Null,
	/* ObjId_64           */ Obj_Null,
	/* ObjId_65           */ Obj_Null,
	/* ObjId_66           */ Obj_Null,
	/* ObjId_67           */ Obj_Null,
	/* ObjId_68           */ Obj_Null,
	/* ObjId_69           */ Obj_Null,
	/* ObjId_6A           */ Obj_Null,
	/* ObjId_6B           */ Obj_Null,
	/* ObjId_6C           */ Obj_Null,
	/* ObjId_6D           */ Obj_Null,
	/* ObjId_6E           */ Obj_Null,
	/* ObjId_6F           */ Obj_Null,
	/* ObjId_70           */ Obj_Null,
	/* ObjId_71           */ Obj_Null,
	/* ObjId_72           */ Obj_Null,
	/* ObjId_73           */ Obj_Null,
	/* ObjId_74           */ Obj_Null,
	/* ObjId_75           */ Obj_Null,
	/* ObjId_76           */ Obj_Null,
	/* ObjId_77           */ Obj_Null,
	/* ObjId_78           */ Obj_Null,
	/* ObjId_79           */ Obj_Null,
	/* ObjId_7A           */ Obj_Null,
	/* ObjId_7B           */ Obj_Null,
	/* ObjId_7C           */ Obj_Null,
	/* ObjId_7D           */ Obj_Null,
	/* ObjId_7E           */ Obj_Null,
	/* ObjId_7F           */ Obj_Null,
	/* ObjId_80           */ Obj_Null,
	/* ObjId_81           */ Obj_Null,
	/* ObjId_82           */ Obj_Null,
	/* ObjId_83           */ Obj_Null,
	/* ObjId_84           */ Obj_Null,
	/* ObjId_85           */ Obj_Null,
	/* ObjId_86           */ Obj_Null,
	/* ObjId_87           */ Obj_Null,
	/* ObjId_88           */ Obj_Null,
	/* ObjId_89           */ Obj_Null,
	/* ObjId_Credits      */ Obj_Credits,
	/* ObjId_8B           */ Obj_Null,
	/* ObjId_8C           */ Obj_Null,
};

//Object functions
Object *FindFreeObj()
{
	Object *obj = level_objects;
	for (int i = 0; i < LEVEL_OBJECTS; i++, obj++)
		if (obj->type == ObjId_Null)
			return obj;
	return NULL; //Original would return the address at the end of object space, I believe
}

Object *FindNextFreeObj(Object *obj)
{
	for (; (obj - objects) < OBJECTS; obj++)
		if (obj->type == ObjId_Null)
			return obj;
	return NULL; //Original would return the address at the end of object space, I believe
}

int ExecuteObjects_i;

void ExecuteObjects()
{
	Object *obj;
	
	if (player->routine < 6)
	{
		//Run all objects
		obj = objects;
		ExecuteObjects_i = OBJECTS - 1;
		do
		{
			if (obj->type)
				object_func[obj->type](obj);
			obj++;
		} while (ExecuteObjects_i-- > 0);
	}
	else
	{
		//Run reserved objects
		obj = objects;
		ExecuteObjects_i = RESERVED_OBJECTS - 1;
		do
		{
			if (obj->type)
				object_func[obj->type](obj);
			obj++;
		} while (ExecuteObjects_i-- > 0);
		
		//Draw level objects
		ExecuteObjects_i = LEVEL_OBJECTS - 1;
		do
		{
			if (obj->type && obj->render.f.on_screen)
				DisplaySprite(obj);
			obj++;
		} while (ExecuteObjects_i-- > 0);
	}
}

//Object drawing
void BuildSprites_Draw(uint16_t **sprite, uint8_t *sprite_i, uint16_t x, uint16_t y, Object *obj, const uint8_t *mappings, uint8_t pieces)
{
	if (obj->render.f.x_flip)
	{
		if (obj->render.f.y_flip)
		{
			//XY flip
			while(pieces-- > 0)
			{
				//Don't overflow the sprite buffer
				if (*sprite_i >= BUFFER_SPRITES)
					break;
				
				//Read mappings
				int8_t map_y = *mappings++;
				uint8_t map_size = *mappings++;
				uint16_t map_tile = (mappings[0] << 8) | (mappings[1] << 0);
				mappings += 2;
				int8_t map_x = *mappings++;
				
				//Write sprite
				*(*sprite)++ = y - map_y - (((map_size << 3) & 0x18) + 8); //y
				*(*sprite)++ = (map_size << 8) | ++(*sprite_i); //size and link
				*(*sprite)++ = (map_tile + obj->tile) ^ (TILE_Y_FLIP_AND | TILE_X_FLIP_AND); //tile
				uint16_t px = x - map_x - (((map_size << 1) & 0x18) + 8);
				#if (SCREEN_WIDTH <= 320)
					if ((px &= 0x1FF) == 0)
						px++; //Prevent sprite from being x=0 (acts as a mask)
				#else
					if (px == 0)
						px++;
				#endif
				*(*sprite)++ = px; //x
			}
		}
		else
		{
			//X flip
			while(pieces-- > 0)
			{
				//Don't overflow the sprite buffer
				if (*sprite_i >= BUFFER_SPRITES)
					break;
				
				//Read mappings
				int8_t map_y = *mappings++;
				uint8_t map_size = *mappings++;
				uint16_t map_tile = (mappings[0] << 8) | (mappings[1] << 0);
				mappings += 2;
				int8_t map_x = *mappings++;
				
				//Write sprite
				*(*sprite)++ = y + map_y; //y
				*(*sprite)++ = (map_size << 8) | ++(*sprite_i); //size and link
				*(*sprite)++ = (map_tile + obj->tile) ^ TILE_X_FLIP_AND; //tile
				uint16_t px = x - map_x - (((map_size << 1) & 0x18) + 8);
				#if (SCREEN_WIDTH <= 320)
					if ((px &= 0x1FF) == 0)
						px++; //Prevent sprite from being x=0 (acts as a mask)
				#else
					if (px == 0)
						px++;
				#endif
				*(*sprite)++ = px; //x
			}
		}
	}
	else if (obj->render.f.y_flip)
	{
		//Y flip
		while(pieces-- > 0)
		{
			//Don't overflow the sprite buffer
			if (*sprite_i >= BUFFER_SPRITES)
				break;
			
			//Read mappings
			int8_t map_y = *mappings++;
			uint8_t map_size = *mappings++;
			uint16_t map_tile = (mappings[0] << 8) | (mappings[1] << 0);
			mappings += 2;
			int8_t map_x = *mappings++;
			
			//Write sprite
			*(*sprite)++ = y - map_y - (((map_size << 3) & 0x18) + 8); //y
			*(*sprite)++ = (map_size << 8) | ++(*sprite_i); //size and link
			*(*sprite)++ = (map_tile + obj->tile) ^ TILE_Y_FLIP_AND; //tile
			uint16_t px = x + map_x;
			#if (SCREEN_WIDTH <= 320)
				if ((px &= 0x1FF) == 0)
					px++; //Prevent sprite from being x=0 (acts as a mask)
			#else
				if (px == 0)
					px++;
			#endif
			*(*sprite)++ = px; //x
		}
	}
	else
	{
		//No flip
		while(pieces-- > 0)
		{
			//Don't overflow the sprite buffer
			if (*sprite_i >= BUFFER_SPRITES)
				break;
			
			//Read mappings
			int8_t map_y = *mappings++;
			uint8_t map_size = *mappings++;
			uint16_t map_tile = (mappings[0] << 8) | (mappings[1] << 0);
			mappings += 2;
			int8_t map_x = *mappings++;
			
			//Write sprite
			*(*sprite)++ = y + map_y; //y
			*(*sprite)++ = (map_size << 8) | ++(*sprite_i); //size and link
			*(*sprite)++ = map_tile + obj->tile; //tile
			uint16_t px = x + map_x;
			#if (SCREEN_WIDTH <= 320)
				if ((px &= 0x1FF) == 0)
					px++; //Prevent sprite from being x=0 (acts as a mask)
			#else
				if (px == 0)
					px++;
			#endif
			*(*sprite)++ = px; //x
		}
	}
}

void BuildSprites()
{
	//Draw each sprite priority queue
	uint16_t *sprite = &sprite_buffer[0][0];
	uint8_t sprite_i = 0;
	struct SpriteQueue *queue = sprite_queue;
	
	for (int i = 0; i < 8; i++, queue++)
	{
		//Iterate through all queued objects
		for (int j = 0; queue->size != 0; j++, queue->size--)
		{
			Object *obj = queue->obj[j];
			if (obj->mappings == NULL) //This line isn't in the original, but without it, the title screen segfaults
				continue;              //Basically, the bug that causes the 'PRESS START BUTTON' text to not appear gives the object null mappings
			if (obj->type != ObjId_Null)
			{
				//Get object position on screen and check if visible
				obj->render.f.on_screen = false;
				
				uint16_t x, y;
				if (obj->render.f.align_bg || obj->render.f.align_fg)
				{
					//Get screen position to use
					static int16_t *bs_scrpos[4][2] = {
						{NULL, NULL},
						{&scrpos_x.f.u,     &scrpos_y.f.u},
						{&bg_scrpos_x.f.u,  &bg_scrpos_y.f.u},
						{&bg3_scrpos_x.f.u, &bg3_scrpos_y.f.u},
					};
					int16_t **scrpos = bs_scrpos[(obj->render.f.align_bg << 1) | obj->render.f.align_fg];
					
					//Get object X position
					int16_t ox = obj->pos.l.x.f.u - *scrpos[0];
					if ((ox + obj->width_pixels) < 0 || (ox - obj->width_pixels) >= SCREEN_WIDTH)
						continue;
					x = 128 + ox; //VDP sprites start at 128
					
					//Get object Y position
					if (obj->render.f.yrad_height)
					{
						int16_t oy = obj->pos.l.y.f.u - *scrpos[1];
						if ((oy + obj->y_rad) < 0 || (oy - obj->y_rad) >= SCREEN_HEIGHT)
							continue;
						y = 128 + oy; //VDP sprites start at 128
					}
					else
					{
						int16_t oy = obj->pos.l.y.f.u - *scrpos[1] + 0x80;
						if (oy < 0x60 || oy >= (0x180 + SCREEN_TALLADD))
							continue;
						y = oy;
					}
				}
				else
				{
					//Positions map directly to VDP coordinates
					x = obj->pos.s.x;
					y = obj->pos.s.y;
				}
				
				//Get object mappings to use
				const uint8_t *mappings;
				uint8_t pieces;
				
				if (!obj->render.f.raw_mappings)
				{
					//Index mapping by frame
					const uint8_t *mapping_ind = (const uint8_t*)obj->mappings + (obj->frame << 1);
					mappings = obj->mappings + ((mapping_ind[0] << 8) | (mapping_ind[1] << 0));
					pieces = *mappings++;
				}
				else
				{
					//Directly use object mappings pointer
					mappings = obj->mappings;
					pieces = 0;
				}
				
				//Draw object
				BuildSprites_Draw(&sprite, &sprite_i, x, y, obj, mappings, pieces);
				obj->render.f.on_screen = true;
			}
		}
	}
	
	//Terminate end of sprite list
	if (sprite_i >= BUFFER_SPRITES)
	{
		sprite[-2] &= 0xFF00; //Clear link byte
	}
	else
	{
		*sprite++ = 0;
		*sprite++ = 0;
	}
}

//Object functions
void AnimateSprite(Object *obj, const uint8_t *anim_script)
{
	//Check if animation changed
	uint8_t anim = obj->anim;
	if (anim != obj->prev_anim)
	{
		//Reset animation state
		obj->prev_anim = anim;
		obj->anim_frame = 0;
		obj->frame_time.b = 0;
	}
	
	//Wait for current animation frame to end
	if (--obj->frame_time.b >= 0)
		return;
	
	//Get animation script to use
	anim <<= 1;
	anim_script += (anim_script[anim] << 8) | (anim_script[anim + 1] << 0);
	obj->frame_time.b = anim_script[0];
	
	//Read current animation command
	uint8_t cmd = anim_script[1 + obj->anim_frame];
	
	if (!(cmd & 0x80))
	{
		Anim_Next:
		//Set animation frame
		obj->frame = cmd & 0x1F;
		obj->render.f.x_flip = obj->status.o.f.x_flip ^ ((cmd >> 5) & 1);
		obj->render.f.y_flip = obj->status.o.f.y_flip ^ ((cmd >> 6) & 1);
		obj->anim_frame++;
	}
	else
	{
		if (++cmd == 0) //0xFF
		{
			//Restart animation
			obj->anim_frame = 0;
			cmd = anim_script[1];
			goto Anim_Next;
		}
		if (++cmd == 0) //0xFE
		{
			//Go back (next byte) frames
			obj->anim_frame -= anim_script[2 + obj->anim_frame];
			cmd = anim_script[1 + obj->anim_frame];
			goto Anim_Next;
		}
		if (++cmd == 0) //0xFD
		{
			//Change animation
			obj->anim = anim_script[2 + obj->anim_frame];
		}
		if (++cmd == 0) //0xFC
		{
			//Increment routine
			obj->routine += 2;
		}
		if (++cmd == 0) //0xFB
		{
			//Clear secondary routine
			obj->routine_sec = 0;
		}
		if (++cmd == 0) //0xFA
		{
			//Increment secondary routine
			obj->routine_sec += 2;
		}
	}
}

void DisplaySprite(Object *obj)
{
	//Get queue to use
	struct SpriteQueue *queue = &sprite_queue[obj->priority & 7];
	
	//Push to queue
	if (queue->size >= (sizeof(queue->obj) / sizeof(Object*)))
		return;
	queue->obj[queue->size++] = obj;
}

void ObjectDelete(Object *obj)
{
	//Clear object memory
	memset(obj, 0, sizeof(Object));
	obj->mappings = NULL; //NULL isn't guaranteed to be 0
}

void SpeedToPos(Object *obj)
{
	obj->pos.l.x.v += obj->xsp << 8;
	obj->pos.l.y.v += obj->ysp << 8;
}

void ObjectFall(Object *obj)
{
	obj->pos.l.x.v += obj->xsp << 8;
	obj->pos.l.y.v += obj->ysp << 8;
	obj->ysp += 0x38;
}

void RememberState(Object *obj)
{
	if (IS_OFFSCREEN(obj->pos.l.x.f.u))
	{
		//Off-screen
		if (obj->respawn_index)
			objstate[obj->respawn_index] &= 0x7F;
		ObjectDelete(obj);
	}
	else
	{
		//On-screen
		DisplaySprite(obj);
	}
}

//Platform and solid objects
void MvSonicOnPtfm(Object *obj, int16_t y, int16_t prev_x)
{
	//Check if player can be moved
	if (lock_multi & 0x80)
		return;
	if (player->routine >= 6)
		return;
	if (debug_use)
		return;
	
	player->pos.l.y.f.u = y - player->y_rad;
	player->pos.l.x.f.u += obj->pos.l.x.f.u - prev_x;
}

void PlatformObject(Object *obj, uint16_t x_rad)
{
	//Check if player is colliding with platform
	if (player->ysp < 0)
		return;
	
	int16_t x_off = player->pos.l.x.f.u - obj->pos.l.x.f.u + x_rad;
	if (x_off < 0 || x_off >= (x_rad << 1))
		return;
	
	Platform3(obj, obj->pos.l.y.f.u - 8);
}

void Platform3(Object *obj, int16_t top)
{
	Scratch_Sonic *scratch = (Scratch_Sonic*)&player->scratch;
	
	//Check if player is touching the top of platform
	int16_t py = player->pos.l.y.f.u;
	int16_t by = py + player->y_rad + 4;
	if (top > by)
		return;
	top -= by;
	if (top < -16)
		return;
	
	//Check if player can collide with platform
	if ((lock_multi & 0x80) || player->routine >= 6)
		return;
	
	//Clip on top of platform
	player->pos.l.y.f.u = top + py + 3;
	
	//Modify platform state
	obj->routine += 2;
	
	if (player->status.p.f.object_stand)
	{
		//Release from last standing object
		Object *prv = objects + scratch->standing_obj;
		prv->status.o.f.player_stand = false;
		prv->routine_sec = 0;
		if (prv->routine == 4)
			prv->routine = 2;
	}
	
	//Modify player state
	scratch->standing_obj = obj - objects;
	player->angle = 0;
	player->ysp = 0;
	player->inertia = player->xsp;
	if (player->status.p.f.in_air)
		Sonic_ResetOnFloor(player);
	
	player->status.p.f.object_stand = true;
	obj->status.o.f.player_stand = true;
}

bool ExitPlatform(Object *obj, uint16_t x_rad, uint16_t x_dia2, int16_t *x_off_p)
{
	uint16_t x_dia = x_dia2 << 1;
	
	//Check if we've jumped off
	if (!player->status.p.f.in_air)
	{
		//Check if we've walked off
		int16_t x_off = player->pos.l.x.f.u - obj->pos.l.x.f.u + x_rad;
		if (x_off_p != NULL)
			*x_off_p = x_off;
		if (x_off >= 0 && x_off < x_dia)
			return false;
	}
	
	//Release player from platform
	player->status.p.f.object_stand = false;
	obj->routine = 2;
	obj->status.o.f.player_stand = false;
	return true;
}

static void Solid_ResetFloor(Object *obj, Object *pla)
{
	Scratch_Sonic *scratch = (Scratch_Sonic*)&player->scratch;
	
	//Release player from last standing object
	if (player->status.p.f.object_stand)
	{
		Object *prv = objects + scratch->standing_obj;
		prv->status.o.f.player_stand = false;
		prv->routine_sec = 0;
	}
	
	//Modify player state
	scratch->standing_obj = obj - objects;
	player->angle = 0;
	player->ysp = 0;
	player->inertia = player->xsp;
	if (player->status.p.f.in_air)
		Sonic_ResetOnFloor(pla);
	
	player->status.p.f.object_stand = true;
	obj->status.o.f.player_stand = true;
}

static int Solid_ChkEnter(Object *obj, uint16_t x_rad, uint16_t y_rad, int16_t *x_off, int16_t *y_off)
{
	//Check if player is in horizontal range
	*x_off = player->pos.l.x.f.u - obj->pos.l.x.f.u + x_rad;
	uint16_t x_dia = x_rad << 1;
	if (*x_off >= 0 && *x_off <= x_dia)
	{
		//Check if player is in vertical range
		y_rad += player->y_rad;
		*y_off = player->pos.l.y.f.u - obj->pos.l.y.f.u + 4 + y_rad;
		uint16_t y_dia = y_rad << 1;
		
		if (*y_off >= 0 && *y_off < y_dia)
		{
			//Check if player can collide with object
			if (!(lock_multi & 0x80))
			{
			#ifdef SCP_REV00
				if (player->routine >= 6)
				{
					if (debug_use)
						return 0;
			#else
				if (player->routine >= 6 || debug_use)
					return 0;
				{
			#endif
					//Get X clip
					uint16_t x_clip = *x_off;
					if (x_rad < *x_off)
					{
						*x_off -= x_dia;
						x_clip = -*x_off;
					}
					
					//Get Y clip
					uint16_t y_clip = *y_off;
					if (y_rad < *y_off)
					{
						*y_off -= (4 + y_dia);
						y_clip = -*y_off;
					}
					
					//Check if we're hitting the top/bottom or sides
					if (x_clip <= y_clip)
					{
						//Left/right
						if (y_clip > 4)
						{
							//Stop speed going towards object
							if (*x_off > 0)
							{
								if (player->xsp > 0)
								{
									player->xsp = 0;
									player->inertia = 0;
								}
							}
							else if (*x_off < 0)
							{
								if (player->xsp < 0)
								{
									player->xsp = 0;
									player->inertia = 0;
								}
							}
							
							//Clip and change push flags
							player->pos.l.x.f.u -= *x_off;
							if (!player->status.p.f.in_air)
							{
								//On ground, set push flags
								obj->status.o.f.player_push = true;
								player->status.p.f.pushing = true;
								return 1;
							}
						}
						
						//Mid-air or near edges, clear push flags
						obj->status.o.f.player_push = false;
						player->status.p.f.pushing = false;
						return 1;
					}
					else if (*y_off < 0)
					{
						//Bottom
						if (player->ysp != 0)
						{
							//Check if we should be clipped out the bottom
							if (player->ysp < 0 && *y_off < 0)
							{
								player->pos.l.y.f.u -= *y_off;
								player->ysp = 0;
							}
						}
						else if (!player->status.p.f.in_air)
						{
							//Squish Sonic
							KillSonic(player, obj);
						}
						return -1;
					}
					else
					{
						//Top
						//Check if we're going to land on the object
						if (*y_off < 16)
						{
							*y_off -= 4;
							
							//Check if we're within horizontal range and moving downwards
							uint16_t lx_rad = obj->width_pixels;
							uint16_t lx_dia = lx_rad << 1;
							int16_t lx_off = player->pos.l.x.f.u - obj->pos.l.x.f.u + lx_rad;
							if (lx_off >= 0 && lx_off < lx_dia && player->ysp >= 0)
							{
								//Land on object
								player->pos.l.y.f.u -= *y_off;
								Solid_ResetFloor(obj, player);
								obj->routine_sec = 2;
								obj->status.o.f.player_stand = true;
								return -1;
							}
							return 0;
						}
					}
				}
			}
		}
	}
	
	//Clear pushing state
	if (obj->status.o.f.player_push)
	{
		player->anim = SonAnimId_Run; //Not Walk
		obj->status.o.f.player_push = false;
		player->status.p.f.pushing = false;
	}
	return 0;
}

int SolidObject(Object *obj, uint16_t x_rad, uint16_t y_rad1, uint16_t y_rad2, int16_t prev_x, int16_t *x_off, int16_t *y_off)
{
	if (obj->routine_sec)
	{
		uint16_t x_dia = x_rad << 1;
		
		//Check if we've jumped off
		if (!player->status.p.f.in_air)
		{
			//Check if we've walked off
			int16_t x_off = player->pos.l.x.f.u - obj->pos.l.x.f.u + x_rad;
			if (x_off >= 0 && x_off <= x_dia)
			{
				//Move on platform
				MvSonicOnPtfm(obj, obj->pos.l.y.f.u - y_rad2, prev_x);
				return 0;
			}
		}
		
		//Release player from platform
		player->status.p.f.object_stand = false;
		obj->status.o.f.player_stand = false;
		obj->routine_sec = 0;
		return 0;
	}
	
	int16_t x_off_t = 0, y_off_t = 0;
	int res = Solid_ChkEnter(obj, x_rad, y_rad1, &x_off_t, &y_off_t);
	if (x_off != NULL)
		*x_off = x_off_t;
	if (y_off != NULL)
		*y_off = y_off_t;
	return res;
}
