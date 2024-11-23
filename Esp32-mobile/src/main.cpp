
// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <time.h>


// // WiFi credentials
// const char* ssid = "Ragulkumar";
// const char* password = "1212121212";

// // Firebase settings
// const char* FIREBASE_PROJECT_ID = "crud-1bb79";
// const char* FIRESTORE_COLLECTION = "notes";
// const char* FIREBASE_WEB_API_KEY = "AIzaSyBu5fdDOmgp4npwkTY0mtF0BvKlG3sAu24";

// // Firestore REST API URL
// String firestoreURL = "https://firestore.googleapis.com/v1/projects/" + String(FIREBASE_PROJECT_ID) + "/databases/(default)/documents/" + FIRESTORE_COLLECTION + "?key=" + String(FIREBASE_WEB_API_KEY);

// // Define touch sensor pin
// const int touchPin1 = T0;
// const int touchPin2 = T7;
// const int touchPin3 = T4;
// const int touchPin4 = T5;
// const int touchPin5 = T6;

// // Define touch threshold
// const int touchThreshold = 40; // Adjust this threshold as needed

// void setup() {
//   Serial.begin(115200);

//   // Connect to WiFi
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//   }
//   Serial.println("Connected to WiFi");

//   // Initialize time with NTP server
//   configTime(0, 0, "pool.ntp.org", "time.nist.gov");
//   Serial.println("Time initialized from NTP server");
//   delay(2000);  // Wait for time to be set
// }

// String getCurrentTime() {
//   struct tm timeinfo;
//   if (!getLocalTime(&timeinfo)) {
//     Serial.println("Failed to obtain time");
//     return "unknown";
//   }

//   // Format time as YYYY-MM-DD HH:MM:SS
//   char timeString[20];
//   strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
//   return String(timeString);
// }

// void sendToFirestore(String message ) {
//   if (WiFi.status() == WL_CONNECTED) {
//     HTTPClient http;

//     String currentTime = getCurrentTime();
//     Serial.print("Current Time: ");
//     Serial.println(currentTime);
//     String note=message;

//     // JSON data with "hi" message and current time
//     String jsonData = "{\"fields\": {\"note\": {\"stringValue\": \""+ note +"\"}, \"timestamp\": {\"stringValue\": \"" + currentTime + "\"}}}";
    
//     http.begin(firestoreURL);
//     http.addHeader("Content-Type", "application/json");

//     int httpResponseCode = http.POST(jsonData);

//     if (httpResponseCode > 0) {
//       String response = http.getString();
//       Serial.println("Message sent to Firestore successfully:");
//       Serial.println(response);
//     } else {
//       Serial.print("Error sending message. HTTP code: ");
//       Serial.println(httpResponseCode);
//     }

//     http.end();
//   } else {
//     Serial.println("WiFi not connected");
//   }
// }

// void loop() {
//   int touchValue1 = touchRead(touchPin1);
//   int touchValue2 = touchRead(touchPin2);
//   int touchValue3 = touchRead(touchPin3);
//   int touchValue4 = touchRead(touchPin4);
//   int touchValue5 = touchRead(touchPin5);
//   Serial.print("Touch value: ");
//   Serial.println(touchValue1);

//   if (touchValue1 < touchThreshold) {
//     sendToFirestore("HELP");
//     delay(100); // Delay to avoid multiple triggers
//   }
//   else if(touchValue2<touchThreshold){
//     sendToFirestore("WATER");
//     delay(100);
//   }
//     else if(touchValue3<touchThreshold){
//     sendToFirestore("COME HERE");
//     delay(100);
//   }
//     else if(touchValue4<touchThreshold){
//     sendToFirestore("PUT THE FAN");
//     delay(100);
//   }
//     else if(touchValue5<touchThreshold){
//     sendToFirestore("NEED FOOD");
//     delay(100);
//   }
//   else{

//   }
//   delay(100);
// }
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

// WiFi credentials
const char* ssid = "Ragulkumar";
const char* password = "1212121212";

// Firebase settings
const char* FIREBASE_PROJECT_ID = "crud-1bb79";
const char* FIRESTORE_COLLECTION = "notes";
const char* FIREBASE_WEB_API_KEY = "AIzaSyBu5fdDOmgp4npwkTY0mtF0BvKlG3sAu24";

// Firestore REST API URL
String firestoreURL = "https://firestore.googleapis.com/v1/projects/" + String(FIREBASE_PROJECT_ID) + "/databases/(default)/documents/" + FIRESTORE_COLLECTION + "?key=" + String(FIREBASE_WEB_API_KEY);

// Define touch sensor pin
const int touchPins[] = {T0, T7, T4, T5, T6};
const char* messages[] = {"HELP", "WATER", "COME HERE", "PUT THE FAN", "NEED FOOD"};

// Define touch threshold
const int touchThreshold = 40; // Adjust this threshold as needed

HTTPClient http;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");

  // Initialize time with NTP server
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  delay(2000);  // Wait for time to be set

  // Initialize persistent HTTP connection
  http.begin(firestoreURL);
  http.addHeader("Content-Type", "application/json");
}

String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "unknown";
  }

  // Format time as YYYY-MM-DD HH:MM:SS
  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeString);
}

void sendToFirestore(const char* message) {
  if (WiFi.status() == WL_CONNECTED) {
    // Get the current time just before sending data
    String currentTime = getCurrentTime();
    String note = message;
    String jsonData = "{\"fields\": {\"note\": {\"stringValue\": \""+ note +"\"}, \"timestamp\": {\"stringValue\": \"" + currentTime + "\"}}}";

    // Send the data with the already-open HTTP connection
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Message sent to Firestore successfully:");
      Serial.println(response);
    } else {
      Serial.print("Error sending message. HTTP code: ");
      Serial.println(httpResponseCode);
    }
  } else {
    Serial.println("WiFi not connected");
  }
}

void loop() {
  // Iterate through each touch pin to check if it was touched
  for (int i = 0; i < 5; i++) {
    int touchValue = touchRead(touchPins[i]);
    if (touchValue < touchThreshold) {
      sendToFirestore(messages[i]);
      delay(500);  // Debounce to avoid multiple triggers
      break;  // Break to avoid checking other sensors after one is triggered
    }
  }

  delay(100);
}

