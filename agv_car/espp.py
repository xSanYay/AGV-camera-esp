import requests

# ESP8266 IP address
esp8266_ip = '192.168.30.42'  # Change this to your ESP8266 IP address

# Function to send a number to ESP8266
def send_number(number):
    response = requests.get(f'http://{esp8266_ip}/number?value={number}')
    print(response.text)

# Get user input for the number
number = int(input("Enter a single digit number to send to ESP8266: "))

# Send the number to ESP8266
send_number(number)
