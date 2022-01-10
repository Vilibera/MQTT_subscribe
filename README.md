# MQTT_subscribe
MQTT_subscribe with curl library


# Program configuration:
Firstly open your device web go to the Services/MQTT/Broker and press enable
Second step is allow anonymous in Services/MQTT/Broker/BROKER SETTINGS/Miscellaneous
If all went well go to Services/MQTT/Subscriber and fill all the fields:

  1: Press enable
	
  2: Write your device ip address (For example: 192.168.1.1)
	
  3: Write mqtt broker port (In default is 1883)
	
  4: Write topic name 
	
  5: QoS Level (Quality of Service)
	0 - QoS 0 - at most once
	The minimal QoS level is zero. This service level guarantees a best-effort delivery. There is no guarantee of delivery.		
			1 - QoS 1 - at least once
					QoS level 1 guarantees that a message is delivered at least one time to the receiver.		
			2 - QoS 2 - exactly once
					QoS 2 is the highest level of service in MQTT. This level guarantees that each message is received only once by the intended recipients. 
					QoS 2 is the safest and slowest quality of service level.
				
 6: Type( String or Decimal)
 
 7: Key (JsoN key - using when you want to publish message to subscriber)
 
 8: Comparison( If you select equal comparison and type decimal write in value field for example 10. And when your publish message and topic message will be equal
 this program will sent email letter to yours email address)
 
 9: Value(Topic value)
 
 10: Recipients email address( In this field write email where program should sent message about program) 
 
 Program configuration is done

# How use program
1: Firstly connect to device terminal with ssh( For example ssh@(your router ip address)

2: If connected write mosquitto_program -h (device ip address) -p(port(default is 1883))

3: Go to pc terminal and write mosquitto_pub -h(device where is a broker ip address) -p(port equal as broker) -t(topic name) -m(message value JSON format(for example -m '{"key":"message value"}'
