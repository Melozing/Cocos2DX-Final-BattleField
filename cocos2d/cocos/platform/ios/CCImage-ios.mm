
#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import "platform/CCImage.h"
#import "platform/CCCommon.h"
#import <string>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include <math.h>

NS_CC_BEGIN

bool cocos2d::Image::saveToFile(const std::string& filename, bool isToRGB)
{
    //only support for Texture2D::PixelFormat::RGB888 or Texture2D::PixelFormat::RGBA8888 uncompressed data
    if (isCompressed() || (_renderFormat != Texture2D::PixelFormat::RGB888 && _renderFormat != Texture2D::PixelFormat::RGBA8888))
    {
        CCLOG("cocos2d: Image: saveToFile is only support for Texture2D::PixelFormat::RGB888 or Texture2D::PixelFormat::RGBA8888 uncompressed data for now");
        return false;
    }
    bool saveToPNG = false;
    bool needToCopyPixels = false;

    std::string basename(filename);
    std::transform(basename.begin(), basename.end(), basename.begin(), ::tolower);
    if (std::string::npos != basename.find(".png"))
    {
        saveToPNG = true;
    }
        
    int bitsPerComponent = 8;            
    int bitsPerPixel = hasAlpha() ? 32 : 24;
    if ((! saveToPNG) || isToRGB)
    {
        bitsPerPixel = 24;
    }            
    
    int bytesPerRow    = (bitsPerPixel/8) * _width;
    int myDataLength = bytesPerRow * _height;
    
    unsigned char *pixels    = _data;
    
    // The data has alpha channel, and want to save it with an RGB png file,
    // or want to save as jpg,  remove the alpha channel.
    if (hasAlpha() && bitsPerPixel == 24)
    {
        pixels = new (std::nothrow) unsigned char[myDataLength];
        
        for (int i = 0; i < _height; ++i)
        {
            for (int j = 0; j < _width; ++j)
            {
                pixels[(i * _width + j) * 3] = _data[(i * _width + j) * 4];
                pixels[(i * _width + j) * 3 + 1] = _data[(i * _width + j) * 4 + 1];
                pixels[(i * _width + j) * 3 + 2] = _data[(i * _width + j) * 4 + 2];
            }
        }
        
        needToCopyPixels = true;
    }
        
    // make data provider with data.
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    if (saveToPNG && hasAlpha() && (! isToRGB))
    {
        bitmapInfo |= kCGImageAlphaPremultipliedLast;
    }
    CGDataProviderRef provider        = CGDataProviderCreateWithData(nullptr, pixels, myDataLength, nullptr);
    CGColorSpaceRef colorSpaceRef    = CGColorSpaceCreateDeviceRGB();
    CGImageRef iref                    = CGImageCreate(_width, _height,
                                                        bitsPerComponent, bitsPerPixel, bytesPerRow,
                                                        colorSpaceRef, bitmapInfo, provider,
                                                        nullptr, false,
                                                        kCGRenderingIntentDefault);
        
    UIImage* image                    = [[UIImage alloc] initWithCGImage:iref];
        
    CGImageRelease(iref);    
    CGColorSpaceRelease(colorSpaceRef);
    CGDataProviderRelease(provider);

    // NOTE: Prevent memory leak. Requires ARC enabled.
    @autoreleasepool {
        NSData *data;
        if (saveToPNG) {
            data = UIImagePNGRepresentation(image);
        } else {
            data = UIImageJPEGRepresentation(image, 1.0f);
        }
        [data writeToFile:[NSString stringWithUTF8String:filename.c_str()] atomically:YES];
    }

    [image release];

    if (needToCopyPixels)
    {
        delete [] pixels;
    }
    
    return true;
}

NS_CC_END

#endif // CC_PLATFORM_IOS

