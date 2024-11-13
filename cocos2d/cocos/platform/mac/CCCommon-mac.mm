
#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#include "platform/CCCommon.h"

#include "base/CCDirector.h"

#include <stdarg.h>
#include <stdio.h>
#include <Cocoa/Cocoa.h>

NS_CC_BEGIN

void LuaLog(const char * format)
{
    puts(format);
}

// ios no MessageBox, use log instead
void MessageBox(const char * msg, const char * title)
{
    NSString * tmpTitle = (title) ? [NSString stringWithUTF8String : title] : nil;
    NSString * tmpMsg = (msg) ? [NSString stringWithUTF8String : msg] : nil;

    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText:tmpMsg];
    [alert setInformativeText:tmpTitle];
    [alert setAlertStyle:NSWarningAlertStyle];

    auto glview = Director::getInstance()->getOpenGLView();
    id window = glview->getCocoaWindow();
    [alert beginSheetModalForWindow:window
                      modalDelegate:[window delegate]
                     didEndSelector:nil
                        contextInfo:nil];
}

NS_CC_END

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_MAC
