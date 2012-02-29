#重新统计某个用户的ac题目数
#参数：uid 要统计的用户id
DROP PROCEDURE IF EXISTS update_user_ac;
DELIMITER |
CREATE PROCEDURE update_user_ac(uid BIGINT)
BEGIN
    DECLARE c BIGINT;
    DECLARE solved_cur CURSOR FOR SELECT COUNT(DISTINCT problemId) FROM status WHERE judgeStatus = 'Accepted' AND authorId = uid;

    OPEN solved_cur;
    FETCH solved_cur INTO c;
    CLOSE solved_cur;

    UPDATE author SET ac_total = c WHERE user_id = uid;
END
|
DELIMITER ;

#重新统计某个题目的ac次数
#参数：pid 要统计的题目id
DROP PROCEDURE IF EXISTS update_problem_ac;
DELIMITER |
CREATE PROCEDURE update_problem_ac(pid BIGINT)
BEGIN
    DECLARE c BIGINT;
    DECLARE solved_cur CURSOR FOR SELECT COUNT(*) FROM status WHERE judgeStatus = 'Accepted' AND problemId = pid;

    OPEN solved_cur;
    FETCH solved_cur INTO c;
    CLOSE solved_cur;

    UPDATE problem SET ac_total = c WHERE problemId = pid;
END
|
DELIMITER ;

#重新统计每个用户的总提交次数和总ac题目数量
DROP PROCEDURE IF EXISTS refresh_user_ac_submit;
DELIMITER |
CREATE PROCEDURE refresh_user_ac_submit()
BEGIN
    DECLARE done INT DEFAULT 0;
    DECLARE uid, c BIGINT;
    DECLARE solved_cur CURSOR FOR SELECT COUNT(DISTINCT problemId), authorId FROM status WHERE judgeStatus = 'Accepted' GROUP BY authorId;
    DECLARE total_cur CURSOR FOR SELECT COUNT(*), authorId FROM status GROUP BY authorId;
    DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;

    OPEN solved_cur;
    REPEAT
        FETCH solved_cur INTO c, uid;
        UPDATE author SET ac_total = c WHERE user_id = uid;
    UNTIL done END REPEAT;
    CLOSE solved_cur;

    SET done = 0;
    OPEN total_cur;
    REPEAT
        FETCH total_cur INTO c, uid;
        UPDATE author SET total_submit = c WHERE user_id = uid;
    UNTIL done END REPEAT;
    CLOSE total_cur;
END
|
DELIMITER ;

#重新统计每个题目的总提交次数和总ac次数
DROP PROCEDURE IF EXISTS refresh_problem_ac_submit;
DELIMITER |
CREATE PROCEDURE refresh_problem_ac_submit()
BEGIN
    DECLARE done INT DEFAULT 0;
    DECLARE pid, c BIGINT;
    DECLARE solved_cur CURSOR FOR SELECT COUNT(*), problemId FROM status WHERE judgeStatus = 'Accepted' GROUP BY problemId;
    DECLARE total_cur CURSOR FOR SELECT COUNT(*), problemId FROM status GROUP BY problemId;
    DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;

    OPEN solved_cur;
    REPEAT
        FETCH solved_cur INTO c, pid;
        UPDATE problem SET ac_total = c WHERE problemId = pid;
    UNTIL done END REPEAT;
    CLOSE solved_cur;

    SET done = 0;
    OPEN total_cur;
    REPEAT
        FETCH total_cur INTO c, pid;
        UPDATE problem SET total_submit = c WHERE problemId = pid;
    UNTIL done END REPEAT;
    CLOSE total_cur;
END
|
DELIMITER ;

#当用户提交一条比赛评测时，此存储过程更新相应的contest_submit表和contest_standing表
#参数：
#uid  提交代码的用户id
#pid  提交代码对应的题目id
#cid  提交代码所对应的比赛id
DROP PROCEDURE IF EXISTS user_contest_submit;
DELIMITER |
CREATE PROCEDURE user_contest_submit(uid BIGINT, pid BIGINT, cid INT)
BEGIN
    DECLARE p_ac_time, p_submits BIGINT DEFAULT 0;
    DECLARE t_uid BIGINT DEFAULT 0;

    SELECT ac_time,submits INTO p_ac_time,p_submits FROM contest_submit WHERE problemId = pid AND user_id = uid AND contest_id = cid;
    SELECT user_id INTO t_uid FROM contest_standing WHERE contest_id = cid AND user_id = uid;

    IF (t_uid = 0) THEN
        REPLACE INTO contest_standing (contest_id,user_id,ac_total,penalty) VALUES (cid,uid,0,0);
    END IF;

    IF (p_ac_time = 0) THEN
        REPLACE INTO contest_submit (problemId,user_id,contest_id,submits) VALUES (pid,uid,cid,p_submits+1);
    END IF;
END
|
DELIMITER ;

#当用户提交的比赛代码ac时，调用此存储过程更新相应的contest_submit表和contest_standing表
#参数:
#uid  提交代码的用户id
#pid  提交代码对应的题目id
#cid  提交代码所对应的比赛id
#stime 提交代码的时间
DROP PROCEDURE IF EXISTS user_contest_ac;
DELIMITER |
CREATE PROCEDURE user_contest_ac(uid BIGINT, pid BIGINT, cid INT, stime BIGINT)
BEGIN
    DECLARE p_ac_time,p_submits BIGINT DEFAULT 0;
    
    SELECT ac_time,submits INTO p_ac_time,p_submits FROM contest_submit WHERE problemId = pid AND user_id = uid AND contest_id = cid;

    IF (p_ac_time = 0) THEN
        BEGIN
            UPDATE contest_submit SET ac_time = stime WHERE problemId = pid AND user_id = uid AND contest_id = cid;
            UPDATE contest_standing SET ac_total = ac_total + 1, penalty = penalty + stime + (p_submits-1)*1200 WHERE contest_id = cid AND user_id = uid;
        END;
    END IF;
END
|
DELIMITER ;

#rejudge一条记录
#参数：rid 运行记录id
DROP PROCEDURE IF EXISTS rejudge_runId;
DELIMITER |
CREATE PROCEDURE rejudge_runId(rid BIGINT)
BEGIN
    DECLARE pid, uid BIGINT DEFAULT 0;
    DECLARE ccid INT DEFAULT 0;
    DECLARE jstatus VARCHAR(50);
    DECLARE stime BIGINT;
    DECLARE ac_tot BIGINT DEFAULT 0;

    SELECT problemId, authorId, judgeStatus, cid, submitTime INTO pid, uid, jstatus, ccid, stime FROM status WHERE runId = rid;
    IF (ccid > 0 AND jstatus = 'Accepted') THEN
        BEGIN
            SELECT COUNT(*) INTO ac_tot FROM status WHERE problemId = pid AND authorId = uid AND cid = ccid AND judgeStatus = 'Accepted';
            IF (ac_tot = 1) THEN
                UPDATE contest_submit SET ac_time = 0 WHERE problemId = pid AND user_id = uid AND contest_id = ccid;
                UPDATE contest_standing SET `ac_total` = `ac_total` - 1, penalty = penalty - stime WHERE contest_id = ccid AND user_id = uid;
            END IF;
        END;
    END IF;
    UPDATE status set judgeStatus='Waiting',poolStatus='0',`case`=0,`memory`=0,`time`=0,errInfo='' WHERE runId=rid;
END
|
DELIMITER ;

#rejudge一个题目
#参数：pid 题目id
DROP PROCEDURE IF EXISTS rejudge_problem;
DELIMITER |
CREATE PROCEDURE rejudge_problem(pid BIGINT)
BEGIN
    DECLARE done INT DEFAULT 0;
    DECLARE rid BIGINT;
    DECLARE rid_cur CURSOR FOR SELECT runId FROM status WHERE problemId = pid;
    DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;

    OPEN rid_cur;
    REPEAT
        FETCH rid_cur INTO rid;
        CALL rejudge_runId(rid);
    UNTIL done END REPEAT;
    CLOSE rid_cur;
END
|
DELIMITER ;

#rejudge一个比赛
#参数：cid 比赛id
DROP PROCEDURE IF EXISTS rejudge_contest;
DELIMITER |
CREATE PROCEDURE rejudge_contest(ccid INT)
BEGIN
    UPDATE contest_submit SET ac_time = 0 WHERE contest_id = ccid;
    UPDATE contest_standing SET penalty = 0, ac_total = 0 WHERE contest_id = ccid;
    UPDATE status SET judgeStatus = 'Waiting', poolStatus = '0', `time` = 0, `memory` = 0, `case` = 0, `errInfo` = '' WHERE cid = ccid;
END
|
DELIMITER ;

#当用户提交一条运行记录时（往status表插入）调用此触发器
DROP TRIGGER IF EXISTS submission_insert;
DELIMITER | 
CREATE TRIGGER submission_insert
AFTER INSERT ON status
FOR EACH ROW BEGIN
    UPDATE author SET total_submit = total_submit + 1 WHERE user_id = NEW.authorId;
    UPDATE problem SET total_submit = total_submit + 1 WHERE problemId = NEW.problemId;
    IF (NEW.cid > 0) THEN
        CALL user_contest_submit(NEW.authorId, NEW.problemId, NEW.cid);
    END IF;
END
|
DELIMITER ;

#当status表更新时调用此存储过程
DROP TRIGGER IF EXISTS status_update; 
DELIMITER |
CREATE TRIGGER status_update
AFTER UPDATE ON status
FOR EACH ROW BEGIN
    IF (NEW.judgeStatus != OLD.judgeStatus) THEN
    BEGIN
        IF (NEW.judgeStatus = 'Accepted' OR OLD.judgeStatus = 'Accepted') THEN
            CALL update_user_ac(NEW.authorId);
            CALL update_problem_ac(NEW.problemId);
        END IF;

        IF (NEW.cid > 0 AND NEW.judgeStatus = 'Accepted') THEN
            CALL user_contest_ac(NEW.authorId, NEW.problemId, NEW.cid, NEW.submitTime);
        END IF;
    END;
    END IF;
END
|
DELIMITER ;

