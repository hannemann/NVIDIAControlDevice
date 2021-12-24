#include <Arduino.h>

const char captiveHTML[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="mobile-web-app-capable" content="yes">
    <meta name="color-scheme" content="dark light">
    <title>BLE-LIRC</title>
    <link rel="icon" href="data:,">
    <style>
        :root {
            --hue-bg: 225;
            --sat-bg: 14%;
            --lit-bg: 20%;
            --hue-fg: 0;
            --sat-fg: 0%;
            --lit-fg: 80%;
            --clr-bg: hsl(var(--hue-bg), var(--sat-bg), var(--lit-bg));
            --clr-fg: hsl(var(--hue-fg), var(--sat-fg), var(--lit-fg));
            --clr-border: slategrey;
            --gap: 1rem;
            --gap-small: calc(var(--gap) / 2);
            --gap-big: calc(var(--gap) + var(--gap) / 4);
            --gap-minimal: calc(var(--gap) / 4);
            --font-size: 16px;
            --line-height: 1.2;
            font-size: var(--font-size);
            line-height: var(--line-height);
        }
        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }
        body {
            background-color: var(--clr-bg);
            color: var(--clr-fg);
            font-family: sans-serif;
        }
        form {
            margin: var(--gap-small);
        }
        form > * {
            margin-bottom: var(--gap);
        }
        fieldset {
            padding: var(--gap-small);
            border-color: var(--clr-border);
        }
        h1,
        legend,
        label {
            user-select: none;
        }
        button {
            background-color: dodgerblue;
            color: white;
            font-size: 1.5em;
            padding: .75rem 1.5rem;
            border: none;
            border-radius: .25rem;
        }
        input:not([type="checkbox"]) {
            font-size: 1.5em;
            width: 100%;
            padding: var(--gap-minimal);
            border: 1px solid var(--clr-border);
        }
        input {
            accent-color: dodgerblue;
            background-color: inherit;
            color: inherit;
        }
        label {
            padding-top: var(--gap-small);
            display: block;
        }
        p {
            text-align: right;
        }
        ::selection {
            background-color: dodgerblue;
            color: white;
        }
    </style>
</head>
<body>
    <form action="/wifi-setup" method="post">
        <h1>Please enter your WIFI credentials.</h1>
        <fieldset>
            <legend>SSID:</legend>
            <input name="ssid" placeholder="MyAwesomeSauce" required>
        </fieldset>
        <fieldset>
            <legend>PSK:</legend>
            <input name="psk" type="password" required>
            <label>Show: <input type="checkbox"></label>
        </fieldset>
        <p>
            <button type="submit">Save</button>
        </p>
    </form>
    <script>
        document.querySelector('input').focus();
        document.querySelector('input[type="checkbox"]').addEventListener('change', e => {
            if (e.target.checked) {
                document.querySelector('input[name="psk"]').type = "text";
            } else {
                document.querySelector('input[name="psk"]').type = "password";
            }
        })
    </script>
</body>
</html>
)=====";