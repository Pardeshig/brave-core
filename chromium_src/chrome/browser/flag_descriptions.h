/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_CHROME_BROWSER_FLAG_DESCRIPTIONS_H_
#define BRAVE_CHROMIUM_SRC_CHROME_BROWSER_FLAG_DESCRIPTIONS_H_

#include "brave/components/brave_rewards/common/buildflags/buildflags.h"

#include "../../../../chrome/browser/flag_descriptions.h"

namespace flag_descriptions {
extern const char kUseDevUpdaterUrlName[];
extern const char kUseDevUpdaterUrlDescription[];
extern const char kBraveNTPBrandedWallpaperName[];
extern const char kBraveNTPBrandedWallpaperDescription[];
extern const char kBraveNTPBrandedWallpaperDemoName[];
extern const char kBraveNTPBrandedWallpaperDemoDescription[];
extern const char kBraveAdblockCnameUncloakingName[];
extern const char kBraveAdblockCnameUncloakingDescription[];
extern const char kBraveAdblockCollapseBlockedElementsName[];
extern const char kBraveAdblockCollapseBlockedElementsDescription[];
extern const char kBraveAdblockCosmeticFilteringName[];
extern const char kBraveAdblockCosmeticFilteringNativeName[];
extern const char kBraveAdblockCosmeticFilteringDescription[];
extern const char kBraveAdblockCosmeticFilteringNativeDescription[];
extern const char kBraveAdblockCspRulesDescription[];
extern const char kBraveAdblockCspRulesName[];
extern const char kBraveDomainBlockName[];
extern const char kBraveDomainBlockDescription[];
extern const char kBraveExtensionNetworkBlockingName[];
extern const char kBraveExtensionNetworkBlockingDescription[];
extern const char kBraveDarkModeBlockName[];
extern const char kBraveDarkModeBlockDescription[];
extern const char kBraveSidebarName[];
extern const char kBraveSidebarDescription[];
extern const char kBraveVPNName[];
extern const char kBraveVPNDescription[];
extern const char kBraveSpeedreaderName[];
extern const char kBraveSpeedreaderDescription[];
extern const char kBraveSpeedreaderLegacyName[];
extern const char kBraveSpeedreaderLegacyDescription[];
extern const char kBraveSyncName[];
extern const char kBraveSyncDescription[];
extern const char kBraveIpfsName[];
extern const char kBraveIpfsDescription[];

#if BUILDFLAG(ENABLE_GEMINI_WALLET)
extern const char kBraveRewardsGeminiName[];
extern const char kBraveRewardsGeminiDescription[];
#endif

extern const char kBraveRewardsVerboseLoggingName[];
extern const char kBraveRewardsVerboseLoggingDescription[];
extern const char kBraveAdsCustomNotificationsName[];
extern const char kBraveAdsCustomNotificationsDescription[];
extern const char kNativeBraveWalletName[];
extern const char kNativeBraveWalletDescription[];
extern const char kBraveSearchDefaultAPIName[];
extern const char kBraveSearchDefaultAPIDescription[];
extern const char kBraveDecentralizedDnsName[];
extern const char kBraveDecentralizedDnsDescription[];
extern const char kBraveSuperReferralName[];
extern const char kBraveSuperReferralDescription[];
extern const char kBraveEphemeralStorageName[];
extern const char kBraveEphemeralStorageDescription[];
extern const char kBraveEphemeralStorageKeepAliveName[];
extern const char kBraveEphemeralStorageKeepAliveDescription[];

extern const char kBraveTalkName[];
extern const char kBraveTalkDescription[];
}  // namespace flag_descriptions

#endif  // BRAVE_CHROMIUM_SRC_CHROME_BROWSER_FLAG_DESCRIPTIONS_H_
