<html><head><title>WiFi connection</title>
<link rel="stylesheet" type="text/css" href="style.css">
<script type="text/javascript" src="140medley.min.js"></script>
<script type="text/javascript" src="a.js"></script>
<script type="text/javascript">
var xhr=j();
var currAp="%currSsid%";
window.onload=function(e) {
	scanAPs();
};
</script>
</head>
<body>
<div id="main">
<p>
Current WiFi mode: %WiFiMode%
</p>
<p>
Note: %WiFiapwarn%
</p>
<form name="wifiform" action="connect.cgi" method="post">
<p>
To connect to a WiFi network, please select one of the detected networks...<br>
<div id="aps">Scanning...</div>
<br>
WiFi password, if applicable: <br />
<input type="text" name="passwd" val="%WiFiPasswd%"> <br />
<input type="submit" name="connect" value="Connect!">
</p>
</div>
</body>
</html>
