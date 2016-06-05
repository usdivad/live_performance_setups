// route midi input to cc messages
// try chuck --probe first

// user-defined settings
4 => int pin; // port in
0 => int pout; // port out

MidiIn min;
if (!min.open(pin)) { me.exit(); }

MidiOut mout;
mout.open(pout);

<<< "receiving from", min.name(), "(port",pin,")", "and sending to", mout.name(), "(port",pout,")" >>>;

// check for messages every 10 milliseconds
while(10::ms => now){
    MidiMsg msg; 
    while( min.recv(msg) ){
        msg.data1 => int controller;
        msg.data2 => int midiNote;
        msg.data3 => int velocity;

        if (velocity > 0) {
            // the note itself
            mout.send(msg);
            <<<"MIDI message:", msg.data1,msg.data2,msg.data3>>>;

            // velocity as CC 1
            MidiMsg velMsg;
            177 => velMsg.data1;
            1 => velMsg.data2;
            velocity => velMsg.data3;

            // only send if hitting hi tom
            if (midiNote == 41) {
                mout.send(velMsg);
                <<<"velocity message (CC1):", velMsg.data1,velMsg.data2,velMsg.data3>>>;
            }
        }
    }

}