/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/eligible_ads/eligible_ads_util.h"

#include <algorithm>
#include <numeric>

#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"

namespace ads {

// TODO(Moritz Haller): define alias for vector
std::vector<double> ToAdFeatureWeights(const std::string& param_value) {
  const std::vector<std::string> components = base::SplitString(
      param_value, ",", base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);

  std::vector<double> weights;
  for (const auto& component : components) {
    double value_as_double;
    if (!base::StringToDouble(component, &value_as_double)) {
      return {};
    }

    if (value_as_double < 0) {
      return {};
    }

    weights.push_back(value_as_double);
  }

  double sum = std::accumulate(weights.begin(), weights.end(),
                               decltype(weights)::value_type(0));
  if (sum <= 0) {
    return {};
  }

  return weights;
}

bool DoesMatchSegments(const SegmentList& user_segments,
                       const SegmentList& ad_segments) {
  std::vector<std::string> v1 = user_segments;
  std::vector<std::string> v2 = ad_segments;

  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());

  std::vector<std::string> intersection;
  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::back_inserter(intersection));

  if (intersection.size() > 0) {
    return true;
  }

  return false;
}

unsigned int CalculateAdLastSeenInHoursFeature(
    const AdEventList& ad_events,
    const std::string& creative_instance_id) {
  // ad_events are ordered by timestamp desc in DB
  const auto iter = std::find_if(
      ad_events.begin(), ad_events.end(),
      [&creative_instance_id](const AdEventInfo& ad_event) -> bool {
        return (ad_event.creative_instance_id == creative_instance_id &&
                ad_event.confirmation_type == ConfirmationType::kViewed);
      });

  if (iter == ad_events.end()) {
    return 0;
  }

  const base::Time now = base::Time::Now();
  const base::Time timestamp = base::Time::FromDoubleT(iter->timestamp);
  return (now - timestamp).InHours();
}

unsigned int CalculateAdvertiserLastSeenInHoursFeature(
    const AdEventList& ad_events,
    const std::string& advertiser_id) {
  // ad_events are ordered by timestamp desc in DB
  const auto iter = std::find_if(
      ad_events.begin(), ad_events.end(),
      [&advertiser_id](const AdEventInfo& ad_event) -> bool {
        return (ad_event.advertiser_id == advertiser_id &&
                ad_event.confirmation_type == ConfirmationType::kViewed);
      });

  if (iter == ad_events.end()) {
    return 0;
  }

  const base::Time now = base::Time::Now();
  const base::Time timestamp = base::Time::FromDoubleT(iter->timestamp);
  return (now - timestamp).InHours();
}

}  // namespace ads
