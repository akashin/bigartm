// Copyright 2014, Additive Regularization of Topic Models.

#include "artm/core/score_manager.h"

#include "boost/exception/diagnostic_information.hpp"

#include "glog/logging.h"

#include "artm/core/exceptions.h"
#include "artm/core/helpers.h"
#include "artm/core/instance_schema.h"

namespace artm {
namespace core {

void ScoreManager::Append(std::shared_ptr<InstanceSchema> schema,
                          const ScoreName& score_name,
                          const std::string& score_blob) {
  auto score_calculator = schema->score_calculator(score_name);
  if (score_calculator == nullptr) {
    LOG(ERROR) << "Unable to find score calculator: " << score_name;
    return;
  }

  auto score_inc = score_calculator->CreateScore();
  if (!score_inc->ParseFromString(score_blob)) {
    LOG(ERROR) << "Merger was unable to parse score blob. The scores might be inacurate.";
    return;
  }

  // Note that the following operation must be atomic
  // (e.g. finding score / append score / setting score).
  // This is the reason to use explicit lock around score_map_ instead of ThreadSafeCollectionHolder.
  boost::lock_guard<boost::mutex> guard(lock_);
  auto iter = score_map_.find(score_name);
  if (iter != score_map_.end()) {
    score_calculator->AppendScore(*iter->second, score_inc.get());
    iter->second = score_inc;
  } else {
    score_map_.insert(std::pair<ScoreName, std::shared_ptr<Score>>(score_name, score_inc));
  }
}

void ScoreManager::Clear() {
  boost::lock_guard<boost::mutex> guard(lock_);
  score_map_.clear();
}

bool ScoreManager::RequestScore(std::shared_ptr<InstanceSchema> schema,
                                const ScoreName& score_name,
                                ScoreData *score_data) const {
  auto score_calculator = schema->score_calculator(score_name);
  if (score_calculator == nullptr)
    BOOST_THROW_EXCEPTION(InvalidOperation(
      std::string("Attempt to request non-existing score: " + score_name)));

  if (!score_calculator->is_cumulative())
    return false;

  {
    boost::lock_guard<boost::mutex> guard(lock_);
    auto iter = score_map_.find(score_name);
    if (iter != score_map_.end()) {
      score_data->set_data(iter->second->SerializeAsString());
    } else {
      score_data->set_data(score_calculator->CreateScore()->SerializeAsString());
    }
  }

  score_data->set_type(score_calculator->score_type());
  score_data->set_name(score_name);
  return true;
}

}  // namespace core
}  // namespace artm
