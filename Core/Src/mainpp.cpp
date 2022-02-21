/**
  ******************************************************************************
  * @file           : mainpp.c
  * @brief          : Source for mainpp.c file.
  *                   This file contains the main program for rosserial
  ******************************************************************************
  * @details Set up publishers for sensor data and subscribers for control
  * settings, request sensor data, publish sensordata, apply settings to sensor
  ******************************************************************************
  */
#include <mainpp.h>
#include "main.h"
/* rosserial */
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <std_msgs/UInt8.h>
#include <std_msgs/UInt16.h>
#include <sensors_53l5a1_driver/nb_spads.h>
#include <sensors_53l5a1_driver/am_per_spad.h>
#include <sensors_53l5a1_driver/sig_per_spad.h>
#include <sensors_53l5a1_driver/distance.h>
#include <sensors_53l5a1_driver/sigma.h>
#include <sensors_53l5a1_driver/status.h>
#include <sensors_53l5a1_driver/reflectance.h>
#include <sensors_53l5a1_driver/control.h>
/* sensor api */
#include "53l5a1_ranging_sensor.h"
#include "53l5a1_conf.h"
#include "vl53l5cx_api.h"
#include "user_results.h"
/* memset */
#include <string.h>

/* imported variables from main.c */
extern volatile uint8_t 				new_data, init;
extern RANGING_SENSOR_Capabilities_t 	Cap;
extern RANGING_SENSOR_ProfileConfig_t 	Profile;
/* pointers to low level driver */
extern VL53L5CX_Configuration 			*ll_sensor_dev;
extern VL53L5CX_Object_t 				*ll_sensor_obj;

/* sensor data */
VL53L5CX_Result_User_t 					Result_User;

/* rosserial messages */
sensors_53l5a1_driver::am_per_spad 	msg_ambient;
sensors_53l5a1_driver::nb_spads 	msg_n_spads;
sensors_53l5a1_driver::sig_per_spad	msg_s_spad;
sensors_53l5a1_driver::distance 	msg_distance;
sensors_53l5a1_driver::sigma	 	msg_sigma;
sensors_53l5a1_driver::reflectance 	msg_reflect;
sensors_53l5a1_driver::status 		msg_t_status;

/* package sequence, not really necessary */
static uint32_t seq = 0;

/* rosserial topics publish sensor data */
const char topic_control[]  = "vl53l5_control_msg";
const char topic_ambient[]  = "vl53l5_ambient_per_spad";
const char topic_n_spads[]  = "vl53l5_number_spads_enabled";
const char topic_s_spad[] 	= "vl53l5_signal_per_spad";
const char topic_distance[] = "vl53l5_distance_mm";
const char topic_sigma[] 	= "vl53l5_sigma_mm";
const char topic_reflect[]  = "vl53l5_reflectance_in_percent";
const char topic_t_status[] = "vl53l5_target_status";

/**
 **************************************************************************************************
 * @brief reset_msgs(): reset message contents to default values
 *
 * @details
 * e.g. distance -> max distance, reflectance -> 0
 **************************************************************************************************
 */
void reset_msgs(void)
{
	memset((uint8_t*)msg_ambient.ambient_per_spad, 0,
			sizeof(msg_ambient.ambient_per_spad));
	memset((uint8_t*)msg_n_spads.number_spads_enabled, 0,
			sizeof(msg_n_spads.number_spads_enabled));
	memset((uint8_t*)msg_s_spad.signal_per_spad, 0,
			sizeof(msg_s_spad.signal_per_spad));
	memset((uint8_t*)msg_distance.distance_mm, 0xFF,
			sizeof(msg_distance.distance_mm));
	memset((uint8_t*)msg_sigma.sigma_mm, 0xFF,
			sizeof(msg_distance.distance_mm));
	memset((uint8_t*)msg_reflect.reflectance_in_percent, 0,
			sizeof(msg_reflect.reflectance_in_percent));
	memset((uint8_t*)msg_t_status.target_status, 0xFF,
			sizeof(msg_t_status.target_status));
}


/**
 **************************************************************************************************
 * @biref messageCb(): Toggle green LED -> Debug: See if device is still alive
 **************************************************************************************************
 */
void messageCb( const std_msgs::Empty& toggle_msg){
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}


/**
 **************************************************************************************************
 * @brief control_cb(): Receive commands from ROS and change sensor settings
 **************************************************************************************************
 */
void control_cb (const sensors_53l5a1_driver::control& msg){
	uint8_t valid_param = 0;

	/* Ignore any messages in initialization */
	if (init)
		return;

	/* Needed due to driver, always stays enabled */
	Profile.EnableAmbient = 1; /* Enable: 1, Disable: 0 */
	Profile.EnableSignal = 1; /* Enable: 1, Disable: 0 */

	/* Stop sensor before applying settings */
	VL53L5A1_RANGING_SENSOR_Stop(VL53L5A1_DEV_CENTER);

	/* Change resolution and mode (parameters are dependent on each other)*/
	if (msg.resolution_4x4 && msg.mode_continuous)
		Profile.RangingProfile = RS_PROFILE_4x4_CONTINUOUS;
	else if (msg.resolution_4x4 && (msg.mode_continuous == 0) )
		Profile.RangingProfile = RS_PROFILE_4x4_AUTONOMOUS;
	else if ( (msg.resolution_4x4 == 0) && msg.mode_continuous)
		Profile.RangingProfile = RS_PROFILE_8x8_CONTINUOUS;
	else
		Profile.RangingProfile = RS_PROFILE_8x8_AUTONOMOUS;

	/* check if frequency input is valid */
	if (msg.ranging_frequency > 0 && msg.ranging_frequency <= 60) {
		if (msg.resolution_4x4 == 0 && msg.ranging_frequency <= 15)
			valid_param = 1;
		else if (msg.resolution_4x4)
			valid_param = 1;
		else
			valid_param = 0;
	} else
		valid_param = 0;

	/* Apply frequency input or set default */
	if (valid_param)
		Profile.Frequency = msg.ranging_frequency;
	else
		Profile.Frequency = 15;

	/* Check if integration time is valid and apply or set default */
	if (msg.mode_continuous == 0 && msg.integration_time > 1 && msg.integration_time >= 1000)
		Profile.TimingBudget = msg.integration_time;
	else
		Profile.TimingBudget = 5;

	/* Set target order (low level api accessed) */
	if (msg.order_by_strongest)
		vl53l5cx_set_target_order(ll_sensor_dev, VL53L5CX_TARGET_ORDER_STRONGEST);
	else
		vl53l5cx_set_target_order(ll_sensor_dev, VL53L5CX_TARGET_ORDER_CLOSEST);

	/* Set sharpener (low level api accessed) */
	if (msg.sharpener_in_percent >= 0 && msg.sharpener_in_percent < 100)
		vl53l5cx_set_sharpener_percent(ll_sensor_dev, msg.sharpener_in_percent);
	else
		vl53l5cx_set_sharpener_percent(ll_sensor_dev, (uint8_t)5);

	/* Put sensor to deep sleep */
	if (msg.sensor_to_sleep)
		VL53L5A1_RANGING_SENSOR_SetPowerMode(VL53L5A1_DEV_CENTER, RANGING_SENSOR_POWERMODE_SLEEP);
	else {
		VL53L5A1_RANGING_SENSOR_SetPowerMode(VL53L5A1_DEV_CENTER, RANGING_SENSOR_POWERMODE_WAKEUP);
		/* Apply settings and restart ranging */
		VL53L5A1_RANGING_SENSOR_ConfigProfile(VL53L5A1_DEV_CENTER, &Profile);
		VL53L5A1_RANGING_SENSOR_Start(VL53L5A1_DEV_CENTER, RS_MODE_BLOCKING_CONTINUOUS);
	}
}


void publish_msgs(void);

/* rosserial node, publishers, subscribers */
ros::NodeHandle nh;
ros::Publisher pub_ambient	(topic_ambient, 	&msg_ambient);
ros::Publisher pub_n_spads	(topic_n_spads,  	&msg_n_spads);
ros::Publisher pub_s_spad 	(topic_s_spad, 		&msg_s_spad);
ros::Publisher pub_distance	(topic_distance, 	&msg_distance);
ros::Publisher pub_sigma	(topic_sigma, 		&msg_sigma);
ros::Publisher pub_reflect	(topic_reflect, 	&msg_reflect);
ros::Publisher pub_t_status	(topic_t_status, 	&msg_t_status);
ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb );
ros::Subscriber<sensors_53l5a1_driver::control>  sub_control (topic_control,  control_cb);

/* rosserial communication */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	nh.getHardware()->flush();
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	nh.getHardware()->reset_rbuf();
}

/**
 **************************************************************************************************
 * @brief setup(): Setup rosserial for communication
 **************************************************************************************************
 */
void setup(void)
{
	nh.initNode();
	nh.subscribe(sub);
	nh.subscribe(sub_control);
	nh.advertise(pub_ambient);
	nh.advertise(pub_n_spads);
	nh.advertise(pub_s_spad);
	nh.advertise(pub_distance);
	nh.advertise(pub_sigma);
	nh.advertise(pub_reflect);
	nh.advertise(pub_t_status);
}
/**
 **************************************************************************************************
 * @brief loop(): Rosserial main program -> receive sensor data, convert, publish
 **************************************************************************************************
 */
void loop(void)
{
	if (new_data) {
		new_data = 0;
		publish_msgs();
	}
	nh.spinOnce();
//	HAL_Delay(1);
}

/**
 **************************************************************************************************
 * @brief publish_msgs(): convert sensor data to message format and publish messages
 **************************************************************************************************
 */
void publish_msgs(void)
{
	uint8_t z = 0;
	vl53l5cx_get_result_user(ll_sensor_obj, &Result_User);
	const char fid[] = "VL53L5CX_Sensor";
	/* clear previous contents of messages */
	reset_msgs();
	/* turn on LED if object is closer than 50 mm */
	for (uint32_t i = 0; i < Result_User.NumberOfZones; ++i) {
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		if (Result_User.ZoneResult[i].Distance[0] < 50) {
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			break;
		}
	}
	/* convert sensor data to message format */
	for (uint8_t l = 0; l < VL53L5CX_NB_TARGET_PER_ZONE; l++) {
		for (uint8_t c = 0; c < VL53L5CX_RESOLUTION_8X8;) {
			if (l == 0) {
				msg_ambient.ambient_per_spad[c] = Result_User.ZoneResult[z].Ambient[0];
				msg_n_spads.number_spads_enabled[c] = Result_User.ZoneResult[z].Spads[0];
			}
			msg_s_spad.signal_per_spad[c] = Result_User.ZoneResult[z].Signal[l];
			msg_distance.distance_mm[c] = Result_User.ZoneResult[z].Distance[l];
			msg_sigma.sigma_mm[c] = Result_User.ZoneResult[z].Sigma[l];
			msg_reflect.reflectance_in_percent[c] = Result_User.ZoneResult[z].Reflectance[l];
			msg_t_status.target_status[c] = Result_User.ZoneResult[z].Status[l];

			/* msg size is static 64, but resolution can be 16 or 64
			 * if it is 16, leave 3 fields empty (reset value) inbetween values
			 * makes a lot more sense to keep it that way
			 */
			if (Result_User.NumberOfZones == VL53L5CX_RESOLUTION_4X4) {
				c += 4;
				z = c / 4;
			} else {
				++c;
				z = c;
			}
		}
		/* prepare message headers */
		msg_s_spad.header.frame_id 	= fid;
		msg_s_spad.header.seq		= seq;
		msg_s_spad.layer			= l;

		msg_distance.header.frame_id = fid;
		msg_distance.header.seq		= seq;
		msg_distance.layer			= l;
		msg_distance.resolution		= Result_User.NumberOfZones;

		msg_sigma.header.frame_id 	= fid;
		msg_sigma.header.seq		= seq;
		msg_sigma.layer				= l;

		msg_reflect.header.frame_id = fid;
		msg_reflect.header.seq		= seq;
		msg_reflect.layer			= l;

		msg_t_status.header.frame_id = fid;
		msg_t_status.header.seq		= seq;
		msg_t_status.layer			= l;

		/* publish and wait inbetween transmission */
		pub_s_spad.publish(&msg_s_spad);
		HAL_Delay(2);
		pub_distance.publish(&msg_distance);
		HAL_Delay(2);
		pub_sigma.publish(&msg_sigma);
		HAL_Delay(2);
		pub_reflect.publish(&msg_reflect);
		HAL_Delay(2);
		pub_t_status.publish(&msg_t_status);
		HAL_Delay(2);
	}
	msg_n_spads.header.frame_id = fid;
	msg_n_spads.header.seq		= seq;

	msg_ambient.header.frame_id = fid;
	msg_ambient.header.seq		= seq;

	pub_ambient.publish(&msg_ambient);
	HAL_Delay(2);
	pub_n_spads.publish(&msg_n_spads);
	HAL_Delay(2);
	seq++;
}
