{include file="header.tpl"}
    {literal}
    <style type="text/css">
    .row0{background:#FFFFFF;height:25px;}
    .row1{background:#E9EFF8;height:25px;}
    </style>
    {/literal}
    <div id="main-left">
        {include file="main_left.tpl"}
    </div>
    <div id="main-right">
        <div class="rectdiv">
            <div class="cat-head">::Problem Status</div>
            <div style="text-align:left;padding:10px;">
                <center>
                <a href="problem_html/{$problemId}.html">{$problemId} -- {$problemName}</a>
                </center>
                <table style="border:none;border-collapse:separate;width:100%;margin:10px 0;">
                    <tr style="background:#CCCCFF;height:25px;width:100%;text-align:center;color:red;">
                        <td style="color:blue;">AC</td>
                        <td>WA</td>
                        <td>PE</td>
                        <td>FPE</td>
                        <td>SF</td>
                        <td>RE</td>
                        <td>TLE</td>
                        <td>MLE</td>
                        <td>CE</td>
                        <td style="color:blue;">Total</td>
                    </tr>
                    <tr>
                    <!-- {foreach from=$needs_count key=ename item=fname} -->
                        <td align="center"><a style="text-decoration:none;" href="status.php?proId={$problemId}&status={$fname}">{$counts.$ename.total}({$counts.$ename.percent}%)</a></td>

                    <!-- {/foreach} -->
                    <td align="center"><font color="blue">{$counts.total}</font></td>
                    </tr>
                </table>
                <p>Top Submssions by <a href="problemStatus.php?proId={$problemId}&order={$next_order}">{$next_order_desc}</a></p>
                <table style="border:none;border-collapse:separate;width:100%;text-align:center;">
                <tr style="background:#CCCCFF;height:25px;">
                    <td>RunId</td>
                    <td>Submit Time</td>
                    <td>Language</td>
                    <td><a style="text-decoration:none;" href="problemStatus.php?proId={$problemId}&order=time">Run Time(MS)</a></td>
                    <td><a style="text-decoration:none;" href="problemStatus.php?proId={$problemId}&order=memory">Run Memory(KB)</a></td>
                    <td>User Name</td>
                </tr>
                <!-- {foreach from=$status key=i item=sinfo} -->
                <tr {if $i%2}class="row0"{else}class="row1"{/if}>
                    <td>{if $sinfo.total gt 1}
                    <a href="status.php?proId={$problemId}&authorId={$sinfo.authorId}&status=Accepted">{$sinfo.runId}({$sinfo.total})</a>{else}{$sinfo.runId}{/if}</td>
                    <td>{$sinfo.submitTime|date_format:"%Y-%m-%d %H:%M:%S"}</td>
                    <td>{$sinfo.displayName}</td>
                    <td>{$sinfo.time}</td>
                    <td>{$sinfo.memory}</td>
                    <td><a href="user.php?act=detail&authorId={$sinfo.authorId}">{$sinfo.username}</a></td>
                </tr>
                <!-- {/foreach} -->
                </table>

            </div>
        </div>
    </div>
    <div class="clear"></div>
</div>
{include file="footer.tpl"}

