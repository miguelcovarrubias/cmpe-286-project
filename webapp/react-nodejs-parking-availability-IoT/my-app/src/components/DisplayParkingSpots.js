import React from 'react'

export const DisplayParkingSpots = ({getAllParkingSpots}) => {

    return(
        <div className="display-board">
            <div className="btn">
                <button type="button" onClick={(e) => getAllParkingSpots()} className="btn btn-warning">Refresh Parking Spots</button>
            </div>
        </div>
    )
}
