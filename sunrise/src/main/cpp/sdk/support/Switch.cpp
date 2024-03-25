#include "Switch.h"


#ifdef DEBUG
bool debug = true;
#else
bool debug = false;
#endif

bool Switch::isDebug = debug;
//bool Switch::isDebug = true;

//开关mopub test使用
bool Switch::open_mopub = false;
bool Switch::mopub_enable = !debug || open_mopub;

//开关pangle test使用
bool Switch::open_pangle = true;
bool Switch::pangle_enagle = !debug || open_pangle;

//开关ironSource test使用
bool Switch::open_ironsource = true;
bool Switch::ironsource_enable = !debug || open_ironsource;

//开关offer test使用
bool Switch::open_offer = true;
bool Switch::offer_enable = !debug || open_offer;