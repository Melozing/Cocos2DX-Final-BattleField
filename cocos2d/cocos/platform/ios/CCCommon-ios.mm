

#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "platform/CCCommon.h"

#include <stdarg.h>
#include <stdio.h>

#import <UIKit/UIAlert.h>
#include "base/CCDirector.h"
#include "base/CCConsole.h"

NS_CC_BEGIN

// ios no MessageBox, use log instead
void MessageBox(const char * msg, const char * title)
{
    // only enable it on iOS.
    // FIXME: Implement it for tvOS
#if !defined(CC_TARGET_OS_TVOS)
    NSString * tmpTitle = (title) ? [NSString stringWithUTF8String : title] : nil;
    NSString * tmpMsg = (msg) ? [NSString stringWithUTF8String : msg] : nil;
    UIAlertView * messageBox = [[UIAlertView alloc] initWithTitle: tmpTitle
                                                          message: tmpMsg
                                                         delegate: nil
                                                cancelButtonTitle: @"OK"
                                                otherButtonTitles: nil];
    [messageBox autorelease];
    [messageBox show];
#endif

}

void LuaLog(const char * format)
{
    puts(format);
}

NS_CC_END

#endif // CC_PLATFORM_IOS
