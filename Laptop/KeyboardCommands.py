import websocket
import keyboard
import config

#test gitignore
# WiFi
ESP32_WS_ENDPOINT = f"ws://{config.ESP32_IP}/ws"
ws = websocket.WebSocket()

#for some god forsaken reason in order for this to work need to put "event" in the parameter
def w1(event):
    ws.send('w1')
    print('w1')

def w0(event):
    ws.send('w0')
    print('w0')

def s1(event):
    ws.send('s1')
    print('s1')

def s0(event):
    ws.send('s0')
    print('s0')

def d1(event):
    ws.send('d1')
    print('d1')

def d0(event):
    ws.send('d0')
    print('d0')

def a1(event):
    ws.send('a1')
    print('a1')

def a0(event):
    ws.send('a0')
    print('a0')

def wifi_on(event):
    ws.connect(ESP32_WS_ENDPOINT)
    print('connecting')
    
def wifi_off(event):
    ws.close()
    print('close')


#Wi-Fi on/off commands
keyboard.on_press_key("o", wifi_on)
keyboard.on_press_key("p", wifi_off)

#movement commands
keyboard.on_press_key("w", w1)
keyboard.on_release_key("w", w0)
keyboard.on_press_key("s", s1)
keyboard.on_release_key("s",s0)
keyboard.on_press_key("d", d1)
keyboard.on_release_key("d",d0)
keyboard.on_press_key("a", a1)
keyboard.on_release_key("a",a0)

#exit command
keyboard.wait('esc') 


