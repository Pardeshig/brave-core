/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/eligible_ads/eligible_ads_util.h"

#include <vector>

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"
#include "bat/ads/internal/user_activity/user_activity_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

TEST(BatAdsEligibleAdsUtilTest, ToAdFeatureWeightsForEmptyParamValue) {
  // Arrange

  // Act
  const std::vector<double> weights = ToAdFeatureWeights("");

  // Assert
  const std::vector<double> expected_weights = {};
  EXPECT_EQ(expected_weights, weights);
}

TEST(BatAdsEligibleAdsUtilTest, ToAdFeatureWeightsForNonNumericParamValue) {
  // Arrange

  // Act
  const std::vector<double> weights = ToAdFeatureWeights("1.0, foobar, 2.2");

  // Assert
  const std::vector<double> expected_weights = {};
  EXPECT_EQ(expected_weights, weights);
}

TEST(BatAdsEligibleAdsUtilTest, ToAdFeatureWeightsForAllZeroParamValue) {
  // Arrange

  // Act
  const std::vector<double> weights = ToAdFeatureWeights("0.0, 0.0, 0.0");

  // Assert
  const std::vector<double> expected_weights = {};
  EXPECT_EQ(expected_weights, weights);
}

TEST(BatAdsEligibleAdsUtilTest, ToAdFeatureWeightsForSomeZeroParamValue) {
  // Arrange

  // Act
  const std::vector<double> weights = ToAdFeatureWeights("0.0, 0.1, 0.0");

  // Assert
  const std::vector<double> expected_weights = {0.0, 0.1, 0.0};
  EXPECT_EQ(expected_weights, weights);
}

TEST(BatAdsEligibleAdsUtilTest, ToAdFeatureWeightsForNegativeParamValue) {
  // Arrange

  // Act
  const std::vector<double> weights = ToAdFeatureWeights("1.0, 3.0, -2.0");

  // Assert
  const std::vector<double> expected_weights = {};
  EXPECT_EQ(expected_weights, weights);
}

TEST(BatAdsEligibleAdsUtilTest, ToAdFeatureWeightsForSingleParamValue) {
  // Arrange

  // Act
  const std::vector<double> weights = ToAdFeatureWeights("1.0");

  // Assert
  const std::vector<double> expected_weights = {1.0};
  EXPECT_EQ(expected_weights, weights);
}

TEST(BatAdsEligibleAdsUtilTest, ToAdFeatureWeightsForParamValue) {
  // Arrange

  // Act
  const std::vector<double> weights = ToAdFeatureWeights("1.1, 3.3, 2.2");

  // Assert
  const std::vector<double> expected_weights = {1.1, 3.3, 2.2};
  EXPECT_EQ(expected_weights, weights);
}

TEST(BatAdsEligibleAdsUtilTest, ToAdFeatureWeightsForParamValueWithMixedTypes) {
  // Arrange

  // Act
  const std::vector<double> weights = ToAdFeatureWeights("1, 3, 2.2");

  // Assert
  const std::vector<double> expected_weights = {1.0, 3.0, 2.2};
  EXPECT_EQ(expected_weights, weights);
}

// TODO(Moritz Haller): split out and test all utils

}  // namespace ads
