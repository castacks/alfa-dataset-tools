# Message for outputting a Vector3 navigation information about the vehicle
#

uint8 MAV_FRAME_GLOBAL_INT = 5
uint8 MAV_FRAME_GLOBAL_RELATIVE_ALT_INT = 6
uint8 MAV_FRAME_GLOBAL_TERRAIN_ALT_INT = 11

std_msgs/Header header	# Message header
uint8 coordinate_frame	# Options are listed above

float32 des_x		# Current desired X signal
float32 des_y		# Current desired Y signal
float32 des_z		# Current desired Z signal

float32 meas_x		# Current measured X signal
float32 meas_y		# Current measured Y signal
float32 meas_z		# Current measured Z signal

