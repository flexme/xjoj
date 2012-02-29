{include file="header.tpl"}
    {literal}
    <style type="text/css">
    #newslist li{border-bottom:1px solid gray;margin:10px 0;}
    #newslist div{margin:8px 0;font-size:13px;}
    #newslist h4{margin:0;padding:0;text-align:center;font-size:15px;}
    #newslist font{font-size:13px;}
    </style>
    {/literal}
	<div id="main-left">
		{include file="main_left.tpl"}
	</div>
    <div id="main-right">
	    <div class="rectdiv">
		    <div class="cat-head">::News</div>
            <div style="padding:10px;">
            <ul style="list-style-type:none;" id="newslist">
            {foreach from=$news_list key=i item=news}
            <li id="n{$i}">
            <h4>{$news.title}<font>[{$news.addtime|date_format:"%Y-%m-%d"}]</font></h4>
            <div>{$news.content}</div>
            </li>
            {/foreach}
            </ul>
            </div>
        </div>
    </div>
    <div class="clear"></div>
</div>
{include file="footer.tpl"}

