{include file="header.tpl"}
	<div id="main-left">
		{include file="main_left.tpl"}
	</div>
	<div id="main-right">
		<div class="rectdiv">
			<div class="cat-head">::Message</div>
			<div style="padding:10px;text-align:center;">
				<p>-- {$message.content}</p>
				<!-- {if $message.link} -->
				<span>>> <a href="{$message.href}">{$message.link}</a></span>
				<!-- {/if} -->
			</div>
		</div>
	</div>
	<div style="clear:both;"></div>
</div>
{include file="footer.tpl"}

