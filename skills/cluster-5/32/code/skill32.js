var Engine = require('tingodb')();
var db = new Engine.Db('mydb', {});
const readline = require('readline');
const fs = require('fs');

async function processLineByLine() {
    var logCollection = db.collection('sensorlogs');
    const fileStream = fs.createReadStream('smoke.txt');
  
    const rl = readline.createInterface({
      input: fileStream,
      crlfDelay: Infinity
    });
    // Note: we use the crlfDelay option to recognize all instances of CR LF
    // ('\r\n') in input.txt as a single line break.
  
    for await (const line of rl) {
        var values = line.split('\t').map(value => value.trim());
        if (values[0] != 'Time') {
            var logEntry = { time: values[0], sensor_id: values[1], smoke: values[2], temperature: values[3]};
            logCollection.insert(logEntry, function(err, result) {
                if (err) throw err;
                console.log('Sensor data logged');
              });
        }
    }
  }

// processLineByLine();

// Demonstrate a query to the database to retrieve all instances (time and temperature) when sensor ID 1 has smoke.
db.collection('sensorlogs', function(err, collection) {
	if (err) throw err;
	
  collection.find({ sensor_id: '1', smoke: '1' }).toArray(function(err, returned_data) {
    if (err) throw err;
    console.log("Database says: ", returned_data);
});
  });
