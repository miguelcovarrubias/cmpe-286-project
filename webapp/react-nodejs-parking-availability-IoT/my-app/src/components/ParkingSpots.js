import React from 'react'

export const ParkingSpots = ({parkingspots}) => {

    console.log('users length:::', parkingspots.length)
    if (parkingspots.length === 0) return null

    const SpotRow = (parkingspots,index) => {

        return(
              <tr key = {index} className={index%2 === 0?'odd':'even'} >
                  <td>{parkingspots.parkingId}</td>
                  <td>{parkingspots.spotId}</td>
                  <td style={{color: parkingspots.isAvailable ? '#33cc33' : '#ff3300'}} > {parkingspots.isAvailable ? "Yes" : "No" }</td>
              </tr>
          )
    }

    const ParkingIdRow = (parkingId, totalAvailableSpots) => {

        return(
              <tr >
                  <td>{parkingId}</td>
                  <td style={{color: totalAvailableSpots > 0 ? '#33cc33' : '#ff3300'}} > {totalAvailableSpots}</td>
              </tr>
          )
    }

    const parkingTable = parkingspots.map((parking,index) => SpotRow(parking, index))
    const totalAvailableSpotsTable = ParkingIdRow(parkingspots[0].parkingId, parkingspots[0].totalAvailableSpots)

    return(
        <div className="container">
            <h2>Parking Locations</h2>
            <table className="table table-bordered">
                <thead>
                <tr>
                    <th>Parking Id</th>
                    <th>Spots Available</th>
                </tr>
                </thead>
                <tbody>
                    {totalAvailableSpotsTable}
                </tbody>
            </table>
            <br></br>

            <h2>Parking Availability by Location</h2>
            <table className="table table-bordered">
                <thead>
                <tr>
                    <th>Parking Id</th>
                    <th>Spot Id</th>
                    <th>Is Available</th>
                </tr>
                </thead>
                <tbody>
                    {parkingTable}
                </tbody>
            </table>
        </div>
    )
}
