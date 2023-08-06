import socket

# Send command to a specific port and get the response
def send_command(host, port, command):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(5)  # Set a timeout for the connection
            s.connect((host, port))
            s.sendall(command.encode())
            response = s.recv(1024).decode().strip()
            return response
    except socket.timeout:
        return "Timeout"
    except ConnectionRefusedError:
        return "Connection Refused"
    except Exception as e:
        return str(e)

# Send commands to open ports and get responses
def send_commands_to_open_ports(host, start_port, end_port, command):
    for port in range(start_port, end_port + 1):
        response = send_command(host, port, command)
        print(f"Port {port}: {response}")

# Get user input for the command and host
command_to_send = input("Enter the command to send: ")
host = input("Enter the host to connect to: ")
start_port = int(input("Enter the start port: "))
end_port = int(input("Enter the end port: "))

# Send commands to open ports and get responses
send_commands_to_open_ports(host, start_port, end_port, command_to_send)
