#import <Foundation/Foundation.h>

#include "platform/CCThread.h"

NS_CC_BEGIN

void* ThreadHelper::createAutoreleasePool()
{
    id pool = [[NSAutoreleasePool alloc] init];
    return pool;
}

void ThreadHelper::releaseAutoreleasePool(void *autoreleasePool)
{
    [(NSAutoreleasePool*)autoreleasePool release];
}

NS_CC_END
