# Makefile for RPI Engine, $Revision: 1.7 $
# Copyright (C) 2006, Shadows of Isildur

#.PHONY: rpi_engine

rpi_engine:
	$(MAKE) -C src/


clean:
	rm -f src/*.o bin/server

install: install-libdir install-worldfile install-mysql

install-libdir:
	echo "Installing lib Hierarchy"
	install -dv lib/{tickets,text/chargen,mobprogs,save}
	install -dv lib/save/{objects,mobiles,reboot}
	install -dv lib/save/{player,objs}/{a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z}
	install -v --mode=644 generic/lib/text/actions lib/text/actions 
	install -v --mode=644 generic/lib/text/greetings lib/text/greetings 
	install -v --mode=644 generic/lib/text/menu1 lib/text/menu1 

install-worldfile:
	echo "Installing the regions Heirarchy"
	install -dv regions
	install -v --mode=644 generic/regions/registry regions/registry
	install -v --mode=644 generic/regions/dynamic_registry regions/dynamic_registry
	cd regions && bash ../generic/regions/zone_files.sh && cd -

install-mysql:
	$(MAKE) -C generic/sql

