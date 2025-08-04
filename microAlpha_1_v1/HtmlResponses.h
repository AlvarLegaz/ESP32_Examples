const char* health_response = "<html><h1>MicroAlpha is healthy</h1></html>";
const char* index_response = "<html><h1>MicroAlpha 1 r 1.0</h1></html>";
const char HTML_CONFIG_HEADER[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      font-family: Arial, sans-serif;
      padding: 20px;
      text-align: center;
    }
    .container {
      display: flex;
      flex-direction: row;
      flex-wrap: wrap;
      justify-content: space-around;
      gap: 40px;
      width: 100%;
      max-width: 800px;
      margin: 0 auto;
      text-align: left;
    }
    .form-container, .wifi-container {
      flex: 1;
      min-width: 250px;
    }
    form input[type="text"] {
      width: 100%;
      padding: 8px;
      margin: 6px 0;
      box-sizing: border-box;
    }
    input[type="submit"] {
      padding: 10px;
      width: 100%;
    }
    ul {
      list-style: none;
      padding: 0;
    }
    @media (max-width: 600px) {
      .container {
        flex-direction: column;
      }
    }
  </style>
  <title>Configuración WiFi</title>
</head>
<body>
  <h1>MicroAlpha 1 r1.0</h1>
  <div class="container">
    <div class="form-container">
      <h3>Configurar WiFi</h3>
      <form action="/config" method="post">
        <label for="ssid">SSID:</label><br>
        <input type="text" id="ssid" name="ssid"><br>
        <label for="password">Contraseña:</label><br>
        <input type="text" id="password" name="password"><br><br>
        <input type="submit" value="Enviar">
      </form>
    </div>
    <div class="wifi-container">
      <h3>Redes disponibles</h3>
)rawliteral";

const char HTML_CONFIG_FOOTER[] PROGMEM = R"rawliteral(
    </div>
  </div>
</body>
</html>
)rawliteral";