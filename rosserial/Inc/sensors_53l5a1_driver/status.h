#ifndef _ROS_sensors_53l5a1_driver_status_h
#define _ROS_sensors_53l5a1_driver_status_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace sensors_53l5a1_driver
{

  class status : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _layer_type;
      _layer_type layer;
      uint8_t target_status[64];

    status():
      header(),
      layer(0),
      target_status()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->layer >> (8 * 0)) & 0xFF;
      offset += sizeof(this->layer);
      for( uint32_t i = 0; i < 64; i++){
      *(outbuffer + offset + 0) = (this->target_status[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->target_status[i]);
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
      this->target_status[i] =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->target_status[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "sensors_53l5a1_driver/status"; };
    virtual const char * getMD5() override { return "6bd059e4f62cb23f05c6abb7f626ac58"; };

  };

}
#endif
