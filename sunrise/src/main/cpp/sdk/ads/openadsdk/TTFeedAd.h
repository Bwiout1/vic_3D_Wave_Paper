#ifndef SUNRISE_TTFEEDAD_H
#define SUNRISE_TTFEEDAD_H

#include "../../../android/AndroidClassWrapperBase.h"
class List;


class TTFeedAd : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsFeedAd;
    static jclass clsImage;

    static void init();

public:
    TTFeedAd(const jobject& ttAd);

    jobject getIcon();
    jobject getAdLogo();
    bool isValid(const jobject& icon);
    int getImageMode();
    jobject getAdView();
    jobject getImageList();
    void registerViewForInteraction(const jobject& viewGroup, List& list1, List& list2, const jobject& listener);
    jstring getImageUrl(const jobject& obj);
    jstring getTitle();
    jstring getDescription();
    jstring getButtonText();
    void showPrivacyActivity();
};


#endif //SUNRISE_TTFEEDAD_H
