# Makefile for RPI Engine, $Revision: 1.2 $
# Copyright (C) 2006, Shadows of Isildur

.PHONY: install-mysql

install-mysql:
	echo "Installing MySQL Databases\nPlease enter your root password:\n"
	/usr/local/mysql/bin/mysql --user=root mysql -p < init.sql
	/usr/local/mysql/bin/mysql --user=root mysql -p < load.sql
	/usr/local/mysql/bin/mysql --user=root mysql -p < helpfile.sql 
	/usr/local/mysql/bin/mysql --user=root mysql -p < server_logs.sql
	

