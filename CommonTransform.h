#ifndef COMMON_TRANSFORM_H
#define COMMON_TRANSFORM_H

/// for net
unsigned inSize(unsigned k, unsigned s, unsigned pad, unsigned outSize);
unsigned outSize(unsigned k, unsigned s, unsigned pad, unsigned inSize);
unsigned poolOutSize(unsigned k, unsigned s, unsigned pad, unsigned inSize);

#endif
