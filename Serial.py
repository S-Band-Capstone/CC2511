import serial
import time
import msvcrt
import sys
from rich.console import Console
from rich.markdown import Markdown
from rich.panel import Panel
from rich.progress import Progress, SpinnerColumn, TextColumn
from rich.table import Table
from rich.theme import Theme
from rich.syntax import Syntax

# Constants for message framing
SOF = 0x7E  # Start of Frame
EOF = 0x7F  # End of Frame
CMD_MESSAGE = 0x00  # Command byte for a message

console = Console()

def send_message(ser, text):
    """
    Formats and sends a message over the serial port using the specified protocol.
    
    Format: [size][SOF][CMD][payload][EOF]
    - size: 1 byte, length of the message after the size byte
    - SOF: 1 byte, Start of Frame (0x7E)
    - CMD: 1 byte, Command (0x00 for message)
    - payload: variable length, the text to be sent
    - EOF: 1 byte, End of Frame (0x7F)
    
    Parameters:
        ser (serial.Serial): The serial port instance.
        text (str): The text message to be sent.
    """
    # Convert text to bytes
    payload = text.encode('utf-8')
    
    # Calculate size (SOF + CMD + payload + EOF = payload length + 3)
    size = len(payload) + 3
    if size > 255:
        raise ValueError("Total message length exceeds 255 bytes.")
    
    # Construct the complete message
    message = bytes([size, SOF, CMD_MESSAGE]) + payload + bytes([EOF])
    ser.write(message)
    console.print("Sent text: ", f"{text}", style="bold green")
    console.print("Sent hexa: ", f"[bold green]{message.hex(' ')}[bold green]", style="bold green")

def read_message(ser):
    """
    Reads a message from the serial port using the specified protocol.
    First reads one byte for size, then reads that many bytes.
    
    Parameters:
        ser (serial.Serial): The serial port instance.
    
    Returns:
        tuple: (command, payload) if a valid message is received, or None if no message or invalid.
    """
    # Read the first byte which indicates the size of the remaining message
    size_byte = ser.read(1)
    if not size_byte:
        return None
    
    size = size_byte[0]
    if size < 3:  # Minimum valid size (SOF + CMD + EOF)
        return None
    
    # Read the full message based on the specified size
    message = ser.read(size)
    if len(message) < size:
        return None  # Incomplete message
    
    # # Validate message format
    # if message[0] != SOF or message[-1] != EOF:
    #     return None  # Invalid framing
    
    # Extract command and payload
    cmd = message[1]
    payload = message[2:-1]
    
    console.print("\nReceived raw: ", f"[bold red]{size_byte.hex(' ')}[bold red]", f"[bold red]{message.hex(' ')}[bold red]", style="bold red")
    return (cmd, payload)

def main():
    # Get COM port from command-line arguments or use default
    if len(sys.argv) > 1:
        port = sys.argv[1]
    else:
        port = "COM1"  # Default port
    
    baud_rate = 9600
    print(f"Opening serial port {port} at {baud_rate} baud...")
    
    try:
        ser = serial.Serial(port, baud_rate, timeout=1)
    except serial.SerialException as e:
        print(f"Error opening serial port {port}: {e}")
        return
    
    # Wait for the serial connection to be established
    time.sleep(2)
    print(f"Connected to {port}. Type your message and press Enter to send.")
    print("Press Ctrl+C to exit.")
    
    user_input = ""
    print("Enter a message to send: ", end="", flush=True)
    
    try:
        while True:
            # Check for incoming messages
            if ser.in_waiting > 0:
                result = read_message(ser)
                if result:
                    cmd, payload = result
                    if cmd == CMD_MESSAGE:
                        try:
                            text = payload.decode('utf-8')
                            console.print(f"[bold red]Received message: {text}[bold red]")
                            print(f"Enter a message to send: {user_input}", end="", flush=True)
                        except UnicodeDecodeError:
                            print(f"\nReceived non-text data: {payload.hex(' ')}")
                            print(f"Enter a message to send: {user_input}", end="", flush=True)
            
            # Check for keyboard input
            if msvcrt.kbhit():
                char = msvcrt.getch().decode('utf-8', errors='replace')
                
                if char == '\r':  # Enter key
                    print()  # Move to next line
                    if user_input:
                        send_message(ser, user_input)
                        user_input = ""
                    print("Enter a message to send: ", end="", flush=True)
                
                elif char == '\x08':  # Backspace
                    if user_input:
                        user_input = user_input[:-1]
                        print("\b \b", end="", flush=True)
                
                elif ord(char) >= 32:  # Printable characters
                    user_input += char
                    print(char, end="", flush=True)
            
            time.sleep(0.01)
            
    except KeyboardInterrupt:
        print("\nExiting program.")
    except Exception as e:
        print(f"\nError: {e}")
    finally:
        ser.close()
        print(f"Serial port {port} closed.")

if __name__ == "__main__":
    main()