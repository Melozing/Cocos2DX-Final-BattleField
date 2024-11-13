

#import "ui/UIEditBox/iOS/CCUISingleLineTextField.h"
#import "ui/UIEditBox/iOS/CCUITextInput.h"

#include "base/CCDirector.h"

/**
 * http://stackoverflow.com/questions/18244790/changing-uitextfield-placeholder-font
 */


@implementation CCUISingleLineTextField

#pragma mark - Init & Dealloc

- (void)dealloc
{
    [_placeholderFont release];
    [_placeholderTextColor release];

    [super dealloc];
}

#pragma mark - Properties

- (UIColor *)placeholderTextColor
{
    return _placeholderTextColor;
}

- (UIFont *)placeholderFont
{
    return _placeholderFont;
}

#pragma mark - Public methods

- (void)drawPlaceholderInRect:(CGRect)rect {
	NSDictionary *attributes = @{
		NSForegroundColorAttributeName:_placeholderTextColor,
		NSFontAttributeName:_placeholderFont
	};
    
    // center vertically
    CGSize textSize = [self.placeholder sizeWithAttributes:attributes];
    CGFloat hdif = rect.size.height - textSize.height;
    hdif = MAX(0, hdif);
    rect.origin.y += ceil(hdif/2.0);

    [[self placeholder] drawInRect:rect withAttributes:attributes];
}

- (CGRect)textRectForBounds:(CGRect)bounds
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    
    float padding = CC_EDIT_BOX_PADDING * glview->getScaleX() / glview->getContentScaleFactor();
    return CGRectInset(bounds, padding, padding);
}

- (CGRect)editingRectForBounds:(CGRect)bounds
{
    return [self textRectForBounds:bounds];
}

@end
