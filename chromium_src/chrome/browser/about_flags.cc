/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/about_flags.h"

#include "base/strings/string_util.h"
#include "brave/browser/brave_features_internal_names.h"
#include "brave/common/brave_features.h"
#include "brave/common/pref_names.h"
#include "brave/components/brave_ads/common/features.h"
#include "brave/components/brave_component_updater/browser/features.h"
#include "brave/components/brave_rewards/common/buildflags/buildflags.h"
#include "brave/components/brave_rewards/common/features.h"
#include "brave/components/brave_shields/common/features.h"
#include "brave/components/brave_sync/buildflags/buildflags.h"
#include "brave/components/brave_talk/features.h"
#include "brave/components/brave_vpn/buildflags/buildflags.h"
#include "brave/components/brave_wallet/common/buildflags/buildflags.h"
#include "brave/components/decentralized_dns/buildflags/buildflags.h"
#include "brave/components/ipfs/buildflags/buildflags.h"
#include "brave/components/ntp_background_images/browser/features.h"
#include "brave/components/sidebar/buildflags/buildflags.h"
#include "brave/components/speedreader/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "components/prefs/pref_service.h"
#include "net/base/features.h"

#if BUILDFLAG(ENABLE_BRAVE_VPN)
#include "brave/components/brave_vpn/features.h"
#endif

#if BUILDFLAG(ENABLE_SIDEBAR)
#include "brave/components/sidebar/features.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "brave/components/speedreader/features.h"
#endif

#if BUILDFLAG(ENABLE_BRAVE_SYNC)
#include "brave/components/brave_sync/features.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "brave/components/ipfs/features.h"
#endif

#if BUILDFLAG(BRAVE_WALLET_ENABLED)
#include "brave/components/brave_wallet/common/features.h"
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#include "brave/components/decentralized_dns/features.h"
#endif

using brave_shields::features::kBraveAdblockCnameUncloaking;
using brave_shields::features::kBraveAdblockCollapseBlockedElements;
using brave_shields::features::kBraveAdblockCosmeticFiltering;
using brave_shields::features::kBraveAdblockCosmeticFilteringNative;
using brave_shields::features::kBraveAdblockCspRules;
using brave_shields::features::kBraveDarkModeBlock;
using brave_shields::features::kBraveDomainBlock;
using brave_shields::features::kBraveExtensionNetworkBlocking;
using ntp_background_images::features::kBraveNTPBrandedWallpaper;
using ntp_background_images::features::kBraveNTPBrandedWallpaperDemo;
using ntp_background_images::features::kBraveNTPSuperReferralWallpaper;

// clang-format seems to have a lot of issues with the macros in this
// file so we turn it off for the macro sections.
// clang-format off

#if BUILDFLAG(ENABLE_BRAVE_VPN)
#define BRAVE_VPN_FEATURE_ENTRIES \
    {kBraveVPNFeatureInternalName,                                           \
     flag_descriptions::kBraveVPNName,                                       \
     flag_descriptions::kBraveVPNDescription,                                \
     kOsMac | kOsWin | kOsAndroid,                                           \
     FEATURE_VALUE_TYPE(brave_vpn::features::kBraveVPN)},
#else
#define BRAVE_VPN_FEATURE_ENTRIES
#endif

#if BUILDFLAG(ENABLE_SIDEBAR)
#define SIDEBAR_FEATURE_ENTRIES \
    {kBraveSidebarFeatureInternalName,                                     \
     flag_descriptions::kBraveSidebarName,                                 \
     flag_descriptions::kBraveSidebarDescription,                          \
     kOsMac | kOsWin | kOsLinux,                                           \
     FEATURE_VALUE_TYPE(sidebar::kSidebarFeature)},
#else
#define SIDEBAR_FEATURE_ENTRIES
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#define SPEEDREADER_FEATURE_ENTRIES \
    {"brave-speedreader",                                               \
     flag_descriptions::kBraveSpeedreaderName,                          \
     flag_descriptions::kBraveSpeedreaderDescription, kOsDesktop,       \
     FEATURE_VALUE_TYPE(speedreader::kSpeedreaderFeature)},             \
    {"speedreader-legacy-backend",                                      \
     flag_descriptions::kBraveSpeedreaderLegacyName,                    \
     flag_descriptions::kBraveSpeedreaderLegacyDescription, kOsDesktop, \
     FEATURE_VALUE_TYPE(speedreader::kSpeedreaderLegacyBackend)},
#else
#define SPEEDREADER_FEATURE_ENTRIES
#endif

#if BUILDFLAG(ENABLE_BRAVE_SYNC)
#define BRAVE_SYNC_FEATURE_ENTRIES                                         \
    {"brave-sync-v2",                                                      \
     flag_descriptions::kBraveSyncName,                                    \
     flag_descriptions::kBraveSyncDescription, kOsDesktop,                 \
     FEATURE_VALUE_TYPE(brave_sync::features::kBraveSync)},
#else
#define BRAVE_SYNC_FEATURE_ENTRIES
#endif

#if BUILDFLAG(ENABLE_GEMINI_WALLET)
#define BRAVE_REWARDS_GEMINI_FEATURE_ENTRIES                                \
    {"brave-rewards-gemini",                                                \
     flag_descriptions::kBraveRewardsGeminiName,                            \
     flag_descriptions::kBraveRewardsGeminiDescription, kOsDesktop,         \
     FEATURE_VALUE_TYPE(brave_rewards::features::kGeminiFeature)},
#else
#define BRAVE_REWARDS_GEMINI_FEATURE_ENTRIES
#endif

#if BUILDFLAG(ENABLE_IPFS)
#define BRAVE_IPFS_FEATURE_ENTRIES                                         \
    {"brave-ipfs",                                                         \
     flag_descriptions::kBraveIpfsName,                                    \
     flag_descriptions::kBraveIpfsDescription,                             \
     kOsDesktop | kOsAndroid,                                              \
     FEATURE_VALUE_TYPE(ipfs::features::kIpfsFeature)},
#else
#define BRAVE_IPFS_FEATURE_ENTRIES
#endif

#if BUILDFLAG(BRAVE_WALLET_ENABLED)
#define BRAVE_NATIVE_WALLET_FEATURE_ENTRIES                                  \
    {"native-brave-wallet",                                                  \
     flag_descriptions::kNativeBraveWalletName,                              \
     flag_descriptions::kNativeBraveWalletDescription,                       \
     kOsDesktop | flags_ui::kOsAndroid,                                      \
     FEATURE_VALUE_TYPE(brave_wallet::features::kNativeBraveWalletFeature)},
#else
#define BRAVE_NATIVE_WALLET_FEATURE_ENTRIES
#endif

#if BUILDFLAG(DECENTRALIZED_DNS_ENABLED)
#define BRAVE_DECENTRALIZED_DNS_FEATURE_ENTRIES                             \
    {"brave-decentralized-dns",                                             \
     flag_descriptions::kBraveDecentralizedDnsName,                         \
     flag_descriptions::kBraveDecentralizedDnsDescription,                  \
     kOsDesktop | kOsAndroid,                                               \
     FEATURE_VALUE_TYPE(decentralized_dns::features::kDecentralizedDns)},
#else
#define BRAVE_DECENTRALIZED_DNS_FEATURE_ENTRIES
#endif

#define BRAVE_FEATURE_ENTRIES                                               \
    {"use-dev-updater-url",                                                 \
     flag_descriptions::kUseDevUpdaterUrlName,                              \
     flag_descriptions::kUseDevUpdaterUrlDescription, kOsAll,               \
     FEATURE_VALUE_TYPE(brave_component_updater::kUseDevUpdaterUrl)},       \
    {"brave-ntp-branded-wallpaper",                                         \
     flag_descriptions::kBraveNTPBrandedWallpaperName,                      \
     flag_descriptions::kBraveNTPBrandedWallpaperDescription, kOsAll,       \
     FEATURE_VALUE_TYPE(kBraveNTPBrandedWallpaper)},                        \
    {"brave-ntp-branded-wallpaper-demo",                                    \
     flag_descriptions::kBraveNTPBrandedWallpaperDemoName,                  \
     flag_descriptions::kBraveNTPBrandedWallpaperDemoDescription, kOsAll,   \
     FEATURE_VALUE_TYPE(kBraveNTPBrandedWallpaperDemo)},                    \
    {"brave-adblock-cname-uncloaking",                                      \
     flag_descriptions::kBraveAdblockCnameUncloakingName,                   \
     flag_descriptions::kBraveAdblockCnameUncloakingDescription, kOsAll,    \
     FEATURE_VALUE_TYPE(kBraveAdblockCnameUncloaking)},                     \
    {"brave-adblock-collapse-blocked-elements",                             \
     flag_descriptions::kBraveAdblockCollapseBlockedElementsName,           \
     flag_descriptions::kBraveAdblockCollapseBlockedElementsDescription,    \
     kOsAll, FEATURE_VALUE_TYPE(kBraveAdblockCollapseBlockedElements)},     \
    {"brave-adblock-cosmetic-filtering",                                    \
     flag_descriptions::kBraveAdblockCosmeticFilteringName,                 \
     flag_descriptions::kBraveAdblockCosmeticFilteringDescription, kOsAll,  \
     FEATURE_VALUE_TYPE(kBraveAdblockCosmeticFiltering)},                   \
    {"brave-adblock-cosmetic-filtering-native",                             \
     flag_descriptions::kBraveAdblockCosmeticFilteringNativeName,           \
     flag_descriptions::kBraveAdblockCosmeticFilteringNativeDescription,    \
     kOsMac | kOsWin | kOsLinux,                                            \
     FEATURE_VALUE_TYPE(kBraveAdblockCosmeticFilteringNative)},             \
    {"brave-adblock-csp-rules",                                             \
     flag_descriptions::kBraveAdblockCspRulesName,                          \
     flag_descriptions::kBraveAdblockCspRulesDescription, kOsAll,           \
     FEATURE_VALUE_TYPE(kBraveAdblockCspRules)},                            \
    {"brave-domain-block",                                                  \
     flag_descriptions::kBraveDomainBlockName,                              \
     flag_descriptions::kBraveDomainBlockDescription, kOsAll,               \
     FEATURE_VALUE_TYPE(kBraveDomainBlock)},                                \
    {"brave-extension-network-blocking",                                    \
     flag_descriptions::kBraveExtensionNetworkBlockingName,                 \
     flag_descriptions::kBraveExtensionNetworkBlockingDescription, kOsAll,  \
     FEATURE_VALUE_TYPE(kBraveExtensionNetworkBlocking)},                   \
    {"brave-dark-mode-block",                                               \
     flag_descriptions::kBraveDarkModeBlockName,                            \
     flag_descriptions::kBraveDarkModeBlockDescription, kOsAll,             \
     FEATURE_VALUE_TYPE(kBraveDarkModeBlock)},                              \
    SPEEDREADER_FEATURE_ENTRIES                                             \
    BRAVE_SYNC_FEATURE_ENTRIES                                              \
    BRAVE_IPFS_FEATURE_ENTRIES                                              \
    BRAVE_NATIVE_WALLET_FEATURE_ENTRIES                                     \
    SIDEBAR_FEATURE_ENTRIES                                                 \
    BRAVE_VPN_FEATURE_ENTRIES                                               \
    BRAVE_DECENTRALIZED_DNS_FEATURE_ENTRIES                                 \
    {"brave-super-referral",                                                \
     flag_descriptions::kBraveSuperReferralName,                            \
     flag_descriptions::kBraveSuperReferralDescription,                     \
     flags_ui::kOsMac | flags_ui::kOsWin | flags_ui::kOsAndroid,            \
     FEATURE_VALUE_TYPE(kBraveNTPSuperReferralWallpaper)},                  \
    {"brave-ephemeral-storage",                                             \
     flag_descriptions::kBraveEphemeralStorageName,                         \
     flag_descriptions::kBraveEphemeralStorageDescription, kOsAll,          \
     FEATURE_VALUE_TYPE(net::features::kBraveEphemeralStorage)},            \
    {"brave-ephemeral-storage-keep-alive",                                  \
     flag_descriptions::kBraveEphemeralStorageKeepAliveName,                \
     flag_descriptions::kBraveEphemeralStorageKeepAliveDescription, kOsAll, \
     FEATURE_VALUE_TYPE(net::features::kBraveEphemeralStorageKeepAlive)},   \
    {"brave-rewards-verbose-logging",                                       \
     flag_descriptions::kBraveRewardsVerboseLoggingName,                    \
     flag_descriptions::kBraveRewardsVerboseLoggingDescription,             \
     kOsDesktop | kOsAndroid,                                               \
     FEATURE_VALUE_TYPE(brave_rewards::features::kVerboseLoggingFeature)},  \
    {"brave-ads-custom-notifications",                                      \
     flag_descriptions::kBraveAdsCustomNotificationsName,                   \
     flag_descriptions::kBraveAdsCustomNotificationsDescription,            \
     kOsDesktop | kOsAndroid,                                               \
     FEATURE_VALUE_TYPE(brave_ads::features::kCustomAdNotifications)},      \
    {"brave-talk",                                                          \
     flag_descriptions::kBraveTalkName,                                     \
     flag_descriptions::kBraveTalkDescription,                              \
     kOsDesktop,                                                            \
     FEATURE_VALUE_TYPE(brave_talk::features::kBraveTalk)},                 \
     BRAVE_REWARDS_GEMINI_FEATURE_ENTRIES

#define SetFeatureEntryEnabled SetFeatureEntryEnabled_ChromiumImpl
#include "../../../../chrome/browser/about_flags.cc"  // NOLINT
#undef SetFeatureEntryEnabled
#undef BRAVE_FEATURE_ENTRIES

// clang-format on

namespace about_flags {

void UpdateBraveMediaRouterPref(const std::string& internal_name,
                                Profile* profile) {
  bool enable = false;
  if (base::EndsWith(internal_name, "@1", base::CompareCase::SENSITIVE)) {
    enable = true;
  }
  if (base::StartsWith(internal_name, "load-media-router-component-extension",
                       base::CompareCase::SENSITIVE)) {
    profile->GetPrefs()->SetBoolean(kBraveEnabledMediaRouter, enable);
  }
}

void SetFeatureEntryEnabled(flags_ui::FlagsStorage* flags_storage,
                            const std::string& internal_name,
                            bool enable) {
  UpdateBraveMediaRouterPref(internal_name,
                             ProfileManager::GetActiveUserProfile());
  SetFeatureEntryEnabled_ChromiumImpl(flags_storage, internal_name, enable);
}

}  // namespace about_flags
