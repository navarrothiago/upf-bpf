#ifndef BPF_UTILS_H
#define BPF_UTILS_H 

#ifndef htons
#define htons(x) __constant_htons((x))
#endif

#ifndef htonl
#define htonl(x) __constant_htonl((x))
#endif

#endif