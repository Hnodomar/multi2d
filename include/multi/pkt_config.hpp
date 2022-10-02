#ifndef PKT_CONFIG_HPP
#define PKT_CONFIG_HPP

#include <cstdint>
#include <array>
#include <cstring>

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

    template<typename payload_t>
    static auto make_pkt(pkt_t type, payload_t data)
      -> std::array<uint8_t, sizeof(pkt_header_t) + sizeof(payload_t)>
    {
      constexpr auto pkt_size = sizeof(pkt_header_t) + sizeof(payload_t);
      std::array<uint8_t, pkt_size> buff;
    
      buff[0] = static_cast<uint8_t>(type);

      const uint16_t len = sizeof(payload_t);
      std::copy(std::make_move_iterator(buff.begin()),
                std::make_move_iterator(buff.end()),
                buff.data());
      std::memcpy(buff.data() + 1, &len, sizeof(len));

      uint8_t* pld_ptr = buff.data() + sizeof(pkt_header_t);
      std::memcpy(pld_ptr, &data, len);

      return buff;
    }

  private:

    int32_t client_id_;
    
    uint8_t* data_;
  };

}

#endif
