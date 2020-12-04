
export async function getAllParkingSpots() {

    const response = await fetch('/api/parkingspots');
    return await response.json();
}
