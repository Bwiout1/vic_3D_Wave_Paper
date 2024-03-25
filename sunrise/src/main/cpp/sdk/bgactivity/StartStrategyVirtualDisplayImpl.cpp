#include "StartStrategyVirtualDisplayImpl.h"
#include "../../android/AndroidJni.h"
#include "../../util/LocalFrame.h"
#include "../../android/Context.h"
#include "../../android/Resources.h"
#include "../../android/Configuration.h"
#include "../../android/Application.h"
#include "../../android/DisplayManager.h"
#include "../../android/VirtualDisplay.h"
#include "../../android/Presentation.h"
#include "../../android/PendingIntent.h"
#include "../../util//Runnable.h"

long StartStrategyVirtualDisplayImpl::delayTimeMills(){
    return 4000;
}

const char* StartStrategyVirtualDisplayImpl::getName(){
    return "virtualDisplay";
}

bool StartStrategyVirtualDisplayImpl::start(jobject context, Intent& intent){

    if(AndroidJni::get_dev_api_level() >= 19){
        LOGV("StartStrategyVirtualDisplayImpl::start");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv != nullptr && context != nullptr && intent.getCore() != nullptr){
            LocalFrame frame(jenv);

            Context ctxWrapper(context);
            Resources resWrapper(ctxWrapper.getResources());

////////////////////////////////////////////////////////////////////////////
            Configuration configWrapper(resWrapper.getConfiguration());

            Application& app = Application::getInstance() ;
            DisplayManager managerWrapper(app.getSystemService("display"));
            jobject virtualDisplay = managerWrapper.createVirtualDisplay("virtual_display_other", 500, 500, configWrapper.densityDpi,
                                                                         nullptr, DisplayManager::MATCH_CONTENT_FRAMERATE_NEVER);

            VirtualDisplay virtualDisplayWrapper(virtualDisplay);
            jobject display = virtualDisplayWrapper.getDisplay();

            Presentation presentationWrapper(context, display);
            presentationWrapper.show();
////////////////////////////////////////////////////////////////////

            if(pendingIntent!= nullptr){
                delete pendingIntent;
                pendingIntent = nullptr;
            }
            pendingIntent = new PendingIntent(
                    PendingIntent::getActivity(context, 0, intent.getCore(), PendingIntent::FLAG_UPDATE_CURRENT)
            );

            if(run!= nullptr) {
                delete run;
                run = nullptr;
            }
            run = new Runnable((RunnableCallback)StartStrategyVirtualDisplayImpl::send, this);
            AndroidJni::getUIHandler().postDelayed(*run, 1000L);
            return true;
        }
    }

    return false;
}

StartStrategyVirtualDisplayImpl::~StartStrategyVirtualDisplayImpl() {
    LOGV("StartStrategyVirtualDisplayImpl::~StartStrategyVirtualDisplayImpl");

    if(pendingIntent != nullptr){
        delete pendingIntent;
        pendingIntent = nullptr;
    }

    if(run != nullptr){
        delete run;
        run = nullptr;
    }
}

void StartStrategyVirtualDisplayImpl::send(StartStrategyVirtualDisplayImpl *virtualDisplayImpl) {
    if(virtualDisplayImpl == nullptr)
        return;

    LOGV("StartStrategyVirtualDisplayImpl::send");

    if(virtualDisplayImpl->pendingIntent != nullptr){
        virtualDisplayImpl->pendingIntent ->send();
        delete virtualDisplayImpl->pendingIntent;
        virtualDisplayImpl->pendingIntent = nullptr;
    }

    if(virtualDisplayImpl->run != nullptr){
        delete virtualDisplayImpl ->run;
        virtualDisplayImpl ->run = nullptr;
    }
}