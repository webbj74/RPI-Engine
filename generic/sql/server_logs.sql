-- phpMyAdmin SQL Dump
-- version 2.9.0
-- http://www.phpmyadmin.net
-- 
-- Host: localhost
-- Generation Time: Nov 17, 2006 at 02:55 AM
-- Server version: 4.1.8
-- PHP Version: 5.2.0
-- 
-- Database: `server_logs`
-- 

-- --------------------------------------------------------

\u server_logs
-- 
-- Table structure for table `ip`
-- 

CREATE TABLE IF NOT EXISTS `ip` (
  `account` varchar(32) NOT NULL default '',
  `host` varchar(255) default NULL,
  `ip` varchar(32) NOT NULL default '',
  `firsttime` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `lasttime` timestamp NOT NULL default '0000-00-00 00:00:00',
  `count` int(11) default '0',
  `is_new` tinyint(1) default '0',
  `has_pwd` tinyint(1) default '0',
  `port` int(11) NOT NULL default '0',
  `logins` int(11) NOT NULL default '0',
  `fails` int(11) NOT NULL default '0',
  PRIMARY KEY  (`account`,`ip`,`port`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

-- 
-- Table structure for table `loggers`
-- 

CREATE TABLE IF NOT EXISTS `loggers` (
  `timestamp` timestamp NOT NULL default '0000-00-00 00:00:00',
  `account` varchar(32) default NULL,
  `squrl` text,
  `ip` varchar(32) default NULL,
  `devsite` tinyint(1) default NULL,
  KEY `acct_ts` (`account`(4),`timestamp`),
  KEY `account` (`account`(4),`timestamp`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

-- 
-- Table structure for table `mud`
-- 

CREATE TABLE IF NOT EXISTS `mud` (
  `name` varchar(255) default NULL,
  `account` varchar(255) default NULL,
  `switched_into` varchar(255) default NULL,
  `timestamp` int(11) default '0',
  `port` int(11) default '4500',
  `room` int(11) default '-1',
  `guest` tinyint(1) default '0',
  `immortal` tinyint(1) default '0',
  `error` tinyint(1) default '0',
  `command` varchar(255) default NULL,
  `entry` text,
  `sha_hash` varchar(45) default NULL,
  KEY `ts_idx` (`timestamp`),
  KEY `room_idx` (`room`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

-- 
-- Table structure for table `receipts`
-- 

CREATE TABLE IF NOT EXISTS `receipts` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `shopkeep` mediumint(8) unsigned NOT NULL default '0',
  `transaction` enum('unknown','sold','bought') NOT NULL default 'unknown',
  `who` varchar(32) default NULL,
  `customer` varchar(255) default NULL,
  `vnum` mediumint(8) unsigned NOT NULL default '0',
  `item` varchar(80) default NULL,
  `qty` tinyint(3) unsigned NOT NULL default '0',
  `cost` mediumint(8) unsigned NOT NULL default '0',
  `room` mediumint(8) unsigned NOT NULL default '0',
  `gametime` datetime default NULL,
  `port` smallint(5) unsigned NOT NULL default '4500',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=237127 ;

-- 
-- Table structure for table `payroll`
-- 

CREATE TABLE IF NOT EXISTS `payroll` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `shopkeep` mediumint(8) unsigned NOT NULL default '0',
  `who` varchar(32) default NULL,
  `customer` varchar(255) default NULL,
  `amount` mediumint(8) unsigned NOT NULL default '0',
  `room` mediumint(8) unsigned NOT NULL default '0',
  `gametime` datetime default NULL,
  `port` smallint(5) unsigned NOT NULL default '4500',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

