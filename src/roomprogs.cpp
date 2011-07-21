/*------------------------------------------------------------------------\
|  roomprogs.c : Room Scripting Module                www.middle-earth.us | 
|  Copyright (C) 2004, Shadows of Isildur: Traithe                        |
|  Derived under license from DIKU GAMMA (0.0).                           |
\------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structs.h"
#include "net_link.h"
#include "protos.h"
#include "utils.h"
#include "utility.h"
#include "decl.h"
#include "group.h"
/* #include "dynamicvariable.h" */

/* script program commands */

#define RP_ATECHO 	0
#define RP_GIVE		1
#define RP_TAKE		2
#define RP_TRANS	3
#define RP_LOAD		4
#define RP_FORCE	5
#define RP_LINK		6
#define RP_EXIT		7
#define RP_UNLOCK	8
#define RP_ATLOOK	9
#define RP_VSTR		10
#define RP_OSTR		11
#define RP_UNLINK	12
#define RP_UNEXIT	13
#define RP_PUT		14
#define RP_GET		15
#define RP_LOCK		16
#define RP_GETCASH	17
#define RP_GIVECASH	18
#define RP_LOADMOB	19
#define RP_EXMOB	20
#define RP_IF		21
#define RP_FI		22
#define RP_ELSE		23
#define RP_RFTOG	24
#define RP_PAIN		25
#define RP_VBR		26	/* Force line break to char */
#define RP_TRANSMOB	27
#define RP_ATREAD	28
#define RP_HALT		29
#define RP_PURGE	30
#define RP_LOAD_CLONE   31
#define RP_LOADOBJ	32
#define RP_STAYPUT	33	/* flag a mob stayput */
#define RP_ZONE_ECHO	34	/* echo to a zone (indoor and/or outdoor) */
#define RP_ATWRITE	35	/* leave a board message */
#define RP_SYSTEM	36	/* submit a system message */
#define RP_CLAN_ECHO	37	/* send a message to clan members */
#define RP_TRANS_GROUP	38	/* send a message to clan members */
#define RP_SET		39	/* send a message to clan members */
#define RP_CRIMINALIZE  40      /* criminalize a person or room */
#define RP_STRIP        41      /* takes a person's equipment and puts it all neatly in a bag */
#define RP_CLAN         42      /* adds people to a clan at a certain rank */
#define RP_TAKEMONEY 43 /* Take money from a player's inventory TBA: Take money from room too */
#define RP_DELAY 44 /* Delayed command just like scommand */
#define RP_TEACH 45 /* Teach character skill */
// #define RP_SETVAR 44   /* Japheth's Variable additions */


void rxp (CHAR_DATA * ch, char *prg);
char *next_line (char *old);
// int doit (CHAR_DATA * ch, char *func, char *arg, DynamicVariableList *Variables);
int doit(CHAR_DATA *ch, char *func, char *arg);
void r_link (char *argument);
void r_unlink (char *argument);
void r_exit (char *argument);
void r_give (CHAR_DATA * ch, char *argument);
void r_put (char *argument);
void r_get (char *argument);
void r_take (CHAR_DATA * ch, char *argument);
void r_unexit (char *argument);
void r_atlook (CHAR_DATA * ch, char *argument);
void r_atecho (CHAR_DATA * ch, char *argument);
void r_atread (CHAR_DATA * ch, char *argument);
void r_loadmob (char *argument);
void r_exmob (char *argument);
void r_rftog (char *arg);
void r_force (CHAR_DATA * ch, char *argument);
void r_pain (CHAR_DATA * ch, char *argument);
void r_transmob (CHAR_DATA * ch, char *argument);
void r_painmess (CHAR_DATA * victim, int dam);
void r_purge (CHAR_DATA * ch, char *argument);
void r_load_clone (CHAR_DATA * ch, char *argument);
void r_load_obj (const CHAR_DATA *ch, char *argument);
void r_stayput (CHAR_DATA *ch, char *argument);
void r_zone_echo (CHAR_DATA *ch, char *argument);
void r_atwrite (CHAR_DATA *ch, char *argument);
void r_system (CHAR_DATA *ch, char *argument);
void r_clan_echo (CHAR_DATA *ch, char *argument);
void r_trans_group (CHAR_DATA *ch, char *argument);
void r_set (CHAR_DATA *ch, char *argument);
void r_criminalize (CHAR_DATA *ch, char *argument);
void r_strip (CHAR_DATA *ch, char *argument);
void r_clan (CHAR_DATA *ch, char *argument);
void r_takemoney(CHAR_DATA *ch, char *argument);
void r_delay (CHAR_DATA *ch, char *argument);
void r_teach (CHAR_DATA *ch, char *argument);
// void r_setvar (char *argument);


#define MAX_RPRG_NEST 20
bool ifin[MAX_RPRG_NEST];
int nNest = 1;
int random_number = 0;


const char *rfuncs[] = {
  "atecho",
  "give",
  "take",
  "trans",
  "load",
  "force",
  "link",
  "exit",
  "unlock",
  "atlook",
  "vstr",
  "ostr",
  "unlink",
  "unexit",
  "put",
  "get",
  "lock",
  "getcash",
  "givecash",
  "loadmob",
  "exmob",
  "if",
  "fi",
  "else",
  "rftog",
  "pain",
  "vbr",
  "transmob",
  "atread",
  "halt",
  "purge",
  "load_clone",
  "loadobj",
  "stayput",
  "zone_echo",
  "atwrite",
  "system",
  "clan_echo",
  "trans_group",
  "set",
  "criminalize",
  "strip",
  "clan",
  "takemoney",
  "delay",
  "teach",
//  "setvar",
  "\n"
};


int
r_isname (char *str, char *namelist)
{
  char *curname;
  char *curstr;

  if (!str)
    return 0;

  if (!namelist)
    return 0;

  curname = namelist;
  for (;;)
    {
      for (curstr = str;; curstr++, curname++)
	{
	  if ((!*curstr && !isalpha (*curname)) || !str_cmp (curstr, curname))
	    return (1);

	  if (!*curname)
	    return (0);

	  if (!*curstr || *curname == ' ')
	    break;

	  if (tolower (*curstr) != tolower (*curname))
	    break;
	}

      /* skip to next name */

      for (; isalpha (*curname); curname++);
      if (!*curname)
	return (0);
      curname++;		/* first char of new name */
    }
}

// called by command_interpreter if there is a 
// program in the user's room.
int
r_program (CHAR_DATA * ch, char *argument)
{
  char cmd[MAX_STRING_LENGTH];
  char arg[MAX_STRING_LENGTH];
  half_chop (argument, cmd, arg);
  
  for (struct room_prog *p = ch->room->prg; p; p = p->next)
    {
      if (!p->prog || !*p->prog)
	continue;
      
      if (r_isname (cmd, p->command))
	{
	  if (!arg || isname (arg, p->keys))
	    {
	      rxp (ch, p->prog);
	      return (1);
	    }
	}
    }

  return (0);
}

void
rxp (CHAR_DATA * ch, char *prg)
{
  char func[512], *arg;
  int i, j, n;
  char *tprog, *line;
  char tmpf[MAX_STRING_LENGTH];
 // DynamicVariableList Variables;

  for (i = 0; i < MAX_RPRG_NEST; i++)
    {
      ifin[i] = 0;
    }
  nNest = 1;

  strcpy (tmpf, prg);
  tprog = tmpf;
  line = strtok (tprog, "\n");
  do
    {
      if (*line)
	{

		/* Japheth's Room Program Variables */
/*		std::string strLine(line);
		while (strLine.find("&&") != std::string::npos)
	  {
		  std::size_type first_index = strLine.find_first_of("&&");
		  std::size_type index = first_index + 2;
		  std::string TempString;

		  while (strLine[index] != ' ')
		  {
			  TempString.push_back(strLine[index]);
			  index++;
		  }

		  if (Variables.GetNamedVariable(TempString) != NULL)
		  {
			  strLine.replace(first_index, (index-first_index), (Variables.GetNamedVariable(TempString))->GetVariableContents());
		  }
		  else
		  {
			  strLine.erase(first_index, 2);
		  }
		  
	  }

		const_to_non_const_cstr(strLine.c_str(), line);
*/		  /* End of Japheth's Room Program Variables */

	  *func = '\0';
	  arg = strpbrk (line, " ");
	  if (line == NULL || arg == NULL)
	    return;
	  i = strlen (line);
	  j = strlen (arg);
	  strncat (func, line, i - j);
	  for (; isspace (*arg); arg++)
	    ;
	  n = strlen (arg);
	  for (; isspace (*(arg + n)); n--);
	  /* *(arg + (n - 1)) = '\0'; */
	  *(arg + n) = '\0';
//	  if (!doit (ch, func, arg, &Variables))
	  if (!doit (ch, func, arg))
	    {
	      break;
	    }
	}
      line = strtok (NULL, "\n");
    }
  while (line && *line);	/* KILLER CDR: line NULL when done */
}

/* change the %'s to $'s and remove the trailing \n */
void
ref (char *str, char *return_string)
{
  *return_string = '\0';

  for (; *str; str++, return_string++)
    {
      if (*str == '%')
	*return_string = '$';
      else
	*return_string = *str;
    }

  *return_string = '\0';
}


bool
oexist (int nVirtual, OBJ_DATA * ptrContents, bool bNest)
{
  OBJ_DATA *obj = NULL;

  for (obj = ptrContents; obj; obj = obj->next_content)
    {
      if (obj->nVirtual == nVirtual)
	{
	  return true;
	}
      if (bNest && obj->contains && oexist (nVirtual, obj->contains, bNest))
	{
	  return true;
	}
    }
  return false;
}


/* Handles the if statement -- returns true if the if is false */
/* Yeah I know, it's backwards.....I'll change it when I have time */

void
reval (CHAR_DATA * ch, char *arg)
{
  int i, dsiz, dir, tsiz, nFlag = 0, nStat = 0;
  int nArg1 = 0, nArg2 = 0, nArg3 = 0;
  long virt = 0, who;
  char tmp[80], tmp2[80], *dbuf, rbuf[80], sarg[80];
  CHAR_DATA *tmp_ch, *tch1, *tch2;
  OBJ_DATA *obj;
  bool check;
  ROOM_DATA *troom = NULL;

  *rbuf = '\0';
  strcpy (sarg, arg);
  while (*arg != '(')
    arg++;
  arg++;
  i = 0;
  while (*arg != ')')
    {
      tmp[i] = *arg++;
      i++;
    }
  tmp[i++] = '\0';
  tsiz = strlen (tmp);
  strcpy (tmp2, tmp);
  if ((dbuf = strchr (tmp, ',')))
    {
      dsiz = strlen (dbuf);
      dbuf++;
      for (; isspace (*dbuf); dbuf++);
      strncat (rbuf, tmp2, (tsiz - dsiz));
    }

  // Check to see if mudhour compares with specified logic
  // Syntax: if (hour=x)
  //         if (hour>x)
  //         if (hour<x)
  //	     if (hour!x)
  //	     if (hour#x) - note, this is hour mod x, but % char not allowed in mud input.
  if (!strncmp(sarg, "(hour", 5))
  {
	  int iTest = strtol(sarg+6, 0, 0);
	  bool pass = false;
	  switch (sarg[5])
	  {
	  case '=':
		  pass = (iTest == time_info.hour);
		  break;
	  case '>':
		  pass = (iTest < time_info.hour);
		  break;
	  case '<':
		  pass = (iTest > time_info.hour);
		  break;
	  case '!':
		  pass = (iTest != time_info.hour);
		  break;
	  case '#':
		  pass = !(time_info.hour % iTest);
		  break;
	  }
	  if (!pass)
	  {
		  ifin[nNest] = 1;
	  }
	  return;
  }
  if (!strncmp(sarg, "(day", 4))
  {
	  int iTest = strtol(sarg+5, 0, 0);
	  bool pass = false;
	  switch (sarg[4])
	  {
	  case '=':
		  pass = (iTest == (time_info.day+1));
		  break;
	  case '>':
		  pass = (iTest < (time_info.day +1));
		  break;
	  case '<':
		  pass = (iTest > (time_info.day + 1));
		  break;
	  case '!':
		  pass = (iTest != (time_info.day+1));
		  break;
	  case '#':
		  pass = !((time_info.day+1) % iTest);
		  break;
	  }
	  if (!pass)
	  {
		  ifin[nNest] = 1;
	  }
	  return;
  }
  // Note - first month is Midwinter
  if (!strncmp(sarg, "(month", 6))
  {
	  int iTest = strtol(sarg+7, 0, 0);
	  bool pass = false;
	  switch (sarg[6])
	  {
	  case '=':
		  pass = (iTest == (time_info.month+1));
		  break;
	  case '>':
		  pass = (iTest < (time_info.month+1));
		  break;
	  case '<':
		  pass = (iTest > (time_info.month+1));
		  break;
	  case '!':
		  pass = (iTest != (time_info.month+1));
		  break;
	  case '#':
		  pass = !((time_info.month+1) % iTest);
		  break;
	  }
	  if (!pass)
	  {
		  ifin[nNest] = 1;
	  }
	  return;
  }
  if (!strncmp(sarg, "(year", 5))
  {
	  int iTest = strtol(sarg+6, 0, 0);
	  bool pass = false;
	  switch (sarg[5])
	  {
	  case '=':
		  pass = (iTest == time_info.year);
		  break;
	  case '>':
		  pass = (iTest < time_info.year);
		  break;
	  case '<':
		  pass = (iTest > time_info.year);
		  break;
	  case '!':
		  pass = (iTest != time_info.year);
		  break;
	  case '#':
		  pass = !(time_info.year % iTest);
		  break;
	  }
	  if (!pass)
	  {
		  ifin[nNest] = 1;
	  }
	  return;
  }
  // First season (1) is Spring
  if (!strncmp(sarg, "(season", 7))
  {
	  int iTest = strtol(sarg+8, 0, 0);
	  bool pass = false;
	  switch (sarg[7])
	  {
	  case '=':
		  pass = (iTest == (time_info.season+1));
		  break;
	  case '>':
		  pass = (iTest < (time_info.season+1));
		  break;
	  case '<':
		  pass = (iTest > (time_info.season+1));
		  break;
	  case '!':
		  pass = (iTest != (time_info.hour+1));
		  break;
	  }
	  if (!pass)
	  {
		  ifin[nNest] = 1;
	  }
	  return;
  }

  /* Check to see if you can take specified money from character */
  /* Usage: if can_take_money(amount, currency) */
  if (!strncmp (sarg, "can_take_money", 14))
  {
	  int currency = 0;
	  if (!strncmp (dbuf, "gondorian", 9))
	  {
		  currency = 0;
	  }
	  else if (!strncmp (dbuf, "orkish", 6) || !strncmp(dbuf, "yrkish", 6) || !strncmp(dbuf, "orcish", 6))
	  {
		  currency = 1;
	  }
	  else if (!strncmp (dbuf, "numenorean", 10))
	  {
		  currency = 2;
	  }
	  else
	  {
		  ifin[nNest] = 1;
		  return;
	  }

	  if (!can_subtract_money(ch, atoi(rbuf), currency))
	  {
		  ifin[nNest] = 1;
	  }
  }
/* Check to see if a mob exists in a given room */
/* Usage: if mexist(mobvnum,roomvnum)           */

  if (!strncmp (sarg, "mexist", 6))
    {
      virt = atol (dbuf);
      who = atol (rbuf);
      for (tmp_ch = vtor (virt)->people; tmp_ch;
	   tmp_ch = tmp_ch->next_in_room)
	{
	  if (IS_NPC (tmp_ch) && tmp_ch->mob->nVirtual == who)
	    break;
	}
      if (!tmp_ch)
	{
	  ifin[nNest] = 1;
	  return;
	}
    }

/* Check to see if a obj exists in a given room */
/* Usage: if oexist(objvnum,roomvnum)           */

  if (!strncmp (sarg, "oexist_nested", 13))
    {
      virt = atol (dbuf);
      who = atol (rbuf);
      obj = NULL;

      if (!oexist (who, vtor (virt)->contents, true))
	{
	  ifin[nNest] = 1;
	  return;
	}
      return;
    }

/* Check to see if a obj exists in a given room */
/* Usage: if oexist(objvnum,roomvnum)           */

  if (!strncmp (sarg, "oexist", 6))
    {
      virt = atol (dbuf);
      who = atol (rbuf);
      obj = NULL;

      if (!oexist (who, vtor (virt)->contents, false))
	{
	  ifin[nNest] = 1;
	  return;
	}
      return;
    }

/* Check to see if a flag is set on a given room */
/* Usage: if flag(room-flag,roomvnum)            */

  if (!strncmp (sarg, "flag", 4) || !strncmp (sarg, "rflag", 5))
    {

      if ((nFlag = index_lookup (room_bits, rbuf)) == -1)
	{
	  send_to_char ("Error: if flag: No such room-flag.\n", ch);
	  ifin[nNest] = 1;
	  return;
	}
      virt = strtol (dbuf, NULL, 10);
      if (!(troom = vtor (virt)))
	{
	  send_to_char ("Error: if flag: No such room.\n", ch);
	  ifin[nNest] = 1;
	  return;
	}
      if (!IS_SET (troom->room_flags, (1 << nFlag)))
	{
	  ifin[nNest] = 1;
	  return;
	}
      return;
    }

  /* test against a random number */
  if (!strncmp (sarg, "(random", 7))
    {
      int test_number = strtol (sarg+8,0,0);
      bool pass = false;
      switch (sarg[7])
	{
	case '=':
	  pass = (random_number == test_number);
	  break;

	case '<':
	  pass = (random_number < test_number);
	  break;

	case '>':
	  pass = (random_number > test_number);
	  break;

	case '!':
	  pass = (random_number != test_number);
	  break;
	}
      if (!pass)
	{
	  ifin[nNest] = 1;
	}
      return;
    }

/* Checks if the initiator is a NPC. If so, returns true. No arguments */

 if (!strncmp (sarg, "npc", 3))
   {
    if(!IS_NPC(ch))
     {
        ifin[nNest] = 1;
        return;
     }
    return;
   }

/* Checks to see if initiator is wanted in a certain zone */
/* usage: if wanted(zone, time)                           */
/* will return true if wanted time is equal to or greater than "time". Current zone is -1. */

 if (!strncmp (sarg, "wanted", 6))
   { 
      int zone = atol(rbuf);
      int test = atol(dbuf);

      if(zone == -1)
         zone = ch->room->zone;
      
      if (!get_affect (ch, MAGIC_CRIM_BASE + zone))
      {
        ifin[nNest] = 1;
        return;
      }
      else

      if (!((get_affect(ch, MAGIC_CRIM_BASE + zone)->a.spell.duration) >= test))
      {
        ifin[nNest] = 1;
        return;
      }
      return;
    }

/* Check to see if mob/player has clanning (use shortname) */
/* Usage: if clan(mobvnum,clanname)                        */
/* Only checks in current room. To denote player use -1   */

  if (!strncmp (sarg, "clan", 4))
    {
      who = strtol (rbuf, NULL, 10);
      if (who == -1)
	{
	  tmp_ch = ch;
	}
      else
	{
	  for (tmp_ch = vtor (ch->in_room)->people; tmp_ch;
	       tmp_ch = tmp_ch->next_in_room)
	    {
	      if (tmp_ch->mob && tmp_ch->mob->nVirtual == who)
		break;
	    }
	  if (!tmp_ch)
	    {
	      ifin[nNest] = 1;
	      return;
	    }
	}
      if (!get_clan (tmp_ch, dbuf, &nFlag))
	{
	  ifin[nNest] = 1;
	  return;
	}
      return;
    }
/* Check to see if mob/player has race */
/* Usage: if race(mobvnum,racename)                        */
/* Only checks in current room. To denote player use -1   */

  if (!strncmp (sarg, "race", 4))
    {
      who = strtol (rbuf, NULL, 10);
      if (who == -1)
	{
	  tmp_ch = ch;
	}
      else
	{
	  for (tmp_ch = vtor (ch->in_room)->people; tmp_ch;
	       tmp_ch = tmp_ch->next_in_room)
	    {
	      if (tmp_ch->mob && tmp_ch->mob->nVirtual == who)
		break;
	    }
	  if (!tmp_ch)
	    {
	      ifin[nNest] = 1;
	      return;
	    }
	}
      if (tmp_ch->race != lookup_race_id (dbuf))
	{
	  ifin[nNest] = 1;
	  return;
	}
      return;
    }

/* Check to see if player meets minimum ability score */
/* Usage: if stat(ability,minimum)                    */

  if (!strncmp (sarg, "stat", 4))
    {
      nStat = strtol (dbuf, NULL, 10);
      if (!str_cmp (rbuf, "str") && GET_STR (ch) >= nStat)
	{
	  return;
	}
      else if (!str_cmp (rbuf, "dex") && GET_DEX (ch) >= nStat)
	{
	  return;
	}
      else if (!str_cmp (rbuf, "con") && GET_CON (ch) >= nStat)
	{
	  return;
	}
      else if (!str_cmp (rbuf, "int") && GET_INT (ch) >= nStat)
	{
	  return;
	}
      else if (!str_cmp (rbuf, "wil") && GET_WIL (ch) >= nStat)
	{
	  return;
	}
      else if (!str_cmp (rbuf, "aur") && GET_AUR (ch) >= nStat)
	{
	  return;
	}
      else if (!str_cmp (rbuf, "agi") && GET_AGI (ch) >= nStat)
	{
	  return;
	}
      ifin[nNest] = 1;
      return;
    }

/* Check to see if player meets minimum skill score */
/* Usage: if skillcheck(skillname,XdY)                  */

  if (!strncmp (sarg, "skillcheck", 10))
    {
      nArg2 = strtol (dbuf, NULL, 10);
      dbuf = strchr (dbuf, 'd');
      if (dbuf && dbuf++)
	{
	  nArg3 = strtol (dbuf, NULL, 10);

	  if (((nArg1 = index_lookup (skills, rbuf)) != -1)
	      && (skill_level (ch, nArg1, 0) >= dice (nArg2, nArg3)))
	    {
	      return;
	    }
	}
      ifin[nNest] = 1;
      return;
    }



/* Check to see if player meets minimum skill score */
/* Usage: if skill(skillname,minimum)                  */

  if (!strncmp (sarg, "skill", 5))
    {
      nArg2 = strtol (dbuf, NULL, 10);
      if (((nArg1 = index_lookup (skills, rbuf)) != -1)
	  && (skill_level (ch, nArg1, 0) >= nArg2))
	{
	  return;
	}
      ifin[nNest] = 1;
      return;
    }

/* Check to see if mob/player has object (also checks eq) */
/* Usage: if haso(mobvnum,objvnum)                        */
/* Only checks in current room. To denote player use -1   */

  if (!strncmp (sarg, "haso_nested", 11))
    {
      who = atol (rbuf);
      if (who == -1)
	{
	  tmp_ch = ch;
	}
      else
	{
	  for (tmp_ch = vtor (ch->in_room)->people; tmp_ch;
	       tmp_ch = tmp_ch->next_in_room)
	    {
	      if (tmp_ch->mob && tmp_ch->mob->nVirtual == who)
		break;
	    }
	  if (!tmp_ch)
	    {
	      ifin[nNest] = 1;
	      return;
	    }
	}

      obj = get_obj_in_list_num (atol (dbuf), tmp_ch->right_hand);
      if (!obj)
	obj = get_obj_in_list_num (atol (dbuf), tmp_ch->left_hand);
      if (!obj)
	{
	  if ((check = get_obj_in_equip_num (tmp_ch, atol (dbuf)) == false))
	    {
	      ifin[nNest] = 1;
	      return;
	    }
	}
    }

  if (!strncmp (sarg, "haso", 4))
    {
      who = atol (rbuf);
      if (who == -1)
	{
	  tmp_ch = ch;
	}
      else
	{
	  for (tmp_ch = vtor (ch->in_room)->people; tmp_ch;
	       tmp_ch = tmp_ch->next_in_room)
	    {
	      if (tmp_ch->mob && tmp_ch->mob->nVirtual == who)
		break;
	    }
	  if (!tmp_ch)
	    {
	      ifin[nNest] = 1;
	      return;
	    }
	}

      obj = get_obj_in_list_num (atol (dbuf), tmp_ch->right_hand);
      if (!obj)
	obj = get_obj_in_list_num (atol (dbuf), tmp_ch->left_hand);
      if (!obj)
	{
	  if ((check = get_obj_in_equip_num (tmp_ch, atol (dbuf)) == false))
	    {
	      ifin[nNest] = 1;
	      return;
	    }
	}
    }

/* Check to see if mob/player can see mob/player */
/* Usage: if cansee(seer,seen)                        */
/* Only checks in current room. To denote player use -1   */
/* General check for vision: if cansee(-1,-1) */

  if (!strncmp (sarg, "cansee", 6))
    {
      who = atol (rbuf);
      if (who == -1)
	{
	  tch1 = ch;
	}
      else
	{
	  for (tch1 = vtor (ch->in_room)->people; tch1;
	       tch1 = tch1->next_in_room)
	    {
	      if (tch1->mob && tch1->mob->nVirtual == who)
		break;
	    }
	  if (!tch1)
	    {
	      ifin[nNest] = 1;
	      return;
	    }
	}
      who = atol (dbuf);
      if (who == -1)
	{
	  tch2 = ch;
	}
      else
	{
	  for (tch2 = vtor (ch->in_room)->people; tch2;
	       tch2 = tch2->next_in_room)
	    {
	      if (tch2->mob && tch2->mob->nVirtual == who)
		break;
	    }
	  if (!tch2)
	    {
	      ifin[nNest] = 1;
	      return;
	    }
	}
      if (!CAN_SEE (tch1, tch2))
	{
	  ifin[nNest] = 1;
	  return;
	}
      return;
    }



/* Checks to see if a link exist in a given room and direction */
/* Usage: if link(roomvnum, dir)                               */

  if (!strncmp (sarg, "link", 4))
    {
      virt = atol (rbuf);
      switch (*dbuf)
	{
	case 'n':
	  dir = 0;
	  break;
	case 'e':
	  dir = 1;
	  break;
	case 's':
	  dir = 2;
	  break;
	case 'w':
	  dir = 3;
	  break;
	case 'u':
	  dir = 4;
	  break;
	case 'd':
	  dir = 5;
	  break;
	default:
	  system_log ("Unknown direction in reval::link", true);
	  ifin[nNest] = 1;
	  return;
	}

      if (!(troom = vtor (virt)))
	{
	  system_log ("ERROR: tar room not found in reval::link", true);
	  ifin[nNest] = 1;
	  return;
	}

      if (!troom->dir_option[dir])
	{
	  ifin[nNest] = 1;
	  return;
	}

    }
}

int
//doit (CHAR_DATA * ch, char *func, char *arg, DynamicVariableList *Variables)
doit (CHAR_DATA *ch, char *func, char *arg)
{
  int i;
  char tmp[MAX_STRING_LENGTH];

  for (i = 0; (*rfuncs[i] != '\n'); i++)
    if (!strcmp (rfuncs[i], func))
      break;

  switch (i)
    {
    case RP_ATECHO:
      if (!ifin[nNest])
	r_atecho (ch, arg);
      return 1;
    case RP_LOADOBJ:
	  if (!ifin[nNest])
	     r_load_obj (ch, arg);
      return 1;
	case RP_TAKEMONEY:
		if (!ifin[nNest])
			r_takemoney(ch, arg);
		return 1;
    case RP_EXIT:
      if (!ifin[nNest])
	r_exit (arg);
      return 1;
    case RP_LINK:
      if (!ifin[nNest])
	r_link (arg);
      return 1;
    case RP_ATLOOK:
      if (!ifin[nNest])
	r_atlook (ch, arg);
      return 1;
    case RP_TRANS:
      if (!ifin[nNest])
	{
	  if (!vtor (atoi (arg)))
	    return 1;
	  if (ch->mount)
	    {
	      char_from_room (ch->mount);
	      char_to_room (ch->mount, vtor (atoi (arg))->nVirtual);
	    }
	  char_from_room (ch);
	  char_to_room (ch, vtor (atoi (arg))->nVirtual);
	}
      return 1;
    case RP_TRANSMOB:
      if (!ifin[nNest])
	{
	  r_transmob (ch, arg);
	}
      return 1;
    case RP_VSTR:
      if (!ifin[nNest])
	{
	  ref (arg, tmp);
	  act (tmp, false, ch, 0, 0, TO_CHAR | _ACT_FORMAT);
	}
      return 1;
    case RP_VBR:
      if (!ifin[nNest])
	send_to_char ("\n", ch);
      return 1;
    case RP_OSTR:
      if (!ifin[nNest])
	{
	  ref (arg, tmp);
	  act (tmp, false, ch, 0, 0, TO_ROOM | _ACT_FORMAT);
	}
      return 1;
    case RP_TEACH:
      if (!ifin[nNest])
        r_teach (ch, arg);
      return 1;
    case RP_UNLINK:
      if (!ifin[nNest])
	r_unlink (arg);
      return 1;
    case RP_UNEXIT:
      if (!ifin[nNest])
	r_unexit (arg);
      return 1;
    case RP_GIVE:
      if (!ifin[nNest])
	r_give (ch, arg);
      return 1;
    case RP_TAKE:
      if (!ifin[nNest])
	r_take (ch, arg);
      return 1;
    case RP_PUT:
      if (!ifin[nNest])
	r_put (arg);
      return 1;
    case RP_GET:
      if (!ifin[nNest])
	r_get (arg);
      return 1;
    case RP_GETCASH:
    case RP_GIVECASH:
      return 1;
    case RP_STAYPUT:
      if (!ifin[nNest])
	r_stayput (ch, arg);
      return 1;
    case RP_ZONE_ECHO:
      if (!ifin[nNest])
	r_zone_echo (ch, arg);
      return 1;
    case RP_ATWRITE:
      if (!ifin[nNest])
	r_atwrite (ch, arg);
      return 1;
    case RP_SYSTEM:
      if (!ifin[nNest])
	r_system (ch, arg);
      return 1;
    case RP_CLAN_ECHO:
      if (!ifin[nNest])
	r_clan_echo (ch, arg);
      return 1;
    case RP_TRANS_GROUP:
      if (!ifin[nNest])
	r_trans_group (ch, arg);
      return 1;
    case RP_SET:
      if (!ifin[nNest])
	r_set (ch, arg);
      return 1;
    case RP_LOADMOB:
      if (!ifin[nNest])
	r_loadmob (arg);
      return 1;
    case RP_LOAD_CLONE:
      if (!ifin[nNest])
	r_load_clone (ch, arg);
      return 1;
    case RP_EXMOB:
      if (!ifin[nNest])
	r_exmob (arg);
      return 1;
    case RP_FORCE:
      if (!ifin[nNest])
	r_force (ch, arg);
      return 1;
    case RP_IF:
      if (!ifin[nNest])
	reval (ch, arg);
      ifin[nNest + 1] = ifin[nNest];
      nNest++;
      return 1;
    case RP_FI:
      --nNest;
      ifin[nNest] = ifin[nNest - 1];
      return 1;
    case RP_ELSE:
      nNest--;
      if (!ifin[nNest - 1])
	{
	  ifin[nNest] = !ifin[nNest];
	}
      ifin[nNest + 1] = ifin[nNest];
      nNest++;
      return 1;
    case RP_RFTOG:
      if (!ifin[nNest])
	r_rftog (arg);
      return 1;
    case RP_PAIN:
      if (!ifin[nNest])
	r_pain (ch, arg);
      return 1;
    case RP_ATREAD:
      if (!ifin[nNest])
	r_atread (ch, arg);
      return 1;
    case RP_PURGE:
      if (!ifin[nNest])
	r_purge (ch, arg);
      return 1;
    case RP_CRIMINALIZE:
      if (!ifin[nNest])
        r_criminalize (ch, arg);
       return 1;
    case RP_STRIP:
      if (!ifin[nNest])
        r_strip (ch, arg);
       return 1;
    case RP_CLAN:
      if (!ifin[nNest])
        r_clan (ch, arg);
       return 1;
    case RP_DELAY:
      if (!ifin[nNest])
       r_delay (ch, arg);
      return 1;
    case RP_HALT:
      if (!ifin[nNest])
	return 0;
      else
	return 1;
/*	case RP_SETVAR:
	  if (!ifin[nNest])
        r_setvar(arg, Variables);
	  return 1;*/
    default:
      system_log ("ERROR: unknown command in program", true);
      return 0;
    }
}

void
do_rpadd (CHAR_DATA * ch, char *argument, int cmd)
{
  struct room_prog *t, *old, *tmp;

  CREATE (t, struct room_prog, 1);

  t->next = NULL;
  t->command = NULL;
  t->keys = NULL;
  t->prog = NULL;

  if (!vtor (ch->in_room)->prg)
    {
      vtor (ch->in_room)->prg = t;
    }
  else
    {
      old = vtor (ch->in_room)->prg;
      tmp = old;
      while (tmp)
	{
	  old = tmp;
	  tmp = tmp->next;
	}
      old->next = t;
    }
  send_to_char ("New program initialized.\n\r", ch);
}

void
do_rpdel (CHAR_DATA * ch, char *argument, int cmd)
{
  int i = 1, j;
  struct room_prog *p = 0, *tmp;

  j = atoi (argument);

  for (tmp = vtor (ch->in_room)->prg; tmp; tmp = tmp->next)
    {
      if (i == j)
	{
	  if (!p)
	    vtor (ch->in_room)->prg = tmp->next;
	  else
	    p->next = tmp->next;
	  mem_free (tmp);
	  send_to_char ("Done.\n\r", ch);
	  return;
	}
      p = tmp;
      i++;
    }
  send_to_char
    ("No such program, can you count past ten with your shoes on?\n\r", ch);
  return;
}

void
do_rpcmd (CHAR_DATA * ch, char *argument, int cmd)
{
  char arg1[80], arg2[80];
  int i, j;
  struct room_prog *t;

  half_chop (argument, arg1, arg2);

  if (!isdigit (arg1[0]))
    {
      send_to_char ("Specify a program number to edit.\r\n", ch);
      return;
    }
  if (!arg2[0])
    {
      send_to_char ("Specify command(s) to install.\n\r", ch);
      return;
    }
  i = atoi (arg1);
  for (j = 1, t = vtor (ch->in_room)->prg; t; j++, t = t->next)
    {
      if (i == j)
	{
	  t->command = add_hash (arg2);
	  send_to_char ("Command installed.\n\r", ch);
	  return;
	}
    }
  send_to_char ("That program does not exist.\n\r", ch);
  return;
}

void
do_rpkey (CHAR_DATA * ch, char *argument, int cmd)
{
  char arg1[80], arg2[80];
  int i, j;
  struct room_prog *t;

  half_chop (argument, arg1, arg2);

  if (!isdigit (arg1[0]))
    {
      send_to_char ("Specify a program number to edit.\r\n", ch);
      return;
    }
  i = atoi (arg1);
  for (j = 1, t = vtor (ch->in_room)->prg; t; j++, t = t->next)
    {
      if (i == j)
	{
	  t->keys = add_hash (arg2);
	  send_to_char ("Keywords installed.\n\r", ch);
	  return;
	}
    }
  send_to_char ("That program does not exist.\n\r", ch);
  return;
}

void
do_rpprg (CHAR_DATA * ch, char *argument, int cmd)
{
  char arg1[80];
  int i, j;
  struct room_prog *t;

  one_argument (argument, arg1);

  if (!isdigit (arg1[0]))
    {
      send_to_char ("Specify a program number to edit.\r\n", ch);
      return;
    }
  i = atoi (arg1);
  for (j = 1, t = vtor (ch->in_room)->prg; t; j++, t = t->next)
    {
      if (i == j)
	{
	  make_quiet (ch);
	  send_to_char ("Enter program now, Terminate entry with an '@'\n\r",
			ch);
	  ch->desc->str = &t->prog;
	  t->prog = 0;
	  ch->desc->max_str = MAX_STRING_LENGTH;
	  return;
	}
    }
  send_to_char ("That program does not exist.\n\r", ch);
  return;
}

void
do_rpapp (CHAR_DATA * ch, char *argument, int cmd)
{
  char arg1[80];
  int i, j;
  struct room_prog *t;

  one_argument (argument, arg1);

  if (!isdigit (arg1[0]))
    {
      send_to_char ("Specify a program number to edit.\r\n", ch);
      return;
    }
  i = atoi (arg1);
  for (j = 1, t = vtor (ch->in_room)->prg; t; j++, t = t->next)
    {
      if (i == j)
	{
	  make_quiet (ch);
	  send_to_char
	    ("Append to program now, Terminate entry with an '@'\n\r", ch);
	  ch->desc->str = &t->prog;
	  ch->desc->max_str = 2000;
	  return;
	}
    }
  send_to_char ("That program does not exist.\n\r", ch);
  return;
}

void
do_rpstat (CHAR_DATA * ch, char *argument, int cmd)
{
  char buf[MAX_STRING_LENGTH];
  int i = 1;
  struct room_prog *r;

  *buf = '\0';

  if (!vtor (ch->in_room)->prg)
    {
      send_to_char ("No program for this room.\n\r", ch);
      return;
    }
  for (r = vtor (ch->in_room)->prg; r; r = r->next, i++)
    {
      sprintf (buf + strlen (buf), "Program Number[%d]\n\r", i);
      sprintf (buf + strlen (buf), "Command words[%s]\n\r", r->command);
      sprintf (buf + strlen (buf), "Argument Keywords[%s]\n\r", r->keys);
      sprintf (buf + strlen (buf), "Program -\n\r%s\n\r", r->prog);
    }
  page_string (ch->desc, buf);
}

void
r_link (char *argument)
{
  char buf1[80], buf2[80], buf3[80];
  int dir;
  int location, location2;
  ROOM_DATA *source_room;
  ROOM_DATA *target_room;

  arg_splitter (3, argument, buf1, buf2, buf3);

  if (!*buf1 || !*buf2 || !*buf3)
    {
      system_log ("ERROR: Missing args in r_link", true);
      return;
    }

  switch (*buf2)
    {
    case 'n':
      dir = 0;
      break;
    case 'e':
      dir = 1;
      break;
    case 's':
      dir = 2;
      break;
    case 'w':
      dir = 3;
      break;
    case 'u':
      dir = 4;
      break;
    case 'd':
      dir = 5;
      break;
    default:
      dir = -1;
      break;
    }

  if (dir == -1)
    {
      system_log ("ERROR: Invalid direction in r_link", true);
      return;
    }

  location = atol (buf1);

  location2 = atol (buf3);

  if (!(target_room = vtor (location2)))
    {
      system_log ("ERROR: tar room not found in r_link", true);
      return;
    }

  if (!(source_room = vtor (location)))
    {
      system_log ("ERROR: cha room not found in r_link", true);
      return;
    }

  if (source_room->dir_option[dir])
    vtor (source_room->dir_option[dir]->to_room)->dir_option[rev_dir[dir]] =
      0;

  CREATE (source_room->dir_option[dir], struct room_direction_data, 1);
  source_room->dir_option[dir]->general_description = 0;
  source_room->dir_option[dir]->keyword = 0;
  source_room->dir_option[dir]->exit_info = 0;
  source_room->dir_option[dir]->key = -1;
  source_room->dir_option[dir]->to_room = target_room->nVirtual;

  CREATE (target_room->dir_option[rev_dir[dir]], struct room_direction_data,
	  1);
  target_room->dir_option[rev_dir[dir]]->general_description = 0;
  target_room->dir_option[rev_dir[dir]]->keyword = 0;
  target_room->dir_option[rev_dir[dir]]->exit_info = 0;
  target_room->dir_option[rev_dir[dir]]->key = -1;
  target_room->dir_option[rev_dir[dir]]->to_room = source_room->nVirtual;
}

void
r_exit (char *argument)
{
  char buf1[256], buf2[256], buf3[80];
  int dir;
  int location, location2;
  ROOM_DATA *source_room;
  ROOM_DATA *target_room;

  arg_splitter (3, argument, buf1, buf2, buf3);

  if (!*buf1 || !*buf2 || !*buf3)
    {
      system_log ("ERROR: Missing args in r_link", true);
      return;
    }

  switch (*buf2)
    {
    case 'n':
      dir = 0;
      break;
    case 'e':
      dir = 1;
      break;
    case 's':
      dir = 2;
      break;
    case 'w':
      dir = 3;
      break;
    case 'u':
      dir = 4;
      break;
    case 'd':
      dir = 5;
      break;
    default:
      dir = -1;
      break;
    }

  if (dir == -1)
    {
      system_log ("ERROR: Invalid direction in r_link", true);
      return;
    }
  location = atoi (buf1);

  location2 = atoi (buf3);

  if (!(target_room = vtor (location2)))
    {
      system_log ("ERROR: tar room not found in r_link", true);
      return;
    }

  if (!(source_room = vtor (location)))
    {
      system_log ("ERROR: cha room not found in r_link", true);
      return;
    }

  if (!source_room->dir_option[dir])
    CREATE (source_room->dir_option[dir], struct room_direction_data, 1);

  source_room->dir_option[dir]->general_description = 0;
  source_room->dir_option[dir]->keyword = 0;
  source_room->dir_option[dir]->exit_info = 0;
  source_room->dir_option[dir]->key = -1;
  source_room->dir_option[dir]->to_room = target_room->nVirtual;

}

void
r_atlook (CHAR_DATA * ch, char *argument)
{
  char loc_str[MAX_INPUT_LENGTH];
  int loc_nr, original_loc;
  CHAR_DATA *target_mob;
  ROOM_DATA *troom;

  strcpy (loc_str, argument);

  loc_nr = atoi (loc_str);

  if (!(troom = vtor (loc_nr)))
    {
      system_log ("ERROR: Room not found in r_at", true);
      return;
    }

  original_loc = ch->in_room;
  char_from_room (ch);
  char_to_room (ch, loc_nr);
  do_look (ch, "", 0);
  /* check if the guy's still there */
  for (target_mob = troom->people; target_mob;
       target_mob = target_mob->next_in_room)
    if (ch == target_mob)
      {
	char_from_room (ch);
	char_to_room (ch, original_loc);
      }
}

void
r_set (CHAR_DATA * ch, char *argument)
{
  char var[AVG_STRING_LENGTH];

  argument = one_argument (argument, var);
  if (strcmp (var, "random") == 0)
    {
      char * p;
      int rolls = strtol (argument, &p, 0);
      int die = strtol ((*p)?(p+1):(p), 0, 0);

      if ((rolls > 0 && rolls <= 100)
	  && (die > 0 && die <= 1000)
	  && (*p == ' ' || *p == 'd'))
	{
	  random_number = dice (rolls, die);
	}      
    }
}


// void
// r_atecho (CHAR_DATA * ch, char *argument)
// {
//  char loc_str[MAX_INPUT_LENGTH], buf[4096];
//
//  half_chop (argument, loc_str, buf);
//
//  if (!isdigit (*loc_str))
//    {
//      system_log ("ERROR: atecho location not a digit", true);
//      return;
//    }
//
//  if (!vtor (atoi (loc_str)))
//    {
//      system_log ("ERROR: Room not found in r_atecho", true);
//      return;
//    }
//
//  strcat (buf, "\n\r");
//  send_to_room (buf, vtor (atoi (loc_str))->nVirtual);
// }


void 
r_atecho(CHAR_DATA *ch, char *argument) 
{ 
  char   loc_str[MAX_INPUT_LENGTH] = {'\0'}; 
  char   loc_str1[MAX_INPUT_LENGTH] = {'\0'}; 
  char    *ploc_str; 
  char   *ploc_str1; 
  char   buf[MAX_INPUT_LENGTH] = {'\0'}; 
  char   test_dat[2] = "-"; 
  char   mt1[MAX_INPUT_LENGTH] = "              "; 
  int    room_span = 0; 
  int    first_room = 0; 
  int   last_room = 0; 
    
  half_chop(argument, loc_str, buf); 
  ploc_str = loc_str; 
  ploc_str1 = loc_str1; 
  strcat(buf,"\n\r"); 

  // buf is ready to go.  it's the echo that gets sent out to the rooms. 
    
  while(1) 
    { 
      if (!strncmp(ploc_str, test_dat, 1)) // if it's a '-' set the room_span flag 
	room_span = true; 
      for (; ispunct(*ploc_str); ploc_str++); // bypass any punctuation 
      for (; isdigit(*ploc_str1 = *ploc_str); ploc_str++, ploc_str1++); // read room # into loc_str1 
    
      if ( !isdigit(*loc_str1) ) { 
	return; 
      } 

      if ( !vtor (strtol(loc_str1, NULL, 10)) ) { 
	system_log("ERROR: Room not found in r_atecho", true); 
	//   return; 
      } 
      else { 
	strcat(buf,"\n\r"); 
	send_to_room (buf, vtor (strtol(loc_str1, NULL, 10))->nVirtual); 
      } 


      if(room_span) { // if room_span is set, the last room echoed to was the end of the span 
	// go echo to the rooms in between now. 
	last_room = strtol(loc_str1, NULL, 10); // set the last room as an integer 
          
	// iterate through the span of rooms 
	while(first_room + 1 < last_room) { 
	  if ( !vtor (first_room + 1) ) { // does the room exist? 
	    first_room++; // increment even if the room doesn't exist 
	  } 
	  else { 
	    send_to_room(buf, vtor (first_room + 1)->nVirtual); 
	    first_room++; 
	  } 
	} 
	room_span = 0;    // reset the trigger 
      } 

      first_room = strtol(loc_str1, NULL, 10); // set first_room as the last room echoed to 
      strcpy(loc_str1, mt1); // overwrite loc_str1 
      ploc_str1 = loc_str1; // reset the pointer 
    } 
  return; 
} 


void
r_unlink (char *argument)
{
  char arg1[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int dir;
  int old_rnum, location;
  ROOM_DATA *troom;

  half_chop (argument, arg1, arg2);

  location = atoi (arg2);

  switch (*arg1)
    {
    case 'n':
      dir = 0;
      break;
    case 'e':
      dir = 1;
      break;
    case 's':
      dir = 2;
      break;
    case 'w':
      dir = 3;
      break;
    case 'u':
      dir = 4;
      break;
    case 'd':
      dir = 5;
      break;
    default:
      dir = -1;
      break;
    }

  if (dir == -1)
    {
      system_log ("ERROR: Invalid direction in r_unlink", true);
      return;
    }

  if (!(troom = vtor (location)))
    {
      system_log ("ERROR: cha room not found in r_unlink", true);
      return;
    }

  if (troom->dir_option[dir])
    {
      old_rnum = troom->dir_option[dir]->to_room;
    }
  else
    {
      sprintf (buf, "ERROR: Unknown exit in r_unlink [%d]: %s",
	       troom->nVirtual, argument);
      system_log (buf, true);
      return;
    }

  troom->dir_option[dir] = 0;
  vtor (old_rnum)->dir_option[rev_dir[dir]] = 0;
}

void
r_unexit (char *argument)
{
  char arg1[80], arg2[80];
  int dir;
  int location;
  ROOM_DATA *troom;

  half_chop (argument, arg1, arg2);

  location = atoi (arg2);

  switch (*arg1)
    {
    case 'n':
      dir = 0;
      break;
    case 'e':
      dir = 1;
      break;
    case 's':
      dir = 2;
      break;
    case 'w':
      dir = 3;
      break;
    case 'u':
      dir = 4;
      break;
    case 'd':
      dir = 5;
      break;
    default:
      dir = -1;
      break;
    }

  if (dir == -1)
    {
      system_log ("ERROR: Invalid direction in r_unexit", true);
      return;
    }

  if (!(troom = vtor (location)))
    {
      system_log ("ERROR: cha room not found in r_unexit", true);
      return;
    }

  troom->dir_option[dir] = 0;
}

void
r_give (CHAR_DATA * ch, char *argument)
{
  OBJ_DATA *obj;

  obj = load_object (atoi (argument));

  if (obj)
    obj_to_char (obj, ch);
  else
    system_log ("ERROR: Object does not exist in r_give", true);
}

void
r_take (CHAR_DATA * ch, char *argument)
{
  OBJ_DATA *obj;

  obj = get_obj_in_list_num (atol (argument), ch->right_hand);
  if (!obj)
    obj = get_obj_in_list_num (atol (argument), ch->left_hand);

  if (obj)
    {
      obj_from_char (&obj, 0);
      extract_obj (obj);
    }
  else
    system_log ("ERROR: Object not found in r_take", true);
}

void
r_put (char *argument)
{
  char arg1[80], arg2[80];

  half_chop (argument, arg1, arg2);

  if (!vtor (atoi (arg2)) || !vtoo (atoi (arg1)))
    system_log ("ERROR: Object does not exist in r_put", true);
  else
    obj_to_room (load_object (atoi (arg1)), vtor (atoi (arg2))->nVirtual);
}

// load_clone(<holder>,<obj_keyword>,<recipient>)
// holder: -1 (prog-user) or vnum of mob
// obj_keyword: matches the keys of an object held by holder
// recipient: -1 (room-floor) or vnum of mob
void
r_load_clone (CHAR_DATA * ch, char *argument)
{
  size_t len = strlen (argument);
  char *arg1 = new char [len];
  char *arg2 = new char [len];
  char *arg3 = new char [len];
  OBJ_DATA *obj = 0;
  OBJ_DATA *clone = 0;

  arg_splitter (3, argument, arg1, arg2, arg3);
  int holder_vnum = strtol (arg1, 0, 10);
  int recipient_vnum = strtol (arg3, 0, 10);

  CHAR_DATA* holder = (holder_vnum <= 0) ? ch : 0 ;
  CHAR_DATA* recipient = 0;
 
  CHAR_DATA* i = ch->room->people;
  for (; i; i = i->next_in_room)
    {      
      if (i->deleted)
	continue;
      
      if (!IS_NPC (i))
	continue;
      
      if (i->mob->nVirtual == holder_vnum)
	{
	  holder = i;
	}
      if (i->mob->nVirtual == recipient_vnum)
	{
	  recipient = i;
	}

      if (holder && recipient_vnum == -1)
	break;
    }
  

  if ((obj = get_obj_in_dark (holder, arg2, holder->right_hand))
      ||(obj = get_obj_in_dark (holder, arg2, holder->left_hand)))
    {
      if (IS_SET (obj->obj_flags.extra_flags, ITEM_VARIABLE))
        clone = load_colored_object (obj->nVirtual, obj->var_color);
      else
        clone = load_object (obj->nVirtual);

      clone->o.od.value[0] = obj->o.od.value[0];
      clone->o.od.value[1] = obj->o.od.value[1];
      clone->o.od.value[2] = obj->o.od.value[2];
      clone->o.od.value[3] = obj->o.od.value[3];
      clone->o.od.value[4] = obj->o.od.value[4];
      clone->o.od.value[5] = obj->o.od.value[5];

      clone->size = obj->size;
      
      if (recipient_vnum == -1)
	obj_to_room (clone, holder->room->nVirtual);
      else if (recipient)
	obj_to_char (clone, recipient);
      else 
	extract_obj (clone);
    }

  delete [] arg1;
  delete [] arg2;
  delete [] arg3;
}

void
r_get (char *argument)
{
  char arg1[80], arg2[80];
  OBJ_DATA *obj;
  int virt;
	
  half_chop (argument, arg1, arg2);

  virt = atoi (arg2);

  if (!vtor (virt))
    {
      system_log ("ERROR: Object-room not found in r_get", true);
      return;
    }

  obj = get_obj_in_list_num (atoi (arg1), vtor (virt)->contents);
  if (obj)
    {
      obj_from_room (&obj, 0);
      extract_obj (obj);
    }
  else
    system_log ("ERROR: Object not found in r_get", true);
}

void
r_lock (char *argument)
{
  long virt;
  int dir;
  char arg1[80], arg2[80];

  half_chop (argument, arg1, arg2);

  virt = atol (arg2);

  switch (*arg1)
    {
    case 'n':
      dir = 0;
      break;
    case 'e':
      dir = 1;
      break;
    case 's':
      dir = 2;
      break;
    case 'w':
      dir = 3;
      break;
    case 'u':
      dir = 4;
      break;
    case 'd':
      dir = 5;
      break;
    default:
      dir = -1;
      break;
    }

  if (dir == -1)
    {
      system_log ("ERROR: Invalid direction in r_unexit", true);
      return;
    }
}

void
r_loadmob (char *argument)
{
  char arg1[80], arg2[80];

  half_chop (argument, arg1, arg2);

  if (!vtom (atoi (arg1)) || !vtor (atoi (arg2)))
    {
      system_log ("ERROR: Mobile does not exist in r_loadmob", true);
      return;
    }

  char_to_room (load_mobile (atoi (arg1)), vtor (atoi (arg2))->nVirtual);
}

void
r_exmob (char *argument)
{
  CHAR_DATA *ptrMob = NULL, *tmp_ch = NULL;
  char arg1[80], arg2[80];
  int virt;
  long nMobVnum = 0;

  half_chop (argument, arg1, arg2);

  virt = atol (arg2);

  if (!vtor (virt))
    {
      system_log ("ERROR: Mobile-room does not exist in r_exmob", true);
      return;
    }

  nMobVnum = strtol (arg1, NULL, 10);
  if (nMobVnum > 0 && nMobVnum < 100000)
    {
      for (tmp_ch = vtor (virt)->people; tmp_ch;
	   tmp_ch = tmp_ch->next_in_room)
	{
	  if (tmp_ch->mob && tmp_ch->mob->nVirtual == nMobVnum)
	    {
	      ptrMob = tmp_ch;
	    }
	}
      if (!ptrMob)
	{
	  system_log ("ERROR: Mobile does not exist in r_exmob", true);
	  return;
	}
    }
  else if (!(ptrMob = get_char_room (arg1, virt)))
    {
      system_log ("ERROR: Mobile does not exist in r_exmob", true);
      return;
    }

  extract_char (ptrMob);
}

void
r_rftog (char *arg)
{
  int flag;
  char buf[80], rbuf[80];
  ROOM_DATA *troom;

  *buf = *rbuf = '\0';

  half_chop (arg, buf, rbuf);

  if (!(troom = vtor (atoi (rbuf))))
    {
      system_log ("ERROR: Unknown room in r_rftog.", true);
      return;
    }

  flag = parse_argument (room_bits, buf);

  if (!IS_SET (troom->room_flags, (1 << flag)))
    troom->room_flags |= (1 << flag);
  else
    troom->room_flags &= ~(1 << flag);
}

void
r_force (CHAR_DATA * ch, char *argument)
{
  char arg1[80], arg2[80], arg3[256];
  CHAR_DATA *tmp_ch;
  int room, mob;
  char buf[1024];

  arg_splitter (3, argument, arg1, arg2, arg3);
  *s_buf = '\0';
  mob = atoi (arg1);

  if (mob == -1)
    {
      command_interpreter (ch, arg3);
      return;
    }

  room = atoi (arg2);

  if (!vtor (room))
    {
      system_log ("ERROR: unknown room in r_force.", true);
      return;
    }

  for (tmp_ch = vtor (room)->people; tmp_ch; tmp_ch = tmp_ch->next_in_room)
    if (tmp_ch->mob && tmp_ch->mob->nVirtual == mob)
      {
	sprintf (buf, arg3, GET_NAME (ch));
	command_interpreter (tmp_ch, buf);
      }

}

void
r_transmob (CHAR_DATA * ch, char *argument)
{
  char arg1[80], arg2[80], arg3[256];
  CHAR_DATA *ptrMob = NULL, *tmp_ch;
  int nOriginRoom, nTargetRoom, nMobVnum;

  arg_splitter (3, argument, arg1, arg2, arg3);

  nMobVnum = atoi (arg1);
  nOriginRoom = atoi (arg2);
  nTargetRoom = atoi (arg3);

  if (!vtor (nOriginRoom))
    {
      system_log ("ERROR: unknown origin room in r_transmob.", true);
      nOriginRoom = ch->room->nVirtual;
    }

  if (!vtor (nTargetRoom))
    {
      system_log ("ERROR: unknown desination room in r_transmob.", true);
      return;
    }

  if (nMobVnum == -1)
    {
      ptrMob = ch;
    }
  else
    {
      for (tmp_ch = vtor (nOriginRoom)->people; tmp_ch;
	   tmp_ch = tmp_ch->next_in_room)
	{
	  if (tmp_ch->mob && tmp_ch->mob->nVirtual == nMobVnum)
	    {
	      ptrMob = tmp_ch;
	    }
	}
    }

  if (ptrMob->mount)
    {
      char_from_room (ptrMob->mount);
      char_to_room (ptrMob->mount, vtor (nTargetRoom)->nVirtual);
    }
  char_from_room (ptrMob);
  char_to_room (ptrMob, vtor (nTargetRoom)->nVirtual);
  return;
}


/*
 * r_trans_group
 *
 * trans_group <mob> <from> <to>
 * 
 * if <mob> is -1 assumes program user, and <from> is ignored
 * 
 */
void
r_trans_group (CHAR_DATA * ch, char * argument)
{
  char subject_arg [80];
  char origin [80];
  char destination [256];

  // Parse the arguments into parts (could probably just use strtol)
  arg_splitter (3, argument, subject_arg, origin, destination);

  int subject_mnum = strtol (subject_arg, 0, 10);
  int origin_rnum = strtol (origin, 0, 10);
  int destination_rnum = strtol (destination, 0, 10);

  // If origin_rnum doesn't exist, default to user's room
  if (!vtor (origin_rnum))
    {
      system_log ("ERROR: unknown origin room in r_transmob.", true);
      origin_rnum = ch->room->nVirtual;
    }

  // If the destination room doesn't exist, not need to continue
  if (!vtor (destination_rnum))
    {
      system_log ("ERROR: unknown desination room in r_transmob.", true);
      return;
    }


  CHAR_DATA *subject = 0;
  if (subject_mnum == -1)
    {
      subject = ch;
    }
  else
    {
      for (CHAR_DATA *tmp_ch = vtor (origin_rnum)->people; tmp_ch;
	   tmp_ch = tmp_ch->next_in_room)
	{
	  if (tmp_ch->mob && tmp_ch->mob->nVirtual == subject_mnum)
	    {
	      subject = tmp_ch;
	    }
	}
    }
  /// loop here

  // if subdued we transfer the captors group
  if (IS_SUBDUEE (subject))
    subject = subject->subdue;
  
  if (IS_RIDEE (subject))
    subject = subject->mount;

  // CHAR_DATA * leader = (subject->following) ? subject->following : subject;
  CHAR_DATA * leader = subject;
  
  int queue = 0;
  CHAR_DATA * transfer_queue [128]; // can trans up to 128 people
  CHAR_DATA * tmp_ch = subject->room->people;
  for (; tmp_ch && queue < 124; tmp_ch = tmp_ch->next_in_room)
    {
      if (tmp_ch->following == leader || tmp_ch == leader)
	{
	  if (tmp_ch->mount) 
	    {
	      transfer_queue[queue++] = tmp_ch->mount;
	    }
	  if (IS_SUBDUER(tmp_ch))
	    {
	      transfer_queue[queue++] = tmp_ch->subdue;
	    }
	  if (IS_HITCHER(tmp_ch))
	    {
	      transfer_queue[queue++] = tmp_ch->hitchee;
	    }
	  transfer_queue[queue++] = tmp_ch;
	}
    }  

  // move them
  do 
    {
      char_from_room (transfer_queue[--queue]);
      char_to_room (transfer_queue[queue], destination_rnum);
      do_look (transfer_queue[queue],"",0);
    }
  while (queue);
  return;
}


void
r_stayput (CHAR_DATA * ch, char *argument) {}

void
r_zone_echo (CHAR_DATA * ch, char *argument)
{
  char *sector_message;
  int zone = strtol (argument,&sector_message,10);

  char sector [AVG_STRING_LENGTH];
  argument = one_argument (sector_message, sector);

  if (strcmp (sector, "all") == 0)
    {
      for (ROOM_DATA *room = full_room_list; room; room = room->lnext)
	if (room->people && room->zone == zone)
	  send_to_room (argument, room->nVirtual);
    }
  else if (strcmp (sector, "outside") == 0)
    {
      for (ROOM_DATA *room = full_room_list; room; room = room->lnext)
	if (room->people && room->zone == zone)
	  {
	    switch (room->sector_type)
	      {
	      case SECT_INSIDE:
	      case SECT_UNDERWATER:
	      case SECT_PIT:
		break;
	      default:		
		send_to_room (argument, room->nVirtual);
		break;
	      }
	  }            
    }
  else 
    {
      int sector_flag = parse_argument (sector_types, sector);
      if (sector_flag >= 0)
	{
	  for (ROOM_DATA *room = full_room_list; room; room = room->lnext)
	    if (room->people && room->zone == zone 
		&& room->sector_type == sector_flag)
	      send_to_room (argument, room->nVirtual);
	}
    }
}

void
r_atwrite (CHAR_DATA * ch, char *argument) {}

void
r_system (CHAR_DATA * ch, char *argument)
{
  char buf [AVG_STRING_LENGTH];
  char *token = index (argument, '@');

  if (token) 
    {
      token[0] = 0;
      sprintf (buf, "#6%s#5%s#0#6%s#0\n", argument, ch->tname, token+1);
    }
  else
    {
      sprintf (buf, "#5%s#0: #6%s#0\n", ch->tname, argument);
    }
  send_to_gods (buf);
}

void
r_clan_echo (CHAR_DATA * ch, char *argument) {}

void
r_pain (CHAR_DATA * ch, char *argument)
{
  char arg1[80], arg2[80], arg3[80], arg4[80], arg5[80];
  int high, low, dam, type;
  int room;
  CHAR_DATA *victim;

  arg_splitter (5, argument, arg1, arg2, arg3, arg4, arg5);

  room = atoi (arg1);

  if (!vtor (room))
    {
      system_log ("ERROR: unknown room in r_pain.", true);
      return;
    }
  low = atoi (arg2);
  high = atoi (arg3);
  if ((type = index_lookup (damage_type, arg5)) < 0)
    {
      type = 3;
    }

  if (!strncmp (arg4, "all", 3))
    {

      for (victim = vtor (room)->people; victim;
	   victim = victim->next_in_room)
	{

	  if ((dam = number (low, high)))
	    {
	      wound_to_char (victim, figure_location (victim, number (0, 10)),
			     dam, type, 0, 0, 0);
	    }

	}
    }
  else
    {

      if ((dam = number (low, high)))
	{
	  wound_to_char (ch, figure_location (ch, number (0, 10)), dam, type,
			 0, 0, 0);
	}

    }
}


/* atread <room> <board> <message> */
void
r_atread (CHAR_DATA * ch, char *argument)
{
  char arg1[80], arg2[80], arg3[80], buf[80];
  int room, nMsgNum;
  OBJ_DATA *ptrBoard = NULL;
  ROOM_DATA *ptrRoom = NULL;
  //      CHAR_DATA *victim = NULL;

  arg_splitter (3, argument, arg1, arg2, arg3);

  room = atoi (arg1);

  if (!(ptrRoom = vtor (room)))
    {
      system_log ("ERROR: unknown room in r_atread.", true);
      return;
    }
  else
    {
      if (!(ptrBoard = get_obj_in_list (arg2, ptrRoom->contents))
	  || (GET_ITEM_TYPE (ptrBoard) != ITEM_BOARD))
	{
	  send_to_char ("What board?\n", ch);
	  return;
	}

      if (!isdigit (*arg3) && *arg3 != '-')
	{
	  send_to_char ("Which message on that board?\n", ch);
	  return;
	}
      nMsgNum = atoi (arg3);
      one_argument (ptrBoard->name, buf);
      display_mysql_board_message (ch, buf, nMsgNum, 1);
    }
  return;
}

void
r_purge (CHAR_DATA * ch, char *argument)
{
  char arg1[80], arg2[80], arg3[80];
  int room;
  OBJ_DATA *object = NULL;
  OBJ_DATA *next_object = NULL;
  ROOM_DATA *ptrRoom = NULL;
  //      CHAR_DATA *victim = NULL;

  arg_splitter (3, argument, arg1, arg2, arg3);

  room = atoi (arg1);

  if (!(ptrRoom = vtor (room)))
    {
      system_log ("ERROR: unknown room in r_purge.", true);
      return;
    }
  else
    {
      for (object = ptrRoom->contents; object; object = next_object)
	{
	  next_object = object->next_content;
	  if (GET_ITEM_TYPE (object) == ITEM_DWELLING
	      && object->o.od.value[0] >= 100000)
	    {
	      continue;
	    }
	  extract_obj (object);
	}
    }

  return;
}

// strip <room>
// room = vnum of a room -- where the bag will be dropped off. -1 is character's room.

void
r_strip (CHAR_DATA *ch, char *argument)
{
   int drop_room = atoi(argument);
   OBJ_DATA *obj;
   OBJ_DATA *bag = NULL;
   char buf[MAX_STRING_LENGTH];

   if (drop_room == -1)
     drop_room = ch->room->nVirtual;

   if (!( vtor(drop_room)))
   {
     system_log("ERROR: Room does not exist in r_strip", true);
   }
   else
   {
     bag = load_object (VNUM_JAILBAG);

     if (bag && (ch->right_hand || ch->left_hand || ch->equip))
     {
       sprintf (buf, "A bag belonging to %s sits here.", ch->short_descr);
       bag->description = str_dup (buf);

       sprintf (buf, "a bag labeled '%s'", ch->short_descr);
       bag->short_description = str_dup (buf);

       if (ch->right_hand)
 	{
 	  obj = ch->right_hand;
 	  obj_from_char (&obj, 0);
 	  if (bag)
 	    obj_to_obj (obj, bag);
 	}

      if (ch->left_hand)
	{
	  obj = ch->left_hand;
	  obj_from_char (&obj, 0);
	  if (bag)
	    obj_to_obj (obj, bag);
	}

      while (ch->equip)
	{
	  obj = ch->equip;
	  if (bag)
	    obj_to_obj (unequip_char (ch, obj->location), bag);
	}

      obj_to_room (bag, drop_room);
    }
   }
}

// clan <clan short name> <rank>

void
r_clan (CHAR_DATA *ch, char *argument)
{
  size_t len = strlen (argument);
  char *arg1 = new char [len];
  char *arg2 = new char [len];
  char *arg3 = new char [len];
  arg_splitter (3, argument, arg1, arg2, arg3);
  
  int flags;

 if((!strncmp (arg3, "remove", 6)) || (!strncmp (arg2, "remove", 6)))
 {
   remove_clan(ch, arg1);
 }
 else
 {

      if (!strncmp (arg2, "leader", 6))
	flags = CLAN_LEADER;
      else if (!strncmp (arg2, "memberobj", 9))
	flags = CLAN_MEMBER_OBJ;
      else if (!strncmp (arg2, "leaderobj", 9))
	flags = CLAN_LEADER_OBJ;
      else if (!strncmp (arg2, "recruit", 7))
	flags = CLAN_RECRUIT;
      else if (!strncmp (arg2, "private", 7))
	flags = CLAN_PRIVATE;
      else if (!strncmp (arg2, "corporal", 8))
	flags = CLAN_CORPORAL;
      else if (!strncmp (arg2, "sergeant", 8))
	flags = CLAN_SERGEANT;
      else if (!strncmp (arg2, "lieutenant", 10))
	flags = CLAN_LIEUTENANT;
      else if (!strncmp (arg2, "captain", 7))
	flags = CLAN_CAPTAIN;
      else if (!strncmp (arg2, "general", 7))
	flags = CLAN_GENERAL;
      else if (!strncmp (arg2, "commander", 9))
	flags = CLAN_COMMANDER;
      else if (!strncmp (arg2, "apprentice", 10))
	flags = CLAN_APPRENTICE;
      else if (!strncmp (arg2, "journeyman", 10))
	flags = CLAN_JOURNEYMAN;
      else if (!strncmp (arg2, "master", 6))
	flags = CLAN_MASTER;
      else 
	flags = CLAN_MEMBER;

  add_clan(ch, arg1, flags);
  }

  delete [] arg1;
  delete [] arg2;
  delete [] arg3;
}


// criminalize <target> <zone> <hours>
// target = -1, trigger initiator, or 'all' for all initiator's room.
// zone = game zone
// hours = positive integer

void
r_criminalize (CHAR_DATA *ch, char *argument)
{
  size_t len = strlen (argument);
  char *arg1 = new char [len];
  char *arg2 = new char [len];
  char *arg3 = new char [len];
  arg_splitter (3, argument, arg1, arg2, arg3);
  int zone = atoi(arg2);
  int time = atoi(arg3);

  CHAR_DATA* i = ch->room->people;

  if (time <= 0)
    time = 0;
 
  if ((zone <= 0) || (zone > 100))
    zone = 0;

  if (!strncmp (arg1, "all", 3))
  {
    for (; i; i = i->next_in_room)
      {
        add_criminal_time (i, zone, time);
      }
  }
  else
  {
    add_criminal_time (ch, zone, time);
  }
  
  delete [] arg1;
  delete [] arg2;
  delete [] arg3;
}


// Loadobj <room> <number> <object>
// room = -1 (trigger-puller's room) or vnum
// number = positive integer
// object = vnum

void
r_load_obj (const CHAR_DATA *ch, char *argument)
{
   size_t len = strlen(argument);
   char *arg1 = new char [(int) len];
   char *arg2 = new char [(int) len];
   char *arg3 = new char [(int) len];
   bool exit = false;
   OBJ_DATA *obj = NULL;
   int rvnum = 0, ovnum = 0, count = 0;
   arg_splitter (3, argument, arg1, arg2, arg3);
   rvnum = atoi(arg1);
   count = atoi(arg2);
   ovnum = atoi(arg3);

   if ( rvnum == -1)
   {
	   rvnum = ch->room->nVirtual;
   }

   if (!( vtor(rvnum) ))
   {
		   system_log("ERROR: Room does not exist in r_load_obj", true);
		   exit = true;
   }
   if (count < 1)
   {
	   system_log("ERROR: Negative count specified in r_load_obj", true);
	   exit = true;
   }
   if (count == 1)
   {
	   sprintf(arg2, "%d %d", ovnum, rvnum);
	   r_put(arg2);
	   exit = true;
   }
   if (!exit && !(obj = load_object (ovnum)))
   {
	   system_log("ERROR: Item does not exist in r_load_obj", true);
	   exit = true;
   }

   if ( !exit)
   {
	   if ( IS_SET (obj->obj_flags.extra_flags, ITEM_VARIABLE))
	   {
		   obj_to_room (obj, rvnum);
		   for (int i = 1; i < count; i++)
		   {
			   obj = load_object(ovnum);
			   obj_to_room (obj, rvnum);
		   }
	   }
	   else
	   {
		   obj->count = count;
		   obj_to_room (obj, rvnum);
	   }
   }


delete [] arg1;
delete [] arg2;
delete [] arg3;

}

void r_delay (CHAR_DATA *ch, char *argument)
{
	do_scommand(ch, argument, 1);
}

void r_takemoney ( CHAR_DATA *ch, char * argument)
{
	std::string ArgumentList(argument);
	std::string ThisArgument("");
	int TargetVnum = -1, Count = 0, Currency = 0;

	ArgumentList = one_argument(ArgumentList, ThisArgument);
/*	if (!ThisArgument.empty())
	{
		if (is_number(ThisArgument.c_str()) && ThisArgument.c_str() == "-1") // Room target disabled, must be -1 (player) for now.
		{
			TargetVnum = atoi(ThisArgument.c_str());
		}
		else
		{
			send_to_gods("Error: Non supported target argument in r_takemoney");
			return;
		}
	}
	else
	{
		send_to_gods("Error: Missing target argument in r_takemoney");
		return;
	} */ // Currently unneccisary as the taking money from room functionality is not in place

	ArgumentList = one_argument(ArgumentList, ThisArgument);
	if (!ThisArgument.empty())
	{
		if (is_number(ThisArgument.c_str()) && atoi(ThisArgument.c_str()) > 0)
		{
			Count = atoi(ThisArgument.c_str());
		}
		else
		{
			send_to_gods("Error: Invalid amount argument in r_takemoney");
			return;
		}
	}
	else
	{
		send_to_gods("Error: Missing amount argument in r_takemoney");
		return;
	}

	ArgumentList = one_argument(ArgumentList, ThisArgument);
	if (!ThisArgument.empty())
	{
		if (ThisArgument.find("gondorian") != std::string::npos)
			Currency = 0;
		else if (ThisArgument.find("numenorean") != std::string::npos)
			Currency = 2;
		else if (ThisArgument.find("orkish") != std::string::npos || ThisArgument.find("yrkish") != std::string::npos || ThisArgument.find("orcish") != std::string::npos)
			Currency = 1;
		else
		{
			send_to_gods("Error: Invalid currency type in r_takemoney");
			return;
		}
	}
	else
	{
		send_to_gods("Error: Missing currency argument in r_takemoney");
		return;
	}

	if (TargetVnum == -1)
	{
		subtract_money(ch, (-1*Count), Currency);
		return;
	}
}
/*
void r_setvar (char *argument, DynamicVariableList *Variables)
{
	std::string ArgumentList(argument), ThisArgument;

	ArgumentList = one_argument(ArgumentList, ThisArgument);
	if (ThisArgument.find("declare")  != std::string::npos)
	{
		ArgumentList = one_argument(ArgumentList, ThisArgument);
		if (ThisArgument.empty() || Variables.GetNamedVariable(ThisArgument) != NULL)
		{
			system_log("Error: Already declared variable in setvar.", true);
			return;
		}

		DynamicVariable NewVar(ThisArgument);
		Variables.push_back(NewVar);
	}
	else if (ThisArgument.find("add")  != std::string::npos)
	{
		ArgumentList = one_argument(ArgumentList, ThisArgument);
		if (ThisArgument.empty())
		{
			system_log("Error: No argument after 'setvar arg'", true);
			return;
		}
		std::string VarName(ThisArgument);
		if (Variables.GetNamedVariable(VarName) == NULL)
		{
			system_log("Error: Invalid variable in setvar", true);
			return;
		}

		ArgumentList = one_argument(ArgumentList, ThisArgument);
		if (is_number(ThisArgument.c_str()) && is_number((Variables.GetNamedVariable(VarName))->GetVariableContents()))
		{
			std::ostringstream conversion << atoi(Variables.GetNamedVariable(VarName)->GetVariableContent().c_str())+atoi(ThisArgument.c_str());
			Variables.GetNamedVariable(VarName)->SetVariableContents(converstion.str());
		}
		else
		{
			Variables.GetNamedVariable(VarName)->SetVariableContents(Variables.GetNamedVariable(VarName)->GetVariableContents() + ThisArgument);
		}
	}
	else if (ThisArgument.find("assign") != std::string::npos)
	{
		ArgumentList = one_argument(ArgumentList, ThisArgument);
		if (ThisArgument.empty())
		{
			system_log("Error: No argument after 'setvar arg'", true);
			return;
		}
		std::string VarName(ThisArgument);
		if (Variables.GetNamedVariable(VarName) == NULL)
		{
			system_log("Error: Invalid variable in setvar", true);
			return;
		}

		ArgumentList = one_argument(ArgumentList, ThisArgument);
		Variables.GetNamedVariable(VarName)->SetVariableContent(ThisArgument);
	}
	else if (ThisArgument.find("reset") != std::string::npos)
	{
		ArgumentList = one_argument(ArgumentList, ThisArgument);
		if (ThisArgument.empty())
		{
			system_log("Error: No argument after 'setvar arg'", true);
			return;
		}
		if (Variables.GetNamedVariable(ThisArgument) == NULL)
		{
			system_log("Error: Invalid variable in setvar", true);
			return;
		}
		Variables.GetNamedVariable(ThisArgument)->SetVariableContent("");
	}
	else if (ThisArgument.find("divide") != std::string::npos)
	{
		ArgumentList = one_argument(ArgumentList, ThisArgument);
		if (ThisArgument.empty())
		{
			system_log("Error: No argument after 'setvar arg'", true);
			return;
		}
		std::string VarName(ThisArgument);
		if (Variables.GetNamedVariable(VarName) == NULL)
		{
			system_log("Error: Invalid variable in setvar", true);
			return;
		}

		ArgumentList = one_argument(ArgumentList, ThisArgument);
		if (!is_number(ThisArgument.c_str()))
		{
			system_log("Error: Non numeric value given to divide in setvar", true);
			return;
		}
		std::ostringstream conversion << (atoi(Variables.GetNamedVariable(VarName)->GetVariableContent())/atoi(ThisArgument.c_str()));
		Variables.GetNamedVariable(VarName)->SetVariableContent(conversion.str());
	}
	else if (ThisArgument.find("multiply") != std::string::npos)
	{
		ArgumentList = one_argument(ArgumentList, ThisArgument);
		if (ThisArgument.empty())
		{
			system_log("Error: No argument after 'setvar arg'", true);
			return;
		}
		std::string VarName(ThisArgument);
		if (Variables.GetNamedVariable(VarName) == NULL)
		{
			system_log("Error: Invalid variable in setvar", true);
			return;
		}

		ArgumentList = one_argument(ArgumentList, ThisArgument);
		if (!is_number(ThisArgument.c_str()))
		{
			system_log("Error: Non numeric value given to divide in setvar", true);
			return;
		}
		std::ostringstream conversion << (atoi(Variables.GetNamedVariable(VarName)->GetVariableContent()) * atoi(ThisArgument.c_str()));
		Variables.GetNamedVariable(VarName)->SetVariableContent(conversion.str());
	}
	else if (ThisArgument.find("addrandom") != std::string::npos)
	{
		ArgumentList = one_argument(ArgumentList, ThisArgument);
		if (ThisArgument.empty())
		{
			system_log("Error: No argument after 'setvar arg'", true);
			return;
		}

		if (Variables.GetNamedVariable(VarName) == NULL)
		{
			system_log("Error: Invalid variable in setvar", true);
			return;
		}
		std::ostringstream conversion << random_number;
		Variables.GetNamedVariable(VarName)->SetVariableContent(conversion.str());
	}
	else
	{
		system_log("Unknown argument type for Setvar.", true);
		return;
	}
}
*/

void r_teach (CHAR_DATA *ch, char * argument)
{
	std::string ArgumentList = argument, ThisArgument;
	int index = 0;
	ArgumentList = one_argument (ArgumentList, ThisArgument);
	if ((index = index_lookup(skills, ThisArgument.c_str())) == -1)
	{
		return;
	}
	if (real_skill(ch, index))
	{
		return;
	}
	open_skill(ch, index);
}
