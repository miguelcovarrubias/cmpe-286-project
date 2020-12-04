const express = require('express');
const path = require('path');
const app = express(),
      bodyParser = require("body-parser");
      port = 3080;

// place holder for the data
const users = [];
const parking_spots = []

app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, '../my-app/build')));

app.get('/api/users', (req, res) => {
  console.log('api/users called!')
  res.json(users);
});

app.post('/api/user', (req, res) => {
  const user = req.body.user;
  console.log('Adding user:::::', user);
  users.push(user);
  res.json("user addedd");
});

app.get('/api/parkingspots', (req, res) => {
  console.log('api/parking-spots called!')
  res.json(parking_spots);
});

app.post('/api/updatespot', (req, res) => {
  const spot = req.body;
  console.log('Updating parking status:::::', spot);
  var found = false;
  for (var i=0; i<parking_spots.length; i++) {
    if (parking_spots[i].parkingId == spot.parkingId
        && parking_spots[i].spotId == spot.spotId) {
        parking_spots[i].isAvailable = spot.isAvailable;
        parking_spots[i].totalAvailableSpots = spot.totalAvailableSpots;
        found = true;
        res.json("parking spot updated");
        break;
    }
  }
  if(!found) {
    parking_spots.push(spot);
    res.json("parking spot created");
  }
});

app.get('/', (req,res) => {
  res.sendFile(path.join(__dirname, '../my-app/build/index.html'));
});

app.listen(port, () => {
    console.log(`Server listening on the port::${port}`);
});
