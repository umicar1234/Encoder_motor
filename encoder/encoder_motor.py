import rclpy # type: ignore
from geometry_msgs.msg import Twist # type: ignore
import serial
import time

# Define serial port and baudrate
SERIAL_PORT = '/dev/ttyACM0'  # Change this to match your Arduino's serial port
BAUDRATE = 57600

# Create serial connection
ser = serial.Serial(SERIAL_PORT, BAUDRATE)

def twist_callback(msg):
    # Print a message when receiving a Twist message
    print("Received Twist message:", msg)

    # Convert Twist message to motor commands
    linear_x = msg.linear.x
    angular_z = msg.angular.z

    # Example: Convert linear and angular velocities to motor commands
    left_motor_speed = linear_x - angular_z
    right_motor_speed = linear_x + angular_z

    # Send motor commands to Arduino
    command = f"{left_motor_speed},{right_motor_speed}\n"
    ser.write(command.encode())

def main():
    rclpy.init()
    node = rclpy.create_node('arduino_control_node')
    subscription = node.create_subscription(
        Twist,
        'cmd_vel',
        twist_callback,
        10
    )

    # Wait for messages
    rclpy.spin(node)

    # Close serial connection when node is shut down
    ser.close()

    # Clean up
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
