const char body[] PROGRAM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>ESP8266 Web Server</title>
  <style>
    body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }
  </style>
</head>
<body>
  <h1>ESP8266 Web Server</h1>
  <p>GPIO state is <span id="gpioState">???</span></p>
  <p><button onclick="sendData(1)">GPIO 1 ON</button><button onclick="sendData(0)">GPIO 1 OFF</button></p>
  <script>
    function updateGPIOState(gpio, state) {
      document.getElementById("gpioState").innerHTML = state;
    }
    function sendData(state) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/update?state=" + state, true);
      xhr.send();
      xhr.onloadend = function () {
        updateGPIOState(1, xhr.responseText);
      };
    }
  </script>
</body>
</html>
)=====";
