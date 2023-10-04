// Modules go here
const {SerialPort} = require('serialport');
const {ReadlineParser} = require('@serialport/parser-readline');
const port = new SerialPort({
    path:'/dev/cu.usbserial-019FDB7C',
    baudRate:115200
});
const fs = require('fs');

// New objects go here
const parser = port.pipe(new ReadlineParser({delimiter: '\n'}));

// Opening port 
port.open((err) => {
    if (err) {
        console.error('Error opening port:', err);
    } else {
        console.log('Serial port now open');
    }
});

// Reading data from port and adding line to csv file
parser.on('data', (data) => {
    console.log('The word from ESP32:', data);

    // Get current time
    const currTime = new Date().toLocaleString();
    data = data.replace(/(\r\n|\n|\r)/gm, "");
    // Create line
    const currLine = `${data},${currTime}\n`;

    // Writing line to file
    fs.appendFile("sensor_data.csv", currLine, (err) => {
        if (err)
        {
            console.log("Write failed");
        }
        else
        {
            console.log("Write success");
        }
    });
});

