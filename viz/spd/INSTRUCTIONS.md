## Basic instructions

(Note that the OSC server is hard-coded to port 9001 for now)

1. Assuming you're in the `viz` directory, in terminal: `cd spd`
2. In terminal: `npm install` (this installs dependencies)
3. In terminal: `node bridge.js` (this starts the server)
4. In terminal: `cd ..` (to go back to the `viz` directory)
5. ...
    - **SPD:**
        - i. Open `spd/index.html`, either locally or with localhost on any port (this opens the receiver). **Make sure you're still in the `viz` directory**
        - ii. To open in full-screen Chrome:
            1. `python -m SimpleHTTPServer 8000`
            2. `/Applications/Google\ Chrome.app/Contents/MacOS/Google\ Chrome --app=http://localhost:8000`
        - iii. Open `spd/midi_to_osc_router.pd` (this opens the sender)
    - **Circles:**
        - i. Open `circles/index.html`

Hit stuff!