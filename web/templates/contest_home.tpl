{include file="header.tpl"}
    {literal}
    <style type="text/css">
    .time{color:green;}
    </style>
    {/literal}
    <div id="main-left">
        {include file="contest_left.tpl"}
    </div>
    <div id="main-right">
        <div class="rectdiv">
            <div class="cat-head">::Contest</div>
            <div style="text-align:center;">
		<h3>Contest -- {$contest.title}</h3>
                <div>Start time:  <span class="time">{$contest.startTime|date_format:"%Y-%m-%d %H:%M:%S"}</span>  End time:  <span class="time">{$contest.endTime|date_format:"%Y-%m-%d %H:%M:%S"}</span></div>
                <div>Current System Time:  <span class="time">{$smarty.now|date_format:"%Y-%m-%d %H:%M:%S"}</span> Contest Status: <font color="red">{$contest.status}</font></div>                

                <!--{if $problems}-->
                <table border="0" cellpadding="3" cellspacing="1" align="center" style="width:80%;font-size:16px;margin:15px auto;">
                <tr>
                    <td class="th" width="30%">Problem ID</td>
                    <td class="th">Title</td>
                </tr>
                {foreach from=$problems key=i item=p}
                <tr class="st{$i%2}">
                    <td><strong> Problem {$p.name}</strong></td>
                    <td align="left"><a href="contestProblem.php?cid={$contest.id}&pid={$p.problemId}">{$p.problemName}</a></td>
                </tr>
                {/foreach}
                </table>
                <!--{/if}-->
            </div>
        </div>
    </div>
    <div class="clear"></div>
</div>
{include file="footer.tpl"}

