import socket
import time
import select

def communicate_with_esp32():
    print("Beginning communication script")
    esp32_ip = "10.96.112.57"  # Replace with the ESP32's IP address
    esp32_port = 80             # Replace with the ESP32's port number
    timeout = 5                   # Timeout for receiving data in seconds

    while True:
        try:
            esp32_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            esp32_socket.settimeout(5)

            print(f"Connecting to ESP32 at {esp32_ip}:{esp32_port}")
            esp32_socket.connect((esp32_ip, esp32_port))
            print("Connected to ESP32!")

            while True:
                message = "request"
                print("The loop in continuation...")
                esp32_socket.send(message.encode())
                print("Message sent: 'request'")

                # Use select to wait for data or timeout
                ready, _, _ = select.select([esp32_socket], [], [], timeout)

                if ready:
                    raw_data = esp32_socket.recv(256).decode()
                    print(f"Received data: {raw_data}")
                else:
                    print("No data received from ESP32 within the timeout.")

                time.sleep(1)  # Wait for 1 second before sending the next request

        except socket.error as e:
            print(f"Communication error: {e}")
            print("Connection lost. Retrying...")
            time.sleep(2)  # Wait for a moment before attempting to reconnect

        finally:
            esp32_socket.close()
            print("Connection closed. Reconnecting...")

if __name__ == "__main__":
    communicate_with_esp32()
