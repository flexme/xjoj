{include file="header.tpl"}
    {literal}
    <style type="text/css">
    .st0{font-size:12px;}
    .st1{font-size:12px;}
    </style>
    {/literal}
    <div id="main-left">
        {include file="contest_left.tpl"}
    </div>
    <div id="main-right">
        <div class="rectdiv">
            <div class="cat-head">::Standing</div>
            <div style="text-align:center;">
                <h3>Contest Standing -- {$contest.title} {$snap_info}</h3>
                <table border="0" cellpadding="0" cellspacing="1" align="center" style="width:100%;margin:0 auto;margin-bottom:10px;">
                <tr>
                    <td class="th">Rank</td>
                    <td class="th">User</td>
                    <td class="th">AC</td>
                    <td class="th">Penalty</td>
                    <!--{foreach from=$problems item=p}-->
                    <td class="th"><a href="contestProblem.php?cid={$contest.id}&pid={$p.problemId}">{$p.name}</a></td>
                    <!--{/foreach}-->
                </tr>
                <!--{foreach from=$users item=user key=i}-->
                <tr class="st{$i%2}">
                    <td>{$i+1}</td>
                    <td><a href="user.php?act=detail&authorId={$user.user_id}">{$user.username}{if $user.truename}({$user.truename}){/if}</a></td>
                    <td><a href="status.php?cid={$contest.id}&authorId={$user.user_id}">{$user.acceptes}</a></td>
                    <td>{$user.penaltyTime}</td>
                    {foreach from=$problems item=p}
                    <td>
                    {if $user.problems[$p.problemId]}
                    {if $user.problems[$p.problemId].acTime} {$user.problems[$p.problemId].acTime}
                    {if $user.problems[$p.problemId].totSubmit gt 0}
                    <br/>(-{$user.problems[$p.problemId].totSubmit})
                    {/if}
                    {else}
                    {if $user.problems[$p.problemId].totSubmit gt 0}
                    (-{$user.problems[$p.problemId].totSubmit})
                    {/if}
                    {/if}
                    {/if}
                    </td>
                    {/foreach}
                <!--{/foreach}-->
                </table>

            </div>
        </div>
    </div>
    <div class="clear"></div>
</div>
{include file="footer.tpl"}

