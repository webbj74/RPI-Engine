#!/bin/bash

#for i in `seq  0 99`;
for i in `i=0; i=99; i=i+1`;
do 

  # Initialize Rooms
  if [ ! -f rooms.$i ]; then
    echo -e "#"$[i*1000]"\nZone Base Room~\n  This is a base room for an undeveloped area. Ensure you are building\nin the right zone\041\n~\n92 65544 0\n0\nS\n$~" >> rooms.$i; 
  fi

  # Initialize Mobiles
  if [ ! -f mobs.$i ]; then
    echo -e "$~" >> mobs.$i; 
  fi

  # Initialize Objects
  if [ ! -f objs.$i ]; then
    echo -e "$~" >> objs.$i; 
  fi

  # Initialize Resets
  if [ ! -f resets.$i ]; then
    echo -e "#"$i"\nLead: (null)~\nEmpty Zone~\n0 0 0 1 0 0 0\nS" >> resets.$i; 
  fi

done;
