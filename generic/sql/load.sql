----------------------------------------------------------------------------
--  GRANT the MySQL User
----------------------------------------------------------------------------
--GRANT ALL ON `shadows`.* 
--TO 'root'@'localhost' 
--IDENTIFIED BY PASSWORD '3682201c3daa7588';

--GRANT ALL ON `shadows_pfiles`.* 
--TO 'root'@'localhost'
--IDENTIFIED BY PASSWORD '3682201c3daa7588';

--GRANT ALL ON `server_logs`.* 
--TO 'root'@'localhost'
--IDENTIFIED BY PASSWORD '3682201c3daa7588';

----------------------------------------------------------------------------
--  INSERT the God Account. IGNORE avoids erroring out if the acct exisits.
----------------------------------------------------------------------------
\u shadows
LOCK TABLES `forum_users` WRITE;
INSERT IGNORE INTO `forum_users` VALUES (6995,1,'God',0,'CRMnbdXMhXd/Q','localhost.localdomain',1,1,0,0,0,NOW(),0,0,'0.00',NULL,NULL,'d M Y H:i',0,0,0,NULL,0,NULL,1,1,1,1,1,1,0,1,1,0,NULL,0,'your.email.here@yourmud.org',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,0,0,0,NULL,0);
UNLOCK TABLES;

----------------------------------------------------------------------------
--  INSERT the God Avatar. IGNORE avoids erroring out if the acct exisits.
----------------------------------------------------------------------------
\u shadows_pfiles
LOCK TABLES `pfiles` WRITE;
INSERT IGNORE INTO `pfiles` VALUES (
'God',
'glowing infinite entity God Implementor',
'God',
'a glowing, infinite entity',
'A glowing, infinite entity reposes here in finite form.',
'   Before you is an overworked, underpaid Implementor. \n',
'~',
'none\n',
2,0,0,
'~','~','~','~',
0,0,
5,1,0,1,0,
25,25,25,25,25,25,25,
25,25,25,25,25,25,25,
0,UNIX_TIMESTAMP(NOW()),0,
50,250,250,0,180,180,0,0,2,1,1,0,0,
0,0,0,0,0,1,0,'(null)',75,0,-1,-1,78,3,0,0,
0,100,100,0,0,0,0,0,0,
'(null)','(null)',
'(null)','(null)',
'(null)','(null)',
'~','~',0,29,0,'(null)',0,0,0,0,0,'(null)','(null)');
UNLOCK TABLES;
