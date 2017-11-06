## Basic instructions

(Note that the OSC server is hard-coded to port 9001 for now)

1. In terminal: `npm install` (this installs dependencies)
2. In terminal: `node bridge.js` (this starts the server)
3. Open `index.html`, either locally or with localhost on any port (this opens the receiver)
    - To open in full-screen Chrome:
        1. `python -m SimpleHTTPServer 8000`
        2. `/Applications/Google\ Chrome.app/Contents/MacOS/Google\ Chrome --app=http://localhost:8000`
4. Open `midi_to_osc_router.pd` (this opens the sender)

Hit stuff!