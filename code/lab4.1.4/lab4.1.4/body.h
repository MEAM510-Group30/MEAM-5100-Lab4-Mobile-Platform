const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html>
  <head>
    <title>LED Brightness Control</title>
    <style>
      body {
        margin-top: 0px;
        margin-bottom: 0px;
        margin-left: 20%;
        margin-right: 20%;
      }

      .button {
        padding: 4% 5%;
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
    <h1>Lab4.1.4 Motor Control</h1>
    <p>Name: Bowen Jiang</p>

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
        id="backward_button"
        class="button"
        onclick="activateButton('backward_button')"
      >
        B
      </button>
    </div>

    <h2>Speed Silder</h2>
    <div style="text-align: center">
      <input
        type="range"
        min="0"
        max="100"
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

      function activateButton(buttonId) {
        // get the button element by id
        var forward_button = document.getElementById("forward_button");
        var backward_button = document.getElementById("backward_button");

        if (buttonId == "forward_button") {
          forward_button.classList.add("active"); // add active class to the button
          backward_button.classList.remove("active"); // remove active class from the buttonq
          var xhttp = new XMLHttpRequest();
          xhttp.open("GET", "F", true);
          xhttp.send();
        }

        if (buttonId == "backward_button") {
          backward_button.classList.add("active");
          forward_button.classList.remove("active");
          var xhttp = new XMLHttpRequest();
          xhttp.open("GET", "B", true);
          xhttp.send();
        }
      }

      // forward_button.onclick = function () {
      //   // activateButton("forward_button");
      //   var xhttp = new XMLHttpRequest();
      //   xhttp.open("GET", "direction=F", true);
      //   xhttp.send();
      // };

      // backward_button.onclick = function () {
      //   // activateButton("backward_button");
      //   var xhttp = new XMLHttpRequest();
      //   xhttp.open("GET", "direction=B", true);
      //   xhttp.send();
      // };

      var speed = document.getElementById("speed_slider");

      function display_speed() {
        var output_speed = document.getElementById("speed_val");
        output_speed.innerHTML = speed_slider.value;
      }

      function display() {
        display_speed();
      }

      window.onload = function () {
        display();
        activateButton("forward_button");
      };
    </script>
  </body>
</html>

)===";
