/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ELIGIBLE_ADS_ELIGIBLE_ADS_UTIL_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ELIGIBLE_ADS_ELIGIBLE_ADS_UTIL_H_

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "bat/ads/internal/eligible_ads/ad_features_info.h"
#include "bat/ads/internal/ad_targeting/ad_targeting_segment_util.h"
#include "bat/ads/internal/ad_events/ad_event_info.h"
#include "bat/ads/internal/eligible_ads/eligible_ads_features.h"
#include "base/rand_util.h"
#include "base/time/time.h"

namespace ads {

const int kMatchesIntentChildSegmentWeight = 0;  // TODO(Moritz Haller): where to put magic constants?
const int kMatchesIntentParentSegmentWeight = 1;
const int kMatchesInterestChildSegmentWeight = 2;
const int kMatchesInterestParentSegmentWeight = 3;
const int kAdLastSeenInHoursWeight = 4;
const int kAdvertiserLastSeenInHoursWeight = 5;
const int kPriorityWeight = 6;

std::vector<double> ToAdFeatureWeights(const std::string& param_value);

template <typename T>
std::map<std::string, AdFeaturesInfo<T>> GroupEligibleAdsByCreativeInstanceId(
    const std::vector<T>& eligible_ads) {
  std::map<std::string, AdFeaturesInfo<T>> ads;
  for (const auto& eligible_ad : eligible_ads) {
    const auto iter = ads.find(eligible_ad.creative_instance_id);
    if (iter != ads.end()) {
      iter->second.segments.push_back(eligible_ad.segment);
      continue;
    }

    AdFeaturesInfo<T> ad_features;
    ad_features.segments = {eligible_ad.segment};
    ad_features.creative_ad = eligible_ad;
    ads.insert({eligible_ad.creative_instance_id, ad_features});
  }

  return ads;
}

bool DoesMatchSegments(const SegmentList& user_segments,
                       const SegmentList& ad_segments);

unsigned int CalculateAdLastSeenInHoursFeature(
    const AdEventList& ad_events,
    const std::string& creative_instance_id);

unsigned int CalculateAdvertiserLastSeenInHoursFeature(
    const AdEventList& ad_events,
    const std::string& advertiser_id);

template <typename T>
double CalculateScore(const AdFeaturesInfo<T>& ad_features) {
  // TODO(Moritz Haller) Explain matching and weights
  // currently seven features
  // weights are non-negative reals, where sum_weights > 0 (see test)
  std::vector<double> weights = features::GetAdFeatureWeights();

  double score = 0.0;

  if (ad_features.does_match_intent_child_segments) {
    score += weights.at(kMatchesIntentChildSegmentWeight);
  } else if (ad_features.does_match_intent_parent_segments) {
    score += weights.at(kMatchesIntentParentSegmentWeight);
  }

  if (ad_features.does_match_interest_child_segments) {
    score += weights.at(kMatchesInterestChildSegmentWeight);
  } else if (ad_features.does_match_interest_parent_segments) {
    score += weights.at(kMatchesInterestParentSegmentWeight);
  }

  if (ad_features.ad_last_seen_hours_ago <= base::Time::kHoursPerDay) {
    score += weights.at(kAdLastSeenInHoursWeight) *
             ad_features.ad_last_seen_hours_ago / base::Time::kHoursPerDay;
  }

  if (ad_features.advertiser_last_seen_hours_ago <= base::Time::kHoursPerDay) {
    score += weights.at(kAdvertiserLastSeenInHoursWeight) *
             ad_features.advertiser_last_seen_hours_ago /
             base::Time::kHoursPerDay;
  }

  score += weights.at(kPriorityWeight) /
           ad_features.creative_ad.priority;

  score *= ad_features.creative_ad.ptr;

  return score;
}

template <typename T>
std::map<std::string, AdFeaturesInfo<T>> ComputeFeaturesAndScores(
    const std::map<std::string, AdFeaturesInfo<T>>& ads,
    const AdEventList& ad_events,
    const SegmentList& interest_segments,
    const SegmentList& intent_segments) {
  std::map<std::string, AdFeaturesInfo<T>> ads_with_features;

  for (auto& ad : ads) {
    AdFeaturesInfo<T> ad_features;
    ad_features.does_match_intent_child_segments =
        DoesMatchSegments(intent_segments, ad.second.segments);
    ad_features.does_match_intent_parent_segments = DoesMatchSegments(
          GetParentSegments(intent_segments), ad.second.segments);
    ad_features.does_match_interest_child_segments =
        DoesMatchSegments(interest_segments, ad.second.segments);
    ad_features.does_match_interest_parent_segments = DoesMatchSegments(
          GetParentSegments(interest_segments), ad.second.segments);
    ad_features.ad_last_seen_hours_ago = CalculateAdLastSeenInHoursFeature(
        ad_events, ad.second.creative_ad.creative_instance_id);
    ad_features.advertiser_last_seen_hours_ago =
        CalculateAdvertiserLastSeenInHoursFeature(ad_events,
            ad.second.creative_ad.advertiser_id);
    ad_features.score = CalculateScore(ad.second);

    ads_with_features.insert({ad.first, ad_features});
  }

  return ads_with_features;
}

template <typename T>
double CalculateNormalisingConstant(
    const std::map<std::string, AdFeaturesInfo<T>>& ads) {
  double normalising_constant = 0.0;
  for (const auto& ad : ads) {
    normalising_constant += ad.second.score;
  }

  return normalising_constant;
}

template <typename T>
absl::optional<T> SampleFromAds(
    const std::map<std::string, AdFeaturesInfo<T>>& ads) {
  double normalising_constant = CalculateNormalisingConstant(ads);

  const double rand = base::RandDouble();
  double sum = 0;
  T selected_ad;

  for (const auto& ad : ads) {
    double probability = ad.second.score / normalising_constant;
    sum += probability;
    if (rand < sum) {
      return ad.second.creative_ad;
    }
  }

  return absl::nullopt;
}

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_ELIGIBLE_ADS_ELIGIBLE_ADS_UTIL_H_
