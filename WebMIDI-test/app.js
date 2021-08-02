import { MIDI } from "./script/MIDI.js";

const midi = new MIDI(handleEvent);
midi.initialize().then(() => {
  console.log("initialised")
})

function handleEvent({ device, type, a, b }) {
  console.log(device.type, type)
}
