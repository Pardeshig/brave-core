/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/eip1559_transaction.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "base/values.h"
#include "brave/components/brave_wallet/browser/hd_key.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace brave_wallet {

TEST(Eip1559TransactionUnitTest, GetMessageToSign) {
  std::vector<uint8_t> data;
  EXPECT_TRUE(base::HexStringToBytes("010200", &data));
  EthTransaction::TxData tx_data(
      0x00, 0x00, 0x00,
      EthAddress::FromHex("0x0101010101010101010101010101010101010101"), 0x00,
      data);
  Eip1559Transaction tx(tx_data, 0x04, 0, 0);
  ASSERT_EQ(tx.type(), 2);
  auto* access_list = tx.access_list();
  Eip2930Transaction::AccessListItem item;
  item.address.fill(0x01);

  Eip2930Transaction::AccessedStorageKey storage_key_1;
  storage_key_1.fill(0x01);
  item.storage_keys.push_back(storage_key_1);

  access_list->push_back(item);
  tx.GetMessageToSign(
      0, base::BindOnce([](const std::vector<uint8_t>& m) {
        EXPECT_EQ(
            base::ToLowerASCII(base::HexEncode(m)),
            "fa81814f7dd57bad435657a05eabdba2815f41e3f15ddd6139027e7db56b0dea");
      }));
}

TEST(Eip1559TransactionUnitTest, GetSignedTransaction) {
  const struct {
    uint256_t nonce;
    uint256_t value;
    uint256_t gas_limit;
    uint256_t max_priority_fee_per_gas;
    uint256_t max_fee_per_gas;
    const char* signed_tx;
  } cases[] = {
      {819, 43203529, 35552, 75853, 121212,
       "0x02f86e048203338301284d8301d97c828ae094000000000000000000000000000"
       "000000000aaaa8402933bc980c080a00f924cb68412c8f1cfd74d9b581c71eeaf94ff"
       "f6abdde3e5b02ca6b2931dcf47a07dd1c50027c3e31f8b565e25ce68a5072110f61fce5"
       "eee81b195dd51273c2f83"},
      {353, 61901619, 32593, 38850, 136295,
       "0x02f86d048201618297c283021467827f519400000000000000000000000000000"
       "0000000aaaa8403b08b3380c080a08caf712f72489da6f1a634b651b4b1c7d9be7d1e"
       "8d05ea76c1eccee3bdfb86a5a06aecc106f588ce51e112f5e9ea7aba3e089dc75117188"
       "21d0e0cd52f52af4e45"},
      {985, 32531825, 68541, 66377, 136097,
       "0x02f86f048203d983010349830213a183010bbd940000000000000000000000000"
       "00000000000aaaa8401f0657180c001a08c03a86e85789ee9a1b42fa0a86d316fca26"
       "2694f8c198df11f194678c2c2d35a028f8e7de02b35014a17b6d28ff8c7e7be6860e726"
       "5ac162fb721f1aeae75643c"},
      {623, 21649799, 57725, 74140, 81173,
       "0x02f86e0482026f8301219c83013d1582e17d94000000000000000000000000000"
       "000000000aaaa84014a598780c001a0b87c4c8c505d2d692ac77ba466547e79dd60fe"
       "7ecd303d520bf6e8c7085e3182a06dc7d00f5e68c3f3ebe8ae35a90d46051afde620ac1"
       "2e43cae9560a29b13e7fb"},
      {972, 94563383, 65254, 42798, 103466,
       "0x02f86d048203cc82a72e8301942a82fee69400000000000000000000000000000"
       "0000000aaaa8405a2ec3780c001a006cf07af78c187db104496c58d679f37fcd2d579"
       "0970cecf9a59fe4a5321b375a039f3faafc71479d283a5b1e66a86b19c4bdc516655d89"
       "dbe57d9747747c01dfe"},
      {588, 99359647, 37274, 87890, 130273,
       "0x02f86e0482024c830157528301fce182919a94000000000000000000000000000"
       "000000000aaaa8405ec1b9f80c080a03e2f59ac9ca852034c2c1da35a742ca19fdd91"
       "0aa5d2ed49ab8ad27a2fcb2b10a03ac1c29c26723c58f91400fb6dfb5f5b837467b1c37"
       "7541b47dae474dddbe469"},
      {900, 30402257, 76053, 8714, 112705,
       "0x02f86e0482038482220a8301b8418301291594000000000000000000000000000"
       "000000000aaaa8401cfe6d180c001a0f7ffc5bca2512860f8236360159bf303dcfab715"
       "46b6a0032df0306f3739d0c4a05d38fe2c4edebdc1edc157034f780c53a0e5ae089e572"
       "20745bd48bcb10cdf87"},
      {709, 6478043, 28335, 86252, 94636,
       "0x02f86d048202c5830150ec830171ac826eaf94000000000000000000000000000"
       "000000000aaaa8362d8db80c001a0a61a5710512f346c9996377f7b564ccb64c73a5fdb"
       "615499adb1250498f3e01aa002d10429572cecfaa911a58bbe05f2b26e4c3aee3402202"
       "153a93692849add11"},
      {939, 2782905, 45047, 45216, 91648,
       "0x02f86c048203ab82b0a08301660082aff79400000000000000000000000000000"
       "0000000aaaa832a76b980c001a0191f0f6667a20cefc0b454e344cc01108aafbdc4e4e5"
       "ed88fdd1b5d108495b31a020879042b0f8d3807609f18fe42a9820de53c8a0ea1d0a2d5"
       "0f8f5e92a94f00d"},
      {119, 65456115, 62341, 24721, 107729,
       "0x02f86b04778260918301a4d182f38594000000000000000000000000000000000"
       "000aaaa8403e6c7f380c001a05e40977f4064a2bc08785e422ed8a47b56aa219abe9325"
       "1d2b3b4d0cf937b8c0a071e600cd15589c3607bd9627314b99e9b5976bd427b774aa685"
       "bd0d036b1771e"}};
  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    std::vector<uint8_t> private_key;
    EXPECT_TRUE(base::HexStringToBytes(
        "8f2a55949038a9610f50fb23b5883af3b4ecb3c3bb792cbcefbd1542c692be63",
        &private_key));

    HDKey key;
    key.SetPrivateKey(private_key);
    auto tx = std::make_unique<Eip1559Transaction>(
        EthTransaction::TxData(
            cases[i].nonce, 0x00, cases[i].gas_limit,
            EthAddress::FromHex("0x000000000000000000000000000000000000aaaa"),
            cases[i].value, std::vector<uint8_t>()),
        0x04, cases[i].max_priority_fee_per_gas, cases[i].max_fee_per_gas);

    std::vector<uint8_t> signature;
    int recid;
    tx->GetMessageToSign(0, base::BindOnce(
                                [](std::vector<uint8_t>* signature, int* recid,
                                   HDKey* key, const std::vector<uint8_t>& m) {
                                  *signature = key->Sign(m, recid);
                                },
                                &signature, &recid, &key));

    tx->ProcessSignature(signature, recid, 0);

    tx->GetSignedTransaction(base::BindOnce(
        [](const std::string signed_tx, const std::string& signed_transaction) {
          EXPECT_EQ(signed_transaction, signed_tx);
        },
        cases[i].signed_tx));
  }
}

TEST(Eip1559TransactionUnitTest, GetUpfrontCost) {
  Eip1559Transaction tx(
      EthTransaction::TxData(
          0x00, 0x00, 100,
          EthAddress::FromHex("0x0101010101010101010101010101010101010101"),
          0x06, std::vector<uint8_t>()),
      0x04, 8, 10);
  EXPECT_EQ(tx.GetUpfrontCost(), uint256_t(806));
  EXPECT_EQ(tx.GetUpfrontCost(0), uint256_t(806));
  EXPECT_EQ(tx.GetUpfrontCost(4), uint256_t(1006));
}

TEST(Eip1559TransactionUnitTest, Serialization) {
  Eip1559Transaction tx(
      EthTransaction::TxData(
          0x09, 0x4a817c800, 0x5208,
          EthAddress::FromHex("0x3535353535353535353535353535353535353535"),
          0x0de0b6b3a7640000, std::vector<uint8_t>()),
      5566, 123, 456);
  auto* access_list = tx.access_list();
  Eip2930Transaction::AccessListItem item_a;
  item_a.address.fill(0x0a);
  Eip2930Transaction::AccessedStorageKey storage_key_0;
  storage_key_0.fill(0x00);
  item_a.storage_keys.push_back(storage_key_0);
  access_list->push_back(item_a);

  base::Value tx_value = tx.ToValue();
  auto tx_from_value = Eip1559Transaction::FromValue(tx_value);
  ASSERT_NE(tx_from_value, nullptr);
  EXPECT_EQ(*tx_from_value, tx);
}

}  // namespace brave_wallet
