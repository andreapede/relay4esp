#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <time.h>

const char* ssid = "your_wifi";
const char* wifiPass = "your_wifipasswoed";
const char* mqttServer = "your_mqttserver";
const char* mqttUser = "your_mqttuser";
const char* mqttPass = "your_mqttpassword";
const int mqttPort = 1883;


String mqttDevice = "espdevice"; // here the name of the device as the root topic
String connStatus = mqttDevice + "/connectionStatus"; // Topic used to publish the message for connection
String relComm = mqttDevice + "/relayCommand";  // topic to use for modifying the status of relay. Payload must be 0 or 1
String relStat = mqttDevice + "/relayStatus"; // Topic used for automatic report of the relay status

int relayPin = 16;   // GPIO prt used for the relay control
bool relayStatus = HIGH;   // First time inizialization of relay
int tellstate = 0;
String macAddress = WiFi.macAddress(); // mac address from esp8266

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
	WiFi.mode(WIFI_STA);            // esp8266 enabled only in STA Mode, no AP
	macAddress.replace(':', '-');   
	EEPROM.begin(512);             // Begin eeprom to store on/off state
	pinMode(relayPin, OUTPUT);     // Initialize the relay pin as an output

	relayStatus = EEPROM.read(0);
	digitalWrite(relayPin, relayStatus);

	Serial.begin(115200);
	Serial.println();
	Serial.print("Restore Relay state to ");
	Serial.println(relayStatus);

	setup_wifi();
	client.setServer(mqttServer, mqttPort);
	client.setCallback(callback);

	Serial.print("MAC Address: ");
	Serial.println(macAddress);

	reconnect();
}

void setup_wifi() {

	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, wifiPass);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
//  randomSeed(micros());
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char) payload[i]);
	}
	Serial.println();

	if ((char) payload[0] == '0') {   //on
		digitalWrite(relayPin, HIGH);
		Serial.println("relayPin -> HIGH");
		relayStatus = HIGH;
		EEPROM.write(0, relayStatus);    // Write state to EEPROM
		EEPROM.commit();

	} else if ((char) payload[0] == '1') {  //off
		digitalWrite(relayPin, LOW);
		Serial.println("relayPin -> LOW");
		relayStatus = LOW;
		EEPROM.write(0, relayStatus);    // Write state to EEPROM
		EEPROM.commit();
	} else if ((char) payload[0] == '2') {  //Inverted state
		relayStatus = !relayStatus; //Inverte lo stato
		digitalWrite(relayPin, relayStatus);
		Serial.print("relayPin -> switched to ");
		Serial.println(relayStatus);
		EEPROM.write(0, relayStatus); // Write state to EEPROM
		EEPROM.commit();
	}
	Serial.println();
	relaystatus();
}

void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect((char*) macAddress.c_str(), mqttUser, mqttPass)) {
			Serial.println("connected");
			// Once connected, publish an announcement...
			client.publish(connStatus.c_str(), "Connected!");
			// ... and resubscribe
			client.subscribe(relComm.c_str());
		} else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void relaystatus60() {

	// Tell the current state every 60 seconds
	if ((millis() - tellstate) > 60000) {
		if (digitalRead(relayPin)) {
			client.publish(relStat.c_str(), "1");
		} else {
			client.publish(relStat.c_str(), "0");
		}
		tellstate = millis();
	}
}

void relaystatus() {

		if (digitalRead(relayPin)) {
			client.publish(relStat.c_str(), "1");
		} else {
			client.publish(relStat.c_str(), "0");
		}
}


void loop() {
	client.loop();
	if (!client.connected()) {
		reconnect();
	}
	relaystatus60();
}

