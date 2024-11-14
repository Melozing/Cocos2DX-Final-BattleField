

#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include <mach/mach_time.h>

#import "platform/ios/CCDirectorCaller-ios.h"

#import <Foundation/Foundation.h>
#import <OpenGLES/EAGL.h>

#import "base/CCDirector.h"
#import "platform/ios/CCEAGLView-ios.h"

static id s_sharedDirectorCaller;

@interface NSObject(CADisplayLink)
+(id) displayLinkWithTarget: (id)arg1 selector:(SEL)arg2;
-(void) addToRunLoop: (id)arg1 forMode: (id)arg2;
-(void) setFrameInterval: (NSInteger)interval;
-(void) invalidate;
@end

@implementation CCDirectorCaller

@synthesize interval;

+(id) sharedDirectorCaller
{
    if (s_sharedDirectorCaller == nil)
    {
        s_sharedDirectorCaller = [[CCDirectorCaller alloc] init];
    }
    
    return s_sharedDirectorCaller;
}

+(void) destroy
{
    [s_sharedDirectorCaller stopMainLoop];
    [s_sharedDirectorCaller release];
    s_sharedDirectorCaller = nil;
}


- (instancetype)init
{
    if (self = [super init])
    {
        isAppActive = [UIApplication sharedApplication].applicationState == UIApplicationStateActive;
        NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
        [nc addObserver:self selector:@selector(appDidBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
        [nc addObserver:self selector:@selector(appDidBecomeInactive) name:UIApplicationWillResignActiveNotification object:nil];
        
        self.interval = 1;
    }
    return self;
}

-(void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [displayLink release];
    [super dealloc];
}

- (void)appDidBecomeActive
{
    // initialize initLastDisplayTime, or the dt will be invalid when
    // - the app is lauched
    // - the app resumes from background
    [self initLastDisplayTime];

    isAppActive = YES;
}

- (void)appDidBecomeInactive
{
    isAppActive = NO;
}

-(void) startMainLoop
{
    // Director::setAnimationInterval() is called, we should invalidate it first
    [self stopMainLoop];
    
    displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(doCaller:)];
    [displayLink setFrameInterval: self.interval];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void) stopMainLoop
{
    [displayLink invalidate];
    displayLink = nil;
}

-(void) setAnimationInterval:(double)intervalNew
{
    // Director::setAnimationInterval() is called, we should invalidate it first
    [self stopMainLoop];
        
    self.interval = 60.0 * intervalNew;
        
    displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(doCaller:)];
    [displayLink setFrameInterval: self.interval];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}
                      
-(void) doCaller: (id) sender
{
    if (isAppActive) {
        cocos2d::Director* director = cocos2d::Director::getInstance();
        EAGLContext* cocos2dxContext = [(CCEAGLView*)director->getOpenGLView()->getEAGLView() context];
        if (cocos2dxContext != [EAGLContext currentContext])
            glFlush();
        
        [EAGLContext setCurrentContext: cocos2dxContext];

        CFTimeInterval dt = ((CADisplayLink*)displayLink).timestamp - lastDisplayTime;
        lastDisplayTime = ((CADisplayLink*)displayLink).timestamp;
        director->mainLoop(dt);
    }
}

-(void)initLastDisplayTime
{
    struct mach_timebase_info timeBaseInfo;
    mach_timebase_info(&timeBaseInfo);
    CGFloat clockFrequency = (CGFloat)timeBaseInfo.denom / (CGFloat)timeBaseInfo.numer;
    clockFrequency *= 1000000000.0;
    // convert absolute time to seconds and should minus one frame time interval
    lastDisplayTime = (mach_absolute_time() / clockFrequency) - ((1.0 / 60) * self.interval);
}

@end

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS

