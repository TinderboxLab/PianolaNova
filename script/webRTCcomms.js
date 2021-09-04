var lastPeerId = null;
var firstPeerCreated = false;
var peer = null; // own peer object
var recvId = idFromHash();//"c5dwqeqqb2808-e7ea-4a3f-82eb-a8a8bb905eea";
var locationName = locationNameFromHash();
var remoteLocationName = getRemoteLocationName();
var conn = null;
var status = document.getElementById("status");
var message = document.getElementById("message");
var clearMsgsButton = document.getElementById("clearMsgsButton");
var testMidiButton = document.getElementById("testMidi");
var recordButton = document.getElementById("record");
var playButton = document.getElementById("play");

console.log(window.location.hash);

function idFromHash() {
    return window.location.hash.substring(1).split("@")[0];
}
function locationNameFromHash() {
    const hashSplit = window.location.hash.split("@");
    const location = (hashSplit.length > 1)? hashSplit[1] : "Here";
    return location;
}

function getRemoteLocationName() {
    if (locationName === "Leeds") return "Edinburgh";
    if (locationName === "Edinburgh") return "Leeds";
    return "There";
}

import { MIDI, MIDIEvent } from "./MIDI.js";
import { MIDISequencer } from "./MIDISequencer.js";
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
        midiSequencer.recordMIDIEvent(data)
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
    if (msg) addMessage(remoteLocationName + ": " + msg, "peerMsg");
}

function sendMidiEventToRemote(data) {
    console.log(conn)
    if (conn && conn.open) {
        conn.send(data);
    } else {
        console.log('Connection is closed');
    }
    const msg = describeData(data);
    if (msg) addMessage(locationName + ": " + msg, "selfMsg");
}

function describeData(data) {
    const [func, byte1] = data;
    if (func >= 144 && func <= 159) {
        const msgA = midi.noteNumberToName(byte1);
        return " " + msgA;
    }
}

function testMidi() {
    const noteOn = [144, 60, 120];
    const noteOff = [128, 60, 120];
    sendMidiEventToRemote(noteOn);
    setTimeout(sendMidiEventToRemote, 500, noteOff); 
}

testMidiButton.addEventListener('click', testMidi);

var midiSequencer = new MIDISequencer(sendMidiEventToLocal);

recordButton.addEventListener('click', function () {
    midiSequencer.record(); 
});

playButton.addEventListener('click', function () {
    midiSequencer.stop();
    midiSequencer.play(); 
});

document.addEventListener('keypress', function(e) {
    var event = e || window.event;
    var char = event.which || event.keyCode;
    if (char == 109)
        testMidi();
});

/**
 * Create the Peer object for our end of the connection.
 *
 * Sets up callbacks that handle any events related to our
 * peer object.
 */
function initialize() {
    status.innerHTML = "Awaiting connection.."
    // Create own peer object with connection to shared PeerJS server
    let peerId = (firstPeerCreated)? null: recvId;
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
        if (firstPeerCreated) join();
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
        status.innerHTML = ""; //"Connected";
        if (!firstPeerCreated) ready();
    });
    peer.on('disconnected', function () {
        status.innerHTML = "Connection lost. Please reconnect";
        console.log('Connection lost. Please reconnect');
        peer.reconnect();
    });
    peer.on('close', function() {
        conn = null;
        status.innerHTML = "Connection destroyed. Please refresh";
        console.log('Connection destroyed');
        if (firstPeerCreated) initialize();
    });
    peer.on('error', function (err) {
        console.log(err);
        if (err.message.includes("is taken")) {
            firstPeerCreated = true;
        }
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
        reliable: false
    });

    conn.on('open', function () {
        status.innerHTML = "" //"Connected";
    });
    // Handle incoming data (messages only since this is the signal sender)
    conn.on('data', function (data) {
        if (typeof(data)=== "string") {
            addMessage(remoteLocationName + ": " + msg, "peerMsg");
        } 
        else if (typeof(data)=== "object") {
            handleMidiEventFromRemote(data);
        } 
    });
    conn.on('close', function () {
        status.innerHTML = "Connection closed";
    });
    connectVideo();
};

function ready() {
    conn.on('data', function (data) {
        if (typeof(data)=== "string") {
            addMessage(remoteLocationName + ": " + msg, "peerMsg");
        } 
        else if (typeof(data)=== "object") {
            handleMidiEventFromRemote(data);
        } 
    });
    conn.on('close', function () {
        status.innerHTML = "Connection reset<br>Awaiting connection...";
        conn = null;
    });
    connectVideo();
}

function addMessage(msg, className) {
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

    message.innerHTML = "<div class=\""+className+"\">" + h + ":" + m + ":" + s + "  -  " + msg + "</div>" + message.innerHTML;
};

function clearMessages() {
    message.innerHTML = "";
    addMessage("Msgs cleared");
};

// Clear messages box
clearMsgsButton.addEventListener('click', clearMessages);


/**
 * Video call
 */

var call = null;
var mediaStream = null;
var constraints = { audio: true, video: { width: {exact: 720}, height: {exact: 720} } };
var videoElement = document.getElementById("video");
var connectionTimer = null;
var connectionAttempts = 0;

function displayCall() {
    call.on('stream', function(stream) {
        videoElement.srcObject = stream;       
        videoElement.play();
    });
}

function connectVideo() {
    connectionTimer = setInterval(() => {
        if (!mediaStream && connectionAttempts < 20) {
            tryToConnectVideo();
        }
        else {
            clearInterval(connectionTimer);
        }
        connectionAttempts++;
    }, 10*1000);
}
 
async function tryToConnectVideo() {
    try {
        mediaStream = await navigator.mediaDevices.getUserMedia(constraints);
        if (firstPeerCreated) {
            call = peer.call(recvId, mediaStream);
            displayCall();
        }
        else {
            peer.on('call', function(remoteCall) {
                // Answer the call, providing our mediaStream
                remoteCall.answer(mediaStream);
                call = remoteCall;
                displayCall();
            });
        }       
    } catch(err) {
        /* handle the error */
    }
}
 
// Since all our callbacks are setup, start the process of obtaining an ID
initialize();