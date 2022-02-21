#ifndef _ROS_sensors_53l5a1_driver_sigma_h
#define _ROS_sensors_53l5a1_driver_sigma_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace sensors_53l5a1_driver
{

  class sigma : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _layer_type;
      _layer_type layer;
      uint16_t sigma_mm[64];

    sigma():
      header(),
      layer(0),
      sigma_mm()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->layer >> (8 * 0)) & 0xFF;
      offset += sizeof(this->layer);
      for( uint32_t i = 0; i < 64; i++){
      *(outbuffer + offset + 0) = (this->sigma_mm[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->sigma_mm[i] >> (8 * 1)) & 0xFF;
      offset += sizeof(this->sigma_mm[i]);
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
      this->sigma_mm[i] =  ((uint16_t) (*(inbuffer + offset)));
      this->sigma_mm[i] |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->sigma_mm[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "sensors_53l5a1_driver/sigma"; };
    virtual const char * getMD5() override { return "35fe20af179b747bc4af98c17f84d798"; };

  };

}
#endif
