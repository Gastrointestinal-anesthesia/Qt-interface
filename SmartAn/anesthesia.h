// Generated by gencpp from file smart_topic/Anesthesia.msg
// DO NOT EDIT!


#ifndef SMART_TOPIC_MESSAGE_ANESTHESIA_H
#define SMART_TOPIC_MESSAGE_ANESTHESIA_H


#include <string>
#include <vector>
#include <map>

namespace smart_topic{
template <class ContainerAllocator>
struct Anesthesia_
{
    typedef Anesthesia_<ContainerAllocator> Type;

    Anesthesia_()
        : RATE(0) // 心率
        , DIAP(0) // 舒张压
        , SYSP(0) // 收缩压
        , SpO2(0) // 血氧
        , BISr(0)  { // 脑电双频指数
    }
    Anesthesia_(const ContainerAllocator& _alloc)
        : RATE(0)
        , DIAP(0)
        , SYSP(0)
        , SpO2(0)
        , BISr(0)  {
        (void)_alloc;
    }

    typedef uint16_t _RATE_type;
    _RATE_type RATE;

    typedef uint16_t _DIAP_type;
    _DIAP_type DIAP;

    typedef uint16_t _SYSP_type;
    _SYSP_type SYSP;

    typedef uint8_t _SpO2_type;
    _SpO2_type SpO2;

    typedef uint8_t _BIS_type;
    _BIS_type BISr;

}; // struct Anesthesia_

typedef ::smart_topic::Anesthesia_<std::allocator<void> > Anesthesia;

// constants requiring out of line definition

} // namespace smart_topic



#endif // SMART_TOPIC_MESSAGE_ANESTHESIA_H
