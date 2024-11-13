

#import "ui/UIEditBox/iOS/UITextView+CCUITextInput.h"


@implementation UITextView (CCUITextInput)

- (NSString *)ccui_text
{
    return self.text;
}

- (void)ccui_setText:(NSString *)ccui_text
{
    self.text = ccui_text;
}

- (NSString *)ccui_placeholder
{
    SEL selector = @selector(placeholder);
    if ([self respondsToSelector:selector]) {
        return [self performSelector:selector];
    }
    return nil;
}

- (void)ccui_setPlaceholder:(NSString *)ccui_placeholder
{
    SEL selector = @selector(setPlaceholder:);
    if ([self respondsToSelector:selector]) {
        [self performSelector:selector withObject:ccui_placeholder];
    }
}

- (UIColor *)ccui_textColor
{
    return self.textColor;
}

- (void)ccui_setTextColor:(UIColor *)ccui_textColor
{
    self.textColor = ccui_textColor;
}

- (UIFont *)ccui_font
{
    return self.font;
}

- (void)ccui_setFont:(UIFont *)ccui_font
{
    self.font = ccui_font;
}

- (NSTextAlignment)ccui_alignment
{
    return self.textAlignment;
}

- (void)ccui_setTextHorizontalAlignment:(NSTextAlignment)ccui_alignment
{
    self.textAlignment = ccui_alignment;
}

- (UIColor *)ccui_placeholderTextColor
{
    SEL selector = NSSelectorFromString(@"placeHolderLabel");
    if ([self respondsToSelector:selector]) {
        return ((UILabel *)[self performSelector:selector]).textColor;
    }
    return nil;
}

- (void)ccui_setPlaceholderTextColor:(UIColor *)ccui_placeholderTextColor
{
    SEL selector = NSSelectorFromString(@"placeHolderLabel");
    if ([self respondsToSelector:selector]) {
        ((UILabel *)[self performSelector:selector]).textColor = ccui_placeholderTextColor;
    }
}

- (UIFont *)ccui_placeholderFont
{
    SEL selector = NSSelectorFromString(@"placeHolderLabel");
    if ([self respondsToSelector:selector]) {
        return ((UILabel *)[self performSelector:selector]).font;
    }
    return nil;
}

- (void)ccui_setPlaceholderFont:(UIFont *)ccui_placeholderFont
{
    SEL selector = NSSelectorFromString(@"placeHolderLabel");
    if ([self respondsToSelector:selector]) {
        ((UILabel *)[self performSelector:selector]).font = ccui_placeholderFont;
    }
}

- (BOOL)ccui_secureTextEntry
{
    return self.secureTextEntry;
}

- (void)ccui_setSecureTextEntry:(BOOL)ccui_secureTextEntry
{
    self.secureTextEntry = ccui_secureTextEntry;
}

- (void)ccui_setDelegate:(id<UITextFieldDelegate,UITextViewDelegate>)delegate
{
    self.delegate = delegate;
}

@end


void LoadUITextViewCCUITextInputCategory() {
    
}
