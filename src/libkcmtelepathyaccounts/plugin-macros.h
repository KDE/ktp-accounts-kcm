#ifndef LIB_KCM_TELEPATHY_ACCOUNTS_PLUGIN_MACROS_H
#define LIB_KCM_TELEPATHY_ACCOUNTS_PLUGIN_MACROS_H

#include <kdemacros.h>
#include <KGenericFactory>

#define KCMTELEPATHYACCOUNTS_PLUGIN_EXPORT(libname,classname) \
  K_PLUGIN_FACTORY(factory,registerPlugin<classname>();) \
  K_EXPORT_PLUGIN(factory("kcmtelepathyaccounts_plugin_"#libname))

#endif  // Header guard
