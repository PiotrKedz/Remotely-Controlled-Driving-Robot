# python controller
import asyncio
import websockets
from pynput import keyboard

ESP32_WS_URI = "ws://192.168.161.204:8080"  # Replace with your ESP32 IP and port
COMMANDS = {'w': "forward", 's': "backward", 'a': "left", 'd': "right"}
STOP_COMMAND = "stop"
QUIT_COMMAND = "quit"
RECONNECT_DELAY = 2  # Delay between reconnection attempts


async def send_command(websocket, command):
    """Send a command to the WebSocket server."""
    try:
        await websocket.send(command)
        print(f"Command sent: {command}")
    except websockets.ConnectionClosed:
        print("Connection closed, unable to send command.")
        raise  # Allow the exception to propagate for reconnect handling


async def keyboard_control(websocket, exit_event):
    """Listen to keyboard input and send commands to the robot."""
    print("Use W, A, S, D keys to control the robot. Press Q to quit.")
    current_command = None
    active_key = None

    async def send_loop():
        """Send commands in a continuous loop."""
        nonlocal current_command
        while not exit_event.is_set():  # Exit loop when event is set
            if current_command:
                try:
                    await send_command(websocket, current_command)
                except websockets.ConnectionClosed:
                    break  # Exit loop on disconnection
            await asyncio.sleep(0.1)  # Send commands every 100ms

    def on_press(key):
        """Handle key press events."""
        nonlocal current_command, active_key
        if hasattr(key, 'char') and key.char in COMMANDS:
            if key.char != active_key:  # Prevent duplicate commands
                active_key = key.char
                current_command = COMMANDS[active_key]
        elif hasattr(key, 'char') and key.char == 'q':
            print("Exiting control mode...")
            current_command = QUIT_COMMAND
            exit_event.set()  # Set the exit event to stop the asyncio loop

    def on_release(key):
        """Handle key release events."""
        nonlocal current_command, active_key
        if hasattr(key, 'char') and key.char == active_key:
            active_key = None
            current_command = STOP_COMMAND

    listener = keyboard.Listener(on_press=on_press, on_release=on_release)
    listener.start()

    await send_loop()


async def connect_to_esp32():
    """Establish connection to ESP32 with retry logic."""
    while True:
        try:
            print("Connecting to ESP32...")
            websocket = await websockets.connect(ESP32_WS_URI)
            print("Connected to ESP32.")
            return websocket
        except Exception as e:
            print(f"Connection failed: {e}. Retrying in {RECONNECT_DELAY} seconds...")
            await asyncio.sleep(RECONNECT_DELAY)


async def stop_robot(websocket):
    """Ensure the robot stops when quitting."""
    try:
        print("Stopping the robot...")
        await send_command(websocket, STOP_COMMAND)
    except Exception as e:
        print(f"Error while stopping the robot: {e}")


async def main():
    """Main program loop."""
    websocket = None
    exit_event = asyncio.Event()  # Event to signal the stop of the loop
    try:
        while True:
            websocket = await connect_to_esp32()
            await keyboard_control(websocket, exit_event)
            if exit_event.is_set():
                break
    except websockets.ConnectionClosed:
        print("WebSocket connection lost. Reconnecting...")
    finally:
        if websocket:
            await stop_robot(websocket)  # Ensure the robot stops before quitting
            try:
                await websocket.close()
                print("WebSocket connection closed.")
            except Exception as e:
                print(f"Error closing WebSocket: {e}")


# Run the main event loop
asyncio.run(main())