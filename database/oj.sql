-- phpMyAdmin SQL Dump
-- version 2.9.1-rc1
-- http://www.phpmyadmin.net
-- 
-- 主机: localhost
-- 生成日期: 2009 年 02 月 20 日 19:28
-- 服务器版本: 5.1.30
-- PHP 版本: 5.2.8
-- 
-- 数据库: `oj`
-- 

-- --------------------------------------------------------

-- 
-- 表的结构 `admin`
-- 

CREATE TABLE `admin` (
  `user_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(50) COLLATE utf8_bin NOT NULL,
  `password` varchar(40) COLLATE utf8_bin NOT NULL,
  `last_login` int(10) DEFAULT NULL,
  `last_ip` varchar(20) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin ;

-- --------------------------------------------------------

-- 
-- 表的结构 `author`
-- 

CREATE TABLE `author` (
  `user_id` int(10) NOT NULL AUTO_INCREMENT,
  `username` varchar(50) COLLATE utf8_bin NOT NULL,
  `password` varchar(50) COLLATE utf8_bin NOT NULL,
  `email` varchar(50) COLLATE utf8_bin NOT NULL,
  `plan` text COLLATE utf8_bin NOT NULL,
  `ac_submit` int(10) unsigned NOT NULL DEFAULT '0',
  `total_submit` int(10) unsigned NOT NULL DEFAULT '0',
  `defaultLanguageId` int(10) NOT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin ;

-- --------------------------------------------------------

-- 
-- 表的结构 `contest`
-- 

CREATE TABLE `contest` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(50) COLLATE utf8_bin NOT NULL,
  `startTime` int(11) NOT NULL,
  `endTime` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

-- 
-- 表的结构 `contest_problem`
-- 

CREATE TABLE `contest_problem` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `problemId` int(10) NOT NULL,
  `contestId` int(10) NOT NULL,
  `sort_order` int(5) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `contestId` (`contestId`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

-- 
-- 表的结构 `language`
-- 

CREATE TABLE `language` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `displayName` varchar(50) COLLATE utf8_bin NOT NULL,
  `compileCmd` varchar(100) COLLATE utf8_bin NOT NULL,
  `executeCmd` varchar(100) COLLATE utf8_bin NOT NULL,
  `ext` varchar(10) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

-- 
-- 表的结构 `news`
-- 

CREATE TABLE `news` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(50) COLLATE utf8_bin NOT NULL,
  `content` text COLLATE utf8_bin NOT NULL,
  `addtime` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

-- 
-- 表的结构 `problem`
-- 

CREATE TABLE `problem` (
  `problemId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `problemName` varchar(50) COLLATE utf8_bin NOT NULL,
  `description` text COLLATE utf8_bin NOT NULL,
  `timelimit` int(10) NOT NULL,
  `memorylimit` int(10) NOT NULL,
  `input` text COLLATE utf8_bin NOT NULL,
  `output` text COLLATE utf8_bin NOT NULL,
  `sample_input` text COLLATE utf8_bin NOT NULL,
  `sample_output` text COLLATE utf8_bin NOT NULL,
  `hint` text COLLATE utf8_bin NOT NULL,
  `spj` tinyint(1) NOT NULL,
  `show` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`problemId`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

-- 
-- 表的结构 `source_code`
-- 

CREATE TABLE `source_code` (
  `runId` int(10) NOT NULL,
  `code` blob NOT NULL,
  PRIMARY KEY (`runId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

-- 
-- 表的结构 `status`
-- 

CREATE TABLE `status` (
  `problemId` int(10) NOT NULL,
  `runId` int(10) NOT NULL AUTO_INCREMENT,
  `languageId` int(10) NOT NULL,
  `authorId` int(10) NOT NULL,
  `judgeStatus` varchar(50) COLLATE utf8_bin NOT NULL,
  `case` int(10) NOT NULL,
  `memory` int(10) NOT NULL,
  `time` int(10) NOT NULL,
  `submitTime` int(10) NOT NULL,
  `errInfo` blob NOT NULL,
  `cid` tinyint(1) NOT NULL DEFAULT '0',
  `poolStatus` int(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`runId`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin ;


INSERT INTO `language` (`id`, `displayName`, `compileCmd`, `executeCmd`, `ext`) VALUES 
(1, 0x672b2b, 0x672b2b202d444f4e4c494e455f4a55444745202d4f32202d57616c6c202d6c6d202d6f207b3a626173656e616d657d207b3a6d61696e66696c657d, 0x2e2f7b3a626173656e616d657d, 0x637070),
(2, 0x676363, 0x676363202d444f4e4c494e455f4a55444745202d4f32202d57616c6c202d6c6d202d6f207b3a626173656e616d657d207b3a6d61696e66696c657d, 0x2e2f7b3a626173656e616d657d, 0x63),
(3, 0x4a617661, 0x6a61766163204d61696e2e6a617661, 0x6a617661204d61696e, 0x6a617661);

