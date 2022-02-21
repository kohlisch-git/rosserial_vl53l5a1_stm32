#ifndef _ROS_sensors_53l5a1_driver_am_per_spad_h
#define _ROS_sensors_53l5a1_driver_am_per_spad_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace sensors_53l5a1_driver
{

  class am_per_spad : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      uint32_t ambient_per_spad[64];

    am_per_spad():
      header(),
      ambient_per_spad()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      for( uint32_t i = 0; i < 64; i++){
      *(outbuffer + offset + 0) = (this->ambient_per_spad[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->ambient_per_spad[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->ambient_per_spad[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->ambient_per_spad[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ambient_per_spad[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      for( uint32_t i = 0; i < 64; i++){
      this->ambient_per_spad[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->ambient_per_spad[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->ambient_per_spad[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->ambient_per_spad[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->ambient_per_spad[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "sensors_53l5a1_driver/am_per_spad"; };
    virtual const char * getMD5() override { return "d12642d837e53a27605a4e5b7d88bf8d"; };

  };

}
#endif
