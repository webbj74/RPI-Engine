#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "structs.h"
#include "utils.h"
#include "utility.h"
#include "group.h"
#include "protos.h"

bool
is_with_group (CHAR_DATA * ch)
{
  CHAR_DATA *tch;

  for (tch = ch->room->people; tch; tch = tch->next_in_room)
    {
      if (tch == ch)
	continue;

		if (tch->following == ch ||
				 ch->following == tch ||
				 (ch->following && tch->following == ch->following))
	{
	  return 1;
	}
    }

  return 0;
}

void
do_follow (CHAR_DATA * ch, char *argument, int cmd)
{
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *leader = NULL, *tch = NULL, *orig_leader = NULL;

  argument = one_argument (argument, buf);

  if (!*buf)
    {
      send_to_char ("Follow whom?\n", ch);
      return;
    }

  if (!(leader = get_char_room_vis (ch, buf)))
    {
      send_to_char ("There is nobody here by that name.\n", ch);
      return;
    }

  if (IS_MORTAL (ch) && leader != ch
      && IS_SET (leader->plr_flags, GROUP_CLOSED))
    {
      send_to_char
	("That individual's group is currently closed to new followers.\n",
	 ch);
      act ("$n just attempted to join your group.", true, ch, 0, leader,
	   TO_VICT | _ACT_FORMAT);
      return;
    }

  if (leader != ch)
    {
      if (leader->following == ch)
	{
	  send_to_char
	    ("You'll need to ask them to stop following you first.\n", ch);
	  return;
	}
      orig_leader = leader;
      while (leader->following)
	leader = leader->following;
      if (IS_MORTAL (ch) && leader != ch
	  && IS_SET (leader->plr_flags, GROUP_CLOSED))
	{
	  send_to_char
	    ("That individual's group is currently closed to new followers.\n",
	     ch);
	  act ("$n just attempted to join your group.", true, ch, 0, leader,
	       TO_VICT | _ACT_FORMAT);
	  return;
	}
      ch->following = leader;
      for (tch = ch->room->people; tch; tch = tch->next_in_room)
	{
	  if (tch->following == ch)
	    {
	      tch->following = leader;
	      act ("You fall into stride with the group's new leader, $N.",
		   false, tch, 0, leader, TO_CHAR | _ACT_FORMAT);
	    }
	}
      if (orig_leader != leader)
	{
	  if (!IS_SET (ch->flags, FLAG_WIZINVIS))
	    sprintf (buf,
		     "You begin following #5%s's#0 group's current leader, $N.",
		     char_short (orig_leader));
	  else if (IS_SET (ch->flags, FLAG_WIZINVIS))
	    sprintf (buf,
		     "You will secretly follow #5%s#0's group's current leader, $N.",
		     char_short (orig_leader));
	}
      else
	{
	  if (!IS_SET (ch->flags, FLAG_WIZINVIS))
	    sprintf (buf, "You begin following $N.");
	  else if (IS_SET (ch->flags, FLAG_WIZINVIS))
	    sprintf (buf, "You will secretly follow $N.");
	}
      act (buf, false, ch, 0, ch->following, TO_CHAR | _ACT_FORMAT);
      sprintf (buf, "$n falls into stride with you.");
      if (!IS_SET (ch->flags, FLAG_WIZINVIS))
	act (buf, false, ch, 0, ch->following, TO_VICT | _ACT_FORMAT);
      sprintf (buf, "$n falls into stride with $N.");
      if (!IS_SET (ch->flags, FLAG_WIZINVIS))
	act (buf, false, ch, 0, ch->following, TO_NOTVICT | _ACT_FORMAT);
      return;
    }

  if (leader == ch && ch->following && ch->following != ch)
    {
      sprintf (buf, "You will no longer follow $N.");
      act (buf, false, ch, 0, ch->following, TO_CHAR | _ACT_FORMAT);
      sprintf (buf, "$n is no longer following you.");
      if (!IS_SET (ch->flags, FLAG_WIZINVIS)
	  && ch->room == ch->following->room)
	act (buf, false, ch, 0, ch->following, TO_VICT | _ACT_FORMAT);
      sprintf (buf, "$n stops following $N.");
      if (!IS_SET (ch->flags, FLAG_WIZINVIS)
	  && ch->room == ch->following->room)
	act (buf, false, ch, 0, ch->following, TO_NOTVICT | _ACT_FORMAT);
      ch->following = 0;
      return;
    }

  if (leader == ch && (!ch->following || ch->following == ch))
    {
      send_to_char ("You aren't following anyone!\n", ch);
      return;
    }

}

/*
void
do_follow (CHAR_DATA * ch, char *argument, int cmd)
{
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *leader = NULL, *orig_leader = NULL;

  argument = one_argument (argument, buf);

  if (!*buf)
    {
      send_to_char ("Follow whom?\n", ch);
      return;
    }

  if (!(leader = get_char_room_vis (ch, buf)))
    {
      send_to_char ("There is nobody here by that name.\n", ch);
      return;
    }

  if (IS_MORTAL (ch) && leader != ch
      && IS_SET (leader->plr_flags, GROUP_CLOSED))
    {
      send_to_char
	("That individual's group is currently closed to new followers.\n",
	 ch);
      act ("$n just attempted to join your group.", true, ch, 0, leader,
	   TO_VICT | _ACT_FORMAT);
      return;
    }

  bool wizinvis = IS_SET (ch->flags, FLAG_WIZINVIS);

  if (leader != ch)
    {
      if (leader->following == ch)
	{
	  send_to_char
	    ("You'll need to ask them to stop following you first.\n", ch);
	  return;
	}
      
      // TAKE ME TO YOUR LEADER
      orig_leader = leader;
      while (leader->following)
	{
	  leader = leader->following;
	}

      if (IS_MORTAL (ch) && leader != ch
	  && IS_SET (leader->plr_flags, GROUP_CLOSED))
	{
	  send_to_char ("That individual's group is "
			"currently closed to new followers.\n",
			ch);
	  act ("$n just attempted to join your group.", 
	       true, ch, 0, leader, TO_VICT | _ACT_FORMAT);

	  return;
	}

      ch->following = leader;
      leader->group->insert (ch);

      // copy ch's local following to leader
      std::set<CHAR_DATA*>::iterator i;
      ROOM_DATA *here = ch->room;
      for (i = ch->group->begin (); i != ch->group->end (); ++i)
	{
	  CHAR_DATA *tch = (*i);
	  if (tch->following == ch)
	    {
	      if (tch->room == here)
		{
		  tch->following = leader;
		  leader->group->insert (tch);
		  act ("You fall into stride with the group's new leader, $N.",
		       false, tch, 0, leader, TO_CHAR | _ACT_FORMAT);
		}
	      else
		{
		  tch->following = 0;
		}
	    }
	}
      ch->group->clear ();
      
      if (wizinvis)
	{
	  if (orig_leader != leader)
	    {
	      sprintf (buf,
		       "You will secretly follow #5%s#0's group's "
		       "current leader, $N.",
		       char_short (orig_leader));
	      act (buf, false, ch, 0, ch->following, TO_CHAR | _ACT_FORMAT);
	    }
	  else
	    {
	      act ("You will secretly follow $N.", 
		   false, ch, 0, ch->following, TO_CHAR | _ACT_FORMAT);
	    }
	}
      else
	{
	  if (orig_leader != leader)
	    {
	      sprintf (buf,
		       "You begin following #5%s's#0 group's "
		       "current leader, $N.",
		       char_short (orig_leader));
	      act (buf, false, ch, 0, ch->following, TO_CHAR | _ACT_FORMAT);
	    }
	  else
	    {
	      act ("You begin following $N.", 
		   false, ch, 0, ch->following, TO_CHAR | _ACT_FORMAT);
	    }

	  act ("$n falls into stride with you.", 
	       false, ch, 0, ch->following, TO_VICT | _ACT_FORMAT);

	  act ("$n falls into stride with $N.", 
	       false, ch, 0, ch->following, TO_NOTVICT | _ACT_FORMAT);
	}
      return;
    }

  // leader == ch
  if ((ch->following) && (ch->following != ch))
    {
      act ("You will no longer follow $N.", 
	   false, ch, 0, ch->following, TO_CHAR | _ACT_FORMAT);
      
      if (!wizinvis && ch->room == ch->following->room)
	{
	  act ("$n is no longer following you.", 
	       false, ch, 0, ch->following, TO_VICT | _ACT_FORMAT);
	  act ("$n stops following $N.", 
	       false, ch, 0, ch->following, TO_NOTVICT | _ACT_FORMAT);
	}
      ch->following->group->erase (ch);
    }
  else
    {
      send_to_char ("You aren't following anyone!\n", ch);
    }

  ch->group->erase (ch);
  ch->following = 0;
  return;
}
*/

char *
tactical_status (CHAR_DATA * ch)
{
  CHAR_DATA *tch;
  AFFECTED_TYPE *af;
  static char status[MAX_STRING_LENGTH];
  int i = 0;

  *status = '\0';

  if (get_affect (ch, MAGIC_HIDDEN))
    sprintf (status + strlen (status), " #1(hidden)#0");

  if (get_affect (ch, MAGIC_GUARD) || get_affect (ch, AFFECT_GUARD_DIR))
    sprintf (status + strlen (status), " #6(guarding)#0");

  for (tch = ch->room->people; tch; tch = tch->next_in_room)
    {
      if ((af = get_affect (tch, MAGIC_GUARD))
	  && ((CHAR_DATA *) af->a.spell.t) == ch)
	i++;
    }

  if (i > 0)
    {
      if (i == 1)
	sprintf (status + strlen (status), " #2(guarded)#0");
      else if (i > 1)
	sprintf (status + strlen (status), " #2(guarded x %d)#0", i);
    }

  i = 0;

  if (ch->fighting)
    i++;

  for (tch = ch->room->people; tch; tch = tch->next_in_room)
    {
      if (tch == ch->fighting)
	continue;
      if (tch->fighting == ch)
	i++;
    }

  if (i > 0)
    {
      if (i == 1)
	sprintf (status + strlen (status), " #1(engaged)#0");
      else if (i > 1)
	sprintf (status + strlen (status), " #1(engaged x %d)#0", i);
    }

  return status;
}

void
do_group (CHAR_DATA * ch, char *argument, int cmd)
{
  CHAR_DATA *tch = NULL, *top_leader = NULL;
  char status[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  bool found = false;

  argument = one_argument (argument, buf);

  if (*buf)
    {
      if (is_abbrev (buf, "open"))
	{
	  
	  if (!IS_SET (ch->plr_flags, GROUP_CLOSED))
	    {
	      send_to_char ("Your group is already open!\n", ch);
	      return;
	    }
	  ch->plr_flags &= ~GROUP_CLOSED;
	  send_to_char ("You will now allow people to follow you.\n", ch);
	  return;
	}
      else if (is_abbrev (buf, "close"))
	{
	  if (IS_SET (ch->plr_flags, GROUP_CLOSED))
	    {
	      send_to_char ("Your group is already closed!\n", ch);
	      return;
	    }
	  ch->plr_flags |= GROUP_CLOSED;
	  send_to_char ("You will no longer allow people to follow you.\n", ch);
	  return;
	}
      else if (is_abbrev (buf, "retreat"))
	{
	  char direction_arg[AVG_STRING_LENGTH] = "";
	  int direction = 0;
	  argument = one_argument (argument, direction_arg);
	  
	  if (!*direction_arg 
	      || (direction = index_lookup (dirs, direction_arg)) == -1 )
	    {
	      send_to_char ("Order a retreat in which direction?\n", ch);
	    }
	  else
	    {
	      CHAR_DATA* tch;
	      bool ordered = false;
	      for (tch = ch->room->people; tch; tch = tch->next)
		{
		  if (tch->following == ch)
		    {
		      ordered = true;
		      retreat (tch, direction, ch);
		    }
		}

	      if (ordered)
		{
		  retreat (ch, direction, ch);
		}
	      else
		{
		  retreat (ch, direction, 0);
		}
	    }
	  return;
	}
    }

  if (!(top_leader = ch->following))
    top_leader = ch;

  if (!top_leader)
    {
      send_to_char ("You aren't in a group.\n", ch);
      return;
    }

  *status = '\0';

  sprintf (buf, "#5%s#0 [%s]%s, leading:\n\n", char_short (top_leader),
	   wound_total (top_leader), tactical_status (top_leader));
  buf[2] = toupper (buf[2]);

  for (tch = top_leader->room->people; tch; tch = tch->next_in_room)
    {
      if (tch->following != top_leader)
	continue;
     // if (!CAN_SEE (ch, tch))
	//continue;
      if (found != false)
	sprintf (buf + strlen (buf), ",\n");
      *status = '\0';
      sprintf (buf + strlen (buf), "   #5%s#0 [%s]%s", char_short (tch),
	       wound_total (tch), tactical_status (tch));
      found = true;
    }

  strcat (buf, ".\n");

  if (!found)
    {
      send_to_char ("You aren't in a group.\n", ch);
      return;
    }

  send_to_char (buf, ch);
}
/*
void
do_group (CHAR_DATA * ch, char *argument, int cmd)
{
  CHAR_DATA *tch = NULL, *leader = NULL;
  char buf[MAX_STRING_LENGTH];
  bool found = false;

  argument = one_argument (argument, buf);

  if (*buf)
    {
      if (is_abbrev (buf, "open"))
	{
	  
	  if (!IS_SET (ch->plr_flags, GROUP_CLOSED))
	    {
	      send_to_char ("Your group is already open!\n", ch);
	      return;
	    }
	  ch->plr_flags &= ~GROUP_CLOSED;
	  send_to_char ("You will now allow people to follow you.\n", ch);
	  return;
	}
      else if (is_abbrev (buf, "close"))
	{
	  if (IS_SET (ch->plr_flags, GROUP_CLOSED))
	    {
	      send_to_char ("Your group is already closed!\n", ch);
	      return;
	    }
	  ch->plr_flags |= GROUP_CLOSED;
	  send_to_char ("You will no longer allow people to follow you.\n", ch);
	  return;
	}
      else if (is_abbrev (buf, "retreat"))
	{
	  char direction_arg[AVG_STRING_LENGTH] = "";
	  int direction = 0;
	  argument = one_argument (argument, direction_arg);
	  
	  if (!*direction_arg 
	      || (direction = index_lookup (dirs, direction_arg)) == -1 )
	    {
	      send_to_char ("Order a retreat in which direction?\n", ch);
	    }
	  else
	    {
	      CHAR_DATA* tch;
	      bool ordered = false;
	      for (tch = ch->room->people; tch; tch = tch->next)
		{
		  if (tch->following == ch)
		    {
		      ordered = true;
		      retreat (tch, direction, ch);
		    }
		}

	      if (ordered)
		{
		  retreat (ch, direction, ch);
		}
	      else
		{
		  retreat (ch, direction, 0);
		}
	    }
	  return;
	}
      else
	{
	  send_to_char ("Unknown subcommand.\n", ch);
	  return;
	}
    }

  leader = (ch->following) ? (ch->following) : (ch);
  if (leader->group->empty ())
    {
      send_to_char ("You aren't in a group.\n", ch);
      return;
    }

  sprintf (buf, "#5%s#0 [%s]%s, leading:\n\n", char_short (leader),
	   wound_total (leader), tactical_status (leader));
  buf[2] = toupper (buf[2]);

  std::set<CHAR_DATA*>::iterator i;
  ROOM_DATA *here = ch->room;
  for (i = leader->group->begin (); i != leader->group->end (); ++i)
    {
      CHAR_DATA *tch = (*i);
      if (tch->room != here || !CAN_SEE (ch, tch))
	continue;

      if (found != false)
	sprintf (buf + strlen (buf), ",\n");
      sprintf (buf + strlen (buf), "   #5%s#0 [%s]%s", char_short (tch),
	       wound_total (tch), tactical_status (tch));
      found = true;
    }
  strcat (buf, ".\n");

  if (!found)
    {
      send_to_char ("You aren't in a group.\n", ch);
      return;
    }

  send_to_char (buf, ch);
}
*/
void
followers_follow (CHAR_DATA * ch, int dir, int leave_time, int arrive_time)
{
  CHAR_DATA *tch;
  ROOM_DATA *room_exit;

  for (tch = ch->room->people; tch; tch = tch->next_in_room)
    {

      if (tch == ch || GET_FLAG (tch, FLAG_LEAVING))
	continue;

      if (tch->following != ch)
	{
	  if (!IS_RIDEE (tch) || tch->mount->following != ch)
	    continue;
	}

      if (IS_HITCHEE (tch))
	continue;

      /* Check if this mob tch is allow to go to target room */

      if (IS_NPC (tch) && CAN_GO (tch, dir) && !isguarded (ch->room, dir) &&
	  (room_exit = vtor (EXIT (tch, dir)->to_room)))
	{

	  if (IS_MERCHANT (tch) &&
	      IS_SET (room_exit->room_flags, NO_MERCHANT))
	    continue;

	  if (tch->mob->access_flags &&
	      !(tch->mob->access_flags & room_exit->room_flags))
	    continue;

	  if (IS_SET (tch->act, ACT_STAY_ZONE) &&
	      tch->room->zone != room_exit->zone)
	    continue;
	}

      if (GET_POS (tch) == SIT)
	{
	  act ("You can't follow $N while sitting.",
	       false, tch, 0, ch, TO_CHAR);
	  return;
	}

      else if (GET_POS (tch) == REST)
	{
	  act ("You can't follow $N while resting.",
	       false, tch, 0, ch, TO_CHAR);
	  return;
	}

      else if (GET_POS (tch) < FIGHT)
	return;

      if (get_affect (tch, MAGIC_HIDDEN) && real_skill (tch, SKILL_SNEAK))
	{
	  if (odds_sqrt (skill_level (tch, SKILL_SNEAK, 0)) >= number (1, 100)
	      || !would_reveal (tch))
	    {
	      magic_add_affect (tch, MAGIC_SNEAK, -1, 0, 0, 0, 0);
	    }
	  else
	    {
	      remove_affect_type (tch, MAGIC_HIDDEN);
	      act ("$n attempts to be stealthy.", true, tch, 0, 0, TO_ROOM);
	    }
	}

      move (tch, "", dir, leave_time + arrive_time);
    }
}

void
follower_catchup (CHAR_DATA * ch)
{
  CHAR_DATA *tch;
  QE_DATA *qe;

  if (!ch->room)
    return;

  for (tch = ch->room->people; tch; tch = tch->next_in_room)
    if (ch->following == tch)
      break;

  if (!tch || !GET_FLAG (tch, FLAG_LEAVING) || !CAN_SEE (tch, ch)
      || IS_SWIMMING (tch))
    return;

  for (qe = quarter_event_list; qe->ch != tch; qe = qe->next)
    ;

  if (!qe)
    return;

  if (ch->aiming_at)
    {
      send_to_char ("You lose your aim as you move.\n", ch);
      ch->aiming_at->targeted_by = NULL;
      ch->aiming_at = NULL;
      ch->aim = 0;
    }

  if (get_affect (ch, MAGIC_HIDDEN) && real_skill (ch, SKILL_SNEAK))
    {
      if (odds_sqrt (skill_level (ch, SKILL_SNEAK, 0)) >= number (1, 100)
	  || !would_reveal (ch))
	{
	  magic_add_affect (ch, MAGIC_SNEAK, -1, 0, 0, 0, 0);
	}
      else
	{
	  remove_affect_type (ch, MAGIC_HIDDEN);
	  act ("$n attempts to be stealthy.", true, ch, 0, 0, TO_ROOM);
	}
    }

  move (ch, "", qe->dir, qe->event_time + qe->arrive_time);
}


// called when fleeing or when getting extracted
void
stop_followers (CHAR_DATA * ch)
{
  CHAR_DATA *tch;

  for (tch = character_list; tch; tch = tch->next)
    {
      if (tch->deleted)
	  continue;

      if (tch->following == ch)
	tch->following = 0;
    }
  //  if (ch->group)
  //    {
  //    ch->group->clear ();
  //  }
}

int num_followers (CHAR_DATA * ch)
{
	CHAR_DATA		*top_leader = NULL;
	CHAR_DATA		*tch = NULL;
	int group_count = 0;  
	
	if (!(top_leader = ch->following))
    top_leader = ch;
    
	for (tch = top_leader->room->people; tch; tch = tch->next_in_room)
    {
      if (tch->following != top_leader)
	continue;
      if (!CAN_SEE (ch, tch))
	continue;
     	group_count = group_count + 1;

    }

	return (group_count);

}
