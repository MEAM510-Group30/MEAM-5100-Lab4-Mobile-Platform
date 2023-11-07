const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html>
  <head>
    <title>LED Brightness Control</title>
    <meta charset="UTF-8" />
    <meta
      name="viewport"
      content="width=device-width, initial-scale=1.0"
    />
  </head>
  <body>
    <h1>Lab4.1.3b LED Control</h1>
    <p>Name: Bowen Jiang</p>

    <h2>Frequency Silder</h2>
    <div>
      <input
        type="range"
        min="3"
        max="30"
        value="20"
        class="slider"
        id="freq_slider"
        oninput="display_freq()"
      />
    </div>
    <p>Frequency: <span id="freq_val"></span></p>
    
    <h2>Duty Cycle Silder</h2>
    <div>
      <input
        type="range"
        min="0"
        max="100"
        value="50"
        class="slider"
        id="duty_slider"
        oninput="display_duty()"
      />
    </div>
    <p>Duty Cycle: <span id="duty_val"></span> %</p>
    
    <script>
      freq_slider.onchange = function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("value").innerHTML = this.responseText;
          }
        };
        var str = "freq_slider=";
        var res = str.concat(this.value);
        xhttp.open("GET", res, true);
        xhttp.send();
      };

      duty_slider.onchange = function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("value").innerHTML = this.responseText;
          }
        };
        var str = "duty_slider=";
        var res = str.concat(this.value);
        xhttp.open("GET", res, true);
        xhttp.send();
      };

      var freq = document.getElementById("freq_slider");
      var duty = document.getElementById("duty_slider");

      function display_freq() {
        var output_freq = document.getElementById("freq_val");
        output_freq.innerHTML = freq_slider.value;
      }

      function display_duty() {
        var output_duty = document.getElementById("duty_val");
        output_duty.innerHTML = duty_slider.value;
      }

      function display() {
        display_freq();
        display_duty();
      }

      window.onload = function () {
        display();
      };
    </script>
  </body>
</html>

)===";
