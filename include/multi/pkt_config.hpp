#ifndef PKT_CONFIG_HPP
#define PKT_CONFIG_HPP

#include <cstdint>

namespace multi2d {

  enum class pkt_t : uint8_t
  {
    PLAYER_POSITON_DATA,
    LAST_PKT_TYPE
  };

  struct pkt_header_t
  {
    pkt_t  op_code;
    uint16_t length;
  } __attribute__((packed));

  class pkt_ref_t
  {
  public:
  
    pkt_ref_t(uint8_t* data, uint32_t id)
      : client_id_(id)
      , data_(data)
    {}

    uint8_t* payload() const
    {
      return data_ + sizeof(pkt_header_t);
    }
    
    pkt_t type() const
    {
      return static_cast<pkt_t>(data_[0]);
    }

    int32_t id() const
    {
      return client_id_;
    }

  private:

    int32_t client_id_;
    
    uint8_t* data_;
  };

}

#endif
