#ifndef PKT_CONFIG_HPP
#define PKT_CONFIG_HPP

#include <cstdint>

namespace multi2d {

  enum class pkt_t
  {
    PLAYER_POSITON_DATA
  };

  struct pkt_header_t
  {
    pkt_t  op_code;
    uint16_t length;
  } __attribute__((packed));

  class pkt_ref_t
  {
  public:
    template<typename arr_t>
    pkt_ref_t(arr_t arr)
      : data_(arr.data())
    {}

    const uint8_t const* data() const
    {
      return data_;
    }
    
  private:
    const uint8_t const* data_;
  };

}