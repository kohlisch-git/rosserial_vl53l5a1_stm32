#ifndef _ROS_sensors_53l5a1_driver_control_h
#define _ROS_sensors_53l5a1_driver_control_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace sensors_53l5a1_driver
{

  class control : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef bool _mode_continuous_type;
      _mode_continuous_type mode_continuous;
      typedef bool _power_on_type;
      _power_on_type power_on;
      typedef bool _sensor_to_sleep_type;
      _sensor_to_sleep_type sensor_to_sleep;
      typedef bool _resolution_4x4_type;
      _resolution_4x4_type resolution_4x4;
      typedef bool _order_by_strongest_type;
      _order_by_strongest_type order_by_strongest;
      typedef uint8_t _ranging_frequency_type;
      _ranging_frequency_type ranging_frequency;
      typedef uint8_t _sharpener_in_percent_type;
      _sharpener_in_percent_type sharpener_in_percent;
      typedef uint16_t _integration_time_type;
      _integration_time_type integration_time;

    control():
      header(),
      mode_continuous(0),
      power_on(0),
      sensor_to_sleep(0),
      resolution_4x4(0),
      order_by_strongest(0),
      ranging_frequency(0),
      sharpener_in_percent(0),
      integration_time(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        bool real;
        uint8_t base;
      } u_mode_continuous;
      u_mode_continuous.real = this->mode_continuous;
      *(outbuffer + offset + 0) = (u_mode_continuous.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mode_continuous);
      union {
        bool real;
        uint8_t base;
      } u_power_on;
      u_power_on.real = this->power_on;
      *(outbuffer + offset + 0) = (u_power_on.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->power_on);
      union {
        bool real;
        uint8_t base;
      } u_sensor_to_sleep;
      u_sensor_to_sleep.real = this->sensor_to_sleep;
      *(outbuffer + offset + 0) = (u_sensor_to_sleep.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->sensor_to_sleep);
      union {
        bool real;
        uint8_t base;
      } u_resolution_4x4;
      u_resolution_4x4.real = this->resolution_4x4;
      *(outbuffer + offset + 0) = (u_resolution_4x4.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->resolution_4x4);
      union {
        bool real;
        uint8_t base;
      } u_order_by_strongest;
      u_order_by_strongest.real = this->order_by_strongest;
      *(outbuffer + offset + 0) = (u_order_by_strongest.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->order_by_strongest);
      *(outbuffer + offset + 0) = (this->ranging_frequency >> (8 * 0)) & 0xFF;
      offset += sizeof(this->ranging_frequency);
      *(outbuffer + offset + 0) = (this->sharpener_in_percent >> (8 * 0)) & 0xFF;
      offset += sizeof(this->sharpener_in_percent);
      *(outbuffer + offset + 0) = (this->integration_time >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->integration_time >> (8 * 1)) & 0xFF;
      offset += sizeof(this->integration_time);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        bool real;
        uint8_t base;
      } u_mode_continuous;
      u_mode_continuous.base = 0;
      u_mode_continuous.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->mode_continuous = u_mode_continuous.real;
      offset += sizeof(this->mode_continuous);
      union {
        bool real;
        uint8_t base;
      } u_power_on;
      u_power_on.base = 0;
      u_power_on.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->power_on = u_power_on.real;
      offset += sizeof(this->power_on);
      union {
        bool real;
        uint8_t base;
      } u_sensor_to_sleep;
      u_sensor_to_sleep.base = 0;
      u_sensor_to_sleep.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->sensor_to_sleep = u_sensor_to_sleep.real;
      offset += sizeof(this->sensor_to_sleep);
      union {
        bool real;
        uint8_t base;
      } u_resolution_4x4;
      u_resolution_4x4.base = 0;
      u_resolution_4x4.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->resolution_4x4 = u_resolution_4x4.real;
      offset += sizeof(this->resolution_4x4);
      union {
        bool real;
        uint8_t base;
      } u_order_by_strongest;
      u_order_by_strongest.base = 0;
      u_order_by_strongest.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->order_by_strongest = u_order_by_strongest.real;
      offset += sizeof(this->order_by_strongest);
      this->ranging_frequency =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->ranging_frequency);
      this->sharpener_in_percent =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->sharpener_in_percent);
      this->integration_time =  ((uint16_t) (*(inbuffer + offset)));
      this->integration_time |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->integration_time);
     return offset;
    }

    virtual const char * getType() override { return "sensors_53l5a1_driver/control"; };
    virtual const char * getMD5() override { return "98b89d8dccdc25adb8f736b1958253c5"; };

  };

}
#endif
