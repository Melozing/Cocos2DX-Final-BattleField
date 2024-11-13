

#include "platform/CCPlatformConfig.h"
#include "platform/CCDevice.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#include <Foundation/Foundation.h>
#include <Cocoa/Cocoa.h>
#include <string>

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import <UIKit/UIKit.h>

#endif

#include "base/ccTypes.h"
#include "platform/apple/CCDevice-apple.h"

namespace FontUtils {
    NSMutableParagraphStyle* _calculateParagraphStyle(bool enableWrap, int overflow)
    {
        NSMutableParagraphStyle* paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
        paragraphStyle.lineBreakMode = NSLineBreakByWordWrapping;
        return paragraphStyle;
    }
    
    NSTextAlignment _calculateTextAlignment(cocos2d::Device::TextAlign align)
    {
        unsigned uHoriFlag = (int)align & 0x0f;
        NSTextAlignment nsAlign = (2 == uHoriFlag) ? NSTextAlignmentRight
        : (3 == uHoriFlag) ? NSTextAlignmentCenter
        : NSTextAlignmentLeft;
        
        return nsAlign;
    }
    

    
    CGFloat _calculateTextDrawStartWidth(cocos2d::Device::TextAlign align, CGSize realDimensions, CGSize dimensions)
    {
        CGFloat xPadding = 0;
        unsigned uHoriFlag = (int)align & 0x0f;
        switch (uHoriFlag) {
                //center
            case 3: xPadding = (dimensions.width - realDimensions.width) / 2.0f; break;
                //right
            case 2: xPadding = dimensions.width - realDimensions.width; break;
            default: break;
        }
        return xPadding;
    }

}
