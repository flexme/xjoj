{include file="pageheader.tpl"}
<script type="text/javascript" src="js/utils.js"></script>
<script type="text/javascript" src="js/calendar.php"></script>
<link href="js/calendar/calendar.css" rel="stylesheet" type="text/css" />
<div class="main-div">
    <form action="contest.php" method="post" name="theForm">
    <table width="100%" id="general-table">
    <tr>
        <td class="label">Contest Title</td>
        <td>
            <input type="text" name="title" value="{$contest.title}" size="30" />
        </td>
    </tr>
    <tr>
        <td class="label">Start Time</td>
        <td>
            <input type="text" name="startTime" value="{if $contest.startTime}{$contest.startTime|date_format:"%Y-%m-%d %H:%M:%S"}{/if}" size="20" id="startTime" />
            <input name="selbtn1" type="button" id="selbtn1" onclick="return showCalendar('startTime','%Y-%m-%d %H:%M:%S', 'true', false, 'selbtn1');" value="Select Time" class="button"/>
        </td>
    </tr>
    <tr>
        <td class="label">End Time</td>
        <td>
            <input type="text" name="endTime" value="{if $contest.endTime}{$contest.endTime|date_format:"%Y-%m-%d %H:%M:%S"}{/if}" size="20" id="endTime" />
            <input name="selbtn2" type="button" id="selbtn2" onclick="return showCalendar('endTime','%Y-%m-%d %H:%M:%S', 'true', false, 'selbtn2');" value="Select Time" class="button" />
        </td>
    </tr>
    <tr>
	<td class="label">Private</td>
	<td><input type="checkbox" name="private" value="1" {if $contest.private}checked{/if} /></td>
    </tr>
    </table>
    <div class="button-div">
        <input type="submit" value="Submit" class="button" />
        <input type="reset" value="Reset" class="button" />
    </div>
    <input type="hidden" name="act" value="{$form_act}" />
    <input type="hidden" name="id" value="{$contest.id}" />
    </form>
</div>
{include file="pagefooter.tpl"}

