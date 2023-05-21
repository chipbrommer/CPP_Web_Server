var ws, E = function (id) { return document.getElementById(id); };
var url = E('url'), connect = E('connect'), message = E('message'), send = E('send'), console = E('console');
var enable = function (en) { message.disabled = send.disabled = !en; url.disabled = en; connect.innerHTML = en ? 'disconnect' : 'connect'; };

// set the enable to false by default
enable(false)

// Handle a 'connect' button click event
connect.onclick = function () {
    if (ws) { ws.close(); return; }
    ws = new WebSocket(url.value);
    if (!ws) return;
    ws.onopen = function () { console.innerHTML += 'CONNECTION OPENED<br/>'; }
    ws.onmessage = function (ev) { console.innerHTML += 'RECEIVED: ' + ev.data + '<br/>'; }
    ws.onerror = function (ev) { console.innerHTML += 'ERROR: ' + ev + '<br/>'; }
    ws.onclose = function () { console.innerHTML += 'CONNECTION CLOSED<br/>'; enable(false); ws = null; }
    enable(true);
};

// Handle a 'send' button click event.
send.onclick = function () {
    if (!ws) return;
    console.innerHTML += 'SENT: ' + message.value + '<br/>';
    ws.send(message.value);
}