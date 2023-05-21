var ws, E = function (id) { return document.getElementById(id); };
var url = E('url'), connect = E('connect'), message = E('message'), send = E('send'), log = E('log');
var enable = function (en) { message.disabled = send.disabled = !en; url.disabled = en; connect.innerHTML = en ? 'disconnect' : 'connect'; };

// set the enable to false by default
enable(false)

// Handle a 'connect' button click event
connect.onclick = function () {
    if (ws) { ws.close(); return; }
    ws = new WebSocket(url.value);
    if (!ws) return;
    ws.onopen = function () { log.innerHTML += 'CONNECTION OPENED<br/>'; }
    ws.onmessage = function (ev) { log.innerHTML += 'RECEIVED: ' + ev.data + '<br/>'; }
    ws.onerror = function (ev) { log.innerHTML += 'ERROR: ' + ev + '<br/>'; }
    ws.onclose = function () { log.innerHTML += 'CONNECTION CLOSED<br/>'; enable(false); ws = null; }
    enable(true);
};

// Handle a 'send' button click event.
send.onclick = function () {
    if (!ws) return;
    log.innerHTML += 'SENT: ' + message.value + '<br/>';
    ws.send(message.value);
}