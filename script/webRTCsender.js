var lastPeerId = null;
var peer = null; // own peer object
var recvId = "c5dwqeqqb2808-e7ea-4a3f-82eb-a8a8bb905eea";
var conn = null;
var status = document.getElementById("status");
var message = document.getElementById("message");
var sendMessageBox = document.getElementById("sendMessageBox");
var sendButton = document.getElementById("sendButton");
var clearMsgsButton = document.getElementById("clearMsgsButton");
var testMidiButton = document.getElementById("testMidi");

import { MIDI, MIDIEvent } from "./MIDI.js";
var midiDevice = null;
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
    return msgA + msgB;
}

testMidiButton.addEventListener('click', function () {
    const data = [144, 60, 120];
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
    peer = new Peer(null, {
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
        join();
    });
    peer.on('connection', function (c) {
        // Disallow incoming connections
        c.on('open', function() {
            c.send("Sender does not accept incoming connections");
            setTimeout(function() { c.close(); }, 500);
        });
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
        console.log(err);
        alert('' + err);
    });
};

/**
 * Create the connection between the two Peers.
 *
 * Sets up callbacks that handle any events related to the
 * connection and data received on it.
 */
function join() {
    // Close old connection
    if (conn) {
        conn.close();
    }

    // Create connection to destination peer specified in the input field
    conn = peer.connect(recvId, {
        reliable: true
    });

    conn.on('open', function () {
        status.innerHTML = "Connected";
        
        // Check URL params for comamnds that should be sent immediately
        var command = getUrlParam("command");
        if (command)
            conn.send(command);
    });
    // Handle incoming data (messages only since this is the signal sender)
    conn.on('data', function (data) {
        if (typeof(data)=== "string") {
            addMessage("<span class=\"peerMsg\">Peer:</span> " + data);
        } 
        else if (typeof(data)=== "object") {
            handleMidiEventFromRemote(data);
        }       
    });
    conn.on('close', function () {
        status.innerHTML = "Connection closed";
    });
};

/**
 * Get first "GET style" parameter from href.
 * This enables delivering an initial command upon page load.
 *
 * Would have been easier to use location.hash.
 */
function getUrlParam(name) {
    name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
    var regexS = "[\\?&]" + name + "=([^&#]*)";
    var regex = new RegExp(regexS);
    var results = regex.exec(window.location.href);
    if (results == null)
        return null;
    else
        return results[1];
};

/**
 * Send a signal via the peer connection and add it to the log.
 * This will only occur if the connection is still alive.
 */
    function signal(sigName) {
    if (conn && conn.open) {
        conn.send(sigName);
        console.log(sigName + " signal sent");
        addMessage(cueString + sigName);
    } else {
        console.log('Connection is closed');
    }
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
};

function clearMessages() {
    message.innerHTML = "";
    addMessage("Msgs cleared");
};

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
        console.log("Sent: " + msg);
        addMessage("<span class=\"selfMsg\">Self: </span> " + msg);
    } else {
        console.log('Connection is closed');
    }
});

// Clear messages box
clearMsgsButton.addEventListener('click', clearMessages);

// Since all our callbacks are setup, start the process of obtaining an ID
initialize();