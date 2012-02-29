{include file="header.tpl"}
    {literal}
    <script type="text/javascript">
    function gop() {
        var p=document.getElementById('proId').value;
        if(p&&p==parseInt(p))location.href='problem_html/'+p+'.html';
    }
    function keyd(e) {
        var eve=e?e:window.event;
        if(eve.keyCode==13) gop();
    } 
    </script>
    {/literal}
	<div id="main-left">
        {include file="main_left.tpl"}
	</div>
	<div id="main-right">
		<div class="rectdiv">
			<div class="cat-head">::Welcome</div>
			<div style="text-align:center;">
				<h2 style="color:#99BBE8;padding-top:10px;">Welcome To {#webname#}</h2>
				<div style="text-align:left;padding:10px;">
					<p>Problem Set is the place where you can find large amount of problems from different programming contests.Online Judge System allows you to test your solution for every problem.
					<p>First of all, read carefully <a href="faq.php">Frequently Asked Questions.</a>
					<p>Then, choose a <a href="problems.php">problem</a>, solve it and <a href="submit.php">submit</a> your solution.
					<p>Problem:<input type="text" id="proId" onkeyup="keyd(event);"/><input type="button" value="Go" onclick="gop()"/></p>
					<div>
						<div style="background:#F2F6FB none repeat scroll 0 0;border:1px solid #CCCCCC;float:left;width:300px;margin-left:10px;padding:8px;font-size:12px;">
                        <ul style="list-style-type:none;">
                        {foreach from=$news_list key=i item=news}
                        <li style="margin:4px 0;"><a href="news.php#n{$i}">{$news.title} [{$news.addtime|date_format:"%Y-%m-%d"}]</a></li>
                        {foreachelse}
					    There is no news.
                        {/foreach}
                        </ul>
						</div>
						<div style="background:#F2F6FB none repeat scroll 0 0;border:1px solid #CCCCCC;float:left;width:430px;margin-left:10px;padding:8px;font-size:12px;">
                        <ul style="list-style-type:none;">
                        {foreach from=$contest_list item=contest}
                        <li style="margin:4px 0;"><a href="contest.php?act=home&cid={$contest.id}">{$contest.title}</a>[{$contest.startTime|date_format:"%m-%d %H:%M:%S"}--{$contest.endTime|date_format:"%m-%d %H:%M:%S"}] <font color="green">{$contest.status}</font></li>
                        {foreachelse} 
						There is no coming contest avaliable.
                        {/foreach}
                        </ul>
						</div>
						<div class="clear">
					</div>
					<p>Designer & Developer:<a href="mailto:cs.kunchen@gmail.com">Chen Kun</a>
				</div>
			</div>
		</div>
	</div>
	<div class="clear"></div>
</div>
{include file="footer.tpl"}

