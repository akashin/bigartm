// Copyright 2014, Additive Regularization of Topic Models.

#include "gtest/gtest.h"

#include "artm/core/common.h"
#include "artm/core/topic_model.h"
#include "artm/messages.pb.h"

// artm_tests.exe --gtest_filter=TopicModelTest.Basic
TEST(TopicModelTest, Basic) {
  const float kTolerance = 1e-5f;

  int no_topics = 3;
  int no_tokens = 5;

  google::protobuf::RepeatedPtrField<std::string> topic_name;
  for (int i = 0; i < 3; ++i) {
    std::string* name = topic_name.Add();
    std::string str_name = "topic" + std::to_string(i);
    name = &str_name;
  }
  artm::core::TopicModel topic_model(::artm::core::ModelName(), topic_name);
  topic_model.AddToken(artm::core::Token(artm::core::DefaultClass, "token_1"));
  topic_model.AddToken(artm::core::Token(artm::core::DefaultClass, "token_2"));
  topic_model.AddToken(artm::core::Token(artm::core::DefaultClass, "token_3"));
  topic_model.AddToken(artm::core::Token(artm::core::DefaultClass, "token_4"));
  topic_model.AddToken(artm::core::Token(artm::core::DefaultClass, "token_5"));
  topic_model.InitializeRwt();

  //  test 1
  float real_normalizer = 0;
  for (int i = 0; i < no_tokens; ++i) {
    for (int j = 0; j < no_topics; ++j) {
      topic_model.SetTokenWeight(i, j, 1);
    }
  }
  std::vector<float> n_t = topic_model.FindNormalizers()[::artm::core::DefaultClass];
  for (int j = 0; j < no_topics; ++j) {
    real_normalizer += n_t[j];
  }
  float expected_normalizer = static_cast<float>(no_tokens * no_topics);
  EXPECT_TRUE(std::abs(real_normalizer - expected_normalizer) < kTolerance);

  //  test 2
  real_normalizer = 0;
  for (int i = 0; i < no_tokens; ++i) {
    for (int j = 0; j < no_topics; ++j) {
      topic_model.SetRegularizerWeight(i, j, -0.5f);
    }
  }
  n_t = topic_model.FindNormalizers()[::artm::core::DefaultClass];
  for (int j = 0; j < no_topics; ++j) {
    real_normalizer += n_t[j];
  }
  expected_normalizer = no_tokens * no_topics / 2.0f;
  EXPECT_TRUE(std::abs(real_normalizer - expected_normalizer) < kTolerance);

  //  test 3
  real_normalizer = 0;
  for (int i = 0; i < no_tokens; ++i) {
    for (int j = 0; j < no_topics; ++j) {
      topic_model.SetRegularizerWeight(i, j, -1.5);
    }
  }
  n_t = topic_model.FindNormalizers()[::artm::core::DefaultClass];
  for (int j = 0; j < no_topics; ++j) {
    real_normalizer += n_t[j];
  }
  expected_normalizer = 0;
  EXPECT_TRUE(std::abs(real_normalizer - expected_normalizer) < kTolerance);

  //  test 4
  real_normalizer = 0;
  for (int i = 0; i < no_tokens; ++i) {
    for (int j = 0; j < no_topics; ++j) {
      topic_model.IncreaseTokenWeight(i, j, 0.4f);
    }
  }
  n_t = topic_model.FindNormalizers()[::artm::core::DefaultClass];
  for (int j = 0; j < no_topics; ++j) {
    real_normalizer += n_t[j];
  }
  expected_normalizer = 0;
  EXPECT_TRUE(std::abs(real_normalizer - expected_normalizer) < kTolerance);

  //  test 5
  real_normalizer = 0;
  for (int i = 0; i < no_tokens; ++i) {
    for (int j = 0; j < no_topics; ++j) {
      topic_model.IncreaseTokenWeight(i, j, 0.6f);
    }
  }
  n_t = topic_model.FindNormalizers()[::artm::core::DefaultClass];
  for (int j = 0; j < no_topics; ++j) {
    real_normalizer += n_t[j];
  }
  expected_normalizer = no_tokens * no_topics / 2.0f;
  EXPECT_TRUE(std::abs(real_normalizer - expected_normalizer) < kTolerance);

  //  test 6
  real_normalizer = 0;
  for (int i = 0; i < no_tokens; ++i) {
    for (int j = 0; j < no_topics; ++j) {
      topic_model.SetTokenWeight(i, j, 1);
    }
  }
  n_t = topic_model.FindNormalizers()[::artm::core::DefaultClass];
  for (int j = 0; j < no_topics; ++j) {
    real_normalizer += n_t[j];
  }
  expected_normalizer = 0;
  EXPECT_TRUE(std::abs(real_normalizer - expected_normalizer) < kTolerance);

  //  test 7
  real_normalizer = 0;
  for (int i = 0; i < no_tokens; ++i) {
    for (int j = 0; j < no_topics; ++j) {
      topic_model.SetRegularizerWeight(i, j, -0.5f);
    }
  }
  n_t = topic_model.FindNormalizers()[::artm::core::DefaultClass];
  for (int j = 0; j < no_topics; ++j) {
    real_normalizer += n_t[j];
  }
  expected_normalizer = no_tokens * no_topics / 2.0f;
  EXPECT_TRUE(std::abs(real_normalizer - expected_normalizer) < kTolerance);
}
