const char* health_response = "<html><h1>MicroAlpha is healthy</h1></html>";
const char* index_response = "<html><h1>MicroAlpha 1 r 1.0</h1></html>";
const char HTML_CONFIG_HEADER[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Configuración</title>
  <style>
    body {
      margin: 0;
      font-family: system-ui, sans-serif;
      line-height: 1.5;
      background: #f7f9fc;
      color: #222;
    }

    header {
      background: #222;
      color: #fff;
      padding: 1rem;
    }
    header h1 {
      margin: 0;
      font-size: 1.5rem;
    }

    nav {
      background: #333;
    }
    nav .container {
      display: flex;
      gap: 1rem;
      padding: 0.5rem 1rem;
      max-width: 960px;
      margin: 0 auto;
    }
    nav a {
      color: #fff;
      text-decoration: none;
      padding: 0.5rem 0.75rem;
      border-radius: 6px;
      transition: background 0.2s;
    }
    nav a:hover {
      background: #444;
    }

     nav a:hover { background:#444; }
    nav a.active { background:#555; }
    main {
      max-width:960px;
      margin:1rem auto;
      padding:1rem;
      background:#fff;
      border-radius:8px;
      box-shadow:0 2px 4px rgba(0,0,0,.1);
    }
    form {
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
    label { font-weight:600; }
    input {
      width:100%;
      padding:.6rem .8rem;
      border:1px solid #ccc;
      border-radius:6px;
    }
    button {
      padding:.6rem 1rem;
      border:none;
      border-radius:6px;
      background:#4da3ff;
      color:#fff;
      font-weight:600;
      cursor:pointer;
    }
    button:hover { background:#2f7fd6; }
    .hidden { display:none; }

    .switch {
    display: flex;
    align-items: center;
    justify-content: flex-start;
    flex-direction: row-reverse; /* invierte: primero label, luego checkbox */
    gap: 0.5rem;
    margin: 0.5rem 0;
    }

    .switch label {
    font-weight: 500;
    cursor: pointer;
    }
    .switch input[type=checkbox] {
    margin: 0;
    }

  </style>
</head>

<body>
    
  <header>
  <div class="container" style="max-width:960px;margin:0 auto; display:flex; justify-content:space-between; align-items:center;">
    <h1 data-i18n="title.config">Configuración</h1>
      <div>
        <label for="lang" class="sr-only">Idioma</label>
        <select id="lang" aria-label="Idioma">
            <option value="es">ES</option>
            <option value="en">EN</option>
            <option value="fr">FR</option>
            <option value="pt">PT</option>
        </select>
      </div>
    </div>
  </header>

  <nav>
    <div class="container">
      <a href="#" id="tab-red" data-i18n="nav.network">RED</a>
    <a href="#" id="tab-seg" data-i18n="nav.security">SEGURIDAD</a>
    </div>
  </nav>

  <main>
    <!-- Formulario RED -->

      <form id="form-red" action="/config" method="post">
        <label for="ssid">SSID:</label><br>
        <input type="text" id="ssid" name="ssid"><br>
        <label for="password" data-i18n="label.pass">Contraseña:</label><br>
        <input type="text" id="password" name="password"><br><br>
        <input type="submit" value="Enviar">

        <div id="form-red" class="wifi-container">
        <h3>Redes disponibles</h3>
      

      
)rawliteral";

const char HTML_CONFIG_FOOTER[] PROGMEM = R"rawliteral(
      </div>
    </form>

    <!-- Formulario SEGURIDAD -->
    <form id="form-seg" class="hidden">

      <div class="switch">
      <input type="checkbox" id="enable-https">
      <label for="enable-https">HABILITAR HTTPS</label>
      </div>

      <div class="switch">
        <input type="checkbox" id="enable-apikey">
        <label for="enable-apikey">ESTABLECER API KEY</label>
      </div>

      <div>
        <label for="apikey">API Key</label>
        <input type="text" id="apikey" placeholder="Introduce tu API Key">
      </div>
    </form>

  </main>

    <script type="module">
    // Espera al DOM si el script estuviera en <head> (si lo pones al final de <body>, puedes omitirlo)
    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', init);
    } else {
        init();
    }

    function init() {
        // -------- Tabs
        const tabRed = document.getElementById('tab-red');
        const tabSeg = document.getElementById('tab-seg');
        const formRed = document.getElementById('form-red');
        const formSeg = document.getElementById('form-seg');

        if (tabRed && tabSeg && formRed && formSeg) {
        tabRed.addEventListener('click', (e) => {
            e.preventDefault();
            tabRed.classList.add('active');
            tabSeg.classList.remove('active');
            formRed.classList.remove('hidden');
            formSeg.classList.add('hidden');
        });

        tabSeg.addEventListener('click', (e) => {
            e.preventDefault();
            tabSeg.classList.add('active');
            tabRed.classList.remove('active');
            formSeg.classList.remove('hidden');
            formRed.classList.add('hidden');
        });
        }

        // -------- i18n
        const dicts = {
        es: { "title.config":"Configuración", "nav.network":"RED", "nav.security":"SEGURIDAD", "label.pass":"Contraseña","intro":"Aquí irá el contenido de configuración." },
        en: { "title.config":"Configuration", "nav.network":"NETWORK", "nav.security":"SECURITY", "label.pass":"Password", "intro":"Configuration content goes here." },
        fr: { "title.config":"Configuration", "nav.network":"RÉSEAU", "nav.security":"SÉCURITÉ", "intro":"Le contenu de configuration apparaîtra ici." },
        pt: { "title.config":"Configuração", "nav.network":"REDE", "nav.security":"SEGURANÇA", "intro":"O conteúdo de configuração vai aqui." },
        };

        const params = new URLSearchParams(location.search);
        const fromQuery = params.get('lang')?.toLowerCase();
        const fromStore = localStorage.getItem('lang')?.toLowerCase();
        const fromNav = (navigator.language || 'es').split('-')[0].toLowerCase();
        const supported = (l) => l && Object.prototype.hasOwnProperty.call(dicts, l);

        let lang = supported(fromQuery) ? fromQuery
                : supported(fromStore) ? fromStore
                : supported(fromNav) ? fromNav
                : 'es';

        const sel = document.getElementById('lang');
        if (sel) sel.value = lang;

        function applyI18n(l) {
        const t = dicts[l] || dicts.es;
        document.documentElement.lang = l;

        document.querySelectorAll('[data-i18n]').forEach(el => {
            const key = el.getAttribute('data-i18n');
            if (t[key]) el.textContent = t[key];
        });

        const titleEl = document.querySelector('title');
        const titleKey = titleEl?.getAttribute('data-i18n');
        if (titleKey && t[titleKey]) document.title = t[titleKey];
        }

        applyI18n(lang);

        if (sel) {
        sel.addEventListener('change', () => {
            lang = sel.value.toLowerCase();
            localStorage.setItem('lang', lang);
            const url = new URL(location.href);
            url.searchParams.set('lang', lang);
            history.replaceState({}, '', url);
            applyI18n(lang);
        });
        }
    }
    </script>

</body>
</html>
)rawliteral";