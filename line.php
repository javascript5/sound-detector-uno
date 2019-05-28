 <?php
  

function send_LINE($msg){
 $access_token = '0Z37B9U2ron2uVnCO4tuVT8zal7n2uuig9k35tcIkyiWsK36v+JLHMy5XAYeklEllAEjk9RL9dDhFPZjM0XmD9PP0gWaCxt9BgCySZ4OwPx8xya1UtfLRFbrH3kSL9Jyts/Ky6AoUlpu6TcwUMTgjwdB04t89/1O/w1cDnyilFU='; 

  $messages = [
        'type' => 'text',
        'text' => $msg
        //'text' => $text
      ];

      // Make a POST Request to Messaging API to reply to sender
      $url = 'https://api.line.me/v2/bot/message/push';
      $data = [

        'to' => 'Ucb9b4c882a1b34cc6c55b426dd9f7305',
        'messages' => [$messages],
      ];
      $post = json_encode($data);
      $headers = array('Content-Type: application/json', 'Authorization: Bearer ' . $access_token);

      $ch = curl_init($url);
      curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
      curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
      curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
      curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
      $result = curl_exec($ch);
      curl_close($ch);

      echo $result . "\r\n"; 
}

?>
