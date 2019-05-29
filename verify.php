<?php
$access_token = 'r3oJHN6+mZM8V0Z+hLpd/BIzhFx8XK9sz2YQgVqd+AzBjzcqcoc+5Rvd2uW+0yVcm4JYONBoktX8rbo69zj6iQy0EwWzoEVj+bS7KM3dZE0LG5SUXG9Vk4J2cgrfdst/9mwBiZ6K9pdaPgyWq/9/tQdB04t89/1O/w1cDnyilFU=';

$url = 'https://api.line.me/v1/oauth/verify';

$headers = array('Authorization: Bearer ' . $access_token);

$ch = curl_init($url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
$result = curl_exec($ch);
curl_close($ch);

echo $result;
