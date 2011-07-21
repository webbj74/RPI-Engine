//////////////////////////////////////////////////////////////////////////////
//
/// constants.h - General Pre-Defined Constants
//
/// Shadows of Isildur RPI Engine++
/// Copyright (C) 2005-2006 C. W. McHenry
/// Authors: C. W. McHenry (traithe@middle-earth.us)
///          Jonathan W. Webb (sighentist@middle-earth.us)
/// URL: http://www.middle-earth.us
//
/// May includes portions derived from Harshlands
/// Authors: Charles Rand (Rassilon)
/// URL: http://www.harshlands.net
//
/// May include portions derived under license from DikuMUD Gamma (0.0)
/// which are Copyright (C) 1990, 1991 DIKU
/// Authors: Hans Henrik Staerfeldt (bombman@freja.diku.dk)
///          Tom Madson (noop@freja.diku.dk)
///          Katja Nyboe (katz@freja.diku.dk)
///          Michael Seifert (seifert@freja.diku.dk)
///          Sebastian Hammer (quinn@freja.diku.dk)
//
//////////////////////////////////////////////////////////////////////////////


#ifndef _rpie_constants_h_
#define _rpie_constants_h_

#define MYSQL_USERNAME		"shadows"
#define MYSQL_PASS		""
#define MYSQL_HOST		"localhost"

#define PRIMARY_DATABASE	"shadows"
#define PFILES_DATABASE		"shadows_pfiles"
#define LOG_DATABASE		"server_logs"
#define ENCRYPT_PASS		""

#define STAFF_EMAIL		"staff@middle-earth.us"

#define APP_EMAIL		"staff@middle-earth.us"
#define CODE_EMAIL		"staff@middle-earth.us"
#define PET_EMAIL		"staff@middle-earth.us"
#define REPORT_EMAIL		"staff@middle-earth.us"

#define IMPLEMENTOR_ACCOUNT	"God"
#define SERVER_LOCATION		"http://www.middle-earth.us"

#define MUD_NAME		"Shadows of Isildur"
#define MUD_EMAIL		"shadows@middle-earth.us"

/* Be sure to define without trailing slashes! */

#define PATH_TO_SENDMAIL	"/usr/sbin/sendmail"

/* Define top directory containing all three ports */

//#ifndef MACOSX
//#define PATH_TO_TOPDIR		"/home/shadows/shadows"
//#else
//#define PATH_TO_TOPDIR		"/Users/chad/Shadows\\ of\\ Isildur/"
//#endif

/* Define the individual port directories */

#define PATH_TO_PP		PATH_TO_TOPDIR "/pp"
#define PATH_TO_BP		PATH_TO_TOPDIR "/bp"
#define PATH_TO_TP		PATH_TO_TOPDIR "/tp"

/* Other miscellaneous filepath defines; absolute filepaths only! */

#define PATH_TO_WEBSITE		"/home/shadows/public_html"

#define B_BUF_SIZE				200000


/* Misc defines */

#define OOC_LOUNGE			121
#define MUSEUM_FOYER			120
#define AMPITHEATRE			70

#define JUNKYARD			75
#define LINKDEATH_HOLDING_ROOM		122

#define PREGAME_ROOM_PROTOTYPE		315
#define PREGAME_ROOM_NAME		"A Private Room in Club Endore"

#define OOC_TERMINAL_OBJECT		125

#define MINAS_TIRITH_START_LOC		371
#define MINAS_MORGUL_START_LOC		6120
#define OSGILIATH_START_LOC		371
#define EDEN_START_LOC			6120

#define LEANTO_OBJ_VNUM			95
#define POWER_GRID_FLUX			25
#define POWER_GRID_START_FLUX		25
#define HEALER_KIT_VNUM			900

#define LAST_PC_RACE			29
#define LAST_ROLE			9
#define MAX_SPECIAL_ROLES		50

#define CONSTITUTION_MULTIPLIER		3	/* Damage absorption limit for any */
						/* humanoid N/PC is 50 + con x multiplier */

#define COMBAT_BRUTALITY        1.85	/* A quick way to adjust the brutality of */
					/* combat; a setting of 175% seems pretty nasty, */
					/* brutish, and short. Be careful with this! */

/* color system */

#define BELL "\007"

/* main loop pulse control */

#define PULSES_PER_SEC		4

#define PULSE_ZONE		(PULSES_PER_SEC * 60)
#define PULSE_MOBILE		40
#define PULSE_STATS		(PULSES_PER_SEC * 60 * 5)
#define PULSE_AUTOSAVE		(PULSES_PER_SEC * 60)
#define PULSE_DELAY		4
#define PULSE_SMART_MOBS	(PULSES_PER_SEC * 1)
#define PULSE_MOB_SURVIVAL	(PULSES_PER_SEC * 1)

/* string stuff */

//#ifndef shroud
#define MAX_STRING_LENGTH	49152
//#else
//#define MAX_STRING_LENGTH	49152
//#endif

#define AVG_STRING_LENGTH	256	/* more useful string len */
#define ERR_STRING_LENGTH 512
#define MAX_NAME_LENGTH		15	/* player character name */

#define MAX_INPUT_LENGTH     8000
#define MAX_MESSAGES          60
#define MAX_ITEMS            153

#define MAX_TEXTS			 100


#define SECS_PER_REAL_MIN   60
#define SECS_PER_REAL_HOUR  (60*SECS_PER_REAL_MIN)
#define SECS_PER_REAL_DAY   (24*SECS_PER_REAL_HOUR)
#define SECS_PER_REAL_YEAR  (365*SECS_PER_REAL_DAY)

#define UPDATE_PULSE		(4 * 4)
#define SECOND_PULSE		(4)

#define MAX_CONNECTIONS		400

#define CURRENCY_TIRITH		0
#define CURRENCY_MORGUL		1
#define CURRENCY_EDEN		2



#endif // _rpie_constants_h_
