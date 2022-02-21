#ifndef _ROS_sensors_53l5a1_driver_nb_spads_h
#define _ROS_sensors_53l5a1_driver_nb_spads_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace sensors_53l5a1_driver
{

  class nb_spads : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      uint32_t number_spads_enabled[64];

    nb_spads():
      header(),
      number_spads_enabled()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      for( uint32_t i = 0; i < 64; i++){
      *(outbuffer + offset + 0) = (this->number_spads_enabled[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->number_spads_enabled[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->number_spads_enabled[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->number_spads_enabled[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->number_spads_enabled[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      for( uint32_t i = 0; i < 64; i++){
      this->number_spads_enabled[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->number_spads_enabled[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->number_spads_enabled[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->number_spads_enabled[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->number_spads_enabled[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "sensors_53l5a1_driver/nb_spads"; };
    virtual const char * getMD5() override { return "8dcd29376cfd243dd0979996d5c9b362"; };

  };

}
#endif
