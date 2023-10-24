// Modules go here
var express = require('express');
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
const {SerialPort} = require('serialport');
const {ReadlineParser} = require('@serialport/parser-readline');
const port = new SerialPort({
    path:'/dev/cu.usbserial-019FDB7C',
    baudRate:115200
});
const fs = require('fs');
var tempData = [];
var photocellData = [];
var chartData = { temperature: [], photocell: [] };
const dataFilePath = './data.txt';
const dataFileStream = fs.createWriteStream(dataFilePath);

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

// Get current time
const currTime = new Date().toLocaleString();
var currentTime = 0;

app.get('/', function(req, res){
    res.sendFile(__dirname + '/index.html');
});

io.on('connection', function(socket){
    console.log('a user connected');
    socket.emit('initChartData', chartData);
    socket.on('disconnect', function(){
        console.log('user disconnected');
    });
});

parser.on('data', (data) => {
    console.log('The word from ESP32:', data);
    io.emit('serialData', data);

    // Parse data
    var dataArray = data.split(' ');
    if (dataArray.length === 2) {
        var temperature = parseFloat(dataArray[0]);
        var photocell = parseFloat(dataArray[1]);
        var timestamp = currentTime;
        var dataLine = `[${timestamp}, ${temperature}, ${photocell}]\n`;

        // Save data to a local file
        dataFileStream.write(dataLine);
        currentTime++;
    }
});

http.listen(8085, function() {
    console.log('listening on *:8085');
});


