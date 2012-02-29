ALTER TABLE status CHANGE `cid` `cid` INT(10) NOT NULL DEFAULT '0';

ALTER TABLE author CHANGE `ac_submit` `ac_total` INT(10) UNSIGNED NOT NULL DEFAULT '0';

ALTER TABLE contest ADD `private` TINYINT(1) NOT NULL DEFAULT '0';

ALTER TABLE author ADD `super` TINYINT(1) NOT NULL DEFAULT '0';

CREATE TABLE `contest_submit` (
  `problemId` int(10) NOT NULL,
  `user_id` int(10) NOT NULL,
  `contest_id` int(10) NOT NULL,
  `ac_time` int(12) NOT NULL DEFAULT '0',
  `submits` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`problemId`,`user_id`,`contest_id`),
  KEY `user_id` (`user_id`),
  KEY `contest_id` (`contest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE `contest_standing` (
  `contest_id` int(10) NOT NULL,
  `user_id` int(10) NOT NULL,
  `ac_total` int(5) NOT NULL,
  `penalty` bigint(15) NOT NULL,
  PRIMARY KEY (`contest_id`,`user_id`),
  KEY `contest_id` (`contest_id`),
  KEY `user_id` (`user_id`),
  KEY `contest_id_2` (`contest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS `assemb` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `title` varchar(50) collate utf8_bin NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

CREATE TABLE IF NOT EXISTS `assemb_problem` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `assembId` int(10) NOT NULL,
  `problemId` int(10) NOT NULL,
  `level` int(5) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

ALTER TABLE  `status` ADD INDEX (  `judgeStatus` );
ALTER TABLE  `status` ADD INDEX (  `problemId` );
ALTER TABLE  `status` ADD INDEX (  `languageId` );
ALTER TABLE  `status` ADD INDEX (  `authorId` );
ALTER TABLE  `status` ADD INDEX (  `cid` );
ALTER TABLE  `status` ADD INDEX (  `poolStatus` );

ALTER TABLE  `problem` ADD  `total_submit` INT( 10 ) NOT NULL ,
ADD  `ac_total` INT( 10 ) NOT NULL ;
ALTER TABLE  `problem` ADD INDEX (  `show` );

ALTER TABLE  `author` ADD  `truename` VARCHAR( 50 ) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL AFTER  `username` ;

ALTER TABLE  `contest` ADD  `private` TINYINT( 1 ) NOT NULL ;

