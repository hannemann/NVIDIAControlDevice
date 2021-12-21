#include <Arduino.h>

const char indexHTML[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="color-scheme" content="dark light">
    <title>BLE-LIRC</title>
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
        --clr-btn-bg: hsl(var(--hue-bg), var(--sat-bg), calc(var(--lit-bg) + 10%));
        --clr-btn-border: hsl(var(--hue-bg), var(--sat-bg), calc(var(--lit-bg) + 50%));
        --gap: 1em;
        --gap-small: calc(var(--gap) / 2);
        --gap-big: calc(var(--gap) + var(--gap) / 4);
        --gap-minimal: calc(var(--gap) / 4);
    }
    body {
        background: var(--clr-bg);
        color: var(--clr-fg);
        margin: 0;
    }
    nav {
        background: var(--clr-btn-bg);
        padding: var(--gap-small);
        color: var(--clr-fg);
        display: flex;
        gap: var(--gap-small);
        font-size: 2em;
        margin-bottom: var(--gap);
        position: sticky;
        inset: 0 0 auto;
    }
    nav a {
        display: inline-block;
        color: inherit;
        text-decoration: none;
    }
    nav a:last-of-type {
        margin-left: auto;
    }
    button {
        background: hsl(var(--hue-bg), var(--sat-bg), calc(var(--lit-bg) + 10%));
        border: 1px solid var(--clr-btn-border);
        color: var(--clr-fg);
    }
    button:active {
        --lit-bg: 10%;
    }
    .ws-off button {
        opacity: .5;
        pointer-event: none;
    }
    .buttons {
        display: inline-grid;
        gap: var(--gap);
        place-items: center;
        margin-bottom: var(--gap);
    }
    button[data-key] {
        aspect-ratio: 1;
        font-size: 2em;
        height: 1.4em;
        border: none;
        border-radius: .2em;
    }
    .col-3 {
        grid-template-columns: repeat(3, 1fr);
    }
    .col-4 {
        grid-template-columns: repeat(4, 1fr);
    }
    .col-5 {
        grid-template-columns: repeat(5, 1fr);
        padding-block: var(--gap);
    }
    .p-block {
        padding-block: var(--gap);
    }
    .p-inline {
        padding-inline: var(--gap);
    }
    section.remote {
        display: grid;
        grid-template-columns: 1fr;
        place-items: center;
    }
    section.keyboard {
        display: grid;
        gap: var(--gap-minimal);
        width: calc(100% - 2 * var(--gap-minimal));
        padding-inline: var(--gap-minimal);
    }
    main:not([data-page="learn"]) section.keyboard {
        position: fixed;
        bottom: var(--gap-minimal);
    }
    main[data-page="home"] section.keyboard,
    main:not([data-page="learn"]) section.config,
    main[data-page="keyboard"] section.remote > div:not(.dpad):not(.functional) {
        display: none;
    }
    .keyboard > div {
        display: flex;
        justify-content: center;
        gap: var(--gap-minimal);
        margin-bottom: var(--gap-minimal);
    }
    .keyboard button[data-key] {
        aspect-ratio: revert;
        padding-inline: .4em;
        flex-grow: .1;
        height: 1.6em;
    }
    .keyboard > div:last-child {
        justify-content: space-between;
    }
    .keyboard > div:last-child button:nth-child(2) {
        flex-grow: 1;
    }
    @media (max-width: 480px) {
        :root {
            --gap: .85em;
        }
        .keyboard button[data-key] {
            padding-inline: .3em;
            font-size: 1.5em;
        }
    }
    </style>
    <link rel="icon" href="data:,">
</head>
<body class="ws-off">
    <nav>
        <a href="/">&#x1f4fa;</a>
        <a href="/keyboard">&#x2328;</a>
        <a href="/learn">&#x2699;</a>
    </nav>
    <main data-page="home">
        <section class="config p-inline">
            <h1>Configure</h1>
            <p>Hit the button you want learn and press the button on your IR remote.</p>
            <p>To delete the entire configuration hit &quot;Clear Configuration&quot; at the bottom of the page.</p>
        </section>
        <section class="remote"></section>
        <section class="keyboard"></section>
        <section class="config p-block p-inline">
            <button name="clear">Clear Configuration</button>
        </section>
    </main>
    <script>
        let ws;
        let blocked = false;

        const buttons = {
            remote: {
                numbers: {"classes":["col-3"],"buttons":[{"key":26,"type":1,"label":"1"},{"key":27,"type":1,"label":"2"},{"key":28,"type":1,"label":"3"},{"key":29,"type":1,"label":"4"},{"key":30,"type":1,"label":"5"},{"key":31,"type":1,"label":"6"},{"key":32,"type":1,"label":"7"},{"key":33,"type":1,"label":"8"},{"key":34,"type":1,"label":"9"},{"key":-1,"type":0,"label":""},{"key":35,"type":1,"label":"0"},{"key":-1,"type":0,"label":""}]},
                functional: {"classes":["col-3"],"buttons":[{"key":37,"type":1,"label":"&#x21b0;"},{"key":0,"type":2,"label":"PWR"},{"key":1,"type":2,"label":"M"}]},
                dpad: {"classes":["col-5","p-block"],"buttons":[{"key":27,"type":2,"label":"&plus;"},{"key":-1,"type":0,"label":""},{"key":77,"type":1,"label":"&utrif;"},{"key":-1,"type":0,"label":""},{"key":70,"type":1,"label":"&plus;"},{"key":-1,"type":0,"label":"Vol"},{"key":75,"type":1,"label":"&ltrif;"},{"key":36,"type":1,"label":"&#x2386"},{"key":74,"type":1,"label":"&rtrif;"},{"key":-1,"type":0,"label":"CH"},{"key":28,"type":2,"label":"&minus;"},{"key":-1,"type":0,"label":""},{"key":76,"type":1,"label":"&dtrif;"},{"key":-1,"type":0,"label":""},{"key":73,"type":1,"label":"&minus;"},{"key":-1,"type":0,"label":""},{"key":26,"type":2,"label":"&#x1f507;"},{"key":-1,"type":0,"label":""},{"key":7,"type":2,"label":"&#x1f504;"},{"key":-1,"type":0,"label":""}]},
                media: {"classes":["col-4"],"buttons":[{"key":22,"type":2,"label":"&#x23EE;"},{"key":20,"type":2,"label":"&#x23EA;"},{"key":19,"type":2,"label":"&#x23E9;"},{"key":21,"type":2,"label":"&#x23ED;"},{"key":18,"type":2,"label":"&#x23FA;"},{"key":23,"type":2,"label":"&#x23f9;"},{"key":25,"type":2,"label":"&#x23EF;"}]},
                colors: {"classes":["col-4"],"buttons":[{"key":3,"type":2,"label":"&#128997;"},{"key":4,"type":2,"label":"&#129001;"},{"key":6,"type":2,"label":"&#129000;"},{"key":5,"type":2,"label":"&#128998;"}]}
            },
            keyboard: {
                rowN: {"buttons":[{"key":26,"type":1,"label":"1"},{"key":27,"type":1,"label":"2"},{"key":28,"type":1,"label":"3"},{"key":29,"type":1,"label":"4"},{"key":30,"type":1,"label":"5"},{"key":31,"type":1,"label":"6"},{"key":32,"type":1,"label":"7"},{"key":33,"type":1,"label":"8"},{"key":34,"type":1,"label":"9"},{"key":35,"type":1,"label":"0"}]},
                row1: {"buttons":[{"key":16,"type":1,"label":"Q"},{"key":22,"type":1,"label":"W"},{"key":4,"type":1,"label":"E"},{"key":17,"type":1,"label":"R"},{"key":19,"type":1,"label":"T"},{"key":25,"type":1,"label":"Z"},{"key":20,"type":1,"label":"U"},{"key":8,"type":1,"label":"I"},{"key":14,"type":1,"label":"O"},{"key":15,"type":1,"label":"P"}]},
                row2: {"buttons":[{"key":0,"type":1,"label":"A"},{"key":18,"type":1,"label":"S"},{"key":3,"type":1,"label":"D"},{"key":5,"type":1,"label":"F"},{"key":6,"type":1,"label":"G"},{"key":7,"type":1,"label":"H"},{"key":9,"type":1,"label":"J"},{"key":10,"type":1,"label":"K"},{"key":11,"type":1,"label":"L"}]},
                row3: {"buttons":[{"key":-1,"type":0,"label":""},{"key":24,"type":1,"label":"Y"},{"key":23,"type":1,"label":"X"},{"key":2,"type":1,"label":"C"},{"key":21,"type":1,"label":"V"},{"key":1,"type":1,"label":"B"},{"key":13,"type":1,"label":"N"},{"key":12,"type":1,"label":"M"},{"key":38,"type":1,"label":"&#x232b"}]},
                row4: {"buttons":[{"key":-1,"type":0,"label":"&nbsp;"},{"key":40,"type":1,"label":"&nbsp"},{"key":36,"type":1,"label":"&#x23ce"}]}
            }
        };

        const deactivate = () => document.body.classList.add("ws-off");
        const activate = () => document.body.classList.remove("ws-off");

        const renderButtons = function (parent, config, part) {
            const container = parent.appendChild(document.createElement("div"));
            container.classList.add("buttons", part);
            if (config.classes) {
                config.classes.forEach((c) => container.classList.add(c));
            }
            config.buttons.forEach((b) => {
                let btn;
                if (b.type > 0) {
                    btn = container.appendChild(document.createElement("button"));
                    btn.dataset.key = b.key;
                    btn.dataset.type = b.type;
                } else {
                    btn = container.appendChild(document.createElement("span"));
                }
                btn.innerHTML = b.label;
            });
        };

        const handleWebsocketResults = function (result) {
            const data = JSON.parse(result.data);
            if (data.buttons) {
                // console.log(JSON.stringify(data.buttons));
                renderButtons(document.querySelector("section.remote"), data.buttons);
            }
            if (data.result === "OK") {
                blocked = false;
            }
        };

        const addEvents = function () {
            const btnClear = document.querySelector('button[name="clear"]');
            const btnsKey = document.querySelectorAll("button[data-key]");
            const btnsNav = document.querySelectorAll("nav a");
            const main = document.querySelector("main");
            if (btnsKey) {
                btnsNav.forEach((b) => {
                    b.addEventListener("click", (e) => {
                        e.preventDefault();
                        switch (e.target.pathname) {
                            case "/":
                                main.dataset.page = "home";
                                break;
                            case "/keyboard":
                                main.dataset.page = "keyboard";
                                break;
                            case "/learn":
                                main.dataset.page = "learn";
                                break;
                        }
                    });
                });
                btnsKey.forEach((b) => {
                    b.addEventListener("pointerdown", (e) => {
                        if (main.dataset.page === "learn" || blocked) return;
                        blocked = true;
                        ws.send(
                            JSON.stringify({
                                method: "keydown",
                                params: e.target.dataset,
                            })
                        );
                    });
                    b.addEventListener("pointerup", (e) => {
                        if (main.dataset.page === "learn") {
                            ws.send(
                                JSON.stringify({
                                    method: "learn",
                                    params: e.target.dataset,
                                })
                            );
                        } else {
                            ws.send(
                                JSON.stringify({
                                    method: "keyup",
                                    params: e.target.dataset,
                                })
                            );
                        }
                    });
                });
            }
            if (btnClear) {
                btnClear.addEventListener("pointerup", (e) => {
                    if (confirm("Delete Configuration?")) {
                        ws.send(JSON.stringify({ method: "clear" }));
                    }
                });
            }
        };

        const initButtons = function () {
            addEvents();
            document.body.classList.remove("ws-off");
        };

        setTimeout(() => {
            ws = new WebSocket("ws://192.168.178.218:2339/jsonrpc");
            ws.addEventListener("error", deactivate);
            ws.addEventListener("close", deactivate);
            ws.addEventListener("message", handleWebsocketResults);
            ws.addEventListener("open", initButtons);
        }, 1000);

        Object.keys(buttons.remote).forEach(b => {
            renderButtons(document.querySelector("section.remote"), buttons.remote[b], b);
        });
        Object.keys(buttons.keyboard).forEach(b => {
            renderButtons(document.querySelector("section.keyboard"), buttons.keyboard[b], b);
        });
    </script>
</body>
</html>
)=====";