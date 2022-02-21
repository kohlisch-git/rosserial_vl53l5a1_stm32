#ifndef _ROS_sensors_53l5a1_driver_sig_per_spad_h
#define _ROS_sensors_53l5a1_driver_sig_per_spad_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace sensors_53l5a1_driver
{

  class sig_per_spad : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _layer_type;
      _layer_type layer;
      uint32_t signal_per_spad[64];

    sig_per_spad():
      header(),
      layer(0),
      signal_per_spad()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->layer >> (8 * 0)) & 0xFF;
      offset += sizeof(this->layer);
      for( uint32_t i = 0; i < 64; i++){
      *(outbuffer + offset + 0) = (this->signal_per_spad[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->signal_per_spad[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->signal_per_spad[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->signal_per_spad[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->signal_per_spad[i]);
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
      this->signal_per_spad[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->signal_per_spad[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->signal_per_spad[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->signal_per_spad[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->signal_per_spad[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "sensors_53l5a1_driver/sig_per_spad"; };
    virtual const char * getMD5() override { return "5af842b4d93978fd421a6e5e9202a012"; };

  };

}
#endif
