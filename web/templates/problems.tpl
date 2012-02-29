{include file="header.tpl"}
	<div id="main-left">
		{include file="main_left.tpl"}
	</div>
	<div id="main-right">
		<div class="rectdiv">
			<div class="cat-head">::{if $action eq show_volumes}Volumes{else}Problemset Archive{/if}</div>
			<!-- {if $action eq show_problems or $action eq show_asm_problems} -->
			<div style="text-align:center;padding-bottom:10px;">
				<h4 style="margin:0;padding:10px;">
				<!-- {if $action eq show_problems} -->
				volume: {$volume}
				<!-- {else} -->
				Assemble: {$assemb_title}
				<!-- {/if} -->
				</h4>
				<div style="text-align:center;">
					<table align="center" width="90%" border="0" cellpadding="1" cellspacing="1" style="font-size:16px;">
					<tr>
                        {if $login_user}<td class="th" width="5%"></td>{/if}
						<td class="th" width="10%"><a href="?act={$action}&assembId={$assembId}&order=id">ID</a></td>
						<td class="th">TITLE</td>
						<td width="15%" class="th">ACCEPTED</td>
						<td width="15%" class="th">TOTAL</td>
						{if $action eq show_asm_problems}<td width="25px" class="th"><a href="?act=show_asm_problems&assembId={$assembId}&order=level">Level</a></td>{/if}
					</tr>
					<!-- {foreach from=$problems key=i item=problem} -->
					<tr class="st{$i%2}">
                        {if $login_user}<td>{if $problem.solved}<font color="red">OK</span>{/if}</td>{/if}
						<td><div>{if $problem.spj}<span>S</span>{/if}<a href="problem_html/{$problem.problemId}.html">{$problem.problemId}</a></div></td>
						<td align="left">{$problem.problemName}</td>
						<td><a href="status.php?proId={$problem.problemId}&status=Accepted">{$problem.accepted}</a></td>
						<td><a href="status.php?proId={$problem.problemId}">{$problem.total}</a></td>
						{if $action eq show_asm_problems}<td><em class="rank_star"><a class="rank{$problem.level}"></a></em></td>{/if}
					</tr>
					<!-- {/foreach} -->
					</table>
				</div>
			</div>
			<!-- {elseif $action eq show_volumes} -->
			<div style="text-align:center;padding-bottom:10px;">
				<h3>Volumes</h3>
				<!--{foreach from=$volumes_range key=i item=r}-->
				<p>Volume ({$r.minr} - {$r.maxr})  <a href="?act=show_problems&volume={$i}">[{$i}]</a>
				<!--{/foreach}-->
				<!-- {if $assemb} -->
				<h3>Assemble</h3>
				<!-- {foreach from=$assemb item=asm} -->
				<p><a href="problems.php?act=show_asm_problems&assembId={$asm.id}">{$asm.title}</a></p>
				<!-- {/foreach} -->
				<!-- {/if} -->
			</div>
			<!-- {/if} -->
		</div>
	</div>
	<div class="clear"></div>
</div>
{include file="footer.tpl"}

