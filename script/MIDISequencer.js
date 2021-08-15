/* MIDI sequencer */
export class MIDISequencer {
    constructor(onEvent = () => {}) {
      this.sequence = [];
      this.recordedSequences = [];
      this.recordStart = 0;
      this.onEvent = onEvent;
    }
    record() {
        this.sequence = [];
        this.recordStart = performance.now();
    }
    stop() {
        this.recordedSequences.push(this.sequence);
        this.sequence = [];
    }
    play(seq = this.recordedSequences[this.recordedSequences.length -1]) {
        seq.forEach((e)=> {
            setTimeout(() => { this.onEvent(e.midi) }, e.time)
        })
    }

    quantise(seq, bpm = 120, steps = 32) {
        const stepDuration = 1000/(bpm/60)/steps;
        let qSeq = seq.map( e => e.time - e.time % stepDuration);
    }

    recordMIDIEvent(midiEvent) {
        this.sequence.push({
            midi: midiEvent,
            time: performance.now() - this.recordStart
        })
    }
}