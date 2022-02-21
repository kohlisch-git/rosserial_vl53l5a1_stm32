#ifndef _ROS_sensors_53l5a1_driver_reflectance_h
#define _ROS_sensors_53l5a1_driver_reflectance_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace sensors_53l5a1_driver
{

  class reflectance : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _layer_type;
      _layer_type layer;
      uint8_t reflectance_in_percent[64];

    reflectance():
      header(),
      layer(0),
      reflectance_in_percent()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->layer >> (8 * 0)) & 0xFF;
      offset += sizeof(this->layer);
      for( uint32_t i = 0; i < 64; i++){
      *(outbuffer + offset + 0) = (this->reflectance_in_percent[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->reflectance_in_percent[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->layer =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->layer);
      for( uint32_t i = 0; i < 64; i++){
      this->reflectance_in_percent[i] =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->reflectance_in_percent[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "sensors_53l5a1_driver/reflectance"; };
    virtual const char * getMD5() override { return "8f3eba0570aa10ad6cf86060e770041e"; };

  };

}
#endif
