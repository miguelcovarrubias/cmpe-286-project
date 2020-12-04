import React, { Component } from 'react';
import 'bootstrap/dist/css/bootstrap.min.css';
import './App.css';
import { Header } from './components/Header'
import { ParkingSpots } from './components/ParkingSpots'
import { DisplayParkingSpots } from './components/DisplayParkingSpots'
import { getAllParkingSpots } from './services/ParkingSpotsService'

class App extends Component {

  state = {
    parking: {},
    parking_spots: []
  }

  getAllParkingSpots = () => {
    getAllParkingSpots()
      .then(parkingspots => {
        console.log(parkingspots)
        this.setState({parking_spots: parkingspots})
      });
  }

  //setInterval(this.getAllParkingSpots, 3000); // runs every 3 seconds.

  render() {

    return (
      <div className="App">
        <Header></Header>
        <div className="row mrgnbtm">
          <ParkingSpots parkingspots={this.state.parking_spots}></ParkingSpots>
        </div>
        <div className="container mrgnbtm">
          <div className="row">
            <div className="col-md-4">
                <DisplayParkingSpots
                  getAllParkingSpots={this.getAllParkingSpots}
                >
                </DisplayParkingSpots>
            </div>
          </div>
        </div>
      </div>
    );
  }
}

export default App;
