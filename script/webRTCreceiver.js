var lastPeerId = null;
var peer = null; // Own peer object
var peerId = "c5dwqeqqb2808-e7ea-4a3f-82eb-a8a8bb905eea";
var firstPeerCreated = false;
var conn = null;
var status = document.getElementById("status");
var message = document.getElementById("message");
var sendMessageBox = document.getElementById("sendMessageBox");
var sendButton = document.getElementById("sendButton");
var clearMsgsButton = document.getElementById("clearMsgsButton");
var testMidiButton = document.getElementById("testMidi");


import { MIDI, MIDIEvent } from "./MIDI.js";
var midi = new MIDI(handleMidiEventFromLocal); 
/**
 * Initialise MIDI and define handlers for locol MIDI events
 */
midi.initialize().then(() => {
    console.log("initialised")
    console.log(midi.inputs)
    console.log(midi.outputs)
})

function handleMidiEventFromLocal( device, midiMessage ) {
    if (midiMessage) {
        var data = [].slice.call(midiMessage)
        sendMidiEventToRemote(data);
    }
}

function sendMidiEventToLocal(data) {
    const midiMessage = Uint8Array.from(data);
    Object.values(midi.outputs).forEach((output) => {
        output.send( midiMessage );  //omitting the timestamp means send immediately.
    })
}

function handleMidiEventFromRemote(data) {
    sendMidiEventToLocal(data)
    const msg = describeData(data);
    if (msg) addMessage("<span class=\"peerMsg\">Peer Midi: </span>" + msg);
}

function sendMidiEventToRemote(data) {
    if (conn && conn.open) {
        conn.send(data);
    } else {
        console.log('Connection is closed');
    }
    const msg = describeData(data);
    if (msg) addMessage("<span class=\"selfMsg\">Self Midi: </span>" + msg);
}

function describeData(data) {
    const midiEvent = new MIDIEvent(null, data);
    const msgA = (midiEvent.a)? " " + midiEvent.a.type + ":" + midiEvent.a.value : "";
    const msgB = (midiEvent.b)? " " + midiEvent.b.type + ":" + midiEvent.b.value : "";
    return " " + midiEvent.type + " - " + msgA + msgB;
}

testMidiButton.addEventListener('click', function () {
    const data = [144, 48, 120];
    sendMidiEventToRemote(data);
});

/**
 * Create the Peer object for our end of the connection.
 *
 * Sets up callbacks that handle any events related to our
 * peer object.
 */
function initialize() {
    // Create own peer object with connection to shared PeerJS server
    peer = new Peer(peerId, {
        debug: 2
    });

    peer.on('open', function (id) {
        // Workaround for peer.reconnect deleting previous id
        if (peer.id === null) {
            console.log('Received null id from peer open');
            peer.id = lastPeerId;
        } else {
            lastPeerId = peer.id;
        }
        status.innerHTML = "Awaiting connection...";
    });
    peer.on('connection', function (c) {
        // Allow only a single connection
        if (conn && conn.open) {
            c.on('open', function() {
                c.send("Already connected to another client");
                setTimeout(function() { c.close(); }, 500);
            });
            return;
        }

        conn = c;
        status.innerHTML = "Connected";
        ready();
    });
    peer.on('disconnected', function () {
        status.innerHTML = "Connection lost. Please reconnect";
        console.log('Connection lost. Please reconnect');

        // Workaround for peer.reconnect deleting previous id
        peer.id = lastPeerId;
        peer._lastServerId = lastPeerId;
        peer.reconnect();
    });
    peer.on('close', function() {
        conn = null;
        status.innerHTML = "Connection destroyed. Please refresh";
        console.log('Connection destroyed');
    });
    peer.on('error', function (err) {
        // can we use this to work out if there is already a peer and then subscribe instead?
        console.log(err);
        // alert('' + err);
        if (err.message.includes("is taken")) console.log("id is taken");
    });
};

/**
 * Triggered once a connection has been achieved.
 * Defines callbacks to handle incoming data and connection events.
 */
function ready() {
    conn.on('data', function (data) {
        if (typeof(data)=== "string") {
            addMessage("<span class=\"peerMsg\">Peer:</span> " + data);
        } 
        else if (typeof(data)=== "object") {
            handleMidiEventFromRemote(data);
        } 
    });
    conn.on('close', function () {
        status.innerHTML = "Connection reset<br>Awaiting connection...";
        conn = null;
    });
}

function addMessage(msg) {
    var now = new Date();
    var h = now.getHours();
    var m = addZero(now.getMinutes());
    var s = addZero(now.getSeconds());

    if (h > 12)
        h -= 12;
    else if (h === 0)
        h = 12;

    function addZero(t) {
        if (t < 10)
            t = "0" + t;
        return t;
    };

    message.innerHTML = "<br><span class=\"msg-time\">" + h + ":" + m + ":" + s + "</span>  -  " + msg + message.innerHTML;
}

function clearMessages() {
    message.innerHTML = "";
    addMessage("Msgs cleared");
}

// Listen for enter in message box
sendMessageBox.addEventListener('keypress', function (e) {
    var event = e || window.event;
    var char = event.which || event.keyCode;
    if (char == '13')
        sendButton.click();
});
// Send message
sendButton.addEventListener('click', function () {
    if (conn && conn.open) {
        var msg = sendMessageBox.value;
        sendMessageBox.value = "";
        conn.send(msg);
        console.log("Sent: " + msg)
        addMessage("<span class=\"selfMsg\">Self: </span>" + msg);
    } else {
        console.log('Connection is closed');
    }
});

// Clear messages box
clearMsgsButton.addEventListener('click', clearMessages);

initialize();
