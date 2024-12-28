// Firebase Configuration (replace with your own credentials)
import { initializeApp } from "https://www.gstatic.com/firebasejs/11.1.0/firebase-app.js";
import { getDatabase, ref, get } from "https://www.gstatic.com/firebasejs/11.1.0/firebase-database.js";

// Firebase configuration object
const firebaseConfig = {
    apiKey: "AIzaSyBeoetlU6poGtdXfKcuXYInysdQhdIK8Vw",
    authDomain: "gasta-e5b66.firebaseapp.com",
    databaseURL: "https://gasta-e5b66-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "gasta-e5b66",
    storageBucket: "gasta-e5b66.firebasestorage.app",
    messagingSenderId: "1015069930052",
    appId: "1:1015069930052:web:1c03a9d7788ae8c9724211",
    measurementId: "G-KQT7GMQR5W"
  };

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

// Function to fetch and update the dashboard with Firebase data
async function fetchDataFromFirebase() {
  const humidityRef = ref(database, 'DHT11/data/kelembapan'); // Path to your humidity data
  const temperatureRef = ref(database, 'DHT11/data/temperature'); // Path to your temperature data
  const moistureRef = ref(database, 'Moisture/soil_moisture'); // Path to your moisture data

  // Fetch humidity data
  try {
    const humiditySnapshot = await get(humidityRef);
    if (humiditySnapshot.exists()) {
      const humidity = humiditySnapshot.val();
      document.getElementById('humidity').innerText = `${humidity}%`;
    } else {
      console.log('No humidity data available');
    }
  } catch (error) {
    console.error('Error fetching humidity data:', error);
  }

  // Fetch temperature data
  try {
    const temperatureSnapshot = await get(temperatureRef);
    if (temperatureSnapshot.exists()) {
      const temperature = temperatureSnapshot.val();
      document.getElementById('temperature').innerText = `${temperature}°C`;
    } else {
      console.log('No temperature data available');
    }
  } catch (error) {
    console.error('Error fetching temperature data:', error);
  }

  // Fetch moisture data
  try {
    const moistureSnapshot = await get(moistureRef);
    if (moistureSnapshot.exists()) {
      const moisture = moistureSnapshot.val();
      document.getElementById('moisture').innerText = `${moisture}%`;
    } else {
      console.log('No moisture data available');
    }
  } catch (error) {
    console.error('Error fetching moisture data:', error);
  }
}

// Fetch data every 5 seconds
setInterval(fetchDataFromFirebase, 5000);

// Initial fetch on page load
window.onload = fetchDataFromFirebase;
