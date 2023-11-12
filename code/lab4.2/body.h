const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8" />
    <title>ESP8266 Web Server</title>
    <style>
      body {
        margin-top: 0px;
        margin-bottom: 0px;
        margin-left: 10%;
        margin-right: 10%;
      }

      .button {
        padding: 8% 10%;
        font-size: 100%;
        text-align: center;
        cursor: pointer;
        outline: none;
        color: #ffffff;
        background-color: #3168cf;
        border: none;
        border-radius: 10px;
        box-shadow: 0 5px #999;
        margin-left: 5px;
        margin-right: 5px;
        margin-top: 5px;
        margin-bottom: 5px;
      }

      /* .button:hover {
        background-color: #b60000;
      } */

      .active {
        background-color: #009f05;
        box-shadow: 0 2px #666;
        transform: translateY(4px);
      }

      .slider {
        width: 50%;
        height: 15px;
        border-radius: 5px;
        outline: none;
        opacity: 0.7;
        -webkit-transition: 0.2s;
        transition: opacity 0.2s;
      }
    </style>
    <meta charset="UTF-8" />
    <meta
      name="viewport"
      content="width=device-width, initial-scale=1.0"
    />
  </head>

  <body>
    <h1>ESP32C3 Web Server</h1>
    <p style="text-align: center">Group 6 @ Lab 4</p>
    <p style="text-align: center">Zhanqian Wu, Enlin Gu, Bowen Jiang</p>

    <h2>Connection Info</h2>
    <p>Delay: <span id="BatteryVoltage">?</span> ms</p>
    <p>Battery Voltage: <span id="BatteryVoltage">?.?</span> V</p>

    <h2>Direction</h2>
    <div style="text-align: center">
      <button
        id="forward_button"
        class="button"
        onclick="activateButton('forward_button')"
      >
        F
      </button>
    </div>
    <div style="text-align: center">
      <button
        id="left_button"
        class="button"
        onclick="activateButton('left_button')"
      >
        L
      </button>
      <button
        id="stop_button"
        class="button"
        onclick="activateButton('stop_button')"
      >
        O
      </button>
      <button
        id="right_button"
        class="button"
        onclick="activateButton('right_button')"
      >
        R
      </button>
    </div>
    <div style="text-align: center">
      <button
        id="backward_button"
        class="button"
        onclick="activateButton('backward_button')"
      >
        B
      </button>
    </div>

    <h2>Turn Rate Control</h2>
    <div style="text-align: center">
      <input
        type="range"
        min="0"
        max="100"
        value="20"
        class="slider"
        id="turn_rate_slider"
        oninput="display_turn_rate()"
      />
    </div>
    <p>Turn rate: <span id="turn_rate_val"></span>%</p>    

    <h2>Speed Control</h2>
    <div style="text-align: center">
      <input
        type="range"
        min="0"
        max="16383"
        value="0"
        class="slider"
        id="speed_slider"
        oninput="display_speed()"
      />
    </div>
    <p>Speed: <span id="speed_val"></span></p>

    <script>
      speed_slider.onchange = function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("value").innerHTML = this.responseText;
          }
        };
        var str = "speed_slider=";
        var res = str.concat(this.value);
        xhttp.open("GET", res, true);
        xhttp.send();
      };

      turn_rate_slider.onchange = function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("value").innerHTML = this.responseText;
          }
        };
        var str = "turn_rate_slider=";
        var res = str.concat(this.value);
        xhttp.open("GET", res, true);
        xhttp.send();
      };

      function activateButton(buttonId) {
        // get the button element by id
        var forward_button = document.getElementById("forward_button");
        var backward_button = document.getElementById("backward_button");
        var left_button = document.getElementById("left_button");
        var right_button = document.getElementById("right_button");
        var stop_button = document.getElementById("stop_button");

        remove_all_active(); // remove all active class from the buttons

        if (buttonId == "forward_button") {
          //remove_all_active(); // remove all active class from the buttons
          forward_button.classList.add("active"); // add active class to the button
          var xhttp = new XMLHttpRequest();
          xhttp.open("GET", "F", true);
          xhttp.send();
        }

        if (buttonId == "left_button") {
          //remove_all_active();
          left_button.classList.add("active");
          var xhttp = new XMLHttpRequest();
          xhttp.open("GET", "L", true);
          xhttp.send();
        }

        if (buttonId == "stop_button") {
          //remove_all_active();
          stop_button.classList.add("active");
          var xhttp = new XMLHttpRequest();
          xhttp.open("GET", "O", true);
          xhttp.send();
        }

        if (buttonId == "right_button") {
          //remove_all_active();
          right_button.classList.add("active");
          var xhttp = new XMLHttpRequest();
          xhttp.open("GET", "R", true);
          xhttp.send();
        }

        if (buttonId == "backward_button") {
          //remove_all_active();
          backward_button.classList.add("active");
          var xhttp = new XMLHttpRequest();
          xhttp.open("GET", "B", true);
          xhttp.send();
        }
      }

      function remove_all_active() {
        var forward_button = document.getElementById("forward_button");
        var backward_button = document.getElementById("backward_button");
        var left_button = document.getElementById("left_button");
        var right_button = document.getElementById("right_button");
        var stop_button = document.getElementById("stop_button");

        forward_button.classList.remove("active");
        backward_button.classList.remove("active");
        left_button.classList.remove("active");
        right_button.classList.remove("active");
        stop_button.classList.remove("active");
      }

      document.addEventListener("keypress", function (event) {
        var forward_button = document.getElementById("forward_button");
        var backward_button = document.getElementById("backward_button");
        var left_button = document.getElementById("left_button");
        var right_button = document.getElementById("right_button");
        var stop_button = document.getElementById("stop_button");
        
        if (event.key == 'w') {
          forward();
          activateButton("forward_button");
        }
        if (event.key == "s") {
          backward();
          activateButton("backward_button");
        }
        if (event.key == "a") {
          turn_left();
          activateButton("left_button");
        }
        if (event.key == "d") {
          turn_right();
          activateButton("right_button"); 
        }
        if (event.key == "x") {
          stop();
          activateButton("stop_button");
        }
      });

      // release
      document.addEventListener("keyup", function (event) {
        stop();
        activateButton("stop_button");
      });

      var speed = document.getElementById("speed_slider");
      var turn_Rate = document.getElementById("turn_rate_slider");

      function display_speed() {
        var output_speed = document.getElementById("speed_val");
        output_speed.innerHTML = speed_slider.value;
      }

      function display_turn_rate() {
        var output_turn_rate = document.getElementById("turn_rate_val");
        output_turn_rate.innerHTML = turn_rate_slider.value;
      }

      function display() {
        display_speed();
        display_turn_rate();
      }

      window.onload = function () {
        display();
        activateButton("stop_button");
      };
    </script>
  </body>
</html>

)===";
