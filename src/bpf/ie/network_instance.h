#if !defined(IE_NETWORK_INTANCE_H)
#define IE_NETWORK_INTANCE_H

#include <ie/ie_base.h>

#define NETWORK_INSTANCE_MAX_SIZE 100
typedef struct network_instance
{
  ie_base_t_ base;
  char network_instance[NETWORK_INSTANCE_MAX_SIZE];

}network_instance_t_;


#endif // IE_NETWORK_INTANCE_H
