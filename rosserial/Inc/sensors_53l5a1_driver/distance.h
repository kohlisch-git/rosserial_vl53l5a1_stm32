#ifndef _ROS_sensors_53l5a1_driver_distance_h
#define _ROS_sensors_53l5a1_driver_distance_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace sensors_53l5a1_driver
{

  class distance : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _layer_type;
      _layer_type layer;
      typedef uint8_t _resolution_type;
      _resolution_type resolution;
      uint16_t distance_mm[64];

    distance():
      header(),
      layer(0),
      resolution(0),
      distance_mm()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->layer >> (8 * 0)) & 0xFF;
      offset += sizeof(this->layer);
      *(outbuffer + offset + 0) = (this->resolution >> (8 * 0)) & 0xFF;
      offset += sizeof(this->resolution);
      for( uint32_t i = 0; i < 64; i++){
      *(outbuffer + offset + 0) = (this->distance_mm[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->distance_mm[i] >> (8 * 1)) & 0xFF;
      offset += sizeof(this->distance_mm[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->layer =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->layer);
      this->resolution =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->resolution);
      for( uint32_t i = 0; i < 64; i++){
      this->distance_mm[i] =  ((uint16_t) (*(inbuffer + offset)));
      this->distance_mm[i] |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->distance_mm[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "sensors_53l5a1_driver/distance"; };
    virtual const char * getMD5() override { return "ea031c368ea421bba7084554872dc176"; };

  };

}
#endif
