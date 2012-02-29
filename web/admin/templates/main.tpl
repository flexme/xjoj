{include file="pageheader.tpl"}
<div class="list-div">
<table cellspacing='1' cellpadding='3'>
  <tr>
    <th colspan="4" class="group-title">System Info</th>
  </tr>
  <tr>
    <td width="20%">Operating System</td>
    <td width="30%">{$sys_info.os} ({$sys_info.ip})</td>
    <td width="20%">Web Server</td>
    <td width="30%">{$sys_info.web_server}</td>
  </tr>
  <tr>
    <td>PHP Version</td>
    <td>{$sys_info.php_ver}</td>
    <td>Mysql Version</td>
    <td>{$sys_info.mysql_ver}</td>
  </tr>
  <tr>
    <td>Safe Mode</td>
    <td>{$sys_info.safe_mode}</td>
    <td>Safe Mode GID</td>
    <td>{$sys_info.safe_mode_gid}</td>
  </tr>
  <tr>
    <td>Socket Support</td>
    <td>{$sys_info.socket}</td>
    <td>Timezone</td>
    <td>{$sys_info.timezone}</td>
  </tr>
  <tr>
    <td>Upload File Size</td>
    <td>{$sys_info.max_filesize}</td>
    <td>ZLib</td>
    <td>{$sys_info.zlib}</td>
  </tr>
</table>
</div>
{include file="pagefooter.tpl"}
