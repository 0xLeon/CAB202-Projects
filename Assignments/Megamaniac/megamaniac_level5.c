#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "game_types.h"
#include "game.h"
#include "game_object.h"
#include "megamaniac.h"
#include "megamaniac_go_data.h"
#include "megamaniac_go_types.h"
#include "megamaniac_game_objects.h"
#include "megamaniac_level5.h"

#define LEVEL5_LEVEL_NAME			"Level 5 - Aggressive"
#define LEVEL5_ENEMY_ROW_COUNT			3
#define LEVEL5_ENEMY_ROW_ODD_COUNT		3
#define LEVEL5_ENEMY_ROW_EVEN_COUNT		4
#define LEVEL5_ENEMY_VERTICAL_SPACING		2
#define LEVEL5_ENEMY_HORIZONTAL_SPACING		5

#define LEVEL5_ENEMY_START_IDIVIDUAL_DELAY_MIN		1000
#define LEVEL5_ENEMY_START_INIDIVIAL_DELAY_MAX		7000
#define LEVEL5_ENEMY_PARABOLIC_DISPLACEMENT_MIN		10
#define LEVEL5_ENEMY_PARABOLIC_DISPLACEMENT_MAX		30


//-------------------------------------------------------
// Level Creation Functions
//-------------------------------------------------------

// TODO: null checks for game objects
game_level_p megamaniac_create_level5(game_p megamaniac) {
	return NULL;
}
