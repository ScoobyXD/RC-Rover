import websocket
import time
import config


# Replace with your ESP32's IP address
ESP32_WS_ENDPOINT = f"ws://{config.ESP32_IP}/ws"

# Function to send commands
def send_command(ws, command):
    print(f"Sending command: {command}")
    ws.send(command)  # Send the command to the ESP32
    time.sleep(1)

# Callback when a message is received from the ESP32
def on_message(ws, message):
    print(f"Received from ESP32: {message}")

# Callback for errors
def on_error(ws, error):
    print(f"Error: {error}")

# Callback when the connection is closed
def on_close(ws, close_status_code, close_msg):
    print("Connection closed")

# Callback when the connection is opened
def on_open(ws):
    print("Connected to ESP32 WebSocket server")
    # Example commands to send to the ESP32
    send_command(ws, config.GO)
    send_command(ws, config.BACK)
    send_command(ws, config.RIGHT)
    send_command(ws, config.LEFT)
    send_command(ws, config.STOP)
    #ws.close()

if __name__ == "__main__":
    # Connect to the ESP32 WebSocket server
    ws = websocket.WebSocketApp(
        ESP32_WS_ENDPOINT,
        on_message=on_message,
        on_error=on_error,
        on_close=on_close,
        on_open=on_open
    )
    ws.run_forever()