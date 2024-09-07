import paho.mqtt.client as mqtt
import tkinter as tk
from tkinter import PhotoImage
from PIL import Image, ImageTk
import json

# Callback function for when a message is received from the MQTT broker
def on_message(client, userdata, message):
    try:
        # Decode the message payload and parse it as JSON
        data = json.loads(message.payload.decode())
        print(f"Received new data: {data}")
        # Update the GUI with the new data
        update_gui(data)

    except Exception as e:
        print(f"Error processing message: {e}")
        # Use default data in case of an error
        data = {
            "distance": 120,
            "temperature": 22.5,
            "humidity": 55,
            "motion": "No"
        }
        # Update the GUI with the default data
        update_gui(data)

# Function to update the GUI labels with the new data
def update_gui(data):
    distance_label.config(text=f"Distance: {data['distance']} cm")
    temperature_label.config(text=f"Temperature: {data['temperature']} °C")
    humidity_label.config(text=f"Humidity: {data['humidity']} %")
    motion_label.config(text=f"Motion Detected: {data['motion']}")

    # Update icons based on motion detection
    if data['motion'] == "Yes":
        motion_icon_label.config(image=motion_detected_img)
    else:
        motion_icon_label.config(image=no_motion_img)

# Function to connect to the MQTT broker and subscribe to the topic
def connect_mqtt():
    client = mqtt.Client()
    client.on_message = on_message
    client.connect("test.mosquitto.org", 1883)  # Replace with your broker address and port
    client.subscribe("esp8266/cs667paras432")  # Replace with your MQTT topic
    client.loop_start()

# Function to resize images to a fixed size
def resize_image(image_path, size=(50, 50)):
    image = Image.open(image_path)
    image = image.resize(size, Image.ANTIALIAS)
    return ImageTk.PhotoImage(image)

# Create the main application window
root = tk.Tk()
root.title("Real-Time Sensor Data")

# Load and resize images for sensor icons and motion detection
temperature_img = resize_image("temp2.png")
humidity_img = resize_image("hum.png")
distance_img = resize_image("distance.png")
motion_detected_img = resize_image("on1.png")
no_motion_img = resize_image("off1.png")

# Create and place the labels and icons in the GUI with colorful fonts
distance_icon_label = tk.Label(root, image=distance_img)
temperature_icon_label = tk.Label(root, image=temperature_img)
humidity_icon_label = tk.Label(root, image=humidity_img)
motion_icon_label = tk.Label(root, image=no_motion_img)

distance_label = tk.Label(root, text="Distance: ", font=("Helvetica", 16), fg="blue")
temperature_label = tk.Label(root, text="Temperature: ", font=("Helvetica", 16), fg="red")
humidity_label = tk.Label(root, text="Humidity: ", font=("Helvetica", 16), fg="green")
motion_label = tk.Label(root, text="Motion Detected: ", font=("Helvetica", 16), fg="purple")

# Organize the icons and labels in the GUI vertically
# Create a frame to hold the icons and labels
frame = tk.Frame(root)
frame.pack(padx=10, pady=10)

# Add each icon and label to the frame
distance_icon_label.pack(pady=5, side=tk.TOP)
distance_label.pack(pady=5, side=tk.TOP)
temperature_icon_label.pack(pady=5, side=tk.TOP)
temperature_label.pack(pady=5, side=tk.TOP)
humidity_icon_label.pack(pady=5, side=tk.TOP)
humidity_label.pack(pady=5, side=tk.TOP)
motion_icon_label.pack(pady=10, side=tk.TOP)
motion_label.pack(pady=10, side=tk.TOP)

# Start the MQTT connection
connect_mqtt()

# Run the GUI loop
root.mainloop()
