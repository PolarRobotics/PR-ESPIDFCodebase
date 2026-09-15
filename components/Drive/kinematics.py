# Use uv for python with this
# Run using (while in Display_inverse_kin folder): uv run kinematics.py
# Also, currently everything is in inches

# TODO: update_arrows function: add code to calculate the inverse kinematics stuff

import matplotlib.pyplot as plt
import numpy as np
import time
import matplotlib.patches as patches
import keyboard
from matplotlib.widgets import TextBox 
import numbers

baseplate_velocity = 0
baseplate_angle = 0

wheel1_offset = np.array([4.55, 4.55])  # x,y offset from center of robot
wheel2_offset = np.array([-4.55, 4.55])
wheel3_offset = np.array([-4.55, -4.55])
wheel4_offset = np.array([4.55, -4.55])

# Turn on interactive mode for data plotting
plt.ion()

# Create initial data
x = np.linspace(0, 6, 100)
y = np.sin(x)

fig, ax = plt.subplots()
fig.subplots_adjust(bottom=0.2)  # Make room for the text box
axbox = fig.add_axes([0.1, 0.05, 0.8, 0.075])  # [left, bottom, width, height]
text_box = TextBox(axbox, 'Angular Velocity', initial='0')
current_ang_vel = 0.0

# Set fixed axis limits so view doesn't move
ax.set_xlim(-10, 10)
ax.set_ylim(-10, 10)

# Draw base outline for wheels, centered at (0,0)
# base for wheels is roughly 9.1 x 9.1 inches
base_wheels_width = 9.1
base_wheels_height = 9.1
robot_base_for_wheels = patches.Rectangle((-base_wheels_width/2, -base_wheels_height/2), 
                                          base_wheels_width, base_wheels_height, linewidth=2, edgecolor='black', facecolor='none')
ax.add_patch(robot_base_for_wheels)

# base plate is roughly 15.5 x 15.5 inches
# TODO: add the base plate rectangle later

# create wheels
# wheels are 4 x 1.3 inches
wheel_width = 1.3
wheel_height = 4
#x_wheel_2 = -4.55 - 1.3/2
#y_wheel_2 = 4.55 - 4/2
y_wheel_2 = base_wheels_height/2 - wheel_height/2
wheel1 = patches.Rectangle(((base_wheels_width/2 - wheel_width/2), (base_wheels_height/2 - wheel_height/2)), 
                           wheel_width, wheel_height, linewidth=2, edgecolor='k', facecolor='none')
wheel2 = patches.Rectangle(((-base_wheels_width/2 - wheel_width/2), (base_wheels_height/2 - wheel_height/2)), 
                           wheel_width, wheel_height, linewidth=2, edgecolor='k', facecolor='none')
wheel3 = patches.Rectangle(((-base_wheels_width/2 - wheel_width/2), (-base_wheels_height/2 - wheel_height/2)), 
                           wheel_width, wheel_height, linewidth=2, edgecolor='k', facecolor='none')
wheel4 = patches.Rectangle(((base_wheels_width/2 - wheel_width/2), (-base_wheels_height/2 - wheel_height/2)), 
                           wheel_width, wheel_height, linewidth=2, edgecolor='k', facecolor='none')
# Add wheels to display
ax.add_patch(wheel1)
ax.add_patch(wheel2)
ax.add_patch(wheel3)
ax.add_patch(wheel4)

# create and add angular and linear velocity arrows/indicators for each wheel
#arr1 = patches.Arrow(x = 4.55, y = 4.55, dx = 0, dy = 2, width = .5, color = 'b')
arr1 = patches.FancyArrowPatch(posA=(4.55, 4.55), posB=(4.55, 6.55), arrowstyle='->', mutation_scale=10,linewidth=2, color = 'b')
arr2 = patches.FancyArrowPatch(posA=(-4.55, 4.55), posB=(-4.55, 6.55), arrowstyle='->', mutation_scale=10,linewidth=2, color = 'b')
arr3 = patches.FancyArrowPatch(posA=(-4.55, -4.55), posB=(-4.55, -2.55), arrowstyle='->', mutation_scale=10,linewidth=2, color = 'b')
arr4 = patches.FancyArrowPatch(posA=(4.55, -4.55), posB=(4.55, -2.55), arrowstyle='->', mutation_scale=10,linewidth=2, color = 'b')
ax.add_patch(arr1)
ax.add_patch(arr2)
ax.add_patch(arr3)
ax.add_patch(arr4)
# baseplate arrow
baseplate_arrow = patches.FancyArrowPatch(posA=(0, 0), posB=(0, 2), arrowstyle='->', mutation_scale=10,linewidth=2, color = 'b')
ax.add_patch(baseplate_arrow)


def calculate_inverse_kinematics(lin_vel_x, lin_vel_y, angular_vel):
    r1 = wheel1_offset
    r2 = wheel2_offset
    r3 = wheel3_offset
    r4 = wheel4_offset
    
    def calculate_wheel_velocity(position, linear_vx, linear_vy, omega):
        # Calculate cross product term (angular velocity contribution)
        cross_product_x = -omega * position[1]
        cross_product_y = omega * position[0]
        
        # Total velocity components
        vx = linear_vx + cross_product_x
        vy = linear_vy + cross_product_y
        
        # Calculate magnitude and angle using atan2 for proper quadrant handling
        velocity_magnitude = np.sqrt(vx*vx + vy*vy)
        angle_radians = np.arctan2(vy, vx)
        angle_degrees = np.degrees(angle_radians)
        
        return velocity_magnitude, angle_degrees
    
    # Calculate velocities for each wheel
    v1_mag, angle1 = calculate_wheel_velocity(r1, lin_vel_x, lin_vel_y, angular_vel)
    v2_mag, angle2 = calculate_wheel_velocity(r2, lin_vel_x, lin_vel_y, angular_vel)
    v3_mag, angle3 = calculate_wheel_velocity(r3, lin_vel_x, lin_vel_y, angular_vel)
    v4_mag, angle4 = calculate_wheel_velocity(r4, lin_vel_x, lin_vel_y, angular_vel)
    
    # Baseplate calculation
    v0_mag = np.sqrt(lin_vel_x*lin_vel_x + lin_vel_y*lin_vel_y)
    angle0 = np.degrees(np.arctan2(lin_vel_y, lin_vel_x))
    
    # testing print statements
    #print(f"Wheel 1: magnitude={v1_mag:.2f}, angle={angle1:.2f}°")
    #print(f"Wheel 2: magnitude={v2_mag:.2f}, angle={angle2:.2f}°")
    #print(f"Wheel 3: magnitude={v3_mag:.2f}, angle={angle3:.2f}°")
    #print(f"Wheel 4: magnitude={v4_mag:.2f}, angle={angle4:.2f}°")
    #print(f"Baseplate: magnitude={v0_mag:.2f}, angle={angle0:.2f}°")
    
    # Update arrows with correct magnitudes and angles
    update_arrows(arr1, v1_mag, angle1)
    update_arrows(arr2, v2_mag, angle2)
    update_arrows(arr3, v3_mag, angle3)
    update_arrows(arr4, v4_mag, angle4)
    update_arrows(baseplate_arrow, v0_mag, angle0)

# update_Arrows: updates the arrows on the plot based on the current velocities
# baseplate_velocity: velocity of the baseplate
# baseplate_angle: angle of the baseplate
def update_arrows(arrow, baseplate_velocity, baseplate_angle):
    start = arrow.get_path().vertices[0]

    # Decay based function to represent different velocities 
    t = baseplate_velocity      # baseplate_velocity will serve as the independent variable in the decay function
    A = 5.0                     # Maximum length of arrow
    min = 1.0                   # Minimum length of arrow
    k = .2                      # rate constant (controls how fast it approaches the limit)
    #Returns a value that approaches A as t increases.
    arr_length = min + (A-min)*(1-np.exp(-k*t))

    angle_rad = np.radians(baseplate_angle)

    new_x = start[0] + arr_length * np.cos(angle_rad)
    new_y = start[1] + arr_length * np.sin(angle_rad)    

    arrow.set_positions(posA=None, posB=(new_x, new_y)) #posA=None saved this project
    pass

# This should help the Code TextBox not to interfere with keyboard events (crash less often)        
def setup_event_handling(self):
    # Disconnect existing handlers
    self.fig.canvas.mpl_disconnect(self.fig.canvas.manager.key_press_handler_id)
    
    # Store original handlers
    self.original_handlers.append(
        self.fig.canvas.mpl_connect('key_press_event', 
                                    lambda event: None))
    
    # Reconnect handlers after text entry
    def on_submit(text):
        self.fig.canvas.mpl_disconnect(self.fig.canvas.manager.key_press_handler_id)
        self.fig.canvas.manager.key_press_handler_id = (
            self.fig.canvas.mpl_connect('key_press_event',
                                        self.fig.canvas.manager.key_press_handler))
        
    self.text_box.on_submit(on_submit)

# onclick(event): defines what happens when the mouse is clicked on the display
def onclick(event):
    if event.inaxes == ax:
        x_data, y_data = event.xdata, event.ydata
        calculate_inverse_kinematics(lin_vel_x=x_data, lin_vel_y=y_data, angular_vel=current_ang_vel) 

        fig.canvas.draw() # tells matplotlib to redraw the canvas immediatly
        fig.canvas.flush_events() # Basically makes sure the drawing updates happens right away

# Connects the click event
fig.canvas.mpl_connect('button_press_event', onclick)

# submit_text(text): grabs the text from the text box, will update next time onClick is called
# new_ang_vel: the new angular velocity that was supplied by the text box
def submit_text(new_ang_vel):
    print(f"Text entered: {new_ang_vel}")
    global current_ang_vel

    try:
        current_ang_vel = float(new_ang_vel)
        #print(f"The string '{new_ang_vel}' converted to a double is: {new_ang_vel}")
    except ValueError as e:
        print(f"Error converting '{new_ang_vel}' to a double: {e}")

def display_graph():
    # Update the plot in real time
    fig.canvas.draw()
    fig.canvas.flush_events()

    text_box.on_submit(submit_text)

    while (True):
        fig.canvas.start_event_loop(0.1)  # Check for events
        if keyboard.is_pressed('q'):
            break
        elif keyboard.is_pressed('w'):    # set coordinate input for testing
            calculate_inverse_kinematics(lin_vel_x=7.58, lin_vel_y=-1.13, angular_vel=0) 

            fig.canvas.draw() # tells matplotlib to redraw the canvas immediatly
            fig.canvas.flush_events() # Basically makes sure the drawing updates happens right away

            time.sleep(0.5)  # small delay so updates are visible

        # Force a redraw if needed
        fig.canvas.draw_idle()
        fig.canvas.flush_events()
            
            #Need to make another thing that updates based on keyboard input because as of now its only updating when w is pressed
            # HUZZAH THIS IS SUCH GOOD NEWS! (code has since been deleted but this was when I figured out a big bug in the code)
        #elif keyboard.is_pressed():

    plt.ioff()
    plt.show()

display_graph()