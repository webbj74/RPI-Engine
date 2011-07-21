/*------------------------------------------------------------------------\
|  somatics.c : Short and Long Term Somatic Effects   www.middle-earth.us |
|  Copyright (C) 2004, Shadows of Isildur: Sighentist                     |
|  Derived under license from DIKU GAMMA (0.0).                           |
\------------------------------------------------------------------------*/

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "protos.h"
#include "utils.h"
#include "decl.h"

void
soma_stat (CHAR_DATA * ch, AFFECTED_TYPE * af)
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];

  switch (af->type)
    {
    case SOMA_MUSCULAR_CRAMP:
      sprintf (buf2, "a muscle cramp");
      break;
    case SOMA_MUSCULAR_TWITCHING:
      sprintf (buf2, "twitching");
      break;
    case SOMA_MUSCULAR_TREMOR:
      sprintf (buf2, "tremors");
      break;
    case SOMA_MUSCULAR_PARALYSIS:
      sprintf (buf2, "paralysis");
      break;
    case SOMA_DIGESTIVE_ULCER:
      sprintf (buf2, "stomach ulcer");
      break;
    case SOMA_DIGESTIVE_VOMITING:
      sprintf (buf2, "vomiting");
      break;
    case SOMA_DIGESTIVE_BLEEDING:
      sprintf (buf2, "vomiting blood");
      break;
    case SOMA_EYE_BLINDNESS:
      sprintf (buf2, "blindness");
      break;
    case SOMA_EYE_BLURRED:
      sprintf (buf2, "blurred vision");
      break;
    case SOMA_EYE_DOUBLE:
      sprintf (buf2, "double vision");
      break;
    case SOMA_EYE_DILATION:
      sprintf (buf2, "dilated pupils");
      break;
    case SOMA_EYE_CONTRACTION:
      sprintf (buf2, "contracted pupils");
      break;
    case SOMA_EYE_LACRIMATION:
      sprintf (buf2, "lacrimation");
      break;
    case SOMA_EYE_PTOSIS:
      sprintf (buf2, "ptosis");
      break;
    case SOMA_EAR_TINNITUS:
      sprintf (buf2, "tinnitus");
      break;
    case SOMA_EAR_DEAFNESS:
      sprintf (buf2, "deafness");
      break;
    case SOMA_EAR_EQUILLIBRIUM:
      sprintf (buf2, "ear imbalance");
      break;
    case SOMA_NOSE_ANOSMIA:
      sprintf (buf2, "anosmia");
      break;
    case SOMA_NOSE_RHINITIS:
      sprintf (buf2, "rhinitis");
      break;
    case SOMA_MOUTH_SALIVATION:
      sprintf (buf2, "salivation");
      break;
    case SOMA_MOUTH_TOOTHACHE:
      sprintf (buf2, "toothache");
      break;
    case SOMA_MOUTH_DRYNESS:
      sprintf (buf2, "dry mouth");
      break;
    case SOMA_MOUTH_HALITOSIS:
      sprintf (buf2, "halitosis");
      break;
    case SOMA_CHEST_DIFFICULTY:
      sprintf (buf2, "difficulty breathing");
      break;
    case SOMA_CHEST_WHEEZING:
      sprintf (buf2, "wheezing");
      break;
    case SOMA_CHEST_RAPIDBREATH:
      sprintf (buf2, "rapid breathing");
      break;
    case SOMA_CHEST_SLOWBREATH:
      sprintf (buf2, "shallow breathing");
      break;
    case SOMA_CHEST_FLUID:
      sprintf (buf2, "fluidous lungs");
      break;
    case SOMA_CHEST_PALPITATIONS:
      sprintf (buf2, "heart palpitations");
      break;
    case SOMA_CHEST_COUGHING:
      sprintf (buf2, "coughing fits");
      break;
    case SOMA_CHEST_PNEUMONIA:
      sprintf (buf2, "pneumonia");
      break;
    case SOMA_NERVES_PSYCHOSIS:
      sprintf (buf2, "psychosis");
      break;
    case SOMA_NERVES_DELIRIUM:
      sprintf (buf2, "delerium ");
      break;
    case SOMA_NERVES_COMA:
      sprintf (buf2, "a comatose state");
      break;
    case SOMA_NERVES_CONVULSIONS:
      sprintf (buf2, "convulsions");
      break;
    case SOMA_NERVES_HEADACHE:
      sprintf (buf2, "a headache");
      break;
    case SOMA_NERVES_CONFUSION:
      sprintf (buf2, "confusion");
      break;
    case SOMA_NERVES_PARETHESIAS:
      sprintf (buf2, "parethesias");
      break;
    case SOMA_NERVES_ATAXIA:
      sprintf (buf2, "ataxia");
      break;
    case SOMA_NERVES_EQUILLIBRIUM:
      sprintf (buf2, "nervous imbalance");
      break;
    case SOMA_SKIN_CYANOSIS:
      sprintf (buf2, "cyanosis of the skin");
      break;
    case SOMA_SKIN_DRYNESS:
      sprintf (buf2, "dryness of the skin");
      break;
    case SOMA_SKIN_CORROSION:
      sprintf (buf2, "corrosion of the skin");
      break;
    case SOMA_SKIN_JAUNDICE:
      sprintf (buf2, "jaundice of the skin");
      break;
    case SOMA_SKIN_REDNESS:
      sprintf (buf2, "redness of the skin");
      break;
    case SOMA_SKIN_RASH:
      sprintf (buf2, "a rash on the skin");
      break;
    case SOMA_SKIN_HAIRLOSS:
      sprintf (buf2, "hairloss");
      break;
    case SOMA_SKIN_EDEMA:
      sprintf (buf2, "edema of the skin");
      break;
    case SOMA_SKIN_BURNS:
      sprintf (buf2, "burns on the skin");
      break;
    case SOMA_SKIN_PALLOR:
      sprintf (buf2, "pallor of the skin");
      break;
    case SOMA_SKIN_SWEATING:
      sprintf (buf2, "the sweats");
      break;
    case SOMA_GENERAL_WEIGHTLOSS:
      sprintf (buf2, "weight loss");
      break;
    case SOMA_GENERAL_LETHARGY:
      sprintf (buf2, "lethargy");
      break;
    case SOMA_GENERAL_APPETITELOSS:
      sprintf (buf2, "appetite loss");
      break;
    case SOMA_GENERAL_PRESSUREDROP:
      sprintf (buf2, "low blood pressure");
      break;
    case SOMA_GENERAL_PRESSURERISE:
      sprintf (buf2, "high blood pressure");
      break;
    case SOMA_GENERAL_FASTPULSE:
      sprintf (buf2, "a fast pulse");
      break;
    case SOMA_GENERAL_SLOWPULSE:
      sprintf (buf2, "a slow pulse");
      break;
    case SOMA_GENERAL_HYPERTHERMIA:
      sprintf (buf2, "hyperthermia");
      break;
    case SOMA_GENERAL_HYPOTHERMIA:
      sprintf (buf2, "hypothermia");
      break;
    default:
      sprintf (buf2, "an unknown somatic effect");
      break;
    }

	send_to_char("\n", ch);
	if (!IS_MORTAL (ch))
		{
  sprintf (buf,
	   "#2%5d#0   Suffers from %s for %d more in-game hours.\n        Latency: %d hrs Power: %d to %d (%d @ %d min)\n        A: %d min, D: %d min, S: %d min, R: %d min\n",
	   af->type, buf2, af->a.soma.duration, af->a.soma.latency,
	   af->a.soma.max_power, af->a.soma.lvl_power, af->a.soma.atm_power,
	   af->a.soma.minute, af->a.soma.attack, af->a.soma.decay,
	   af->a.soma.sustain, af->a.soma.release);
	   }
	 else
	 {
	 sprintf (buf, "You suffer from %s.", buf2);
	 }
  send_to_char (buf, ch);
}



void
soma_ten_second_affect (CHAR_DATA * ch, AFFECTED_TYPE * af)
{
  int save = 0, stat = 0;
  char *locat = NULL;
  char buf2[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  
  stat = GET_CON (ch);
  if ((number (1, 1000) > af->a.soma.atm_power)
      || (number (1, (stat > 20) ? stat : 20) <= stat))
    return;

  switch (af->type)
    {
      /*    case SOMA_MUSCULAR_CRAMP:       sprintf(buf2,"a muscle cramp"); break;
         case SOMA_MUSCULAR_TWITCHING:      sprintf(buf2,"twitching"); break;
         case SOMA_MUSCULAR_TREMOR: sprintf(buf2,"tremors"); break;
         case SOMA_MUSCULAR_PARALYSIS:      sprintf(buf2,"paralysis"); break;
         case SOMA_DIGESTIVE_ULCER: sprintf(buf2,"stomach ulcer"); break; 
         case SOMA_DIGESTIVE_VOMITING:      sprintf(buf2,"vomiting"); break;
         case SOMA_DIGESTIVE_BLEEDING:      sprintf(buf2,"vomiting blood"); break; 
         case SOMA_EYE_BLINDNESS:   sprintf(buf2,"blindness"); break;
         case SOMA_EYE_BLURRED:             sprintf(buf2,"blurred vision"); break;
         case SOMA_EYE_DOUBLE:              sprintf(buf2,"double vision"); break;
         case SOMA_EYE_DILATION:            sprintf(buf2,"dilated pupils"); break; 
         case SOMA_EYE_CONTRACTION: sprintf(buf2,"contracted pupils"); break; 
         case SOMA_EYE_LACRIMATION: sprintf(buf2,"lacrimation"); break;
         case SOMA_EYE_PTOSIS:              sprintf(buf2,"ptosis"); break;
         case SOMA_EAR_TINNITUS:            sprintf(buf2,"tinnitus"); break;
         case SOMA_EAR_DEAFNESS:            sprintf(buf2,"deafness"); break;
         case SOMA_EAR_EQUILLIBRIUM:        sprintf(buf2,"ear imbalance"); break;
         case SOMA_NOSE_ANOSMIA:            sprintf(buf2,"anosmia"); break;
         case SOMA_NOSE_RHINITIS:   sprintf(buf2,"rhinitis"); break;
         case SOMA_MOUTH_SALIVATION:        sprintf(buf2,"salivation"); break;
         case SOMA_MOUTH_TOOTHACHE: sprintf(buf2,"toothache"); break;
         case SOMA_MOUTH_DRYNESS:   sprintf(buf2,"dry mouth"); break;
         case SOMA_MOUTH_HALITOSIS: sprintf(buf2,"halitosis"); break;
         case SOMA_CHEST_DIFFICULTY:        sprintf(buf2,"difficulty breathing"); break;
         case SOMA_CHEST_RAPIDBREATH:       sprintf(buf2,"rapid breathing"); break;
         case SOMA_CHEST_SLOWBREATH:        sprintf(buf2,"shallow breathing"); break;
         case SOMA_CHEST_FLUID:             sprintf(buf2,"fluidous lungs"); break;
         case SOMA_CHEST_PALPITATIONS:      sprintf(buf2,"heart palpitations"); break; */

    case SOMA_CHEST_COUGHING:

      stat = GET_WIL (ch);
      save = number (1, (stat > 20) ? stat : 20);

      if (get_affect (ch, MAGIC_HIDDEN) && would_reveal (ch))
	{
	  if (save > stat)
	    {
	      remove_affect_type (ch, MAGIC_HIDDEN);
	      act ("$n reveals $mself with an audible cough.", true, ch, 0, 0,
		   TO_ROOM);
	    }
	  else if (save > (stat / 2))
	    {
	      act ("You hear a muffled sound from somewhere nearby.", true,
		   ch, 0, 0, TO_ROOM);
	    }
	}
      else if ((save <= stat) && (save > (stat / 2)))
	{
	  act ("$n tries to stifle a cough.", true, ch, 0, 0, TO_ROOM);
	}

      if (save > stat)
	{
	  act ("You cough audibly.", true, ch, 0, 0, TO_CHAR);
	}
      else
	{
	  act ("You try to stifle a cough silently.", true, ch, 0, 0,
	       TO_CHAR);
	}
      break;

	case SOMA_CHEST_WHEEZING:

      stat = GET_WIL (ch);
      save = number (1, (stat > 20) ? stat : 20);

      if (get_affect (ch, MAGIC_HIDDEN) && would_reveal (ch))
	{
	  if (save > stat)
	    {
	      remove_affect_type (ch, MAGIC_HIDDEN);
	      act ("$n reveals $mself with an audible wheeze.", true, ch, 0, 0,
		   TO_ROOM);
	    }
	  else if (save > (stat / 2))
	    {
	      act ("You hear a muffled sound from somewhere nearby.", true,
		   ch, 0, 0, TO_ROOM);
	    }
	}
      else if ((save <= stat) && (save > (stat / 2)))
	{
	  act ("$n tries to stifle their wheezing.", true, ch, 0, 0, TO_ROOM);
	}

      if (save > stat)
	{
	  act ("You wheeze audibly.", true, ch, 0, 0, TO_CHAR);
	}
      else
	{
	  act ("You try to stifle your wheezing.", true, ch, 0, 0,
	       TO_CHAR);
	}
      break;


	case SOMA_NERVES_HEADACHE:

      stat = GET_WIL (ch);
      save = number (1, (stat > 20) ? stat : 20);

      if (save > stat)
	{
	  act ("Your head pounds with a headache.", true, ch, 0, 0, TO_CHAR);
	}
      else
	{
	  act ("You manage to ignore the pounding in your head.", true, ch, 0, 0,
	       TO_CHAR);
	}
      break;

	case SOMA_MUSCULAR_CRAMP:
	
			stat = GET_WIL (ch);
      save = number (1, (stat > 20) ? stat : 20);
			locat = expand_wound_loc(figure_location(ch, number(0,2)));

      if (save > stat)
				{
				sprintf(buf, "You get an intense cramp in your %s which persist for several minutes beofre finally relaxing\n", locat);
	  		act (buf, true, ch, 0, 0, TO_CHAR);
	  		 act ("$n suddenly cringes in pain.", true, ch, 0, 0, TO_ROOM);
				}
      else
				{
				sprintf(buf, "You get an intense cramp in your %s, but you shake it off quickly\n", locat);
	  		act (buf, true, ch, 0, 0, TO_CHAR);
	  		act ("$n suddenly cringes in pain, but quickly recovers.", true, ch, 0, 0, TO_ROOM);
				}
      break;

     
    case SOMA_MUSCULAR_TWITCHING:
    	stat = GET_WIL (ch);
      save = number (1, (stat > 20) ? stat : 20);
			locat = expand_wound_loc(figure_location(ch, number(0,2)));
			
      if (save > stat)
				{
				sprintf(buf, "You feel a strong twitching in your %s which persist for several minutes before finally relaxing\n", locat);
	  		act (buf, true, ch, 0, 0, TO_CHAR);
	  		sprintf(buf2, "$n suddenly cringes in pain, as $s %s twitches.", locat);
	  		act (buf2, true, ch, 0, 0, TO_ROOM);
				}
      else
				{
				sprintf(buf, "You feel a strong twitching in your %s, but you control it quickly\n", locat);
	  		act (buf, true, ch, 0, 0, TO_CHAR);
	  		sprintf(buf2, "$n cringes in pain, as $s %s twitches momentarily.", locat);
	  		act (buf2, true, ch, 0, 0, TO_ROOM);
	  		
				}
      break;      
      

      /*  case SOMA_CHEST_PNEUMONIA:      sprintf(buf2,"pneumonia"); break;
         case SOMA_NERVES_PSYCHOSIS:      sprintf(buf2,"psychosis"); break;
         case SOMA_NERVES_DELIRIUM:       sprintf(buf2,"delerium "); break;
         case SOMA_NERVES_COMA:           sprintf(buf2,"a comatose state"); break;
         case SOMA_NERVES_CONVULSIONS:    sprintf(buf2,"convulsions"); break;
         case SOMA_NERVES_CONFUSION:      sprintf(buf2,"confusion"); break;
         case SOMA_NERVES_PARETHESIAS:    sprintf(buf2,"parethesias"); break;
         case SOMA_NERVES_ATAXIA: sprintf(buf2,"ataxia"); break;
         case SOMA_NERVES_EQUILLIBRIUM:   sprintf(buf2,"nervous imbalance"); break;
         case SOMA_SKIN_CYANOSIS: sprintf(buf2,"cyanosis of the skin"); break;
         case SOMA_SKIN_DRYNESS:          sprintf(buf2,"dryness of the skin"); break;
         case SOMA_SKIN_CORROSION:        sprintf(buf2,"corrosion of the skin"); break;
         case SOMA_SKIN_JAUNDICE: sprintf(buf2,"jaundice of the skin"); break;
         case SOMA_SKIN_REDNESS:          sprintf(buf2,"redness of the skin"); break;
         case SOMA_SKIN_RASH:             sprintf(buf2,"a rash on the skin"); break;
         case SOMA_SKIN_HAIRLOSS: sprintf(buf2,"hairloss"); break;
         case SOMA_SKIN_EDEMA:            sprintf(buf2,"edema of the skin"); break;
         case SOMA_SKIN_BURNS:            sprintf(buf2,"burns on the skin"); break;
         case SOMA_SKIN_PALLOR:           sprintf(buf2,"pallor of the skin"); break;
         case SOMA_SKIN_SWEATING: sprintf(buf2,"the sweats"); break;
         case SOMA_GENERAL_WEIGHTLOSS:    sprintf(buf2,"weight loss"); break;
         case SOMA_GENERAL_LETHARGY:      sprintf(buf2,"lethargy"); break;
         case SOMA_GENERAL_APPETITELOSS:  sprintf(buf2,"appetite loss"); break;
         case SOMA_GENERAL_PRESSUREDROP:  sprintf(buf2,"low blood pressure"); break;
         case SOMA_GENERAL_PRESSURERISE:  sprintf(buf2,"high blood pressure"); break;
         case SOMA_GENERAL_FASTPULSE:     sprintf(buf2,"a fast pulse"); break;
         case SOMA_GENERAL_SLOWPULSE:     sprintf(buf2,"a slow pulse"); break;
         case SOMA_GENERAL_HYPERTHERMIA:  sprintf(buf2,"hyperthermia"); break;
         case SOMA_GENERAL_HYPOTHERMIA:   sprintf(buf2,"hypothermia"); break;
       */
    default:
      break;
    }
}


void
soma_rl_minute_affect (CHAR_DATA * ch, AFFECTED_TYPE * af)
{
  unsigned short int minute = ++af->a.soma.minute;
  unsigned short int max_power = af->a.soma.max_power;
  unsigned short int lvl_power = af->a.soma.lvl_power;

  unsigned short int attack = af->a.soma.attack;
  unsigned short int decay = af->a.soma.decay;
  unsigned short int sustain = af->a.soma.sustain;
  unsigned short int release = af->a.soma.release;

  switch (af->type)
    {
      /*    case SOMA_MUSCULAR_CRAMP:       sprintf(buf2,"a muscle cramp"); break;
         case SOMA_MUSCULAR_TWITCHING:      sprintf(buf2,"twitching"); break;
         case SOMA_MUSCULAR_TREMOR: sprintf(buf2,"tremors"); break;
         case SOMA_MUSCULAR_PARALYSIS:      sprintf(buf2,"paralysis"); break;
         case SOMA_DIGESTIVE_ULCER: sprintf(buf2,"stomach ulcer"); break; 
         case SOMA_DIGESTIVE_VOMITING:      sprintf(buf2,"vomiting"); break;
         case SOMA_DIGESTIVE_BLEEDING:      sprintf(buf2,"vomiting blood"); break; 
         case SOMA_EYE_BLINDNESS:   sprintf(buf2,"blindness"); break;
         case SOMA_EYE_BLURRED:             sprintf(buf2,"blurred vision"); break;
         case SOMA_EYE_DOUBLE:              sprintf(buf2,"double vision"); break;
         case SOMA_EYE_DILATION:            sprintf(buf2,"dilated pupils"); break; 
         case SOMA_EYE_CONTRACTION: sprintf(buf2,"contracted pupils"); break; 
         case SOMA_EYE_LACRIMATION: sprintf(buf2,"lacrimation"); break;
         case SOMA_EYE_PTOSIS:              sprintf(buf2,"ptosis"); break;
         case SOMA_EAR_TINNITUS:            sprintf(buf2,"tinnitus"); break;
         case SOMA_EAR_DEAFNESS:            sprintf(buf2,"deafness"); break;
         case SOMA_EAR_EQUILLIBRIUM:        sprintf(buf2,"ear imbalance"); break;
         case SOMA_NOSE_ANOSMIA:            sprintf(buf2,"anosmia"); break;
         case SOMA_NOSE_RHINITIS:   sprintf(buf2,"rhinitis"); break;
         case SOMA_MOUTH_SALIVATION:        sprintf(buf2,"salivation"); break;
         case SOMA_MOUTH_TOOTHACHE: sprintf(buf2,"toothache"); break;
         case SOMA_MOUTH_DRYNESS:   sprintf(buf2,"dry mouth"); break;
         case SOMA_MOUTH_HALITOSIS: sprintf(buf2,"halitosis"); break;
         case SOMA_CHEST_DIFFICULTY:        sprintf(buf2,"difficulty breathing"); break;
         case SOMA_CHEST_WHEEZING:  sprintf(buf2,"wheezing"); break;
         case SOMA_CHEST_RAPIDBREATH:       sprintf(buf2,"rapid breathing"); break;
         case SOMA_CHEST_SLOWBREATH:        sprintf(buf2,"shallow breathing"); break;
         case SOMA_CHEST_FLUID:             sprintf(buf2,"fluidous lungs"); break;
         case SOMA_CHEST_PALPITATIONS:      sprintf(buf2,"heart palpitations"); break; */

				case SOMA_MUSCULAR_CRAMP:
        case SOMA_MUSCULAR_TWITCHING:
				case SOMA_CHEST_COUGHING:
				case SOMA_CHEST_WHEEZING:
				case SOMA_NERVES_HEADACHE:

      if (minute <= attack)
	{
	  af->a.soma.atm_power = (max_power * minute) / attack;
	}
      else if (minute <= decay)
	{
	  af->a.soma.atm_power =
	    max_power -
									(((max_power - lvl_power) *
									(minute - attack)) /
									(decay - attack));
						}

      else if (minute <= sustain)
	{
	  af->a.soma.atm_power = lvl_power;
	}
      else if (minute <= release)
	{
	  af->a.soma.atm_power =
	    lvl_power -
									(((lvl_power) *
									(minute - sustain)) / 
									(release - sustain));
						}

      else
	{
	  affect_remove (ch, af);
	}
      break;

      /*
         case SOMA_CHEST_PNEUMONIA: sprintf(buf2,"pneumonia"); break;
         case SOMA_NERVES_PSYCHOSIS:        sprintf(buf2,"psychosis"); break;
         case SOMA_NERVES_DELIRIUM: sprintf(buf2,"delerium "); break;
         case SOMA_NERVES_COMA:             sprintf(buf2,"a comatose state"); break;
         case SOMA_NERVES_CONVULSIONS:      sprintf(buf2,"convulsions"); break;
         case SOMA_NERVES_HEADACHE: sprintf(buf2,"headache"); break;
         case SOMA_NERVES_CONFUSION:        sprintf(buf2,"confusion"); break;
         case SOMA_NERVES_PARETHESIAS:      sprintf(buf2,"parethesias"); break;
         case SOMA_NERVES_ATAXIA:   sprintf(buf2,"ataxia"); break;
         case SOMA_NERVES_EQUILLIBRIUM:     sprintf(buf2,"nervous imbalance"); break;
         case SOMA_SKIN_CYANOSIS:   sprintf(buf2,"cyanosis of the skin"); break;
         case SOMA_SKIN_DRYNESS:            sprintf(buf2,"dryness of the skin"); break;
         case SOMA_SKIN_CORROSION:  sprintf(buf2,"corrosion of the skin"); break;
         case SOMA_SKIN_JAUNDICE:   sprintf(buf2,"jaundice of the skin"); break;
         case SOMA_SKIN_REDNESS:            sprintf(buf2,"redness of the skin"); break;
         case SOMA_SKIN_RASH:               sprintf(buf2,"a rash on the skin"); break;
         case SOMA_SKIN_HAIRLOSS:   sprintf(buf2,"hairloss"); break;
         case SOMA_SKIN_EDEMA:              sprintf(buf2,"edema of the skin"); break;
         case SOMA_SKIN_BURNS:              sprintf(buf2,"burns on the skin"); break;
         case SOMA_SKIN_PALLOR:             sprintf(buf2,"pallor of the skin"); break;
         case SOMA_SKIN_SWEATING:   sprintf(buf2,"the sweats"); break;
         case SOMA_GENERAL_WEIGHTLOSS:      sprintf(buf2,"weight loss"); break;
         case SOMA_GENERAL_LETHARGY:        sprintf(buf2,"lethargy"); break;
         case SOMA_GENERAL_APPETITELOSS:    sprintf(buf2,"appetite loss"); break;
         case SOMA_GENERAL_PRESSUREDROP:    sprintf(buf2,"low blood pressure"); break;
         case SOMA_GENERAL_PRESSURERISE:    sprintf(buf2,"high blood pressure"); break;
         case SOMA_GENERAL_FASTPULSE:       sprintf(buf2,"a fast pulse"); break;
         case SOMA_GENERAL_SLOWPULSE:       sprintf(buf2,"a slow pulse"); break;
         case SOMA_GENERAL_HYPERTHERMIA:    sprintf(buf2,"hyperthermia"); break;
         case SOMA_GENERAL_HYPOTHERMIA:     sprintf(buf2,"hypothermia"); break;
       */
    default:
      break;
    }
}

int
lookup_soma (char *argument)
{
	if (!argument)
		return (-1);
		
	if (!strcmp(argument, "cramps"))
		return (SOMA_MUSCULAR_CRAMP);
	
	else if (!strcmp(argument,"twitching")) 
		return (SOMA_MUSCULAR_TWITCHING);
		
	else if (!strcmp(argument, "cough"))
		return (SOMA_CHEST_COUGHING);
		
	else if (!strcmp(argument, "wheeze"))
		return (SOMA_CHEST_WHEEZING);
		
	else if (!strcmp(argument, "headache"))
		return (SOMA_NERVES_HEADACHE);
	
	else
		return (-1);
	
}
